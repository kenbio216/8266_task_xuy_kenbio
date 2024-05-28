/*
 * @Author: xuyang
 * @Date: 2024-05-26 22:24:39
 * @LastEditors: xuyang
 * @LastEditTime: 2024-05-28 10:57:14
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
void setup()
{
    Dai_tone_init();
    scheduler.init();
}
void loop()
{
    scheduler.run();
}
// void key_proc(void)
// {
//     if (key[1].short_flag)
//     {
//         Serial.println("按键2被按下咯");
//         digitalWrite(pin_led_01, !digitalRead(pin_led_01)); // 翻转2号灯
//         music_flag = 1;
//         key[1].short_flag = 0;
//     }
//     if (key[0].short_flag)
//     {
//         Serial.println("按键1被按下咯");
//         digitalWrite(pin_led_01, !digitalRead(pin_led_01)); // 翻转2号灯
//         music_flag = 0;
//         noTone(tonepin);
//         key[0].short_flag = 0;
//     }
// }