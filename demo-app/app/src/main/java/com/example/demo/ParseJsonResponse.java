package com.example.demo;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

public class ParseJsonResponse {

    public static void main(String[] args) {
        String jsonResponse = "{\"messages\":[{\"role\":\"assistant\",\"type\":\"answer\",\"content\":\"你好呀！这么晚还来找我，是不是有什么想分享的？\",\"content_type\":\"text\"},{\"role\":\"assistant\",\"type\":\"verbose\",\"content\":\"{\\\"msg_type\\\":\\\"generate_answer_finish\\\",\\\"data\\\":\\\"\\\",\\\"from_module\\\":null,\\\"from_unit\\\":null}\",\"content_type\":\"text\"},{\"role\":\"assistant\",\"type\":\"follow_up\",\"content\":\"你有哪些特别的爱好？\",\"content_type\":\"text\"},{\"role\":\"assistant\",\"type\":\"follow_up\",\"content\":\"你有没有自己喜欢的电影或书籍？\",\"content_type\":\"text\"},{\"role\":\"assistant\",\"type\":\"follow_up\",\"content\":\"你可以给我推荐一些音乐吗？\",\"content_type\":\"text\"}],\"conversation_id\":\"123\",\"code\":0,\"msg\":\"success\"}";

        try {
            JSONObject jsonObject = new JSONObject(jsonResponse);

            // 解析messages数组
            JSONArray messages = jsonObject.getJSONArray("messages");
            for (int i = 0; i < messages.length(); i++) {
                JSONObject message = messages.getJSONObject(i);
                String role = message.getString("role");
                String type = message.getString("type");
                String content = message.getString("content");

                System.out.println("Message " + (i + 1) + ":");
                System.out.println("Role: " + role);
                System.out.println("Type: " + type);
                System.out.println("Content: " + content);
                System.out.println();
            }

            // 获取conversation_id和code
            String conversationId = jsonObject.getString("conversation_id");
            int code = jsonObject.getInt("code");
            String msg = jsonObject.getString("msg");

            System.out.println("Conversation ID: " + conversationId);
            System.out.println("Code: " + code);
            System.out.println("Message: " + msg);

        } catch (JSONException e) {
            e.printStackTrace();
        }
    }
}

