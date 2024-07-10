#include "mybase.h"
#include "bmath.h"
void BaseNode::cmd_vel_1(const geometry_msgs::msg::Twist::SharedPtr msg) {
    vc_x=msg->linear.x;
    vc_y=msg->linear.y;
    thetac_=(msg->angular.z);
    const double RAD_PER_SEC_TO_RPM = 60.0 / (2.0 * M_PI);

    if (-330 < std::abs(vc_x) && std::abs(vc_x) < 330 && -330 < std::abs(vc_y) && std::abs(vc_y) < 330 &&  -330 < std::abs(thetac_) && std::abs(thetac_) < 330) {
        vc_x=vc_x;
        vc_y = vc_y;
        thetac_ = thetac_;

    }else {
        // 否则将速度设置为零并打印错误消息
        vc_x = 0.0;
        vc_y = 0.0;
        thetac_ = 0.0;
        RCLCPP_ERROR(this->get_logger(), "Received velocity or angular velocity out of range.");
    }
    Vector e={1,0};
    Vector v1={vc_x-thetac_*r_y,vc_y+thetac_*r_x};
    Vector v2={vc_x-thetac_*r_y,vc_y-thetac_*r_x};
    Vector v3={vc_x+thetac_*r_y,vc_y-thetac_*r_x};
    Vector v4={vc_x+thetac_*r_y,vc_y+thetac_*r_x};

    armquadrover_msgs::msg::DDSM115 base_control_msg;
    base_control_msg.front_left =((sqrt((vc_x-thetac_*r_y)*(vc_x-thetac_*r_y)+(vc_y+thetac_*r_x)*(vc_y+thetac_*r_x)))/0.05125)*RAD_PER_SEC_TO_RPM;
    base_control_msg.rear_left=((sqrt((vc_x-thetac_*r_y)*(vc_x-thetac_*r_y)+(vc_y-thetac_*r_x)*(vc_y-thetac_*r_x)))/0.05125)*RAD_PER_SEC_TO_RPM;
    base_control_msg.rear_right=((sqrt((vc_x+thetac_*r_y)*(vc_x+thetac_*r_y)+(vc_y-thetac_*r_x)*(vc_y-thetac_*r_x)))/0.05125)*RAD_PER_SEC_TO_RPM;
    base_control_msg.front_right=((sqrt((vc_x+thetac_*r_y)*(vc_x+thetac_*r_y)+(vc_y+thetac_*r_x)*(vc_y+thetac_*r_x)))/0.05125)*RAD_PER_SEC_TO_RPM;
    if (vc_x<0) {
        base_control_msg.front_left = -base_control_msg.front_left;
        base_control_msg.rear_left = -base_control_msg.rear_left;
        base_control_msg.rear_right = -base_control_msg.rear_right;
        base_control_msg.front_right = -base_control_msg.front_right;
    }
    base_control_msg.front_left_servo= (angleBetweenVectors(v1,e));
    base_control_msg.rear_left_servo= (angleBetweenVectors(v2,e));
    base_control_msg.rear_right_servo= (angleBetweenVectors(v3,e));
    base_control_msg.front_right_servo= (angleBetweenVectors(v4,e));
    if(vc_x>=0){
        base_control_msg.front_left = (base_control_msg.front_left_servo > PI / 2 || base_control_msg.front_left_servo < -PI / 2) ? -base_control_msg.front_left : base_control_msg.front_left;
        base_control_msg.rear_left = (base_control_msg.rear_left_servo > PI / 2 || base_control_msg.rear_left_servo < -PI / 2) ? -base_control_msg.rear_left : base_control_msg.rear_left;
        base_control_msg.rear_right = (base_control_msg.rear_right_servo > PI / 2 || base_control_msg.rear_right_servo < -PI / 2) ? -base_control_msg.rear_right : base_control_msg.rear_right;
        base_control_msg.front_right = (base_control_msg.front_right_servo > PI / 2 || base_control_msg.front_right_servo < -PI / 2) ? -base_control_msg.front_right : base_control_msg.front_right;
    }else{
        base_control_msg.front_left = (base_control_msg.front_left_servo > PI / 2 || base_control_msg.front_left_servo < -PI / 2) ? base_control_msg.front_left : -base_control_msg.front_left;
        base_control_msg.rear_left = (base_control_msg.rear_left_servo > PI / 2 || base_control_msg.rear_left_servo < -PI / 2) ? base_control_msg.rear_left : -base_control_msg.rear_left;
        base_control_msg.rear_right = (base_control_msg.rear_right_servo > PI / 2 || base_control_msg.rear_right_servo < -PI / 2) ? base_control_msg.rear_right : -base_control_msg.rear_right;
        base_control_msg.front_right = (base_control_msg.front_right_servo > PI / 2 || base_control_msg.front_right_servo < -PI / 2) ? base_control_msg.front_right : -base_control_msg.front_right;
    }
    base_control_msg.front_left_servo= (adjustAngle(angleBetweenVectors(v1,e)))+(2.0/3.0*M_PI);
    base_control_msg.rear_left_servo= (adjustAngle(angleBetweenVectors(v2,e)))+(2.0/3.0*M_PI);
    base_control_msg.rear_right_servo= (adjustAngle(angleBetweenVectors(v3,e)))+(2.0/3.0*M_PI);
    base_control_msg.front_right_servo= (adjustAngle(angleBetweenVectors(v4,e)))+(2.0/3.0*M_PI);

    double speed1  = base_control_msg.front_left;
    double speed2  = base_control_msg.rear_left;
    double speed3  = base_control_msg.rear_right;
    double speed4  = base_control_msg.front_right;


    if (((base_control_msg.rear_left_servo == base_control_msg.front_right_servo) &&
         (base_control_msg.front_right_servo != base_control_msg.front_left_servo) &&
         (base_control_msg.rear_left == -base_control_msg.front_right)) ||
        ((base_control_msg.front_left_servo > (2.0/3.0*M_PI + M_PI/2-0.00001)) &&
         (base_control_msg.rear_left_servo > (2.0/3.0*M_PI + M_PI/2-0.00001))) ||
        ((base_control_msg.front_left_servo < (2.0/3.0*M_PI - M_PI/2+0.00001)) &&
         (base_control_msg.rear_left_servo < (2.0/3.0*M_PI - M_PI/2+0.00001))))

    {
        // base_control_msg.front_left = 0;
        // base_control_msg.rear_left = 0;
        // base_control_msg.rear_right = 0;
        // base_control_msg.front_right = 0;

        // pub_base_control_->publish(base_control_msg);
        // 延时 500 毫秒
        // std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        base_control_msg.front_left = speed1;
        base_control_msg.rear_left = speed2;
        base_control_msg.rear_right = speed3;
        base_control_msg.front_right = speed4;

        pub_base_control_->publish(base_control_msg);
    }
    else{
        pub_base_control_->publish(base_control_msg);
    }
}
void BaseNode::cmd_vel_2(const geometry_msgs::msg::Twist::SharedPtr msg) {
    vc_x=msg->linear.x;
    vc_y=msg->linear.y;
    thetac_=(msg->angular.z);
    const double RAD_PER_SEC_TO_RPM = 60.0 / (2.0 * M_PI);

    if (-330 < std::abs(vc_x) && std::abs(vc_x) < 330 && -330 < std::abs(vc_y) && std::abs(vc_y) < 330 &&  -330 < std::abs(thetac_) && std::abs(thetac_) < 330) {
        vc_x=vc_x;
        vc_y = vc_y;
        thetac_ = thetac_;

    }else {
        // 否则将速度设置为零并打印错误消息
        vc_x = 0.0;
        vc_y = 0.0;
        thetac_ = 0.0;
        RCLCPP_ERROR(this->get_logger(), "Received velocity or angular velocity out of range.");
    }
    Vector e={1,0};
    Vector v1={vc_x-thetac_*r_y,vc_y+thetac_*r_x};
    Vector v2={vc_x-thetac_*r_y,vc_y-thetac_*r_x};
    Vector v3={vc_x+thetac_*r_y,vc_y-thetac_*r_x};
    Vector v4={vc_x+thetac_*r_y,vc_y+thetac_*r_x};

    armquadrover_msgs::msg::DDSM115 base_control_msg;
    base_control_msg.front_left =((sqrt((vc_x-thetac_*r_y)*(vc_x-thetac_*r_y)+(vc_y+thetac_*r_x)*(vc_y+thetac_*r_x)))/0.05125)*RAD_PER_SEC_TO_RPM;
    base_control_msg.rear_left=((sqrt((vc_x-thetac_*r_y)*(vc_x-thetac_*r_y)+(vc_y-thetac_*r_x)*(vc_y-thetac_*r_x)))/0.05125)*RAD_PER_SEC_TO_RPM;
    base_control_msg.rear_right=((sqrt((vc_x+thetac_*r_y)*(vc_x+thetac_*r_y)+(vc_y-thetac_*r_x)*(vc_y-thetac_*r_x)))/0.05125)*RAD_PER_SEC_TO_RPM;
    base_control_msg.front_right=((sqrt((vc_x+thetac_*r_y)*(vc_x+thetac_*r_y)+(vc_y+thetac_*r_x)*(vc_y+thetac_*r_x)))/0.05125)*RAD_PER_SEC_TO_RPM;
    if (vc_x<0) {
        base_control_msg.front_left = -base_control_msg.front_left;
        base_control_msg.rear_left = -base_control_msg.rear_left;
        base_control_msg.rear_right = -base_control_msg.rear_right;
        base_control_msg.front_right = -base_control_msg.front_right;
    }
    base_control_msg.front_left_servo= (angleBetweenVectors(v1,e));
    base_control_msg.rear_left_servo= (angleBetweenVectors(v2,e));
    base_control_msg.rear_right_servo= (angleBetweenVectors(v3,e));
    base_control_msg.front_right_servo= (angleBetweenVectors(v4,e));
    if(vc_x>=0){
        base_control_msg.front_left = (base_control_msg.front_left_servo > PI / 2 || base_control_msg.front_left_servo < -PI / 2) ? -base_control_msg.front_left : base_control_msg.front_left;
        base_control_msg.rear_left = (base_control_msg.rear_left_servo > PI / 2 || base_control_msg.rear_left_servo < -PI / 2) ? -base_control_msg.rear_left : base_control_msg.rear_left;
        base_control_msg.rear_right = (base_control_msg.rear_right_servo > PI / 2 || base_control_msg.rear_right_servo < -PI / 2) ? -base_control_msg.rear_right : base_control_msg.rear_right;
        base_control_msg.front_right = (base_control_msg.front_right_servo > PI / 2 || base_control_msg.front_right_servo < -PI / 2) ? -base_control_msg.front_right : base_control_msg.front_right;
    }else{
        base_control_msg.front_left = (base_control_msg.front_left_servo > PI / 2 || base_control_msg.front_left_servo < -PI / 2) ? base_control_msg.front_left : -base_control_msg.front_left;
        base_control_msg.rear_left = (base_control_msg.rear_left_servo > PI / 2 || base_control_msg.rear_left_servo < -PI / 2) ? base_control_msg.rear_left : -base_control_msg.rear_left;
        base_control_msg.rear_right = (base_control_msg.rear_right_servo > PI / 2 || base_control_msg.rear_right_servo < -PI / 2) ? base_control_msg.rear_right : -base_control_msg.rear_right;
        base_control_msg.front_right = (base_control_msg.front_right_servo > PI / 2 || base_control_msg.front_right_servo < -PI / 2) ? base_control_msg.front_right : -base_control_msg.front_right;
    }
    base_control_msg.front_left_servo= (adjustAngle(angleBetweenVectors(v1,e)))+(2.0/3.0*M_PI);
    base_control_msg.rear_left_servo= (adjustAngle(angleBetweenVectors(v2,e)))+(2.0/3.0*M_PI);
    base_control_msg.rear_right_servo= (adjustAngle(angleBetweenVectors(v3,e)))+(2.0/3.0*M_PI);
    base_control_msg.front_right_servo= (adjustAngle(angleBetweenVectors(v4,e)))+(2.0/3.0*M_PI);

    double speed1  = base_control_msg.front_left;
    double speed2  = base_control_msg.rear_left;
    double speed3  = base_control_msg.rear_right;
    double speed4  = base_control_msg.front_right;


    if (((base_control_msg.rear_left_servo == base_control_msg.front_right_servo) &&
         (base_control_msg.front_right_servo != base_control_msg.front_left_servo) &&
         (base_control_msg.rear_left == -base_control_msg.front_right)) ||
        ((base_control_msg.front_left_servo > (2.0/3.0*M_PI + M_PI/2-0.00001)) &&
         (base_control_msg.rear_left_servo > (2.0/3.0*M_PI + M_PI/2-0.00001))) ||
        ((base_control_msg.front_left_servo < (2.0/3.0*M_PI - M_PI/2+0.00001)) &&
         (base_control_msg.rear_left_servo < (2.0/3.0*M_PI - M_PI/2+0.00001))))

    {
        // base_control_msg.front_left = 0;
        // base_control_msg.rear_left = 0;
        // base_control_msg.rear_right = 0;
        // base_control_msg.front_right = 0;

        // pub_base_control_->publish(base_control_msg);
        // 延时 500 毫秒
        // std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        base_control_msg.front_left = speed1;
        base_control_msg.rear_left = speed2;
        base_control_msg.rear_right = speed3;
        base_control_msg.front_right = speed4;

        pub_base_control_->publish(base_control_msg);
    }
    else{
        pub_base_control_->publish(base_control_msg);
    }
}
void  BaseNode::base_states_1(const armquadrover_msgs::msg::DDSM115::SharedPtr  msg) {
    double v1 = 0;
    double v2 = 0;
    double v3 = 0;
    double v4 = 0;

    double theta_1 = 0;
    double theta_2 = 0;
    double theta_3 = 0;
    double theta_4 = 0;
    const double RPM_TO_RAD_PER_SEC = 2.0 * M_PI / 60.0;
    v1 = msg->front_left*RPM_TO_RAD_PER_SEC*0.05125;
    v2 = msg->rear_left*RPM_TO_RAD_PER_SEC*0.05125;
    v3 = -msg->rear_right*RPM_TO_RAD_PER_SEC*0.05125;
    v4 = -msg->front_right*RPM_TO_RAD_PER_SEC*0.05125;
    theta_1 = (4.2-msg->front_left_servo)-(2.0/3.0*M_PI);
    theta_2 = (4.2-msg->rear_left_servo)-(2.0/3.0*M_PI);
    theta_3 = (4.2-msg->rear_right_servo)-(2.0/3.0*M_PI);
    theta_4 = (4.2-msg->front_right_servo)-(2.0/3.0*M_PI);
    v_cx =(cos(theta_1)*v1+cos(theta_2)*v2+ cos(theta_3)*v3+cos(theta_4)*v4)/4;
    v_cy =(sin(theta_1)*v1+sin(theta_2)*v2+ sin(theta_3)*v3+sin(theta_4)*v4)/4;
    theta_c  =((r_x*sin(theta_1)-r_y*cos(theta_1))*v1+(-r_x*sin(theta_2)-r_y*cos(theta_2))*v2+(-r_x*sin(theta_3)+r_y*cos(theta_3))*v3+(r_x*sin(theta_4)+r_y*cos(theta_4))*v4)/(4*r*r);
    vc=sqrt((v_cx)*(v_cx)+(v_cy)*(v_cy));
    if (v_cx < 0 ) {
        vc = -vc;
    }
}
void  BaseNode::base_states_2(const armquadrover_msgs::msg::DDSM115::SharedPtr  msg) {
    double v1 = 0;
    double v2 = 0;
    double v3 = 0;
    double v4 = 0;

    double theta_1 = 0;
    double theta_2 = 0;
    double theta_3 = 0;
    double theta_4 = 0;
    const double RPM_TO_RAD_PER_SEC = 2.0 * M_PI / 60.0;
    v1 = msg->front_left*RPM_TO_RAD_PER_SEC*0.05125;
    v2 = msg->rear_left*RPM_TO_RAD_PER_SEC*0.05125;
    v3 = -msg->rear_right*RPM_TO_RAD_PER_SEC*0.05125;
    v4 = -msg->front_right*RPM_TO_RAD_PER_SEC*0.05125;
    double sum = v1 + v2 + v3 + v4;
    average = sum / 4.0;
}
void BaseNode::time_1() {
    rclcpp::Clock::SharedPtr clock = this->get_clock();
    rclcpp::Time now = clock->now();
    auto time = this->now();
    //    RCLCPP_INFO(this->get_logger(), "Current time: %lld nanoseconds", time.nanoseconds());

    c_time=time.nanoseconds();
    d_time=(c_time-l_time)*0.000000001;
    if (d_time>10){
        d_time=0;
    }
    l_time=c_time;
    nav_msgs::msg::Odometry odom;
    geometry_msgs::msg::TransformStamped transform;
    delta_x = (vc * cos(yaw_)) * d_time;         //th_弧度
    delta_y = (vc * sin(yaw_)) * d_time;
    delta_th = theta_c * d_time;
    th += delta_th;
    x_ +=delta_x;
    y_ +=delta_y;
    odom.header.stamp=this->now();
    odom.header.frame_id="odom";
    odom.child_frame_id="base_link";
    odom.pose.pose.position.x=x_ ;
    odom.pose.pose.position.y=y_;
    odom.pose.pose.position.z=0.0;
    odom.pose.pose.orientation.x = imu_msg.orientation.x;
    odom.pose.pose.orientation.y =  imu_msg.orientation.y;
    odom.pose.pose.orientation.z =  imu_msg.orientation.z;
    odom.pose.pose.orientation.w = imu_msg.orientation.w;
    transform.header.stamp = this->now();
    transform.header.frame_id = "odom";
    transform.child_frame_id = "base_link";
    // Fill in transform.transform.translation
    transform.transform.translation.x = x_;
    transform.transform.translation.y = y_;
    transform.transform.translation.z =0.0;
    // Fill in transform.transform.rotation
    transform.transform.rotation.x = imu_msg.orientation.x;
    transform.transform.rotation.y = imu_msg.orientation.y;
    transform.transform.rotation.z = imu_msg.orientation.z;
    transform.transform.rotation.w =imu_msg.orientation.w;
    tf_broadcaster_->sendTransform(transform);
    // 日志打印
    // RCLCPP_INFO(this->get_logger(), "Publishing: '%f", motorl);
    // 发布消息
    pub_odom_->publish(odom);
}
void BaseNode::time_2() {
    rclcpp::Clock::SharedPtr clock = this->get_clock();
    rclcpp::Time now = clock->now();
    auto time = this->now();
    //    RCLCPP_INFO(this->get_logger(), "Current time: %lld nanoseconds", time.nanoseconds());

    c_time=time.nanoseconds();
    d_time=(c_time-l_time)*0.000000001;
    if (d_time>10){
        d_time=0;
    }
    l_time=c_time;
    nav_msgs::msg::Odometry odom;
    geometry_msgs::msg::TransformStamped transform;
    double ddy;
    ddy=average*d_time;
    d_y+=ddy;
    delta_x = (average * sin(yaw_)) * d_time;         //th_弧度
    delta_y = (average * cos(yaw_)) * d_time;
    delta_th = theta_c * d_time;
    th += delta_th;
    x_ +=delta_x;
    y_ +=delta_y;
    odom.header.stamp=this->now();
    odom.header.frame_id="odom";
    odom.child_frame_id="base_link";
    odom.pose.pose.position.x=x_ ;
    odom.pose.pose.position.y=y_;
    odom.pose.pose.position.z=0.0;
    odom.pose.pose.orientation.x = imu_msg.orientation.x;
    odom.pose.pose.orientation.y =  imu_msg.orientation.y;
    odom.pose.pose.orientation.z =  imu_msg.orientation.z;
    odom.pose.pose.orientation.w = imu_msg.orientation.w;
    transform.header.stamp = this->now();
    transform.header.frame_id = "odom";
    transform.child_frame_id = "base_link";
    // Fill in transform.transform.translation
    transform.transform.translation.x = x_;
    transform.transform.translation.y = y_;
    transform.transform.translation.z =0.0;
    // Fill in transform.transform.rotation
    transform.transform.rotation.x = imu_msg.orientation.x;
    transform.transform.rotation.y = imu_msg.orientation.y;
    transform.transform.rotation.z = imu_msg.orientation.z;
    transform.transform.rotation.w =imu_msg.orientation.w;
    tf_broadcaster_->sendTransform(transform);
    // 日志打印
    // RCLCPP_INFO(this->get_logger(), "Publishing: '%f", motorl);
    // 发布消息
    pub_odom_->publish(odom);
}