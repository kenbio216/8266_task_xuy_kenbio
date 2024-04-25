#define FANPIN 16    //定义风扇信号线接口
int speed;    //用于存储风扇转速值
 
void setup()
{
  Serial.begin(115200);    //串口波特率设置为115200
  pinMode(A0,INPUT);    //定义A0模拟口为输入
}
 
void loop()
{
  speed=map(analogRead(A0), 0, 1023, 0, 1500);    //读取模拟口A0的数值，并将其从0~1023的范围变换到0~1500的范围
  analogWrite(FANPIN,speed);     //向风扇写入速度值
  Serial.print("风扇当前速度：");    //串口输出风扇当前速度值
  Serial.print(speed);
  Serial.println("r/min");
  delay(200);
}
