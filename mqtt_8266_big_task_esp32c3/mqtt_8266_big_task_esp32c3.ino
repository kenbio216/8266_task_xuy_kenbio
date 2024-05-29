/*
 * @Author: xuyang
 * @Date: 2024-05-26 22:24:39
 * @LastEditors: xuyang
 * @LastEditTime: 2024-05-29 11:45:44
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

/* -------------------------------------------------------------------------- */
/*                                   任务调度器部分                                  */
/* -------------------------------------------------------------------------- */
Scheduler scheduler;
uint8_t mode_flag = 0;
uint8_t blinker_flag = 0;

void handleButton1()
{
    Serial.println("Button 1 pressed!");
    mode_flag++;
    mode_flag = mode_flag % 4;
}

void handleButton2()
{
    Serial.println("Button 2 pressed!");
    blinker_flag++;
    blinker_flag = blinker_flag % 2;
}

void setup()
{
    pinMode(MIC_PIN, INPUT); // 设置麦克风引脚为输入模式

    Dai_tone_init();
    scheduler.init();
    scheduler.addTask(led_blink1, 500);
    addKeyEventHandler(0,handleButton1 );
    addKeyEventHandler(1,handleButton2 );

    // scheduler.addTask([](){ Blinker.run(); }, 700);
}

void loop()
{

    // 延迟测试，调试用的
    // static uint32_t timer;
    // Serial.print(millis() - timer);
    // Serial.println("ms");
    // timer = millis();

    scheduler.run();

    /* ------------------------------ 点灯科技，MQTT的部分 ------------------------------ */
    if (blinker_flag)
    {
        Blinker.run();
    }

    switch (mode_flag)
    {
    case 0:
        tone_shang_chun_shan();
        break;
    case 1:
        tone_da_yu();
        break;
    case 2:
        tone_yuan_yu_chou();
        break;
    case 3:
        int micValue = analogRead(MIC_PIN); // 读取麦克风的模拟值
        processSignal(micValue);            // 处理信号并显示
        break;
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

// 按下按键即会执行该函数
void button1_callback(const String &state)
{
    BLINKER_LOG("get button state: ", state);
    mode_flag++;
    mode_flag = mode_flag % 4;
    Serial.println("点灯科技测试成功");
}

// 如果未绑定的组件被触发，则会执行其中内容
void dataRead(const String &data)
{
    static int counter = 0;
    BLINKER_LOG("Blinker readString: ", data);
    counter++;
    Number1.print(counter);
}