/*
 * @Author: xuyang
 * @Date: 2024-05-26 22:24:39
 * @LastEditors: xuyang
 * @LastEditTime: 2024-05-28 12:04:16
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

}

void setup()
{
    Dai_tone_init();
    scheduler.init();
    scheduler.addKeyEventHandler(0, handleButton1);
    scheduler.addKeyEventHandler(1, handleButton2);
    // 添加任务
    scheduler.addTask(led_blink1, 1000);
    scheduler.addTask(led_blink2, 500);
}

void loop()
{
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
