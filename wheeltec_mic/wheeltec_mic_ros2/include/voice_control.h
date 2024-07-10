#ifndef __VOICE_CONTROL_H_
#define __VOICE_CONTROL_H_

#include <cstdio>
#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include <record_param.h>
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>
#include <std_msgs/msg/int8.hpp>
#include "user_interface.h"
#include "wheeltec_mic_msg/srv/get_offline_result.hpp"
using namespace std;

extern UserData asr_data;
extern int whether_finised;
extern char *whole_result;
unsigned char* record_data;

class  SpeechProcess : public rclcpp::Node{
public:
	SpeechProcess(const std::string &node_name);
	~SpeechProcess();
	void run();

private:
	int confidence;
	float time_per_order;

	rclcpp::Time start_time,last_time;
	rclcpp::Publisher<std_msgs::msg::String>::SharedPtr voice_words_pub;
	rclcpp::Publisher<std_msgs::msg::String>::SharedPtr voice_input_pub;//新添加
	rclcpp::Service<wheeltec_mic_msg::srv::GetOfflineResult>::SharedPtr get_offline_result_srv_;

	std::string s2s(const std::string &str);
	Effective_Result show_result(char* str);

	int filesize(const char *fname);
	int finish_record_sound();
	int init_asr_params();
	int get_record_sound(const char *file);
	int business_data_t(unsigned char* record);
	int record_params_init(record_handle_t* pcm_handle,record_params_t* params);
	static snd_pcm_format_t get_formattype_from_params(record_params_t* params);
	
	bool Get_Offline_Recognise_Result(const std::shared_ptr<wheeltec_mic_msg::srv::GetOfflineResult::Request>& request,
										std::shared_ptr<wheeltec_mic_msg::srv::GetOfflineResult::Response>& response);
};

#endif