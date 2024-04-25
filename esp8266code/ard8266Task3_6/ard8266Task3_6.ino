#include <DHT.h>
#define BUTTON 13      //定义按钮信号线接口
#define FANPIN 16      //定义风扇信号线接口
#define DHTPIN 5       //定义温湿度传感器信号线接口
#define DHTTYPE DHT11  //定义温湿度传感器类型为DHT11
boolean flag = 0;
 DHT dht(DHTPIN, DHTTYPE);
void setup() {
  pinMode(BUTTON, INPUT);  //定义按钮为输入
  Serial.begin(115200);
  dht.begin();
}

void loop() {
  float temp = dht.readTemperature();
  Serial.print("当前室温是：");
  Serial.print(temp);
  Serial.println("°C");
  if (isnan(temp)) {
    Serial.println("DHT11读取失败！");
    delay(500);
    return;
  }
   if (digitalRead(BUTTON) == 0)  //按下停止按钮时,风扇停止运转
  {
    flag = 0;
  }
  
  if (digitalRead(BUTTON) == 1)  //按下停止按钮时,风扇停止运转
  {
    analogWrite(FANPIN, 0);  //将转速值0写入风扇信号口
    delay(1000);
    Serial.println("风扇手动停止运转");
    flag = 1;
  }
 

  else if (flag == 0) {
    if (temp > 22.5) {
      analogWrite(FANPIN, 1500);  //将转速值1500写入风扇信号口
      delay(1000);
      Serial.println("风扇正在高速运行");
    }

    else if (temp > 21.5 && temp <= 22.5) {
      analogWrite(FANPIN, 500);  //将转速值500写入风扇信号口
      delay(1000);
      Serial.println("风扇正在低速运行");
    }

    else {
      analogWrite(FANPIN, 0);  //将转速值0写入风扇信号口
      delay(1000);
      Serial.println("风扇自动停止运转");
    }
  } else 
  {delay(1000);
  Serial.println("风扇手动停止运转");
  }
}