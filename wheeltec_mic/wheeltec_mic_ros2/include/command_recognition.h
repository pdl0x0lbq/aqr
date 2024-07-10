#ifndef __CALL_COMMAND_RECOGNITION_H_
#define __CALL_COMMAND_RECOGNITION_H_

#include <iostream>
#include <unistd.h>
#include <play_path.h>
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>
#include <std_msgs/msg/int8.hpp>
#include <std_msgs/msg/int32.hpp>
#include "geometry_msgs/msg/twist.hpp"

using namespace std;
class Command : public rclcpp::Node{
public:
	Command(const std::string &node_name);
	~Command();
	void run();

private:
	int voice_flag = 0;

	rclcpp::Publisher<std_msgs::msg::Int8>::SharedPtr rrt_flag_pub;
	rclcpp::Publisher<std_msgs::msg::Int8>::SharedPtr awake_flag_pub;
	rclcpp::Publisher<std_msgs::msg::Int8>::SharedPtr laser_follow_flag_pub;
	rclcpp::Publisher<std_msgs::msg::Int8>::SharedPtr visual_follow_flag_pub;
    rclcpp::Subscription<std_msgs::msg::Int8>::SharedPtr voice_flag_sub;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr voice_words_sub;
	rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_cmd_vel_;

    void voice_flag_Callback(const std_msgs::msg::Int8::SharedPtr msg);
    void voice_words_Callback(const std_msgs::msg::String::SharedPtr msg);
	
};

#endif
