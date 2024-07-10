#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/point.hpp"
#include "sensor_msgs/msg/camera_info.hpp"
#include "sensor_msgs/msg/image.hpp"
#include "cv_bridge/cv_bridge.h"
#include "opencv2/opencv.hpp"
#include "vision_msgs/msg/detection2_d_array.hpp"
#include "tf2_ros/transform_broadcaster.h"
#include "tf2_ros/static_transform_broadcaster.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.hpp"
#include <cmath>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2/LinearMath/Matrix3x3.h>

double x;
double y;
double Pos_z;

class DepthImageProcessor : public rclcpp::Node
{
public:
    DepthImageProcessor() : Node("depth_image_processor")
    {
        // camera_info_subscription_ = this->create_subscription<sensor_msgs::msg::CameraInfo>(
        //     "/camera/depth/camera_info", rclcpp::SensorDataQoS(), std::bind(&DepthImageProcessor::camera_info_callback, this, std::placeholders::_1));

        depth_image_subscription_ = this->create_subscription<sensor_msgs::msg::Image>(
            "/camera/depth/image_raw", rclcpp::SensorDataQoS(), std::bind(&DepthImageProcessor::depth_image_callback, this, std::placeholders::_1));

        center_point_subscription_ = this->create_subscription<vision_msgs::msg::Detection2DArray>(
            "object_center", 10, std::bind(&DepthImageProcessor::center_point_callback, this, std::placeholders::_1));

        depth_coordinates_publisher_ = this->create_publisher<geometry_msgs::msg::Point>(
            "depth_coordinates", 10);

        static_link_tf_broadcaster_ = std::make_shared<tf2_ros::StaticTransformBroadcaster>(this);
        static_camera_tf_broadcaster_ = std::make_shared<tf2_ros::StaticTransformBroadcaster>(this);
        object_tf_publisher_ = std::make_shared<tf2_ros::TransformBroadcaster>(this);
        timer_ = this->create_wall_timer(std::chrono::milliseconds(20), std::bind(&DepthImageProcessor::timer_callback, this));
        publish_static_transforms();
    }

private:
    void center_point_callback(const vision_msgs::msg::Detection2DArray::SharedPtr msg)
    {
        if ( !depth_image_received_ || depth_image_->image.empty())
        {
            RCLCPP_ERROR(this->get_logger(), "Camera info or depth image not received yet!");
            return;
        }

        double x_c = 0.0, y_c = 0.0;
        for (const auto& detection : msg->detections)
        {
            x_c = detection.bbox.center.position.x;
            y_c = detection.bbox.center.position.y;
            RCLCPP_INFO(this->get_logger(), "Received center point coordinates: x=%f, y=%f", x_c, y_c);
        }
        int u = static_cast<int>(x_c);
        int v = static_cast<int>(y_c);
        uchar num_count = 0;
        double Pos_z_temp = 0;
        if (u < 0 || v < 0 || u >= depth_image_->image.cols || v >= depth_image_->image.rows)
        {
            RCLCPP_ERROR(this->get_logger(), "Invalid image coordinates: u=%d, v=%d", u, v);
            return;
        }
        double depth_scale = 0.001;
        for (int jj = -1; jj < 2; jj++)
        {
            for (int ii = -1; ii < 2; ii++)
            {
                int y_temp = v + ii;
                int x_temp = u + jj;
                if (x_temp >= 0 && y_temp >= 0 && x_temp < depth_image_->image.cols && y_temp < depth_image_->image.rows)
                {
                    uint16_t depth_value = depth_image_->image.at<uint16_t>(y_temp, x_temp);
                    if (depth_value > 0)
                    {
                        Pos_z_temp += depth_value * depth_scale;  // 将深度值转换为米
                        num_count++;
                    }
                }
            }
        }
        if (num_count > 0)
        {
            Pos_z = Pos_z_temp / num_count;
        }
        else
        {
            x = 0;
            y = 0;
            Pos_z = 0;
            RCLCPP_WARN(this->get_logger(), "No valid depth values found in the neighborhood.");
            return;
        }
        double fx = 570.342205;
        double fy = 570.342205;
        double cx = 319.500000;
        double cy = 239.500000;
        x = (x_c - cx) * Pos_z / fx;
        y = (y_c - cy) * Pos_z / fy;
        RCLCPP_INFO(this->get_logger(), "Computed real 3D coordinates: x=%f, y=%f, z=%f", x, y, Pos_z);
        auto depth_point_msg = std::make_unique<geometry_msgs::msg::Point>();
        depth_point_msg->x = x;
        depth_point_msg->y = y;
        depth_point_msg->z = Pos_z;
        depth_coordinates_publisher_->publish(std::move(depth_point_msg));
        timer_callback();
        num_count = 0;
        Pos_z_temp = 0;
    }
    void depth_image_callback(const sensor_msgs::msg::Image::SharedPtr msg)
    {
        if (msg->encoding != sensor_msgs::image_encodings::TYPE_16UC1)
        {
            RCLCPP_ERROR(this->get_logger(), "Invalid depth data format!");
            return;
        }
        try
        {
            depth_image_ = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::TYPE_16UC1);
        }
        catch (cv_bridge::Exception &e)
        {
            RCLCPP_ERROR(this->get_logger(), "cv_bridge exception: %s", e.what());
            return;
        }

        depth_image_received_ = true;
    }

    void timer_callback()
    {
        geometry_msgs::msg::TransformStamped object_transform;
        object_transform.header.stamp = this->now();
        object_transform.header.frame_id = "camera_world";
        object_transform.child_frame_id = "object";
        object_transform.transform.translation.x = x; 
        object_transform.transform.translation.y = y; 
        object_transform.transform.translation.z = Pos_z; 
        tf2::Quaternion q_object;
        q_object.setRPY(0, 0, 0);
        object_transform.transform.rotation.x = q_object.x();
        object_transform.transform.rotation.y = q_object.y();
        object_transform.transform.rotation.z = q_object.z();
        object_transform.transform.rotation.w = q_object.w();
        object_tf_publisher_->sendTransform(object_transform);
    }

    void publish_static_transforms()
    {
        geometry_msgs::msg::TransformStamped link_transform;
        link_transform.header.stamp = this->now();
        link_transform.header.frame_id = "base_link";
        link_transform.child_frame_id = "camera_link";
        link_transform.transform.translation.x = 0.02;
        link_transform.transform.translation.y = 0.05;
        link_transform.transform.translation.z = 1.11;
        link_transform.transform.rotation.x = 0.0;
        link_transform.transform.rotation.y = 0.4848;
        link_transform.transform.rotation.z = 0.0;
        link_transform.transform.rotation.w = 0.8746;
        static_link_tf_broadcaster_->sendTransform(link_transform);
        geometry_msgs::msg::TransformStamped camera_transform;
        camera_transform.header.stamp = this->now();
        camera_transform.header.frame_id = "camera_link";
        camera_transform.child_frame_id = "camera_world";
        camera_transform.transform.translation.x = 0.0;
        camera_transform.transform.translation.y = 0.0;
        camera_transform.transform.translation.z = 0.0;
        tf2::Quaternion q_z;
        q_z.setRPY(0, 0, -M_PI_2);
        tf2::Quaternion q_x;
        q_x.setRPY(0, M_PI_2, 0);
        tf2::Quaternion q_combined = q_x * q_z;
        camera_transform.transform.rotation.x = q_combined.x();
        camera_transform.transform.rotation.y = q_combined.y();
        camera_transform.transform.rotation.z = q_combined.z();
        camera_transform.transform.rotation.w = q_combined.w();
        static_camera_tf_broadcaster_->sendTransform(camera_transform);
    }

    rclcpp::Subscription<vision_msgs::msg::Detection2DArray>::SharedPtr center_point_subscription_;
    rclcpp::Subscription<sensor_msgs::msg::CameraInfo>::SharedPtr camera_info_subscription_;
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr depth_image_subscription_;
    rclcpp::Publisher<geometry_msgs::msg::Point>::SharedPtr depth_coordinates_publisher_;
    std::shared_ptr<tf2_ros::TransformBroadcaster> object_tf_publisher_;
    std::shared_ptr<tf2_ros::StaticTransformBroadcaster> static_link_tf_broadcaster_;
    std::shared_ptr<tf2_ros::StaticTransformBroadcaster> static_camera_tf_broadcaster_;
    bool depth_image_received_ = false;
    cv_bridge::CvImageConstPtr depth_image_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<DepthImageProcessor>());
    rclcpp::shutdown();
    return 0;
}
