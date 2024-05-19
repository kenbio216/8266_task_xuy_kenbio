// 定义引脚
const int buttonPin = 13;

void setup()
{
    // 初始化串口
    Serial.begin(115200);

    pinMode(13, INPUT);

    // attachInterrupt(digitalPinToInterrupt(13), litFnc, FALLING);
}

void loop()
{
    // 主循环内容为空，因为我们使用中断处理
}

// 中断处理函数定义
void litFnc()
{
    // delay(50); // 消抖
    if (digitalRead(13) == HIGH)
    {
        Serial.println("Button 13 pressed");
    }
}
