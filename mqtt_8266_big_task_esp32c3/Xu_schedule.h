/***
 * @Author: xuyang
 * @Date: 2024-05-26 22:24:49
 * @LastEditors: xuyang
 * @LastEditTime: 2024-05-26 22:28:44
 * @FilePath: \8266_task_xuy_kenbio\mqtt_8266_big_task_esp32c3\Xu_schedule.h
 * @Description:
 * @
 * @Copyright (c) 2024 by xuyang, All Rights Reserved
 */
#ifndef _XU_SCHEDULE_H_
#define _XU_SCHEDULE_H_

#include <Arduino.h>
#include <MD_Parola.h>
#include <MD_MAX72XX.h>
#include <SPI.h>

#include "Dai_tone.h"

/* ------------------------------- 引脚定义（这里修改） ------------------------------- */
// LED灯的引脚定义
const int pin_led_01 = 12; // D4灯
const int pin_led_02 = 13; // D5灯

// 定义蜂鸣器引脚
const int tonepin = 6;

// 四合一点阵的引脚定义
#define DATA_PIN 3 // GPIO 13 - D7
#define CS_PIN 7   // GPIO 15 - D8
#define CLK_PIN 2  // GPIO 14 - D5

// 按键的引脚定义
const int pin_button_1 = 0;
const int pin_button_2 = 1;
// const int pin_button_3 = 5;

void Scheduler_init(void); // 初始化任务调度器，必须放在setup中
void Scheduler_run(void);  // 任务调度器，必须放在loop中

/* ---------------------------------- 按键部分 ---------------------------------- */
struct Keys
{
    uint8_t key_sta;
    uint8_t judge_sta;
    uint8_t short_flag;
};

extern Keys key[2];
#endif _XU_SCHEDULE_H_
