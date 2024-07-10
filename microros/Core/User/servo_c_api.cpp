#include "servo_c_api.h"
#include "servo.h"

#ifdef __cplusplus
extern "C" {
#endif
/*extern servo ddsm115_servo1(0x01);
extern servo ddsm115_servo2(0x02);
extern servo ddsm115_servo3(0x03);
extern servo ddsm115_servo4(0x04);
// 因为我们将使用C++的编译方式，用g++编译器来编译 robot_c_api.cpp 这个文件，
// 所以在这个文件中我们可以用C++代码去定义函数 void Robot_sayHi(const char *name)（在函数中使用C++的类 Robot），
// 最后我们用 extern "C" 来告诉g++编译器，不要对 Robot_sayHi(const char *name) 函数进行name mangling
// 这样最终生成的动态链接库中，函数 Robot_sayHi(const char *name) 将生成 C 编译器的符号表示。

void setTargetAngle(float angle1,float angle2,float angle3,float angle4)
{

    ddsm115_servo1.setTargetAngle(angle1);
    ddsm115_servo2.setTargetAngle(angle2);
    ddsm115_servo3.setTargetAngle(angle3);
    ddsm115_servo4.setTargetAngle(angle4);
}
armquadrover__msg__DDSM115Servo getCurrentAngle(void)
{
    armquadrover__msg__DDSM115Servo ddsm115Servo_msg;
    ddsm115Servo_msg.front_left_servo=ddsm115_servo1.getCurrentAngle();
    ddsm115Servo_msg.rear_left_servo=ddsm115_servo2.getCurrentAngle();
    ddsm115Servo_msg.rear_right_servo=ddsm115_servo3.getCurrentAngle();
    ddsm115Servo_msg.front_right_servo=ddsm115_servo4.getCurrentAngle();

    return ddsm115Servo_msg;

}*/
#ifdef __cplusplus
}
#endif