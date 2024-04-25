#include <DHT.h>
#include <Servo.h>
Servo myservo;  // 创建一个舵机控制对象
int pos = 0;    // 该变量用于存储舵机角度位置

#define BUTTON1 14     //定义按钮信号线接口
#define BUTTON2 16     //定义按钮信号线接口
#define BUTTON3 4      //定义按钮信号线接口
#define BUTTON4 5      //定义按钮信号线接口
#define FANPIN 15      //定义风扇信号线接口
#define DHTPIN 0       //定义温湿度传感器信号线接口
#define DHTTYPE DHT11  //定义温湿度传感器类型为DHT11
boolean flag = 0;
uint8_t fanHandFlag = 0;


float temp;

uint8_t Bu1 = 0;
uint8_t Bu2 = 0;
uint8_t Bu3 = 0;
uint8_t Bu4 = 0;

uint8_t fanFlag = 0;
uint8_t fanHighSpeed = 0;
uint8_t fanShakeHead = 0;

DHT dht(DHTPIN, DHTTYPE);
void setup() {
  pinMode(BUTTON1, INPUT);  //定义按钮为输入
  pinMode(BUTTON2, INPUT);  //定义按钮为输入
  pinMode(BUTTON3, INPUT);  //定义按钮为输入
  pinMode(BUTTON4, INPUT);  //定义按钮为输入

  myservo.attach(15, 500, 2500);  // 舵机控制线连接开发板数字4脚

  Serial.begin(115200);
  dht.begin();
}

void xyKeyScan() {  //按钮扫描函数
  Bu1 = digitalRead(BUTTON1);
  Bu2 = digitalRead(BUTTON2);
  Bu3 = digitalRead(BUTTON3);
  Bu4 = digitalRead(BUTTON4);
}
void xyKeyProc() {
  if (digitalRead(BUTTON1)) {
    fanFlag = 1;
  }
  if (digitalRead(BUTTON2)) {
    fanFlag = 0;
  }
  if (digitalRead(BUTTON3)) {
    if (fanHighSpeed == 1)
      fanHighSpeed = 0;
    else if (fanHighSpeed == 0)
      fanHighSpeed = 1;
  }
  if (digitalRead(BUTTON4)) {
    if (fanShakeHead == 1)
      fanShakeHead = 0;
    else if (fanShakeHead == 0)
      fanShakeHead = 1;
  }
}

void mainProc() {
  static uint8_t flagF = 0;
  if (fanShakeHead) {
    if (flagF) {
      myservo.write(0);  // 指定舵机转向的角°，写入角度值
      delay(70);         // 等待15ms让舵机到达指定位置
      flagF = 0;
    }
    if (!flagF) {
      myservo.write(180);  // 指定舵机转向的角°，写入角度值
      delay(70);           // 等待15ms让舵机到达指定位置
      flagF = 1;
    }
  }


  if (fanFlag) {

    if (fanHighSpeed) {
      analogWrite(FANPIN, 1500);  //将转速值写入风扇信号口
      Serial.println("风扇高速运转");
    } else {
      analogWrite(FANPIN, 500);  //将转速值写入风扇信号口
      Serial.println("风扇低速运转");
    }
  } else {
    analogWrite(FANPIN, 0);  //将转速值写入风扇信号口
    Serial.println("风扇手动停止运转");
  }
}

void xyAutoFAN() {  //风扇自动运行
  if (!fanFlag) return;
  if (!fanHandFlag) return;

  if (temp > 30) {
    // analogWrite(FANPIN, 1500);  //将转速值1500写入风扇信号口
    fanHighSpeed = 1;
    // delay(1000);
    Serial.println("风扇自动高速运行");
  }

  else if (temp > 25 && temp <= 30) {
    // analogWrite(FANPIN, 500);  //将转速值500写入风扇信号口
    fanHighSpeed = 0;
    // delay(1000);
    Serial.println("风扇自动低速运行");
  }

  else {
    analogWrite(FANPIN, 0);  //将转速值0写入风扇信号口
    // delay(10);
    Serial.println("风扇自动停止运转");
  }
}

void loop() {
  temp = dht.readTemperature();
  Serial.print("当前室温是：");
  Serial.print(temp);
  Serial.println("°C");
  Serial.print("风扇标志位：");
  Serial.println(fanFlag);
  Serial.print("摇头标志位：");
  Serial.println(fanShakeHead);


  if (isnan(temp))  //检查DHT11是否有问题
  {
    Serial.println("DHT11读取失败！");
    delay(500);
    return;
  }

  xyKeyScan();
  xyKeyProc();
  mainProc();
  xyAutoFAN();
  delay(10);
}