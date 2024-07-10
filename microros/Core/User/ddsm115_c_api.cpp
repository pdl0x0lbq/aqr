#include "ddsm115_c_api.h"
#include "ddsm115.h"
#include "servo.h"
#ifdef __cplusplus
extern "C" {
#endif
ddsm115 wheel1(0x01,0x04);
ddsm115 wheel2(0x02,0xF1);
ddsm115 wheel3(0x03,0x55);
ddsm115 wheel4(0x04,0x02);
servo ddsm115_servo1(0x01);
servo ddsm115_servo2(0x02);
servo ddsm115_servo3(0x03);
servo ddsm115_servo4(0x04);
void setTargetSpeed(int16_t speed1,int16_t speed2,int16_t speed3,int16_t speed4,float angle1,float angle2,float angle3,float angle4)
{
   wheel1.setTargetSpeed(speed1);
   wheel2.setTargetSpeed(speed2);
   wheel3.setTargetSpeed(-speed3);
   wheel4.setTargetSpeed(-speed4);
   ddsm115_servo1.setTargetAngle(4.2-angle1);
   ddsm115_servo2.setTargetAngle(4.2-angle2);
   ddsm115_servo3.setTargetAngle(4.2-angle3);
   ddsm115_servo4.setTargetAngle(4.2-angle4);
}
armquadrover_msgs__msg__DDSM115 getCurrentSpeed(void)
{
    armquadrover_msgs__msg__DDSM115  ddsm115_msg;
    ddsm115_msg.front_left= wheel1.getCurrentSpeed();
    ddsm115_msg.rear_left= wheel2.getCurrentSpeed();
    ddsm115_msg.rear_right= wheel3.getCurrentSpeed();
    ddsm115_msg.front_right= wheel4.getCurrentSpeed();
    ddsm115_msg.front_left_servo=ddsm115_servo1.getCurrentAngle();
    ddsm115_msg.rear_left_servo=ddsm115_servo2.getCurrentAngle();
    ddsm115_msg.rear_right_servo=ddsm115_servo3.getCurrentAngle();
    ddsm115_msg.front_right_servo=ddsm115_servo4.getCurrentAngle();
    return ddsm115_msg;
}
#ifdef __cplusplus
}
#endif