#include <iostream>
#include <string>
#include <atomic>
#include <unistd.h>
#include <regex>
#include "sparkchain.h"
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "MySQLConnector.h"
#include "armquadrover_msgs/msg/ddsm115.hpp"
#include "armquadrover_msgs/msg/task.hpp"
#include "action_encoding.h" // 引入编码头文件



#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define RED "\033[31m"
#define RESET "\033[0m"

using namespace SparkChain;
using namespace std;

#include "nlohmann/json.hpp"
using json = nlohmann::json;

std::unordered_map<std::string, std::string> extractKeywords(const std::string& jsonDatabase);

void checkKeywordCount(const std::unordered_map<std::string, std::string>& extractedKeywords,
                       const std::shared_ptr<rclcpp::Publisher<std_msgs::msg::String>>& pub_res,
                       const std::shared_ptr<rclcpp::Publisher<armquadrover_msgs::msg::Task>>& pub_task);
void CheckKeywordCount(const std::unordered_map<std::string, std::string>& extractedKeywords,
                       const std::shared_ptr<rclcpp::Publisher<std_msgs::msg::String>>& pub_res,
                       const std::shared_ptr<rclcpp::Publisher<armquadrover_msgs::msg::Task>>& pub_task);
std::unordered_map<std::string, std::string> mergeKeywords(
    const std::unordered_map<std::string, std::string>& baseKeywords,
    const std::unordered_map<std::string, std::string>& additionalKeywords);

std::unordered_map<std::string, std::string> oldKeywords;    


// async status tag
static atomic_bool finish(false);
// result cache
string final_result = "";

class SparkCallbacks : public LLMCallbacks
{
public:
    void onLLMResult(LLMResult *result, void *usrContext) override
    {
        int status = result->getStatus();
        printf(GREEN "%d:%s:%s:%s \n" RESET, status, result->getRole(), result->getContent(), (char *)usrContext);
        final_result += string(result->getContent());
        if (status == 2)
        {
            printf(GREEN "tokens:%d + %d = %d\n" RESET, result->getCompletionTokens(), result->getPromptTokens(), result->getTotalTokens());
            finish = true;
        }
    }

    void onLLMEvent(LLMEvent *event, void *usrContext) override
    {
        printf(YELLOW "onLLMEventCB\n  eventID:%d eventMsg:%s\n" RESET, event->getEventID(), event->getEventMsg());
    }

    void onLLMError(LLMError *error, void *usrContext) override
    {
        printf(RED "onLLMErrorCB\n errCode:%d errMsg:%s \n" RESET, error->getErrCode(), error->getErrMsg());
        finish = true;
    }
};





void uninitSDK()
{
    // 全局逆初始化
    SparkChain::unInit();
}
int initSDK()
{
    // 全局初始化
    SparkChainConfig *config = SparkChainConfig::builder();
    config->appID("")        // 你的appid
        ->apiKey("02d59236cc05060b6b209a2166c72508")        // 你的apikey
        ->apiSecret("NTFlM2NmYzk2YWU4OWFkYTk1MDU2YjY3"); // 你的apisecret
    // ->logLevel(0)
    // ->logPath("./aikit.log");
    int ret = SparkChain::init(config);
    printf(RED "\ninit SparkChain result:%d" RESET, ret);
    std::cout << std::endl;
    return ret;
}
void syncLLMTest(char *input , const std::shared_ptr<rclcpp::Publisher<std_msgs::msg::String>> &pub_res, const std::shared_ptr<rclcpp::Publisher<armquadrover_msgs::msg::Task>> &pub_task)
{
    cout << "\n######### 同步调用 #########" << endl;
    // 配置大模型参数
    LLMConfig *llmConfig = LLMConfig::builder();
    llmConfig->domain("patchv3");
    llmConfig->url("ws(s)://spark-api-n.xf-yun.com/v3.1/chat");
    Memory *window_memory = Memory::WindowMemory(5);
    LLM *syncllm = LLM::create(llmConfig, window_memory);

    // Memory* token_memory = Memory::TokenMemory(500);
    // LLM *syncllm = LLM::create(llmConfig,token_memory);

    // char* input = "你好用英语怎么说？";
    if (strcmp(input, "q") == 0)
    {
        uninitSDK();
        // break;
    }
    // 同步请求
    LLMSyncOutput *result = syncllm->run(input);
    if (result->getErrCode() != 0)
    {
        printf(RED "\nsyncOutput: %d:%s\n\n" RESET, result->getErrCode(), result->getErrMsg());
        // continue;
    }
    else
    {
        printf(GREEN "\nsyncOutput: %s:%s\n" RESET, result->getRole(), result->getContent());
        printf(GREEN "\nsyncOutput逆天1: Role: %s\n" RESET, result->getRole());
        printf(YELLOW "Content逆天2: %s\n" RESET, result->getContent());


     auto message = std_msgs::msg::String();
     message.data = result->getContent(); 
  
    // 使用 KeywordChecker 处理关键词并发布消息
    auto extractedKeywords = extractKeywords(message.data);
    auto extractedKeywords_= mergeKeywords(extractedKeywords,oldKeywords);

if(extractedKeywords.find("Location") != extractedKeywords.end() && extractedKeywords.find("Location1") == extractedKeywords.end() && extractedKeywords.find("Location2") == extractedKeywords.end())
{
     std::cout << "Calling checkKeywordCount function..." << std::endl; // 打印调试信息
    checkKeywordCount(extractedKeywords, pub_res, pub_task);
}else{
    CheckKeywordCount(extractedKeywords, pub_res, pub_task);
    }
    // pub_res->publish(message);

    // 清空 final_result
    final_result.clear();

        // std_msgs::msg::String message;
        // message.data = result->getContent();
        // pub_res->publish(message);

        
    }
    // input = "那日语呢？";
    // }

    // 垃圾回收
    if (syncllm != nullptr)
    {
        LLM::destroy(syncllm);
    }
}


void asyncLLMTest()
{
    cout << "\n######### 异步调用 #########" << endl;
    // 配置大模型参数
    LLMConfig *llmConfig = LLMConfig::builder();
    llmConfig->domain("Spark Pro");
    llmConfig->url("wss://spark-api.xf-yun.com/v3.1/chat ");

    Memory *window_memory = Memory::WindowMemory(5);
    LLM *asyncllm = LLM::create(llmConfig, window_memory);

    // Memory* token_memory = Memory::TokenMemory(500);
    // LLM *asyncllm = LLM::create(llmConfig,token_memory);

    if (asyncllm == nullptr)
    {
        printf(RED "\nLLMTest fail, please setLLMConfig before" RESET);
        return;
    }
    // 注册监听回调
    SparkCallbacks *cbs = new SparkCallbacks();
    asyncllm->registerLLMCallbacks(cbs);

    // 异步请求
    int i = 0;
    char *input = "你好用英语怎么说？";
    while (i++ < 2)
    {
        finish = false;
        char *usrContext = (char *)"myContext";
        int ret = asyncllm->arun(input, usrContext);
        if (ret != 0)
        {
            printf(RED "\narun failed: %d\n\n" RESET, ret);
            finish = true;
            continue;
        }

        int times = 0;
        while (!finish)
        { // 等待结果返回退出
            sleep(1);
            if (times++ > 10) // 等待十秒如果没有最终结果返回退出
                break;
        }
        input = "那日语呢？";
    }
    // 垃圾回收

    if (asyncllm != nullptr)
    {
        LLM::destroy(asyncllm);
    }
    if (cbs != nullptr)
        delete cbs;
}
//
void VoiceCallBack(const std_msgs::msg::String::SharedPtr msg,
                   const std::shared_ptr<rclcpp::Publisher<std_msgs::msg::String>> &pub_res,
                   const std::shared_ptr<rclcpp::Publisher<armquadrover_msgs::msg::Task>> &pub_task)
{
    string strListen = msg->data.c_str();
    std::cout << "strListen:::strListen " << strListen << std::endl;
    char *strListenC = new char[strlen(msg->data.c_str()) + 1];
    strcpy(strListenC, msg->data.c_str()); // string转换成C-string
    if (strcmp(strListenC, "q") != 0)
    {
        syncLLMTest(strListenC, pub_res, pub_task);
    }
    else
    {
    }
}


// 合并两个关键词映射，如果重复的就不加，不重复的就添加
std::unordered_map<std::string, std::string> mergeKeywords(
    const std::unordered_map<std::string, std::string>& baseKeywords,
    const std::unordered_map<std::string, std::string>& additionalKeywords) {
    std::unordered_map<std::string, std::string> mergedKeywords = baseKeywords;

    for (const auto& kv : additionalKeywords) {
        if (mergedKeywords.find(kv.first) == mergedKeywords.end()) {
            mergedKeywords[kv.first] = kv.second;
        }
    }

    return mergedKeywords;
}

void checkKeywordCount(const std::unordered_map<std::string, std::string>& extractedKeywords,
                       const std::shared_ptr<rclcpp::Publisher<std_msgs::msg::String>>& pub_res,
                       const std::shared_ptr<rclcpp::Publisher<armquadrover_msgs::msg::Task>>& pub_task) {
    std::vector<std::string> requiredKeywords = {"Location"};
    
    // 判断提取到的关键词数量是否有Location
    if (extractedKeywords.find("Location") != extractedKeywords.end()) {
        MySQLConnector connector;

        // 从关键词集合中获取具体的值
        std::string location = extractedKeywords.find("Location")!= extractedKeywords.end() ? extractedKeywords.at("Location") : "";
        std::string action = extractedKeywords.find("action") != extractedKeywords.end() ? extractedKeywords.at("action") : "";
        std::string FETCH = extractedKeywords.find("FETCH") != extractedKeywords.end() ? extractedKeywords.at("FETCH") : "";
        std::string item = extractedKeywords.find("item") != extractedKeywords.end() ? extractedKeywords.at("item") : "";

        float id = connector.findIdByAttributes(location);
        armquadrover_msgs::msg::Task task_msg;
        float id_float = static_cast<float>(id);
        task_msg.task_parameters.push_back(id_float);

        // 使用 std::map<int, std::string> 按顺序存储动作值
        std::map<int, std::string> actionsMap;
        for (const auto& pair : extractedKeywords) {
            if (pair.first.find("action") == 0 || pair.first.find("FETCH") == 0) {
                std::string actionIndexStr = pair.first.substr(pair.first.find_first_of("0123456789"));
                if (!actionIndexStr.empty()) {
                    int actionIndex = std::stoi(actionIndexStr);
                    actionsMap[actionIndex] = pair.second;
                }
            }
        }

        // 按顺序将动作值插入到任务消息中
        for (const auto& entry : actionsMap) {
            std::string actionValue = entry.second;
            std::vector<float> encodedActions = ActionEncoding::encodeActions({actionValue});  // 使用正确的函数名 encodeActions
            if (!encodedActions.empty()) {
                task_msg.task_parameters.push_back(encodedActions.front());  // 假设取第一个编码作为示例
            } else {
                // 处理无法编码的情况，例如动作值不在映射中
            }
        }

         // 检查item的值并进行编码
        if (!item.empty()) {
            std::vector<float> encodedItems = ActionEncoding::encodeItems({item});
            if (!encodedItems.empty()) {
                task_msg.task_parameters.push_back(encodedItems.front());  // 假设取第一个编码作为示例
            } else {
                // 处理无法编码的情况，例如item值不在映射中
            }
        }


        


        if (id != -1) {
            std::cout << "匹配的记录ID为: " << id << std::endl;
            std_msgs::msg::String message;
            message.data = "匹配的记录ID为: " + std::to_string(id);
            pub_res->publish(message);
            pub_task->publish(task_msg);
        } else {
            std::cout << "我们这里没有这个地方，请再好好的想一想" << std::endl;
            std_msgs::msg::String message;
            message.data = "我们这里没有这个地方，请再好好的想一想";
            pub_res->publish(message);
        }
        std::cout << "逆天1" << std::endl;
        
    } else {
        std::cout << "逆天2" << std::endl;

        // 查找缺少的关键词
        std::unordered_map<std::string, std::string> keywordTranslations = {
            {"Location", "具体位置呢"},
            {"action", "动作"},
            {"FETCH", "请求类型"},
            {"item", "物品"}
        };

        std::string missingKeywordsMsg;
        std::string missingKeywordsCnMsg = "你还没有告诉我";

        for (const auto& keyword : requiredKeywords) {
            if (extractedKeywords.find(keyword) == extractedKeywords.end()) {
                if (!missingKeywordsMsg.empty()) {
                    missingKeywordsMsg += ", ";
                    missingKeywordsCnMsg += "，";
                }
                missingKeywordsMsg += keyword;
                missingKeywordsCnMsg += keywordTranslations[keyword];
            }
        }

        oldKeywords = extractedKeywords;
            // 打印 oldKeywords 的值
        std::cout << "oldKeywords3: ";
        for (const auto& kv : oldKeywords) {
            std::cout << kv.first << ": " << kv.second << ", ";
        }
        std::cout << std::endl;

        // 打印缺少的关键词
        std::cout << "缺少的关键词：" << missingKeywordsMsg << std::endl;
        std::cout << missingKeywordsCnMsg << std::endl;

        // 发布消息包含所有缺少的关键词的中文提示
        std_msgs::msg::String message;
        message.data = missingKeywordsCnMsg;
        pub_res->publish(message);

        // 打印剩余的关键词
        std::cout << "逆天3：话都不说完";
        for (const auto& keyword : extractedKeywords) {
            std::cout << keyword.first << ": " << keyword.second << " ";
        }
        std::cout << std::endl;
    }
}
void CheckKeywordCount(const std::unordered_map<std::string, std::string>& extractedKeywords,
                       const std::shared_ptr<rclcpp::Publisher<std_msgs::msg::String>>& pub_res,
                       const std::shared_ptr<rclcpp::Publisher<armquadrover_msgs::msg::Task>>& pub_task) {
    std::vector<std::string> requiredKeywords = {"Location1", "Location2"};
    

    if (extractedKeywords.find("Location1") != extractedKeywords.end() && extractedKeywords.find("Location2") != extractedKeywords.end()) {
        MySQLConnector connector;

        std::string location1 = extractedKeywords.find("Location1") != extractedKeywords.end() ? extractedKeywords.at("Location1") : "";
        std::string location2 = extractedKeywords.find("Location2") != extractedKeywords.end() ? extractedKeywords.at("Location2") : "";
        std::string item = extractedKeywords.find("item") != extractedKeywords.end() ? extractedKeywords.at("item") : "";

        float id1 = connector.findIdByAttributes(location1);
        float id2 = connector.findIdByAttributes(location2);

        armquadrover_msgs::msg::Task task_msg;
        task_msg.task_parameters.push_back(id1);
        task_msg.task_parameters.push_back(id2);

      // 使用 std::map<int, std::string> 按顺序存储动作值
        std::map<int, std::string> actionsMap;
        for (const auto& pair : extractedKeywords) {
            if (pair.first.find("action") == 0 || pair.first.find("FETCH") == 0) {
                std::string actionIndexStr = pair.first.substr(pair.first.find_first_of("0123456789"));
                if (!actionIndexStr.empty()) {
                    int actionIndex = std::stoi(actionIndexStr);
                    actionsMap[actionIndex] = pair.second;
                }
            }
        }

        // 按顺序将动作值插入到任务消息中
        for (const auto& entry : actionsMap) {
            std::string actionValue = entry.second;
            std::vector<float> encodedActions = ActionEncoding::encodeActions({actionValue});  // 使用正确的函数名 encodeActions
            if (!encodedActions.empty()) {
                task_msg.task_parameters.push_back(encodedActions.front());  // 假设取第一个编码作为示例
            } else {
                // 处理无法编码的情况，例如动作值不在映射中
            }
        }

         // 检查item的值并进行编码
        if (!item.empty()) {
            std::vector<float> encodedItems = ActionEncoding::encodeItems({item});
            if (!encodedItems.empty()) {
                task_msg.task_parameters.push_back(encodedItems.front());  // 假设取第一个编码作为示例
            } else {
                // 处理无法编码的情况，例如item值不在映射中
            }
        }


        if (id1 != -1 && id2 != -1) {
            std::cout << "匹配的记录ID为: " << id1 << " 和 " << id2 << std::endl;
            std_msgs::msg::String message;
            message.data = "匹配的记录ID为: " + std::to_string(id1) + " 和 " + std::to_string(id2);
            pub_res->publish(message);
            pub_task->publish(task_msg);
        } else {
            std::cout << "我们这里没有这个地方，请再好好的想一想" << std::endl;
            std_msgs::msg::String message;
            message.data = "我们这里没有这个地方，请再好好的想一想";
            pub_res->publish(message);
        }
    } else {
        std::unordered_map<std::string, std::string> keywordTranslations = {
            {"Location1", "具体位置1"},
            {"Location2", "具体位置2"},
            {"action", "动作"},
            {"FETCH", "请求类型"},
            {"item", "物品"}
        };

        std::string missingKeywordsMsg;
        std::string missingKeywordsCnMsg = "你还没有告诉我";

        for (const auto& keyword : requiredKeywords) {
            if (extractedKeywords.find(keyword) == extractedKeywords.end()) {
                if (!missingKeywordsMsg.empty()) {
                    missingKeywordsMsg += ", ";
                    missingKeywordsCnMsg += "，";
                }
                missingKeywordsMsg += keyword;
                missingKeywordsCnMsg += keywordTranslations[keyword];
            }
        }

        oldKeywords = extractedKeywords;

        std::cout << "缺少的关键词：" << missingKeywordsMsg << std::endl;
        std::cout << missingKeywordsCnMsg << std::endl;

        std_msgs::msg::String message;
        message.data = missingKeywordsCnMsg;
        pub_res->publish(message);
    }
}



std::unordered_map<std::string, std::string> extractKeywords(const std::string& jsonDatabase) {
    json data = json::parse(jsonDatabase);
    std::vector<std::pair<std::string, std::string>> extractedKeywordsVec;
    std::vector<std::pair<std::string, int>> orderedActions;

    // 添加需要提取的关键词
    std::vector<std::string> requiredKeywords = {"Location", "Location1", "Location2", "item", "action", "FETCH"};

    // 遍历JSON数据，提取关键词及其属性名称
    for (const auto& slot : data["slots"]) {
        std::string name = slot["name"];
        std::string value = slot["value"];

        if (name == "action" || name == "FETCH") {
            // 记录动作指令的顺序和位置
            orderedActions.emplace_back(value, extractedKeywordsVec.size());
        } else if (std::find(requiredKeywords.begin(), requiredKeywords.end(), name) != requiredKeywords.end()) {
            // 提取其他关键词及其属性名称
            extractedKeywordsVec.emplace_back(name, value);
        }
    }

    // 将顺序动作指令添加到 extractedKeywordsVec 中
    for (size_t i = 0; i < orderedActions.size(); ++i) {
        extractedKeywordsVec.emplace_back("action" + std::to_string(i + 1), orderedActions[i].first);
    }

    // 转换为 std::unordered_map
    std::unordered_map<std::string, std::string> extractedKeywords(extractedKeywordsVec.begin(), extractedKeywordsVec.end());

    // 打印提取的关键词
    std::cout << "Extracted Keywords66666666666:" << std::endl;
    for (const auto& kv : extractedKeywords) {
        std::cout << kv.first << ": " << kv.second << std::endl;
    }

    return extractedKeywords;
}

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    auto llm_nh = std::make_shared<rclcpp::Node>("llm_publisher");
    auto nh = std::make_shared<rclcpp::Node>("demo");
    auto pub_res = llm_nh->create_publisher<std_msgs::msg::String>("llm_output", 10);
    auto pub_task = nh->create_publisher<armquadrover_msgs::msg::Task>("task_topic", 10);


    // 创建订阅者时，将 lambda 表达式作为回调函数传递
  auto sub_sr = nh->create_subscription<std_msgs::msg::String>("voice_input", 10, 
    [&pub_res, &pub_task](const std_msgs::msg::String::SharedPtr msg) {
        VoiceCallBack(msg, pub_res, pub_task);
    });
    // ros::Publisher pub_res = nh_.advertise<>();
    cout << "\n######### llm Demo #########" << endl;
    // 全局初始化
    int ret = initSDK();
    if (ret != 0)
    {
        cout << "initSDK failed:" << ret << endl;
        return -1;
    }
    // 同步调用和异步调用选择一个执行
    // syncLLMTest(); // 同步调用
    // asyncLLMTest(); // 异步调用

    rclcpp::Rate r(1);
    while (rclcpp::ok())
    {
        if (!final_result.empty())
        {
            auto message = std_msgs::msg::String();
            message.data = final_result;

            pub_res->publish(message);
            printf("Received message: %s\n", message.data.c_str());
            final_result.clear();
        }
        rclcpp::spin_some(nh);
        r.sleep();
    }
    // 退出
    uninitSDK();
    rclcpp::shutdown();
    return 0;
}
