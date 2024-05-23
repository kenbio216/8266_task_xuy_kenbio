#define BLINKER_WIFI

#include <Blinker.h>

char auth[] = "813f5b2987ae"; // 替代为你的密匙
char ssid[] = "kenbio";       // 2.4Gwifi名称，可以是手机热点，不要有中文
char pswd[] = "123456xuy";    // WiFi密码

// 新建组件对象
BlinkerButton Button1("btn-xy");

// 按下按键即会执行该函数
void button1_callback(const String &state)
{ // 回调函数
    BLINKER_LOG("按键xy被按下 ", state);
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    // Blinker.vibrate();
}

void setup()
{
    // 初始化串口，并开启调试信息，项目成型后可关闭
    Serial.begin(115200);
    BLINKER_DEBUG.stream(Serial);
    // 初始化有LED的IO
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    // 初始化blinker
    Blinker.begin(auth, ssid, pswd);
    Button1.attach(button1_callback); // 绑定回调函数
}

void loop()
{
    Blinker.run();
}