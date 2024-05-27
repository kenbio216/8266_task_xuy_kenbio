/*
 * @Author: xuyang
 * @Date: 2024-05-26 22:24:39
 * @LastEditors: xuyang
 * @LastEditTime: 2024-05-27 22:44:05
 * @FilePath: \8266_task_xuy_kenbio\mqtt_8266_big_task_esp32c3\mqtt_8266_big_task_esp32c3.ino
 * @Description:
 *
 * Copyright (c) 2024 by xuyang, All Rights Reserved
 */
#include "Xu_schedule.h"
#include "Dai_tone.h"

uint8_t music_flag = 0;

void setup()
{
    Dai_tone_init();
    Scheduler_init();
}
void loop()
{

    Scheduler_run();
    key_proc();
    if (music_flag)
    {
        tone_shang_chun_shan();
    }
}

void key_proc(void)
{
    if (key[1].short_flag)
    {
        Serial.println("按键2被按下咯");
        digitalWrite(pin_led_01, !digitalRead(pin_led_01)); // 翻转2号灯
        music_flag = 1;
        key[1].short_flag = 0;
    }
    if (key[0].short_flag)
    {
        Serial.println("按键1被按下咯");
        digitalWrite(pin_led_01, !digitalRead(pin_led_01)); // 翻转2号灯
        music_flag = 0;
        noTone(tonepin);
        key[0].short_flag = 0;
    }
}