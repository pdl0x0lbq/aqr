/****************************************************************/
/* Copyright (c) 2023 WHEELTEC Technology, Inc   				*/
/* function:Functional node feedback							*/
/* 功能：功能节点反馈												*/
/****************************************************************/
#include "node_feedback.h"

/**************************************************************************
函数功能：自主建图开启成功标志位sub回调函数
入口参数：laser_follow_flag.msg  
返回  值：无
**************************************************************************/
void Feedback::rrt_Callback(const std_msgs::msg::Int8::SharedPtr msg){
	rrt_flag = msg->data;
	if (rrt_flag){
		WHOLE = head + audio_path + "/rrt_open.wav";
		system(WHOLE.c_str());
		cout<<"自主建图打开成功"<<endl;
	}
}
/**************************************************************************
函数功能：雷达跟随开启成功标志位sub回调函数
入口参数：laser_follow_flag.msg  
返回  值：无
**************************************************************************/
void Feedback::laser_follow_Callback(const std_msgs::msg::Int8::SharedPtr msg){
	laser_follow_flag = msg->data;
	if (laser_follow_flag){
		WHOLE = head + audio_path + "/rplidar_open.wav";
		system(WHOLE.c_str());
		cout<<"雷达跟随打开成功"<<endl;
	}
}

/**************************************************************************
函数功能：色块跟随开启成功标志位sub回调函数
入口参数：visual_follow_flag.msg  
返回  值：无
**************************************************************************/
void Feedback::visual_follow_Callback(const std_msgs::msg::Int8::SharedPtr msg){
	visual_follow_flag = msg->data;
	if (visual_follow_flag)
	{
		WHOLE = head + audio_path + "/visual_open.wav";
		system(WHOLE.c_str());
		cout<<"色块跟随打开成功"<<endl;
	}
}

void Feedback::voice_words_Callback(const std_msgs::msg::String::SharedPtr msg){
	string str1 = msg->data;    //取传入数据
	string str2 = "关闭雷达跟随";
	string str3 = "关闭色块跟随";
	string str4 = "关闭自主建图";
	string str5 = "关闭导航";

	if (str1 == str2){
		WHOLE = head + audio_path + "/rplidar_close.wav";
		system(WHOLE.c_str());
		cout<<"已关闭雷达跟随"<<endl;
	}
	else if (str1 == str3){
		WHOLE = head + audio_path + "/visual_close.wav";
		system(WHOLE.c_str());
		cout<<"已关闭色块跟随"<<endl;
	}
	else if (str1 == str4){
		WHOLE = head + audio_path + "/rrt_close.wav";
		system(WHOLE.c_str());
		cout<<"已关闭自主建图"<<endl;
	}
	else if (str1 == str5){
		WHOLE = head + audio_path + "/OK.wav";
		system(WHOLE.c_str());
		cout<<"已关闭导航"<<endl;
	}
}

Feedback::Feedback(const std::string &node_name)
: rclcpp::Node(node_name){
	RCLCPP_INFO(this->get_logger(),"%s node init!\n",node_name.c_str());
	/***声明参数并获取***/
	this->declare_parameter<string>("audio_path","");
	this->get_parameter("audio_path",audio_path);
	/***自主建图话题订阅者创建***/
	rrt_sub = this->create_subscription<std_msgs::msg::Int8>(
		"rrt_flag",10,std::bind(&Feedback::rrt_Callback,this,_1));
	/***雷达跟随话题订阅者创建***/
	laser_follow_sub = this->create_subscription<std_msgs::msg::Int8>(
		"laser_follow_flag",10,std::bind(&Feedback::laser_follow_Callback,this,_1));
	/***视觉跟随话题订阅者创建***/
	visual_follow_sub = this->create_subscription<std_msgs::msg::Int8>(
		"visual_follow_flag",10,std::bind(&Feedback::visual_follow_Callback,this,_1));
	/***离线命令词识别结果话题订阅者创建***/
	voice_words_sub = this->create_subscription<std_msgs::msg::String>(
		"voice_words",10,std::bind(&Feedback::voice_words_Callback,this,_1));
}

Feedback::~Feedback(){
	RCLCPP_INFO(this->get_logger(),"node_feedback over!\n");
}

void Feedback::run(){
	while(rclcpp::ok()){
		rclcpp::spin_some(this->get_node_base_interface());
	}
}

int main(int argc, char const *argv[])
{
	rclcpp::init(argc,argv);
	Feedback feedback("node_feedback");
	feedback.run();
	return 0;
}