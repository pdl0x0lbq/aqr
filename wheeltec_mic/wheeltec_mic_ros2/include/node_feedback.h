#ifndef __FEEDBACK_RECOGNITION_H_
#define __FEEDBACK_RECOGNITION_H_

#include <iostream>
#include <unistd.h>
#include <play_path.h>
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>
#include <std_msgs/msg/int8.hpp>
#include <std_msgs/msg/int32.hpp>
using namespace std;
using std::placeholders::_1;

class Feedback : public rclcpp::Node{
public:
	Feedback(const std::string &node_name);
	~Feedback();
	void run();

private:
    int rrt_flag = 0;               //自主建图标志位
    int laser_follow_flag = 0;      //雷达跟随标志位
    int visual_follow_flag = 0;     //色块跟随标志位

    rclcpp::Subscription<std_msgs::msg::Int8>::SharedPtr rrt_sub;
    rclcpp::Subscription<std_msgs::msg::Int8>::SharedPtr laser_follow_sub;
    rclcpp::Subscription<std_msgs::msg::Int8>::SharedPtr visual_follow_sub;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr voice_words_sub;

    void rrt_Callback(const std_msgs::msg::Int8::SharedPtr msg);
    void laser_follow_Callback(const std_msgs::msg::Int8::SharedPtr msg);
    void visual_follow_Callback(const std_msgs::msg::Int8::SharedPtr msg);
    void voice_words_Callback(const std_msgs::msg::String::SharedPtr msg);	
};

#endif