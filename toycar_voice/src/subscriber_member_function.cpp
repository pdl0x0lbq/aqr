#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/int8.hpp"

#include "toycar_voice/qtts.h"
#include "toycar_voice/msp_cmn.h"
#include "toycar_voice/msp_errors.h"
#include <cstdlib>

using std::placeholders::_1;

typedef int SR_DWORD;
typedef short int SR_WORD ;

/* wav音频头部格式 */
typedef struct _wave_pcm_hdr
{
	char            riff[4];                // = "RIFF"
	int				size_8;                 // = FileSize - 8
	char            wave[4];                // = "WAVE"
	char            fmt[4];                 // = "fmt "
	int				fmt_size;				// = 下一个结构体的大小 : 16

	short int       format_tag;             // = PCM : 1
	short int       channels;               // = 通道数 : 1
	int				samples_per_sec;        // = 采样率 : 8000 | 6000 | 11025 | 16000
	int				avg_bytes_per_sec;      // = 每秒字节数 : samples_per_sec * bits_per_sample / 8
	short int       block_align;            // = 每采样点字节数 : wBitsPerSample / 8
	short int       bits_per_sample;        // = 量化比特数: 8 | 16

	char            data[4];                // = "data";
	int				data_size;              // = 纯数据长度 : FileSize - 44 
} wave_pcm_hdr;

/* 默认wav音频头部数据 */
wave_pcm_hdr default_wav_hdr = 
{
	{ 'R', 'I', 'F', 'F' },
	0,
	{'W', 'A', 'V', 'E'},
	{'f', 'm', 't', ' '},
	16,
	1,
	1,
	16000,
	32000,
	2,
	16,
	{'d', 'a', 't', 'a'},
	0  
};
/* 文本合成 */
int text_to_speech(const char* src_text, const char* des_path, const char* params)
{
	int          ret          = -1;
	FILE*        fp           = NULL;
	const char*  sessionID    = NULL;
	unsigned int audio_len    = 0;
	wave_pcm_hdr wav_hdr      = default_wav_hdr;
	int          synth_status = MSP_TTS_FLAG_STILL_HAVE_DATA;

	if (NULL == src_text || NULL == des_path)
	{
		printf("params is error!\n");
		return ret;
	}
	fp = fopen(des_path, "wb");
	if (NULL == fp)
	{
		printf("open %s error.\n", des_path);
		return ret;
	}
	/* 开始合成 */
	sessionID = QTTSSessionBegin(params, &ret);
	if (MSP_SUCCESS != ret)
	{
		printf("QTTSSessionBegin failed, error code: %d.\n", ret);
		fclose(fp);
		return ret;
	}
	ret = QTTSTextPut(sessionID, src_text, (unsigned int)strlen(src_text), NULL);
	if (MSP_SUCCESS != ret)
	{
		printf("QTTSTextPut failed, error code: %d.\n",ret);
		QTTSSessionEnd(sessionID, "TextPutError");
		fclose(fp);
		return ret;
	}
	printf("正在合成 ...\n");
	fwrite(&wav_hdr, sizeof(wav_hdr) ,1, fp); //添加wav音频头，使用采样率为16000
	while (1) 
	{
		/* 获取合成音频 */
		const void* data = QTTSAudioGet(sessionID, &audio_len, &synth_status, &ret);
		if (MSP_SUCCESS != ret)
			break;
		if (NULL != data)
		{
			fwrite(data, audio_len, 1, fp);
		    wav_hdr.data_size += audio_len; //计算data_size大小
		}
		if (MSP_TTS_FLAG_DATA_END == synth_status)
			break;
	}
	printf("\n");
	if (MSP_SUCCESS != ret)
	{
		printf("QTTSAudioGet failed, error code: %d.\n",ret);
		QTTSSessionEnd(sessionID, "AudioGetError");
		fclose(fp);
		return ret;
	}
	/* 修正wav文件头数据的大小 */
	wav_hdr.size_8 += wav_hdr.data_size + (sizeof(wav_hdr) - 8);
	
	/* 将修正过的数据写回文件头部,音频文件为wav格式 */
	fseek(fp, 4, 0);
	fwrite(&wav_hdr.size_8,sizeof(wav_hdr.size_8), 1, fp); //写入size_8的值
	fseek(fp, 40, 0); //将文件指针偏移到存储data_size值的位置
	fwrite(&wav_hdr.data_size,sizeof(wav_hdr.data_size), 1, fp); //写入data_size的值
	fclose(fp);
	fp = NULL;
	/* 合成完毕 */
	ret = QTTSSessionEnd(sessionID, "Normal");
	if (MSP_SUCCESS != ret)
	{
		printf("QTTSSessionEnd failed, error code: %d.\n",ret);
	}

	return ret;
}

class MinimalSubscriber : public rclcpp::Node
{
  public:
    MinimalSubscriber()
    : Node("minimal_subscriber")
    {
      //实例化时创建subscription_ ，主题为“voice_input”，并绑定接收到消息后的回调函数topic_callback
      subscription_ = this->create_subscription<std_msgs::msg::String>(
      "llm_output", 10, std::bind(&MinimalSubscriber::topic_callback, this, _1));

	  // 创建对语音识别的停顿，要先播放完再进行接收语音识别发布器
      publisher_ = this->create_publisher<std_msgs::msg::Int8>("voice_stop", 10);
    }

  private:
    void topic_callback(const std_msgs::msg::String::SharedPtr msg) const
    {
        RCLCPP_INFO(this->get_logger(), "I heard: '%s'", msg->data.c_str());
        const char* text;
        int ret = MSP_SUCCESS;
        const char* session_begin_params = "voice_name = xiaoyan, text_encoding = utf8, sample_rate = 16000, speed = 50, volume = 50, pitch = 50, rdn = 2";
        const char* filename = "/home/coolze/ts_ws1/src/wheeltec_mic/wheeltec_mic_ros2/feedback_voice/tts_sample.wav"; //合成的语音文件名      
        text = msg->data.c_str(); /* 文本合成，将发送来的消息读取其中的文本 */
        printf("开始合成 ...\n");
        //调用静态方法进行转换
        ret = text_to_speech(text, filename, session_begin_params);
        if (MSP_SUCCESS != ret)
        {
            printf("text_to_speech failed, error code: %d.\n", ret);
        }
        else
        {
            printf("合成完毕\n");
            sleep(1);
            
            // 调用系统的 aplay 命令播放合成的语音文件
            std::string play_command = "aplay " + std::string(filename);
            int play_result = std::system(play_command.c_str());

			 // 播放成功后发布消息给语音识别，好让播放完成之后才再进行语音识别
                auto feedback_msg = std_msgs::msg::Int8();
                feedback_msg.data = 1;
                publisher_->publish(feedback_msg);
                RCLCPP_INFO(this->get_logger(), "Published feedback message: 1");


            if (play_result != 0) {
                printf("播放合成的语音失败。\n");
            }
        }
    }
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
	rclcpp::Publisher<std_msgs::msg::Int8>::SharedPtr publisher_;


};

int main(int argc, char * argv[])
{


  int         ret                  = MSP_SUCCESS;
  const char* login_params         = "appid = 9d131769, work_dir = .";//登录参数,appid与msc库绑定,请勿随意改动
  /* 用户登录 */
  ret = MSPLogin(NULL, NULL, login_params); //第一个参数是用户名，第二个参数是密码，第三个参数是登录参数，用户名和密码可在http://www.xfyun.cn注册获取
  if (MSP_SUCCESS != ret)
  {
    printf("MSPLogin failed, error code: %d.\n", ret);
		//登录失败，退出登录
 		printf("按任意键退出 ...\n");
		getchar();
		MSPLogout(); //退出登录
	}
	printf("语音模块初始化成功，请说话... ...\n");
	rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MinimalSubscriber>());
  rclcpp::shutdown();

	printf("按任意键退出 ...\n");
	getchar();
	MSPLogout(); //退出登录
  return 0;
}
