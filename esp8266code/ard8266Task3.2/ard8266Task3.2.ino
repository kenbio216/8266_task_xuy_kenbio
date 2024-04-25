#include <Servo.h> 
#define BUTTON 13  //令变量BUTTON为0，0对应ESP8266开发板上端口0
Servo myservo;       //创建一个舵机控制对象  
int pos;                    //用于存储舵机角度位置的变量
void setup(){
pinMode(BUTTON,INPUT);  //定义按钮为输入
myservo.attach(4);               //舵机控制线连接开发板数字5脚
Serial.begin(9600);}              //串口波特率设置为9600  
void loop(){
if (digitalRead(BUTTON) == 1){       //按下按钮时，舵机连续可逆旋转
  for(pos=0;pos<180;pos++){          //从0°到180°运动，每次步进1°
    myservo.write(pos);                      //向舵机写入角度值     
    delay(20);                                      //等待20ms让舵机到达指定位置         
    Serial.print("舵机当前角度值：");   //串口输出舵机当前角度值
    Serial.println(pos);}
  for(pos=179;pos>=0;pos--){          //从180°到0°运动，每次步进1°
    myservo.write(pos);
    delay(20);
    Serial.print("舵机当前角度值：");
    Serial.println(pos);}}
else{
  myservo.write(0);
  delay(3000);  //舵机静止时，每隔3s向串口输出提示信息         
  Serial.println("按钮未按下，舵机静止、当前角度值：0");}}
