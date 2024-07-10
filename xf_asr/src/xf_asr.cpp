#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "qisr.h"
#include "msp_cmn.h"
#include "msp_errors.h"
#include "speech_recognizer.h"
#include <iconv.h>
 
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/u_int32.hpp"
#include "std_msgs/msg/int8.hpp"
#include "armquadrover_msgs/msg/task.hpp"

#include "nlohmann/json.hpp"

using json = nlohmann::json;
const char* awakename = "/home/coolze/ts_ws1/src/wheeltec_mic/wheeltec_mic_ros2/feedback_voice/tts_sample.wav";
 
#define FRAME_LEN   640 
#define BUFFER_SIZE 4096
 
int wakeupFlag   = 0 ;
int resultFlag   = 0 ;
int feedbackFlag = 1;
bool stopFlag;

static void show_result(char *string, char is_over)
{
    resultFlag=1;   
    printf("\rResult: [ %s ]", string);
    if(is_over)
        putchar('\n');
}
 
static char *g_result = NULL;

static unsigned int g_buffersize = BUFFER_SIZE;
 
void on_result(const char *result, char is_last)
{
    if (result) 
    {
        size_t left = g_buffersize - 1 - strlen(g_result);
        size_t size = strlen(result);
        if (left < size) 
        {
            g_result = (char*)realloc(g_result, g_buffersize + BUFFER_SIZE);
            if (g_result)
                g_buffersize += BUFFER_SIZE;
            else 
            {
                printf("mem alloc failed\n");
                return;
            }
        }
        strncat(g_result, result, size);
        show_result(g_result, is_last);
    }
}
 

void on_speech_begin()
{
    if (g_result)
    {
        free(g_result);
    }
    g_result = (char*)malloc(BUFFER_SIZE);
    g_buffersize = BUFFER_SIZE;
    memset(g_result, 0, g_buffersize);
 
    printf("Start Listening...\n");
}
void on_speech_end(int reason)
{
    if (reason == END_REASON_VAD_DETECT)
        printf("\nSpeaking done \n");
    else
        printf("\nRecognizer error %d\n", reason);
}
 
/* demo recognize the audio from microphone */
static void demo_mic(const char* session_begin_params)
{
    int errcode;
    int i = 0;
 
    struct speech_rec iat;
 
    struct speech_rec_notifier recnotifier = {
        on_result,
        on_speech_begin,
        on_speech_end
    };
 
    errcode = sr_init(&iat, session_begin_params, SR_MIC, &recnotifier);
    if (errcode) {
        printf("speech recognizer init failed\n");
        return;
    }
    errcode = sr_start_listening(&iat);
    if (errcode) {
        printf("start listen failed %d\n", errcode);
    }
    /* demo 10 seconds recording */
    while(i++ < 15)
        sleep(1);
    errcode = sr_stop_listening(&iat);
    if (errcode) {
        printf("stop listening failed %d\n", errcode);
    }
 
    sr_uninit(&iat);
}

/* main thread: start/stop record ; query the result of recgonization.
 * record thread: record callback(data write)
 * helper thread: ui(keystroke detection)
 */
 

void WakeUp(const std_msgs::msg::Int8::SharedPtr awake_msg, const std_msgs::msg::UInt32::SharedPtr angle_msg)
{
    if (awake_msg->data == 1 && angle_msg->data != 0)
     {
        wakeupFlag = 1;
        printf("Both awake signal and angle are 1, setting wakeupFlag to 1.\n");
    }
}
void FeedbackCallback(const std_msgs::msg::Int8::SharedPtr msg)
{
    if (msg->data == 1) {
        feedbackFlag = 1;
        wakeupFlag=1;

        printf("Received feedback flag: %d\n", msg->data);
    }
}
void TaskCallback(const armquadrover_msgs::msg::Task::SharedPtr msg)
{
    std::cout << "Received task message with parameters: ";
    for (const auto& param : msg->task_parameters) {
        std::cout << param << " ";
    }
    if (!msg->task_parameters.empty())
    {
        stopFlag=1;
        /* code */
    }
    
    std::cout << std::endl;

    // 达到指定命令的时候语音识别停止
}



int main(int argc, char* argv[])
{
    // 初始化ROS
    rclcpp::init(argc, argv);
    auto node = std::make_shared<rclcpp::Node>("voiceRecognition");
    rclcpp::WallRate loop_rate(10.0);

    // 声明Publisher和Subscriber
    // 订阅唤醒语音识别的信号
    rclcpp::Subscription<std_msgs::msg::Int8>::SharedPtr awakeFlagSub = node->create_subscription<std_msgs::msg::Int8>(
        "online_awake_flag", 
        10, 
        [](const std_msgs::msg::Int8::SharedPtr msg) {
            // 处理接收到的唤醒信号
            // 例如，设置相应的标志位或触发语音识别
            if (msg->data == 1) {
                // 处理唤醒信号
                printf("Received awake flag: %d\n", msg->data);
                // 进行语音识别等操作
            }
        }
    );

    // 订阅唤醒角度
    rclcpp::Subscription<std_msgs::msg::UInt32>::SharedPtr angleSub = node->create_subscription<std_msgs::msg::UInt32>(
        "online_awake_angle", 
        10, 
        [](const std_msgs::msg::UInt32::SharedPtr msg) {
            // 处理接收到的唤醒角度
            // 例如，更新机器人的角度信息
            printf("Received awake angle: %d\n", msg->data);

            // 在接收到角度消息后，调用 WakeUp 函数处理唤醒信号和角度消息
            auto awake_msg = std_msgs::msg::Int8::SharedPtr(new std_msgs::msg::Int8());
            awake_msg->data = 1;
            WakeUp(awake_msg, msg);
        }
    );

    rclcpp::Subscription<std_msgs::msg::Int8>::SharedPtr feedbackSub = node->create_subscription<std_msgs::msg::Int8>(
        "voice_stop", 
        10, 
        FeedbackCallback
    );

    // 创建订阅者，订阅 task_topic 话题
    auto sub_task = node->create_subscription<armquadrover_msgs::msg::Task>(
        "task_topic", 10, TaskCallback
    );

    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr voiceWordsPub = node->create_publisher<std_msgs::msg::String>("voice_input", 1000);  
    RCLCPP_INFO(node->get_logger(), "Sleeping...");
    int count = 0;

    int ret = MSP_SUCCESS;
    const char* login_params = "appid = 9d131769, work_dir = .";

    const char* session_begin_params =
        "sub = iat, domain = iat, language = zh_cn, "
        "accent = mandarin, sample_rate = 16000, "
        "result_type = plain, result_encoding = utf8";

    ret = MSPLogin(NULL, NULL, login_params);
    if (MSP_SUCCESS != ret) {
        printf("MSPLogin failed , Error code %d.\n", ret);
        goto exit; // login fail, exit the program
    }

    while(rclcpp::ok())
    {
        // 语音识别唤醒     
        while(wakeupFlag && feedbackFlag)
        {
            printf("麦克风识别语音\n");
            printf("请说话，持续15秒\n");
            // 播放音频文件并等待其播放完毕
            std::string command = std::string("aplay ") + awakename;
            system(command.c_str());

            demo_mic(session_begin_params);

            printf("15秒已经过去\n");

            feedbackFlag = false;
            wakeupFlag = false;

            if (stopFlag == 1) {
                break;
            }
        }

        // 语音识别完成
        if (resultFlag) {
            resultFlag = 0;
            std_msgs::msg::String msg;
            msg.data = g_result;
            voiceWordsPub->publish(msg);
        }
        rclcpp::spin_some(node);
        loop_rate.sleep();
        count++;
    }
exit:
    MSPLogout(); // Logout...

    return 0;
}
