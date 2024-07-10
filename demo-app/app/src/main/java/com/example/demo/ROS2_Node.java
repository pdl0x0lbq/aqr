package com.example.demo;

import org.ros2.rcljava.RCLJava;
import org.ros2.rcljava.node.BaseComposableNode;
import org.ros2.rcljava.publisher.Publisher;
import org.ros2.rcljava.subscription.Subscription;
import geometry_msgs.msg.Pose2D;
import geometry_msgs.msg.Twist;
import std_msgs.msg.Bool;
public class ROS2_Node extends BaseComposableNode {
    private final String topic;
    private final String task_count_sub_topic;
    private final String publishTopic;

    private Subscription<Pose2D> subscriber;

    private Subscription<Bool> task_count_subscriber;
    private Publisher<Twist> publisher;
    private Pose2dUpdateListener pose2dUpdateListener;
    private TaskCountUpdateListener taskcountUpdateListener;

    public interface Pose2dUpdateListener {
        void onPose2dReceived(Pose2D occupancyGrid);
    }
    public interface TaskCountUpdateListener {
        void onTaskCountReceived(Bool bool_);
    }
    public ROS2_Node(final String name, final String topic,final String task_count_sub_topic,
                     final String publishTopic, Pose2dUpdateListener listener,TaskCountUpdateListener listener2) {
        super(name);
        this.topic = topic;
        this.task_count_sub_topic = task_count_sub_topic;
        this.publishTopic = publishTopic;
        this.pose2dUpdateListener = listener;
        this.taskcountUpdateListener=listener2;
        // 创建订阅者
        this.subscriber = this.node.createSubscription(geometry_msgs.msg.Pose2D.class, this.topic, this::CallBack);
        this.task_count_subscriber= this.node.createSubscription(std_msgs.msg.Bool.class, this.task_count_sub_topic, this::TaskCountCallBack);

        // 创建发布者
        this.publisher = this.node.createPublisher(geometry_msgs.msg.Twist.class, this.publishTopic);
    }

    private void CallBack(Pose2D msg){
        if (pose2dUpdateListener != null) {
            pose2dUpdateListener.onPose2dReceived(msg);
        }
    }
    private void TaskCountCallBack(Bool msg){
        if (taskcountUpdateListener != null) {
            taskcountUpdateListener.onTaskCountReceived(msg);
        }
    }
    public void publishTwistMessage(Twist twist) {
        this.publisher.publish(twist);
    }

    public void spin() {
        RCLJava.spin(node);
    }
}
