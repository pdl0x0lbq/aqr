/************************************************************************************************/
/* Copyright (c) 2023 WHEELTEC Technology, Inc   												*/
/* function:Command controller, command word recognition results into the corresponding action	*/
/* 功能：命令控制器，命令词识别结果转化为对应的执行动作													*/
/************************************************************************************************/
#include "command_recognition.h"
using namespace std;
using std::placeholders::_1;

/**************************************************************************
函数功能：寻找语音开启成功标志位sub回调函数
入口参数：voice_flag_msg  voice_control.cpp
返回  值：无
**************************************************************************/
void Command::voice_flag_Callback(std_msgs::msg::Int8::SharedPtr msg){
	voice_flag = msg->data;
	WHOLE = head + audio_path + "/voice_control.wav";
	if (voice_flag){
		system(WHOLE.c_str());
		cout<<"语音打开成功"<<endl;
	}
}

/**************************************************************************
函数功能：离线命令词识别结果sub回调函数
入口参数：命令词字符串
返回  值：无
**************************************************************************/
void Command::voice_words_Callback(std_msgs::msg::String::SharedPtr msg){
	/***语音指令***/
	string str1 = msg->data;    //取传入数据
	string str2 = "小车前进";
	string str3 = "小车后退"; 
	string str4 = "小车左转";
	string str5 = "小车右转";
	string str6 = "小车停";
	string str7 = "小车休眠";
	string str8 = "小车过来";
	string str9 = "小车去I点";
	string str10 = "小车去J点";
	string str11 = "小车去K点";
	string str12 = "失败5次";
	string str13 = "失败10次";
	string str14 = "遇到障碍物";
	string str15 = "小车唤醒";
	string str16 = "小车雷达跟随";
	string str17 = "小车色块跟随";
	string str18 = "关闭雷达跟随";
	string str19 = "关闭色块跟随";
	string str20 = "打开自主建图";
	string str21 = "关闭自主建图";
	string str22 = "开始导航";

	if (str1 == str2){
		WHOLE = head + audio_path + "/car_front.wav";
		system(WHOLE.c_str());
		cout<<"好的：小车前进"<<endl;
		    // 创建并填充Twist消息
    auto twist_msg = std::make_shared<geometry_msgs::msg::Twist>();
    twist_msg->linear.x = 1.0;
	twist_msg->linear.y = 0.0;  // 设置线速度为1
    twist_msg->angular.z = 0.0; // 设置角速度为0
    
    // 发布Twist消息到cmd_vel主题
    publisher_cmd_vel_->publish(*twist_msg);

	}
	else if (str1 == str3){
		WHOLE = head + audio_path + "/car_back.wav";
		system(WHOLE.c_str());
		cout<<"好的：小车后退"<<endl;

		geometry_msgs::msg::Twist cmd_vel;
		cmd_vel.linear.x = -1.0;
		cmd_vel.linear.y = 0.0;
		cmd_vel.angular.z = 0.0;

		publisher_cmd_vel_->publish(cmd_vel);
	}
	else if (str1 == str4){
		WHOLE = head + audio_path + "/turn_left.wav";
		system(WHOLE.c_str());
		cout<<"好的：小车左转"<<endl;
		geometry_msgs::msg::Twist cmd_vel;
		cmd_vel.linear.x = 0.5;
		cmd_vel.linear.y = 0.0;
		cmd_vel.angular.z = 1.0;

		publisher_cmd_vel_->publish(cmd_vel);
	}
	else if (str1 == str5){
		WHOLE = head + audio_path + "/turn_right.wav";
		system(WHOLE.c_str());
		cout<<"好的：小车右转"<<endl;
		geometry_msgs::msg::Twist cmd_vel;
		cmd_vel.linear.x = 0.5;
		cmd_vel.linear.y = 0.0;
		cmd_vel.angular.z = -1.0;

		publisher_cmd_vel_->publish(cmd_vel);
	}
	else if (str1 == str6){
		WHOLE = head + audio_path + "/stop.wav";
		system(WHOLE.c_str());
		cout<<"好的：小车停"<<endl;
		geometry_msgs::msg::Twist cmd_vel;
		cmd_vel.linear.x = 0.0;
		cmd_vel.linear.y = 0.0;
		cmd_vel.angular.z = 0.0;
	}
	else if (str1 == str7){
		std_msgs::msg::Int8 awake_flag_msg;
		awake_flag_msg.data = 0;
		awake_flag_pub->publish(awake_flag_msg);

		WHOLE = head + audio_path + "/sleep.wav";
		system(WHOLE.c_str());
		cout<<"小车休眠，等待下一次唤醒"<<endl;
	}
	else if (str1 == str8){
		WHOLE = head + audio_path + "/search_voice.wav";
		system(WHOLE.c_str());
		cout<<"好的：小车寻找声源"<<endl;
	}
	else if (str1 == str9){
       	WHOLE = head + audio_path + "/OK.wav";
		system(WHOLE.c_str());
		cout<<"好的：小车自主导航至I点"<<endl;
	}
	else if (str1 == str10){
		WHOLE = head + audio_path + "/OK.wav";
		system(WHOLE.c_str());
		cout<<"好的：小车自主导航至J点"<<endl;
	}
	else if (str1 == str11){
        WHOLE = head + audio_path + "/OK.wav";
		system(WHOLE.c_str());
		cout<<"好的：小车自主导航至K点"<<endl;
	}
	else if (str1 == str12){
		cout<<"您已经连续【输入空指令or识别失败】5次，累计达15次自动进入休眠，输入有效指令后计数清零"<<endl;
	}
	else if (str1 == str13){
		cout<<"您已经连续【输入空指令or识别失败】10次，累计达15次自动进入休眠，输入有效指令后计数清零"<<endl;
	}
	else if (str1 == str14){
		WHOLE = head + audio_path + "/Tracker.wav";
		system(WHOLE.c_str());
		cout<<"小车遇到障碍物，已停止运动"<<endl;
	}
	else if (str1 == str15){
		WHOLE = head + audio_path + "/awake.wav";
		system(WHOLE.c_str());
		cout<<"小车已被唤醒，请说语音指令"<<endl;
	}
	else if (str1 == str16){
		WHOLE = head + audio_path + "/OK.wav";
		system(WHOLE.c_str());

		std_msgs::msg::Int8 laser_follow_flag_msg;
		laser_follow_flag_msg.data = 0;
		laser_follow_flag_pub->publish(laser_follow_flag_msg);
		cout<<"好的：小车雷达跟随"<<endl;
	}
	else if (str1 == str17){
		WHOLE = head + audio_path + "/OK.wav";
		system(WHOLE.c_str());

		std_msgs::msg::Int8 visual_follow_flag_msg;
		visual_follow_flag_msg.data = 0;
		visual_follow_flag_pub->publish(visual_follow_flag_msg);
		cout<<"好的：小车色块跟随"<<endl;
	}
	else if (str1 == str18){
		WHOLE = head + audio_path + "/OK.wav";
		system(WHOLE.c_str());
		cout<<"好的：关闭雷达跟随"<<endl;
	}
	else if (str1 == str19){
		WHOLE = head + audio_path + "/OK.wav";
		system(WHOLE.c_str());
		cout<<"好的：关闭色块跟随"<<endl;
	}
	else if (str1 == str20){
		WHOLE = head + audio_path + "/OK.wav";
		system(WHOLE.c_str());

		std_msgs::msg::Int8 rrt_flag_msg;
		rrt_flag_msg.data = 0;
		rrt_flag_pub->publish(rrt_flag_msg);
		cout<<"好的：打开自主建图"<<endl;
	}
	else if (str1 == str21){
		WHOLE = head + audio_path + "/OK.wav";
		system(WHOLE.c_str());
		cout<<"好的：关闭自主建图"<<endl;
	}
	else if (str1 == str22){
		WHOLE = head + audio_path + "/OK.wav";
		system(WHOLE.c_str());
		cout<<"好的：小车开始导航"<<endl;
	}
	else {
    WHOLE = head + audio_path + "/tts_sample.wav";
    system(WHOLE.c_str());
    cout << "播放新的语音文件" << endl;
    }
}

Command::Command(const std::string &node_name)
: rclcpp::Node(node_name){
	RCLCPP_INFO(this->get_logger(),"%s node init!\n",node_name.c_str());
	/***声明参数并获取***/
	this->declare_parameter<string>("audio_path","");
	this->get_parameter("audio_path",audio_path);
	/***唤醒标志位话题发布者创建***/
	awake_flag_pub = this->create_publisher<std_msgs::msg::Int8>("awake_flag",10); 
	/***自主建图标志位话题发布者创建***/
	rrt_flag_pub = this->create_publisher<std_msgs::msg::Int8>("rrt_flag",10); 
	/***雷达跟随标志位话题发布者创建***/
	laser_follow_flag_pub = this->create_publisher<std_msgs::msg::Int8>("laser_follow_flag",10); 
	/***色块跟随标志位话题发布者创建***/
	visual_follow_flag_pub = this->create_publisher<std_msgs::msg::Int8>("visual_follow_flag",10); 	
	  // 创建 cmd_vel 主题的发布者
    publisher_cmd_vel_ = this->create_publisher<geometry_msgs::msg::Twist>("cmd_vel", 10);
	/***离线命令词识别结果话题订阅者创建***/
	voice_words_sub = this->create_subscription<std_msgs::msg::String>(
		"voice_words",10,std::bind(&Command::voice_words_Callback,this,_1));
	/***寻找语音开启标志位话题订阅者创建***/
	voice_flag_sub = this->create_subscription<std_msgs::msg::Int8>(
		"voice_flag",10,std::bind(&Command::voice_flag_Callback,this,_1));

	sleep(2);
	cout<<"您可以语音控制啦!"<<endl;
	cout<<"小车前进———————————>向前"<<endl;
	cout<<"小车后退———————————>后退"<<endl;
	cout<<"小车左转———————————>左转"<<endl;
	cout<<"小车右转———————————>右转"<<endl;
	cout<<"小车停———————————>停止"<<endl;
	cout<<"小车休眠———————————>休眠，等待下一次唤醒"<<endl;
	cout<<"小车过来———————————>寻找声源"<<endl;
	cout<<"小车去I点———————————>小车自主导航至I点"<<endl;
	cout<<"小车去J点———————————>小车自主导航至J点"<<endl;
	cout<<"小车去K点———————————>小车自主导航至K点"<<endl;
	cout<<"小车雷达跟随———————————>小车打开雷达跟随"<<endl;
	cout<<"关闭雷达跟随———————————>小车关闭雷达跟随"<<endl;
	cout<<"小车色块跟随———————————>小车打开色块跟随"<<endl;
	cout<<"关闭色块跟随———————————>小车关闭色块跟随"<<endl;
	cout<<"打开自主建图———————————>小车打开自主建图"<<endl;
	cout<<"关闭自主建图———————————>关闭打开自主建图"<<endl;
	cout<<"开始导航———————————>小车开始导航"<<endl;
	cout<<"关闭导航———————————>小车关闭导航"<<endl;
}

void Command::run(){
	while(rclcpp::ok()){
		rclcpp::spin_some(this->get_node_base_interface());
		}
}

Command::~Command(){
	RCLCPP_INFO(this->get_logger(),"command_recognition_node over!\n");
}

int main(int argc, char *argv[])
{
	rclcpp::init(argc,argv);
	Command command("command_recognition");
	command.run();
	rclcpp::shutdown();
	return 0;
}