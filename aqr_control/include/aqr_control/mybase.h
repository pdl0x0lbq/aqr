//
// Created by sw on 24-6-4.
//

#ifndef AQR_CONTROL_MYBASE_H
#define AQR_CONTROL_MYBASE_H
#define PI           (3.1415926)
#define ROBOT_RADIUS (0.0675)   //m
#define ROBOT_TRACK  (0.135)    //m
#define ROBOT_LENGTH (0.15)     //m
#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <nav_msgs/msg/odometry.hpp>
#include "armquadrover_msgs/msg/ddsm115.hpp"
#include "sensor_msgs/msg/imu.hpp"
#include "geometry_msgs/msg/transform_stamped.hpp"
#include "geometry_msgs/msg/pose2_d.hpp"
#include "std_msgs/msg/int32.hpp"
#include <tf2_ros/transform_broadcaster.h>
#include <tf2/utils.h>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2/LinearMath/Matrix3x3.h>
using namespace std::chrono_literals;
enum class ChassisState {
    NORMAL,
    ALIGN,
    ALIGN_Y,
    ALIGN_X
};
class BaseNode : public rclcpp::Node {
public:
    BaseNode() : Node("my_node") {
        // 创建订阅者
        sub_cmd_vel_ = this->create_subscription<geometry_msgs::msg::Twist>(
                "cmd_vel", 10, std::bind(&BaseNode::cmd_vel_callback, this, std::placeholders::_1));
        sub_base_states_ = this->create_subscription<armquadrover_msgs::msg::DDSM115>(
                "base_states", 10, std::bind(&BaseNode::base_states_callback, this, std::placeholders::_1));
        sub_imu_ = this->create_subscription<sensor_msgs::msg::Imu>(
                "unilidar/imu", 10, std::bind(&BaseNode::imu_callback, this, std::placeholders::_1));
        sub_pose_ = this->create_subscription<geometry_msgs::msg::Pose2D>(
                "align_pose", 10, std::bind(&BaseNode::pose_callback, this, std::placeholders::_1));
        // 创建发布者
        pub_base_control_ = this->create_publisher<armquadrover_msgs::msg::DDSM115>("base_control", 10);
        pub_odom_ = this->create_publisher<nav_msgs::msg::Odometry>("odom", 10);
        pub_state_ = this->create_publisher<std_msgs::msg::Int32>("aqr_state", 10);
        tf_broadcaster_ = std::make_unique<tf2_ros::TransformBroadcaster>(this);

        // 定时发布示例（可选）
        timer_ = this->create_wall_timer(
                20ms, std::bind(&BaseNode::timer_callback, this));
    }

    // 回调函数和其他必要的函数...

private:
    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr sub_cmd_vel_;
    rclcpp::Subscription<armquadrover_msgs::msg::DDSM115>::SharedPtr sub_base_states_;
    rclcpp::Subscription<sensor_msgs::msg::Imu>::SharedPtr sub_imu_;
    rclcpp::Subscription<geometry_msgs::msg::Pose2D>::SharedPtr sub_pose_;
    rclcpp::Publisher<armquadrover_msgs::msg::DDSM115>::SharedPtr pub_base_control_;
    rclcpp::Publisher<nav_msgs::msg::Odometry>::SharedPtr pub_odom_;
    rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr pub_state_;
    rclcpp::TimerBase::SharedPtr timer_;
    std::unique_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;
    sensor_msgs::msg::Imu imu_msg;
    geometry_msgs::msg::Pose2D pose2D_msg;
    double r = 0.2038274598;//单位m
    double r_x=0.1304508399;
    double r_y=0.15661485159;
    double v_cx= 0;
    double v_cy= 0;
    double theta_c= 0;

    double vc_x= 0;
    double vc_y= 0;
    double thetac_= 0;
    double delta_x ,delta_y ,delta_th;

    double   th=0;
    double   x_=0;
    double   y_=0;
    double   vc=0;
    double average;
    long long c_time;
    long long  l_time=0;
    double d_time;
    double d_y,d_x;
    volatile  double roll_, pitch_, yaw_;
    ChassisState currentState;

    void cmd_vel_1(const geometry_msgs::msg::Twist::SharedPtr msg);
    void cmd_vel_2(const geometry_msgs::msg::Twist::SharedPtr msg);
    void  base_states_1(const armquadrover_msgs::msg::DDSM115::SharedPtr   msg);
    void  base_states_2(const armquadrover_msgs::msg::DDSM115::SharedPtr msg);
    void time_1(void);
    void time_2(void);
    void cmd_vel_callback(const geometry_msgs::msg::Twist::SharedPtr msg);
    void base_states_callback(const armquadrover_msgs::msg::DDSM115::SharedPtr msg);
    void imu_callback(const sensor_msgs::msg::Imu::SharedPtr msg);
    void pose_callback(const geometry_msgs::msg::Pose2D::SharedPtr msg);
    void  timer_callback();
    void setState(ChassisState newState) {
        currentState = newState;
    }

};

#endif //AQR_CONTROL_MYBASE_H
