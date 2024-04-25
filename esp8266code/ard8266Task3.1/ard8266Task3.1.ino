#include <Servo.h>
Servo myservo; // 创建一个舵机控制对象
int pos = 0;   // 该变量用于存储舵机角度位置
/*~~~~~~~~~~~~~~分割线~~~~~~~~~~~~~~ */
void setup()
{
  myservo.attach(4,500,2500); // 舵机控制线连接开发板数字4脚
}
/*~~~~~~~~~~~~~~分割线 ~~~~~~~~~~~~~~ */
void loop()
{
  for (pos = 0; pos < 180; pos += 1) // 从0°到180°运动
  {                                  // 每次步进1°
    myservo.write(pos);              // 指定舵机转向的角°，写入角度值
    delay(15);                       // 等待15ms让舵机到达指定位置
  }
  for (pos = 180; pos >= 1; pos -= 1) // 从180°到0°运动
  {
    myservo.write(pos); // 指定舵机转向的角度，写入角度值
    delay(15);          // 等待15ms让舵机到达指定位置
  }
}
