#include <memory>
#include <rclcpp/rclcpp.hpp>
#include <moveit/move_group_interface/move_group_interface.h>
#include <std_msgs/msg/bool.hpp>
#include <std_msgs/msg/int32.hpp>
#include "geometry_msgs/msg/pose2_d.hpp"
class waterMoveitNode : public rclcpp::Node
{
public:
    waterMoveitNode() : Node("water_moveit", rclcpp::NodeOptions().automatically_declare_parameters_from_overrides(true))
    {
        moveit_timer_ = this->create_wall_timer(std::chrono::milliseconds(500), std::bind(&waterMoveitNode::moveitCallback, this));

        moveit_mutex_ = std::make_shared<std::mutex>();

        success_left_publisher_ = this->create_publisher<std_msgs::msg::Bool>("/moveit_success_left", 10);
        success_right_publisher_ = this->create_publisher<std_msgs::msg::Bool>("/moveit_success_right", 10);
        stepping_control_publisher_ = this->create_publisher<std_msgs::msg::Int32>("stepping_control", 10);
        align_pose_publisher_ = this->create_publisher<geometry_msgs::msg::Pose2D>("align_pose", 10);
    }

public:
    void moveitCallback()
    {   
        std::lock_guard<std::mutex> lock(*moveit_mutex_);
        auto self = this->shared_from_this();
        move_group_interface_left_ = std::make_shared<moveit::planning_interface::MoveGroupInterface>(self, "leftarm");
        move_group_interface_right_ = std::make_shared<moveit::planning_interface::MoveGroupInterface>(self, "rightarm");
        move_group_interface_both_ = std::make_shared<moveit::planning_interface::MoveGroupInterface>(self, "botharm");
        moveit::planning_interface::MoveGroupInterface::Plan plan_left;
        moveit::planning_interface::MoveGroupInterface::Plan plan_right;
        moveit::planning_interface::MoveGroupInterface::Plan plan_both;
        std_msgs::msg::Bool success_msg_left;
        std_msgs::msg::Bool success_msg_right;
        success_msg_left.data = true;
        success_msg_right.data = true;
        std_msgs::msg::Int32 stepping_control_msg;
        stepping_control_msg.data = 290;
        stepping_control_publisher_->publish(stepping_control_msg);
        success_left_publisher_->publish(success_msg_left);
        success_right_publisher_->publish(success_msg_right);
        rclcpp::sleep_for(std::chrono::seconds(5));
        std::vector<double> joint_group_positions_left3 = {M_PI / 15, M_PI / 6.0, 0.0, 0.0, 0.0};
        move_group_interface_left_->setJointValueTarget(joint_group_positions_left3);
        move_group_interface_left_->plan(plan_left);
        std::vector<double> joint_group_positions_right3 = {-M_PI / 12, M_PI / 2.4, -M_PI / 18, 0, 0 };
        move_group_interface_right_->setJointValueTarget(joint_group_positions_right3);
        move_group_interface_right_->plan(plan_right);
        std::vector<double> joint_values_both3;
        joint_values_both3.insert(joint_values_both3.end(), joint_group_positions_left3.begin(), joint_group_positions_left3.end());
        joint_values_both3.insert(joint_values_both3.end(), joint_group_positions_right3.begin(), joint_group_positions_right3.end());
        move_group_interface_both_->setJointValueTarget(joint_values_both3);
        move_group_interface_both_->plan(plan_both);
        move_group_interface_both_->execute(plan_both);
        rclcpp::sleep_for(std::chrono::seconds(5));
        std::vector<double> joint_group_positions_left = {M_PI / 15, M_PI / 2.1, 0.0, 0.0, 0.0};
        move_group_interface_left_->setJointValueTarget(joint_group_positions_left);
        bool success_left = move_group_interface_left_->plan(plan_left) == moveit::planning_interface::MoveItErrorCode::SUCCESS;
        std::vector<double> joint_group_positions_right = {-M_PI / 12, M_PI / 2.4, -M_PI / 18, 0, 0};
        move_group_interface_right_->setJointValueTarget(joint_group_positions_right);
        bool success_right = move_group_interface_right_->plan(plan_right) == moveit::planning_interface::MoveItErrorCode::SUCCESS;
        std::vector<double> joint_values_both;
        joint_values_both.insert(joint_values_both.end(), joint_group_positions_left.begin(), joint_group_positions_left.end());
        joint_values_both.insert(joint_values_both.end(), joint_group_positions_right.begin(), joint_group_positions_right.end());
        move_group_interface_both_->setJointValueTarget(joint_values_both);
        bool success_both = move_group_interface_both_->plan(plan_both) == moveit::planning_interface::MoveItErrorCode::SUCCESS;
        RCLCPP_INFO(get_logger(), "Planning successful for both robots");
        move_group_interface_both_->execute(plan_both);
        geometry_msgs::msg::Pose2D pose_msg;
        pose_msg.x = 0.03;
        pose_msg.y = 0.0;
        align_pose_publisher_->publish(pose_msg);
        rclcpp::sleep_for(std::chrono::seconds(1));
        geometry_msgs::msg::Pose2D pose_msg1;
        pose_msg1.x = -0.03;
        pose_msg1.y = 0.0;
        align_pose_publisher_->publish(pose_msg1);
        rclcpp::sleep_for(std::chrono::seconds(10));
        std::vector<double> joint_group_positions_left2 = {0.0,  0.0,   0.0,  0.0,  0.0  };
        move_group_interface_left_->setJointValueTarget(joint_group_positions_left2);
        move_group_interface_left_->plan(plan_left);
        std::vector<double> joint_group_positions_right2 = {-M_PI / 12, M_PI / 2.4, -M_PI / 18, 0, 0 };
        move_group_interface_right_->setJointValueTarget(joint_group_positions_right2);
        move_group_interface_right_->plan(plan_right);
        std::vector<double> joint_values_both2;
        joint_values_both2.insert(joint_values_both2.end(), joint_group_positions_left2.begin(), joint_group_positions_left2.end());
        joint_values_both2.insert(joint_values_both2.end(), joint_group_positions_right2.begin(), joint_group_positions_right2.end());
        move_group_interface_both_->setJointValueTarget(joint_values_both2);
        move_group_interface_both_->plan(plan_both);
        move_group_interface_both_->execute(plan_both);
        rclcpp::sleep_for(std::chrono::seconds(3));
        std_msgs::msg::Int32 stepping_control_msg1;
        stepping_control_msg1.data = 200;
        stepping_control_publisher_->publish(stepping_control_msg1);
        rclcpp::sleep_for(std::chrono::seconds(10));
        rclcpp::shutdown();
    }

    std::shared_ptr<moveit::planning_interface::MoveGroupInterface> move_group_interface_left_;
    std::shared_ptr<moveit::planning_interface::MoveGroupInterface> move_group_interface_right_;
    std::shared_ptr<moveit::planning_interface::MoveGroupInterface> move_group_interface_both_;
    rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr success_left_publisher_;
    rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr success_right_publisher_;
    rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr stepping_control_publisher_;
    rclcpp::Publisher<geometry_msgs::msg::Pose2D>::SharedPtr align_pose_publisher_;
    rclcpp::TimerBase::SharedPtr moveit_timer_;
    std::shared_ptr<std::mutex> moveit_mutex_;
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<waterMoveitNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
