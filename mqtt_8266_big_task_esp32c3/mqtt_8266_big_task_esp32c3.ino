/*
 * @Author: xuyang
 * @Date: 2024-05-26 22:24:39
 * @LastEditors: xuyang
 * @LastEditTime: 2024-05-29 10:51:13
 * @FilePath: \8266_task_xuy_kenbio\mqtt_8266_big_task_esp32c3\mqtt_8266_big_task_esp32c3.ino
 * @Description:
 *
 * Copyright (c) 2024 by xuyang, All Rights Reserved
 */
// TODO 一份好的代码注释，可以让你少写很多的文档，因为代码本身就是最好的文档
// TODO 有感而发，好的代码有三个特点：1，运行延迟低卡bug少，比如任务调度器
// TODO  2，可读性高简洁明了，比如OOP编程 3，分工明确，没有无效定义，函数见名知意，比如这个代码

#include "Xu_schedule.h"
#include "Dai_tone.h"

#include "SignalProcessing.h"

#define MIC_PIN 4 // GPIO 4 - 接麦克风输入引脚

const int analogPin = 4; // 使用GPIO4作为模拟输入引脚

/* -------------------------------------------------------------------------- */
/*                                   点灯科技部分                                   */
/* -------------------------------------------------------------------------- */
#define BLINKER_PRINT Serial
#define BLINKER_WIFI

#include <Blinker.h>

char auth[] = "92ee9b0c302e";
char ssid[] = "kenbio";
char pswd[] = "123456xuy";

// 新建组件对象
BlinkerButton Button1("btn-abc");
BlinkerNumber Number1("num-abc");

int counter = 0;

// 按下按键即会执行该函数
void button1_callback(const String &state)
{
    BLINKER_LOG("get button state: ", state);
    // digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    Serial.println("点灯科技测试成功");
}

// 如果未绑定的组件被触发，则会执行其中内容
void dataRead(const String &data)
{
    BLINKER_LOG("Blinker readString: ", data);
    counter++;
    Number1.print(counter);
}

/* -------------------------------------------------------------------------- */
/*                                   任务调度器部分                                  */
/* -------------------------------------------------------------------------- */
Scheduler scheduler;
uint8_t music_flag = 0;

void handleButton1()
{
    Serial.println("Button 1 pressed!");
    music_flag = 1;
}

void handleButton2()
{
    Serial.println("Button 2 pressed!");
    music_flag = 0;
    noTone(tonepin);
}

void setup()
{
    pinMode(MIC_PIN, INPUT); // 设置麦克风引脚为输入模式

    Dai_tone_init();
    scheduler.init();
    scheduler.addKeyEventHandler(0, handleButton1);
    scheduler.addKeyEventHandler(1, handleButton2);
    // 添加任务
    scheduler.addTask(led_blink1, 1000);
    scheduler.addTask(led_blink2, 500);

    /* --------------------------------- 点灯科技部分 --------------------------------- */
#if defined(BLINKER_PRINT)
    BLINKER_DEBUG.stream(BLINKER_PRINT);
#endif
    // 初始化blinker
    Blinker.begin(auth, ssid, pswd);
    Blinker.attachData(dataRead);
    Button1.attach(button1_callback);
}

void loop()
{
    int micValue = analogRead(MIC_PIN); // 读取麦克风的模拟值
    processSignal(micValue);            // 处理信号并显示
    
    // 延迟测试，调试用的
    static uint32_t timer;
    Serial.print(millis() - timer);
    Serial.println("ms");
    timer = millis();

    // Blinker.run();
    scheduler.run();

    if (music_flag)
    {
        tone_shang_chun_shan();
    }

}

// led_blink1 和 led_blink2 函数实现
static inline void led_blink1()
{
    digitalWrite(pin_led_01, !digitalRead(pin_led_01));
}

static inline void led_blink2()
{
    digitalWrite(pin_led_02, !digitalRead(pin_led_02));
}
