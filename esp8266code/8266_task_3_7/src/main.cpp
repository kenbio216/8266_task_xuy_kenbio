/* ---------------------------------- 库文件导入 --------------------------------- */
#include <DHT.h>
#include <Servo.h>
#include "stdio.h"
#include "string.h"
#include "stdbool.h"
/* ---------------------------------- 声明变量 ---------------------------------- */
Servo myservo;  // 创建一个舵机控制对象
int pos = 0;    // 该变量用于存储舵机角度位置
float temp = 0; // 初始化一个温度值

#define BUTTON1 14    // 定义按钮信号线接口
#define BUTTON2 16    // 定义按钮信号线接口
#define BUTTON3 4     // 定义按钮信号线接口
#define BUTTON4 5     // 定义按钮信号线接口
#define FANPIN 15     // 定义风扇信号线接口
#define SERVO_PIN 13  // 定义舵机信号线接口
#define DHTPIN 0      // 定义温湿度传感器信号线接口
#define DHTTYPE DHT11 // 定义温湿度传感器类型为DHT11

#define u8 uint8_t
/* ---------------------------------- 按键部分 ---------------------------------- */
struct Key
{
  uint8_t key_sta;
  uint8_t judge_sta;
  uint8_t short_flag;
};

Key key[4] = {
    {!digitalRead(BUTTON1), 0, 0},
    {!digitalRead(BUTTON2), 0, 0},
    {!digitalRead(BUTTON3), 0, 0},
    {!digitalRead(BUTTON4), 0, 0}};

void key_scan()
{
  for (u8 i = 0; i < 4; i++)
  {
    switch (key[i].judge_sta)
    {
    case 0:
      if (key[i].key_sta == 0)
      {
        key[i].judge_sta = 1;
      }

      break;
    case 1:
      if (key[i].key_sta == 0)
        key[i].judge_sta = 2;
      else
        key[i].judge_sta = 0;
      break;
    case 2:
      if (key[i].key_sta == 1)
      {
        key[i].judge_sta = 0;
        key[i].short_flag = 1;
      }
      else
      {
        key[i].judge_sta = 0;
      }
    }
  }
}
enum FAN_STATE
{
  FAN_STOP = 0,
  FAN_MAN,
  FAN_AUTO
};
enum FAN_INDEX
{
  FAN_HIGH_SPEED,
  FAN_LOW_SPEED,
  FAN_GET_STOP
};
enum SERVO_INDEX
{
  SERVO_ON,
  SERVO_OFF
};
FAN_STATE fanState = FAN_AUTO; // 初始化风扇状态
FAN_INDEX fanIndex = FAN_GET_STOP;
SERVO_INDEX servoIndex = SERVO_OFF;
// 根据按键结果风扇出现不同状态
void key_proc()
{
  if (key[0].short_flag) // 打开总开关，控制停止和启动
  {
    if (fanState != FAN_STOP)
      fanState = FAN_STOP;
    else
      fanState = FAN_AUTO;

    key[0].short_flag = 0;
  }
  if (key[1].short_flag) // 风扇的模式切换
  {
    if (fanState == FAN_AUTO)
    {
      fanState = FAN_MAN;
      fanIndex = FAN_GET_STOP;
    }
    else
      fanState = FAN_AUTO;
    key[1].short_flag = 0;
  }
  if (key[2].short_flag) // 风扇的档位调节
  {
    if (fanIndex == FAN_HIGH_SPEED)
      fanIndex = FAN_LOW_SPEED;
    else if (fanIndex == FAN_LOW_SPEED)
      fanIndex = FAN_GET_STOP;
    else
      fanIndex = FAN_HIGH_SPEED;
    key[2].short_flag = 0;
  }
  if (key[3].short_flag) // 风扇的摇头控制
  {
    if (servoIndex == SERVO_OFF)
      servoIndex = SERVO_ON;
    else
      servoIndex = SERVO_OFF;
    key[3].short_flag = 0;
  }
}
/* ---------------------------------- 风扇控制 ---------------------------------- */
void fan_proc()
{
  switch (fanState)
  {
  case (FAN_STOP):
  {
    analogWrite(FANPIN, 0); // 将转速值写入风扇信号口
    servoIndex = SERVO_OFF;
    Serial.println("风扇处于停止运作状态");
    break;
  }
  case (FAN_MAN):
  {
    Serial.println("风扇处于手动模式");
    switch (fanIndex)
    {
    case FAN_HIGH_SPEED:
    {
      analogWrite(FANPIN, 1500); // 将转速值写入风扇信号口
      Serial.println("风扇手动高速运转");
      break;
    }
    case FAN_LOW_SPEED:
    {
      analogWrite(FANPIN, 500); // 将转速值写入风扇信号口
      Serial.println("风扇手动低速运转");
      break;
    }
    case FAN_GET_STOP:
    {
      analogWrite(FANPIN, 0); // 将转速值写入风扇信号口
      Serial.println("风扇手动停止运转");
      break;
    }
    }
    break;
  }
  case (FAN_AUTO):
  {
    Serial.println("风扇处于自动模式");
    servoIndex = SERVO_ON;
    if (temp > 30)
    {
      analogWrite(FANPIN, 1500); // 将转速值1500写入风扇信号口
      Serial.println("风扇自动高速运行");
    }

    else if (temp > 25 && temp <= 30)
    {
      analogWrite(FANPIN, 500); // 将转速值500写入风扇信号口
      Serial.println("风扇自动低速运行");
    }

    else
    {
      analogWrite(FANPIN, 0); // 将转速值0写入风扇信号口
      Serial.println("风扇自动停止运转");
    }
    break;
  }
  }
}
/* ---------------------------------- 舵机控制 ---------------------------------- */
void servo_proc()
{
  static uint32_t timer, last_timer;
  timer = millis();
  if (timer - last_timer > 1000)
  {
    switch (servoIndex)
    {
    case (SERVO_ON):
    {
      for (pos = 0; pos <= 180; pos += 1) // 从0度到180度
      {
        myservo.write(pos); // 将角度值写入舵机信号口
        delay(15);          // 等待15ms
      }
      for (pos = 180; pos >= 0; pos -= 1) // 从180度到0度
      {
        myservo.write(pos); // 将角度值写入舵机信号口
        delay(15);          // 等待15ms
      }
      break;
    }
    case (SERVO_OFF):
      break;
    }
  }
}
DHT dht(DHTPIN, DHTTYPE);
void setup()
{
  pinMode(BUTTON1, INPUT); // 定义按钮为输入
  pinMode(BUTTON2, INPUT); // 定义按钮为输入
  pinMode(BUTTON3, INPUT); // 定义按钮为输入
  pinMode(BUTTON4, INPUT); // 定义按钮为输入

  myservo.attach(SERVO_PIN, 500, 2500); // 舵机控制线连接开发板数字4脚

  Serial.begin(115200);
  dht.begin();
}

void loop()
{
  temp = dht.readTemperature();
  Serial.print("当前室温是：");
  Serial.print(temp);
  Serial.println("°C");
  // Serial.print("风扇标志位：");
  // Serial.println(fanFlag);
  // Serial.print("摇头标志位：");
  // Serial.println(fanShakeHead);

  if (isnan(temp)) // 检查DHT11是否有问题
  {
    Serial.println("DHT11读取失败！");
    delay(500);
    return;
  }
  key_scan();
  key_proc();
  fan_proc();
  servo_proc();

  delay(10);
}