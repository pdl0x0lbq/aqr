package com.example.demo;

import android.os.AsyncTask;
import android.util.Log;
import android.widget.TextView;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.URL;

public class Coze extends AsyncTask<String, Void, String> {

    private static final String TAG = "Coze";
    private static final String API_URL = "https://api.coze.cn/open_api/v2/chat";
    private static final String AUTH_TOKEN = "";

    private TextView responseTextView;

    public Coze(TextView responseTextView) {
        this.responseTextView = responseTextView;
    }

    @Override
    protected String doInBackground(String... params) {
        if (params.length == 0) {
            Log.e(TAG, "No query parameter provided.");
            return null;
        }

        String userQuery = params[0];
        HttpURLConnection conn = null;
        try {
            // Setup the connection
            URL url = new URL(API_URL);
            conn = (HttpURLConnection) url.openConnection();
            conn.setRequestMethod("POST");
            conn.setRequestProperty("Content-Type", "application/json");
            conn.setRequestProperty("Accept", "*/*");
            conn.setRequestProperty("Authorization", AUTH_TOKEN);
            conn.setRequestProperty("Host", "api.coze.cn");
            conn.setRequestProperty("Connection", "keep-alive");
            conn.setDoOutput(true);

            // Prepare the JSON payload
            String jsonInputString = String.format("{\"conversation_id\":\"123\",\"bot_id\":\"7387787837368303650\",\"user\":\"29032201862555\",\"query\":\"%s\",\"stream\":false}", userQuery);
System.out.println(jsonInputString);
            // Send the JSON payload
            try (OutputStream os = conn.getOutputStream()) {
                byte[] input = jsonInputString.getBytes("utf-8");
                os.write(input, 0, input.length);
            }

            // Read the response
            StringBuilder response = new StringBuilder();
            try (BufferedReader br = new BufferedReader(new InputStreamReader(conn.getInputStream(), "utf-8"))) {
                String responseLine;
                while ((responseLine = br.readLine()) != null) {
                    response.append(responseLine.trim());
                }
            }

            return response.toString();

        } catch (IOException e) {
            Log.e(TAG, "Error sending POST request: " + e.getMessage());
            return null;
        } finally {
            if (conn != null) {
                conn.disconnect();
            }
        }
    }

    @Override
    protected void onPostExecute(String result) {
        if (result != null) {
            Log.d(TAG, "Response from server: " + result);

            try {
                JSONObject jsonObject = new JSONObject(result);

                // Parse the messages array
                JSONArray messages = jsonObject.getJSONArray("messages");
                StringBuilder messageText = new StringBuilder();
                String content = null;

                for (int i = 0; i < messages.length(); i++) {
                    JSONObject message = messages.getJSONObject(i);
                    String type = message.getString("type");

                    // Only handle messages of type "answer"
                    if ("answer".equals(type)) {
                        content = message.getString("content");
                        messageText.append(content).append("\n\n");
                    }
                }

                //responseTextView.setText(messageText.toString());

                if (content != null) {
                    // Call MainActivity's method to update the flowchartView
                    ((MainActivity) responseTextView.getContext()).updateFlowchartView(content);
                }

            } catch (JSONException e) {
                Log.e(TAG, "JSON parsing error: " + e.getMessage());
            }
        } else {

            Log.e(TAG, "No response received from server.");
            // Handle request failure
        }
    }
}
