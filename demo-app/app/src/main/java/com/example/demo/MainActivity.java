package com.example.demo;

import static androidx.constraintlayout.widget.ConstraintLayoutStates.TAG;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.TextView;
import androidx.activity.EdgeToEdge;
import androidx.core.graphics.Insets;
import androidx.core.view.ViewCompat;
import androidx.core.view.WindowInsetsCompat;
import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;

import com.iflytek.cloud.RecognizerListener;
import com.iflytek.cloud.RecognizerResult;
import com.iflytek.cloud.SpeechConstant;
import com.iflytek.cloud.SpeechError;
import com.iflytek.cloud.SpeechRecognizer;
import com.iflytek.cloud.SpeechUtility;


import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.List;

import geometry_msgs.msg.Pose2D;
import geometry_msgs.msg.Twist;
import std_msgs.msg.Bool;

public class MainActivity extends ROSActivity implements ROS2_Node.Pose2dUpdateListener, SensorEventListener ,ROS2_Node.TaskCountUpdateListener{
    private SensorManager sensorManager;
    private Sensor accelerometer;
    private TextView responseTextView;
    private TextView pose2dTextView;
    private TextView tv_content;
    private ROS2_Node ros2Node;
    private JoystickView joystickView;
    int count=0;
    com.example.demo.FlowchartView flowchartView;
    @Override
 public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        EdgeToEdge.enable(this);
        setContentView(R.layout.activity_main);
        flowchartView = findViewById(R.id.flowchart_view);
        SpeechUtility.createUtility(this, SpeechConstant.APPID +"");
        // 初始化控件和传感器
        sensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);
        accelerometer = sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
        pose2dTextView = findViewById(R.id.pose2dTextView);
        joystickView = findViewById(R.id.joystickView);
        tv_content=findViewById(R.id.textView_content);

        // 创建 ROS2 节点实例
        ros2Node = new ROS2_Node("ros2_and", "ts", "task_count", "cmd_vel", this, this);
        responseTextView = findViewById(R.id.responseTextView);

        // 执行 Coze 任务以获取服务器响应
//       Coze cozeTask = new Coze(responseTextView);
//       cozeTask.execute();

        // 设置摇杆监听器
        joystickView.setOnMoveListener(new JoystickView.OnMoveListener() {
            @Override
            public void onMove(int angle, int strength) {
                System.out.println("Angle: " + angle + ", Strength: " + strength);

                double linearVelocity = 3 * Math.sin(angle * Math.PI / 180) * strength / 100;
                double angularVelocity = 3 * Math.cos(angle * Math.PI / 180) * strength / 100;

                // 创建 Twist 消息
                Twist twist = new Twist();
                twist.getLinear().setX(linearVelocity);
                twist.getAngular().setZ(-angularVelocity);
                ros2Node.publishTwistMessage(twist);
            }
        });
        findViewById(R.id.startSpeek).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                count++;
                startSpeechClick(tv_content);
                flowchartView.startBlinking(count); // 闪烁下一个任务
            }
        });
        // 设置窗口边界填充
        ViewCompat.setOnApplyWindowInsetsListener(findViewById(R.id.main), (v, insets) -> {
            Insets systemBars = insets.getInsets(WindowInsetsCompat.Type.systemBars());
            v.setPadding(systemBars.left, systemBars.top, systemBars.right, systemBars.bottom);
            return insets;
        });
    }
    public void startSpeechClick(View v){
        //初始化识别无UI识别对象
        //使用SpeechRecognizer对象，可根据回调消息自定义界面；
        SpeechRecognizer mIat = SpeechRecognizer.createRecognizer(this, null);

        //设置语法ID和 SUBJECT 为空，以免因之前有语法调用而设置了此参数；或直接清空所有参数，具体可参考 DEMO 的示例。
        mIat.setParameter( SpeechConstant.DOMAIN, "iat" );//应用领域
        //设置语音输入语言，zh_cn为简体中文
        mIat.setParameter(SpeechConstant.LANGUAGE, "zh_cn");
        //设置结果返回语言,普通话
        mIat.setParameter(SpeechConstant.ACCENT, "mandarin");
        mIat.setParameter(SpeechConstant.ENGINE_TYPE,SpeechConstant.TYPE_CLOUD);
        //设置返回结果格式
        mIat.setParameter(SpeechConstant.RESULT_TYPE,"plain");
        //设置语音前端点:静音超时时间，单位ms，即用户多长时间不说话则当做超时处理
        //取值范围{1000～10000}
        mIat.setParameter(SpeechConstant.VAD_BOS, "4000");
        //设置语音后端点:后端点静音检测时间，单位ms，即用户停止说话多长时间内即认为不再输入，
        //自动停止录音，范围{0~10000}
        mIat.setParameter(SpeechConstant.VAD_EOS, "1000");
        //设置标点符号,设置为"0"返回结果无标点,设置为"1"返回结果有标点
        mIat.setParameter(SpeechConstant.ASR_PTT,"1");
        //设置听写的动态修正
        mIat.setParameter(SpeechConstant.ASR_DWA,"0");

        //开始识别，并设置监听器
        mIat.startListening(mRecoListener);
    }
    //听写监听器
    private RecognizerListener mRecoListener=new RecognizerListener(){
        //音量值0-30
        @Override
        public void onVolumeChanged(int i, byte[] bytes) {

        }
        //开始录音
        @Override
        public void onBeginOfSpeech() {

        }
        //结束录音
        @Override
        public void onEndOfSpeech() {

        }
        //返回结果
        @Override
        public void onResult(RecognizerResult recognizerResult, boolean b) {
            System.out.println(recognizerResult.getResultString());

            // 获取识别结果
            String userQuery = recognizerResult.getResultString();

            // 检查 userQuery 是否为句号
            if ("。".equals(userQuery) || ".".equals(userQuery)) {
                // 如果是句号，则进行相应处理（此处可以添加需要的处理逻辑）
                System.out.println("识别结果为句号字符，跳过处理。");
            } else {
                // 构建 JSON 字符串，并将识别结果插入到 query 字段中

                // 执行 Coze 任务
                Coze cozeTask = new Coze(responseTextView);
                cozeTask.execute(userQuery);
            }
        }





        @Override
        public void onError(SpeechError speechError) {

        }

        @Override
        public void onEvent(int i, int i1, int i2, Bundle bundle) {

        }
    };
    private void printResult(RecognizerResult results){

    }
    public void updateFlowchartView(String jsonContent) {
        try {
            JSONObject jsonObject = new JSONObject(jsonContent);
            JSONArray stepsArray = jsonObject.getJSONArray("steps");
            List<String> tasks = new ArrayList<>();
            List<int[]> coords = new ArrayList<>();

            // 动态生成任务和坐标
            for (int i = 0; i < stepsArray.length(); i++) {
                JSONObject stepObject = stepsArray.getJSONObject(i);
                String stepDescription = stepObject.getString(String.valueOf(i + 1));
                tasks.add(stepDescription);

                // 假设每个任务在 Y 轴上间隔 200 像素
                coords.add(new int[]{200, 100 + i * 200});
            }

            // 设置任务和坐标到 FlowchartView
            flowchartView.setTasksAndCoords(tasks, coords);
            flowchartView.startBlinking(0); // 开始闪烁第一个任务



        } catch (JSONException e) {
            Log.e(TAG, "JSON 解析错误: " + e.getMessage());
        }
    }


    // 实现 ROS2 节点的回调方法
    @Override
    public void onPose2dReceived(Pose2D pose2D) {
        runOnUiThread(() -> {
            String pose2DText = String.format("x: %f, y: %f, theta: %f", pose2D.getX(), pose2D.getY(), pose2D.getTheta());
            pose2dTextView.setText(pose2DText);
        });
    }

    // 生命周期方法
    @Override
    protected void onStart() {
        super.onStart();
        getExecutor().addNode(ros2Node);
    }

    @Override
    protected void onStop() {
        super.onStop();
        getExecutor().removeNode(ros2Node);
    }

    @Override
    protected void onResume() {
        super.onResume();
        sensorManager.registerListener(this, accelerometer, SensorManager.SENSOR_DELAY_NORMAL);
    }

    @Override
    protected void onPause() {
        super.onPause();
        sensorManager.unregisterListener(this);
    }

    // 加速度传感器事件处理
    @Override
    public void onSensorChanged(SensorEvent event) {
        if (event.sensor.getType() == Sensor.TYPE_ACCELEROMETER) {
            float x = event.values[0];
            float y = event.values[1];
            float z = event.values[2];
/*            Twist twist = new Twist();
            twist.getLinear().setX(x);
            twist.getLinear().setY(y);
            twist.getAngular().setZ(z);
            //ros2Node.publishTwistMessage(twist);*/
        }
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int accuracy) {
        // 精度变化处理
    }

    @Override
    public void onTaskCountReceived(Bool bool_) {
        runOnUiThread(() -> {
            boolean b = bool_.getData();
            if (b){
                count++;
                flowchartView.startBlinking(count); // 开始闪烁第二个任务（任务2）

            }

        });
    }
}
