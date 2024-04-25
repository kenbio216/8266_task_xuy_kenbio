#include <Servo.h>
Servo myservo;  //创建一个舵机控制对象
void setup() {
  myservo.attach(4);  //舵机控制线连接开发板数字4脚
  Serial.begin(115200);
  pinMode(A0, INPUT);  //定义A0模拟口为输入
}
void loop() {
  // Serial.print("开始土壤湿度值：");  //串口输出当前土壤湿度值
  // Serial.println(analogRead(A0));
  if (analogRead(A0) < 20)  //土壤湿度临界值根据实际情况自行调整
  {
    myservo.write(0);  //向舵机写入角度值0°
    delay(500);
    myservo.write(60);  //0.5s后让舵机到达60°
    delay(500);
  } else {
    myservo.write(0);
    delay(3000);
  }
  Serial.print("当前土壤湿度值：");  //串口输出当前土壤湿度值
  Serial.println(analogRead(A0));
}
