#include "mybase.h"
#include "bmath.h"
void BaseNode::cmd_vel_callback(const geometry_msgs::msg::Twist::SharedPtr msg) {
    switch (currentState) {
        case ChassisState::NORMAL: {
            cmd_vel_1(msg);
            break;
        }
        case ChassisState::ALIGN:
        {
            //cmd_vel_2(msg);
            break;
        }
    }
};
void  BaseNode::base_states_callback(const armquadrover_msgs::msg::DDSM115::SharedPtr msg) {
    switch (currentState) {
        case ChassisState::NORMAL:{
            base_states_1(msg);
            break;
        }
        case ChassisState::ALIGN_Y:{
            base_states_2(msg);
            break;
        }
        case ChassisState::ALIGN_X:{
            base_states_2(msg);
            break;
        }
    }

}
void BaseNode::imu_callback(const sensor_msgs::msg::Imu::SharedPtr msg) {
    imu_msg=*msg;
    auto orientation = msg->orientation;
    tf2::Quaternion q(
            orientation.x,
            orientation.y,
            orientation.z,
            orientation.w);
    tf2::Matrix3x3 m(q);
    double roll, pitch, yaw;
    m.getRPY(roll, pitch, yaw);
    roll_=roll;
    pitch_=pitch;
    yaw_=yaw;
}
void BaseNode::timer_callback() {
    switch (currentState) {
        case ChassisState::ALIGN_Y:{
            time_2();
            if (fabs(d_y)- fabs(pose2D_msg.y)<0.1){
                armquadrover_msgs::msg::DDSM115 base_control_msg;
                base_control_msg.front_left_servo= 2.1;
                base_control_msg.rear_left_servo= 2.1;
                base_control_msg.rear_right_servo= 2.1;
                base_control_msg.front_right_servo=2.1;
                base_control_msg.front_left = 0;
                base_control_msg.rear_left = 0;
                base_control_msg.rear_right = 0;
                base_control_msg.front_right = 0;
                pub_base_control_->publish(base_control_msg);
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                if (pose2D_msg.x>0){
                    base_control_msg.front_left = 18;
                    base_control_msg.rear_left = 18;
                    base_control_msg.rear_right = 18;
                    base_control_msg.front_right = 18;
                }else if(pose2D_msg.x<0) {
                    base_control_msg.front_left = -18;
                    base_control_msg.rear_left = -18;
                    base_control_msg.rear_right = -18;
                    base_control_msg.front_right = -18;
                }
                d_x=0;
                setState(ChassisState::ALIGN_X);
                pub_base_control_->publish(base_control_msg);
            }
            break;
        }
        case ChassisState::ALIGN_X:{
            time_1();
            double ddx;
            ddx=average*d_time;
            d_x+=ddx;
            if (fabs(d_x)- fabs(pose2D_msg.x)<0.1){
                armquadrover_msgs::msg::DDSM115 base_control_msg;
                base_control_msg.front_left_servo= 2.1;
                base_control_msg.rear_left_servo= 2.1;
                base_control_msg.rear_right_servo= 2.1;
                base_control_msg.front_right_servo=2.1;
                base_control_msg.front_left = 0;
                base_control_msg.rear_left = 0;
                base_control_msg.rear_right = 0;
                base_control_msg.front_right = 0;
                pub_base_control_->publish(base_control_msg);
                setState(ChassisState::NORMAL);
                std_msgs::msg::Int32 state;
                state.data=1;
                pub_state_->publish(state);
            }
            break;
        }


    }
}
void BaseNode::pose_callback(const geometry_msgs::msg::Pose2D::SharedPtr msg) {
    std_msgs::msg::Int32 state;
    state.data=3;
    pub_state_->publish(state);
    setState(ChassisState::ALIGN);
    pose2D_msg=*msg;
    armquadrover_msgs::msg::DDSM115 base_control_msg;
    base_control_msg.front_left_servo= 0.523;
    base_control_msg.rear_left_servo= 0.523;
    base_control_msg.rear_right_servo= 0.523;
    base_control_msg.front_right_servo=0.523;
    base_control_msg.front_left = 0;
    base_control_msg.rear_left = 0;
    base_control_msg.rear_right = 0;
    base_control_msg.front_right = 0;
    pub_base_control_->publish(base_control_msg);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    base_control_msg.front_left_servo= 0.523;
    base_control_msg.rear_left_servo= 0.523;
    base_control_msg.rear_right_servo= 0.523;
    base_control_msg.front_right_servo=0.523;
    if (pose2D_msg.y>0){
        base_control_msg.front_left = 18;
        base_control_msg.rear_left = 18;
        base_control_msg.rear_right = 18;
        base_control_msg.front_right = 18;
    }else if(pose2D_msg.y<0) {
        base_control_msg.front_left = -18;
        base_control_msg.rear_left = -18;
    base_control_msg.rear_right = -18;
    base_control_msg.front_right = -18;
}
    d_y=0;
    setState(ChassisState::ALIGN_Y);
    pub_base_control_->publish(base_control_msg);
}
int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<BaseNode>());
    rclcpp::shutdown();
    return 0;
}