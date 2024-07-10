#ifndef ACTION_ENCODING_HPP
#define ACTION_ENCODING_HPP

#include <unordered_map>
#include <vector>
#include <string>

namespace ActionEncoding {

// 定义动作指令和浮点数的映射，从130.0开始编码
const std::unordered_map<std::string, float> action_to_float = {
    {"Grasp", 130.0},
    {"come", 131.0},
    {"fetch_water", 132.0},
    {"get", 133.0},
    {"go", 134.0},
    {"go1", 135.0},
    {"go2", 136.0},
    {"hold", 137.0},
    {"left", 138.0},
    {"pour_water", 139.0},
    {"put", 140.0},
    {"right", 141.0},
    {"surrender", 142.0},
    {"take", 143.0},
    {"throw", 144.0}
    // 添加更多指令，继续编码...
};

const std::unordered_map<float, std::string> float_to_action = {
    {130.0, "Grasp"},
    {131.0, "come"},
    {132.0, "fetch_water"},
    {133.0, "get"},
    {134.0, "go"},
    {135.0, "go1"},
    {136.0, "go2"},
    {137.0, "hold"},
    {138.0, "left"},
    {139.0, "pour_water"},
    {140.0, "put"},
    {141.0, "right"},
    {142.0, "surrender"},
    {143.0, "take"},
    {144.0, "throw"}
    // 添加更多指令，继续编码...
};

// 定义 item 和浮点数的映射，从1000.0开始编码
const std::unordered_map<std::string, float> item_to_float = {
    {"乒乓球", 1000.0},
    {"文件夹", 1001.0},
    {"文件", 1002.0},
    {"书", 1003.0},
    {"手机", 1004.0},
    {"杯子", 1005.0},
    {"眼镜", 1006.0},
    {"乒乓球瓶", 1007.0},
    {"书本", 1008.0},
    {"笔记本电脑", 1009.0},
    {"钥匙", 1010.0},
    {"感冒药瓶", 1011.0},
    {"感冒药", 1012.0},
    {"报纸", 1013.0},
    {"充电器", 1014.0},
    {"手电筒", 1015.0},
    {"出水口", 1016.0},
    {"茶杯", 1017.0},
    {"遥控器", 1018.0},
    {"笔记本", 1019.0},
    {"水杯", 1020.0},
    {"剪刀", 1021.0},
    {"手机充电器", 1022.0},
    {"手表", 1023.0},
    {"纸张", 1024.0},
    {"饮料", 1025.0},
    {"鼠标", 1026.0},
    {"可乐杯", 1027.0}
    // 添加更多 item，继续编码...
};

const std::unordered_map<float, std::string> float_to_item = {
    {1000.0, "乒乓球"},
    {1001.0, "文件夹"},
    {1002.0, "文件"},
    {1003.0, "书"},
    {1004.0, "手机"},
    {1005.0, "杯子"},
    {1006.0, "眼镜"},
    {1007.0, "乒乓球瓶"},
    {1008.0, "书本"},
    {1009.0, "笔记本电脑"},
    {1010.0, "钥匙"},
    {1011.0, "感冒药瓶"},
    {1012.0, "感冒药"},
    {1013.0, "报纸"},
    {1014.0, "充电器"},
    {1015.0, "手电筒"},
    {1016.0, "出水口"},
    {1017.0, "茶杯"},
    {1018.0, "遥控器"},
    {1019.0, "笔记本"},
    {1020.0, "水杯"},
    {1021.0, "剪刀"},
    {1022.0, "手机充电器"},
    {1023.0, "手表"},
    {1024.0, "纸张"},
    {1025.0, "饮料"},
    {1026.0, "鼠标"},
    {1027.0, "可乐杯"}
    // 添加更多 item，继续编码...
};

// 将动作指令编码为浮点数
inline std::vector<float> encodeActions(const std::vector<std::string>& actions) {
    std::vector<float> result;
    for (const auto& action : actions) {
        auto it = action_to_float.find(action);
        if (it != action_to_float.end()) {
            result.push_back(it->second);
        }
    }
    return result;
}

// 将浮点数解码为动作指令
inline std::vector<std::string> decodeActions(const std::vector<float>& floats) {
    std::vector<std::string> result;
    for (const auto& value : floats) {
        auto it = float_to_action.find(value);
        if (it != float_to_action.end()) {
            result.push_back(it->second);
        }
    }
    return result;
}

// 将 item 编码为浮点数
inline std::vector<float> encodeItems(const std::vector<std::string>& items) {
    std::vector<float> result;
    for (const auto& item : items) {
        auto it = item_to_float.find(item);
        if (it != item_to_float.end()) {
            result.push_back(it->second);
        }
    }
    return result;
}

// 将浮点数解码为 item
inline std::vector<std::string> decodeItems(const std::vector<float>& floats) {
    std::vector<std::string> result;
    for (const auto& value : floats) {
        auto it = float_to_item.find(value);
        if (it != float_to_item.end()) {
            result.push_back(it->second);
        }
    }
    return result;
}

} // namespace ActionEncoding

#endif // ACTION_ENCODING_HPP
