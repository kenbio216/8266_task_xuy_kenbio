#define FANPIN 16    //定义风扇信号线接口
#define BUTTON 0    //定义按钮信号线接口
 
void setup()
{
  pinMode(BUTTON,INPUT);    //定义按钮为输入
  Serial.begin(9600);    //串口波特率设置为9600
}
 
void loop()
{
  if (digitalRead(BUTTON) == 0)    //按下按钮时，风扇旋转
  {
    analogWrite(FANPIN,1000);    //将转速值1000写入风扇信号口
    delay(2000); 
    Serial.println("风扇正在工作");
  }
  else
  {
    analogWrite(FANPIN,0);    //将转速值0写入风扇信号口
    delay(2000); 
    Serial.println("风扇未转动");
  }
}
