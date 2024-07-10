#include "rclcpp/rclcpp.hpp"
#include "tf2_ros/buffer.h"
#include "tf2_ros/transform_listener.h"
#include "geometry_msgs/msg/transform_stamped.hpp"
#include <memory>
#include <mutex>
#include <moveit/move_group_interface/move_group_interface.h>
#include <geometry_msgs/msg/pose.hpp>
#include <geometry_msgs/msg/point.hpp>
#include "tf2/LinearMath/Quaternion.h"
#include <tf2_geometry_msgs/tf2_geometry_msgs.hpp>
#include "std_msgs/msg/bool.hpp"

class TFListenerNode : public rclcpp::Node
{
public:
    TFListenerNode()
    : Node("tf_listener_and_moveit_node"),
      first_execution_(true),
      received_depth_coordinates_(false)
    {
        tf_buffer_ = std::make_shared<tf2_ros::Buffer>(get_clock());
        tf_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf_buffer_);

        depth_coordinates_subscription_ = this->create_subscription<geometry_msgs::msg::Point>(
            "/depth_coordinates", 10, std::bind(&TFListenerNode::depthCoordinatesCallback, this, std::placeholders::_1));
        
        RCLCPP_INFO(this->get_logger(), "Subscription to /depth_coordinates created.");

        moveit_timer_ = this->create_wall_timer(std::chrono::milliseconds(500), std::bind(&TFListenerNode::moveitCallback, this));

        moveit_mutex_ = std::make_shared<std::mutex>();

        success_left_publisher_ = this->create_publisher<std_msgs::msg::Bool>("/moveit_success_left", 10);
        success_right_publisher_ = this->create_publisher<std_msgs::msg::Bool>("/moveit_success_right", 10);
    }

    void depthCoordinatesCallback(const geometry_msgs::msg::Point::SharedPtr msg)
    {
        RCLCPP_INFO(this->get_logger(), "Received depth coordinates message.");
        received_depth_coordinates_ = true;
        pose_.position.x = msg->x;
        pose_.position.y = msg->y;
        pose_.position.z = msg->z;
        RCLCPP_INFO(this->get_logger(), "Box Pose - position: [%f, %f, %f]", pose_.position.x, pose_.position.y, pose_.position.z);
    }

    geometry_msgs::msg::Pose convertPoseToBaseLink(const geometry_msgs::msg::Pose& pose_camera)
    {
        geometry_msgs::msg::Pose pose_base_link;

        try
        {
            geometry_msgs::msg::TransformStamped transform_base_link = tf_buffer_->lookupTransform("huagan", "camera_world", tf2::TimePointZero);
            tf2::doTransform(pose_camera, pose_base_link, transform_base_link);
        }
        catch (tf2::TransformException& ex)
        {
            RCLCPP_ERROR(this->get_logger(), "Transform error: %s", ex.what());
        }

        return pose_base_link;
    }

    void moveitCallback()
    {
        std::lock_guard<std::mutex> lock(*moveit_mutex_);

        if (!received_depth_coordinates_) {
            RCLCPP_ERROR(this->get_logger(), "No depth coordinates received yet. Waiting...");
            return;
        }

        auto pose_camera = getPose();
        auto pose_base_link = convertPoseToBaseLink(pose_camera);
        auto node = this->shared_from_this();

        auto move_group_interface_left = std::make_shared<moveit::planning_interface::MoveGroupInterface>(
            node, "leftarm", tf_buffer_, std::chrono::seconds(1));
        auto move_group_interface_right = std::make_shared<moveit::planning_interface::MoveGroupInterface>(
            node, "rightarm", tf_buffer_, std::chrono::seconds(1));
        // move_group_interface_left->setGoalOrientationTolerance(0.01);  // 使用 -> 代替 
        // move_group_interface_right->setGoalOrientationTolerance(0.01); // 使用 -> 代替 .
        move_group_interface_left->setGoalPositionTolerance(0.1);
        move_group_interface_right->setGoalPositionTolerance(0.1);
        // double position_tolerance_left = move_group_interface_left->getGoalPositionTolerance();
        // double orientation_tolerance_left = move_group_interface_left->getGoalOrientationTolerance();
        // double position_tolerance_right = move_group_interface_right->getGoalPositionTolerance();
        // double orientation_tolerance_right = move_group_interface_right->getGoalOrientationTolerance();
        // RCLCPP_INFO(node->get_logger(), "left position tolerance: %f", position_tolerance_left);
        // RCLCPP_INFO(node->get_logger(), "left orientation tolerance: %f", orientation_tolerance_left);
        // RCLCPP_INFO(node->get_logger(), "right position tolerance: %f", position_tolerance_right);
        // RCLCPP_INFO(node->get_logger(), "right orientation tolerance: %f", orientation_tolerance_right);
        // pose_base_link.orientation.x = 0.0;
        // pose_base_link.orientation.y = 1;
        // pose_base_link.orientation.z = 0.0;
        // pose_base_link.orientation.w = 0.0;
        RCLCPP_INFO(this->get_logger(), "Target Pose - position: [%f, %f, %f], rotation: [%f, %f, %f, %f]",
                 pose_base_link.position.x, pose_base_link.position.y, pose_base_link.position.z,
                 pose_base_link.orientation.x, pose_base_link.orientation.y, pose_base_link.orientation.z, pose_base_link.orientation.w);

        moveit::core::MoveItErrorCode success_left;
        moveit::core::MoveItErrorCode success_right;

        std::vector<double> joint_values_left;
        std::vector<double> joint_values_right;
        moveit::planning_interface::MoveGroupInterface::Plan plan_left, plan_right;
        move_group_interface_left->setPoseTarget(pose_base_link,"link_l8_1");
        success_left = move_group_interface_left->plan(plan_left);
        if (success_left == moveit::core::MoveItErrorCode::SUCCESS) {
            joint_values_left = plan_left.trajectory_.joint_trajectory.points.back().positions;
        } else {
            RCLCPP_ERROR(this->get_logger(), "Planning failed for left arm!");

            // 设置左臂每个关节角度
            joint_values_left = {
                0,          // l1: 0 degrees
                -M_PI / 4,  // l2: -45 degrees
                M_PI / 2,   // l3: 90 degrees
                M_PI / 2,   // l4: 90 degrees
                0           // l5: 0 degrees
            };

            // 使用设置好的关节角度
            move_group_interface_left->setJointValueTarget(joint_values_left);
            bool success_ik_left = (move_group_interface_left->plan(plan_left) == moveit::core::MoveItErrorCode::SUCCESS);
            if (!success_ik_left) {
                RCLCPP_ERROR(this->get_logger(), "Inverse kinematics failed for left arm!");
            }
        }

        
            move_group_interface_right->setPoseTarget(pose_base_link,"link_r8_1");
            success_right = move_group_interface_right->plan(plan_right);
            if (success_right == moveit::core::MoveItErrorCode::SUCCESS) {
                joint_values_right = plan_right.trajectory_.joint_trajectory.points.back().positions;
            } else {
                RCLCPP_ERROR(this->get_logger(), "Planning failed for right arm!");

                // 设置右臂每个关节角度
                joint_values_right = {
                    0,          // r1: 0 degrees
                    -M_PI / 4,  // r2: -45 degrees
                    M_PI / 2,   // r3: 90 degrees
                    M_PI / 2,   // r4: 90 degrees
                    0           // r5: 0 degrees
                };

                // 使用设置好的关节角度
                move_group_interface_right->setJointValueTarget(joint_values_right);
                bool success_ik_right = (move_group_interface_right->plan(plan_right) == moveit::core::MoveItErrorCode::SUCCESS);
                if (!success_ik_right) {
                    RCLCPP_ERROR(this->get_logger(), "Inverse kinematics failed for right arm!");
                }
            }
        

        if (success_left == moveit::core::MoveItErrorCode::SUCCESS || success_right == moveit::core::MoveItErrorCode::SUCCESS || first_execution_ == true) {
            first_execution_ = false;
            std::vector<double> joint_values_both;
        // 打印左臂和右臂的关节角度数量和值
            RCLCPP_INFO(this->get_logger(), "Number of joint angles for left arm: %zu", joint_values_left.size());
            for (size_t i = 0; i < joint_values_left.size(); ++i) {
                RCLCPP_INFO(this->get_logger(), "Left arm joint value %zu: %f", i, joint_values_left[i]);
            }

            RCLCPP_INFO(this->get_logger(), "Number of joint angles for right arm: %zu", joint_values_right.size());
            for (size_t i = 0; i < joint_values_right.size(); ++i) {
                RCLCPP_INFO(this->get_logger(), "Right arm joint value %zu: %f", i, joint_values_right[i]);
            }

            if (joint_values_left.size() != 5 || joint_values_right.size() != 5) {
                RCLCPP_ERROR(this->get_logger(), "Invalid number of joint angles for left or right arm!");
                return;
            }
            joint_values_both.insert(joint_values_both.end(), joint_values_left.begin(), joint_values_left.end());
            joint_values_both.insert(joint_values_both.end(), joint_values_right.begin(), joint_values_right.end());
            for (size_t i = 0; i < joint_values_both.size(); ++i) {
            RCLCPP_INFO(this->get_logger(), "Joint value %zu: %f", i, joint_values_both[i]);
            }
            auto move_group_interface_both = std::make_shared<moveit::planning_interface::MoveGroupInterface>(node, "botharm");
            move_group_interface_both->setJointValueTarget(joint_values_both);
            moveit::planning_interface::MoveGroupInterface::Plan plan_both;
            auto success_both = move_group_interface_both->plan(plan_both);
            if (success_both == moveit::core::MoveItErrorCode::SUCCESS) {
                RCLCPP_INFO(this->get_logger(), "Planning successful for both arms");
                move_group_interface_both->execute(plan_both);
                std::this_thread::sleep_for(std::chrono::seconds(10));
                // 发布左右手的成功状态
                std_msgs::msg::Bool success_msg_left;
                success_msg_left.data = (success_left == moveit::core::MoveItErrorCode::SUCCESS);
                success_left_publisher_->publish(success_msg_left);
                std_msgs::msg::Bool success_msg_right;
                success_msg_right.data = (success_right == moveit::core::MoveItErrorCode::SUCCESS);
                success_right_publisher_->publish(success_msg_right);

            } else {
                RCLCPP_ERROR(this->get_logger(), "Planning failed for both arms!");

                // 发布左右手的失败状态
                std_msgs::msg::Bool success_msg_left;
                success_msg_left.data = false;
                success_left_publisher_->publish(success_msg_left);

                std_msgs::msg::Bool success_msg_right;
                success_msg_right.data = false;
                success_right_publisher_->publish(success_msg_right);
            }
        }
    }

    geometry_msgs::msg::Pose getPose() const
    {
        return pose_;
    }

    bool hasReceivedDepthCoordinates() const {
        return received_depth_coordinates_;
    }

private:
    std::shared_ptr<tf2_ros::Buffer> tf_buffer_;
    std::shared_ptr<tf2_ros::TransformListener> tf_listener_;
    rclcpp::Subscription<geometry_msgs::msg::Point>::SharedPtr depth_coordinates_subscription_;
    rclcpp::TimerBase::SharedPtr moveit_timer_;
    std::shared_ptr<std::mutex> moveit_mutex_;
    rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr success_left_publisher_;
    rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr success_right_publisher_;
    geometry_msgs::msg::Pose pose_;
    bool first_execution_;
    bool received_depth_coordinates_; // 新增的标志变量
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<TFListenerNode>();

    // 等待直到接收到第一个depth_coordinates消息
    rclcpp::Rate rate(1); // 1 Hz
    while (rclcpp::ok() && !node->hasReceivedDepthCoordinates()) {
        RCLCPP_ERROR(node->get_logger(), "Waiting for depth coordinates...");
        rclcpp::spin_some(node);
        rate.sleep();
    }

    RCLCPP_INFO(node->get_logger(), "Received first depth coordinates message. Starting main loop.");
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
