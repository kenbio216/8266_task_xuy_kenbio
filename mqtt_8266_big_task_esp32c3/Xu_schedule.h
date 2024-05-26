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
/* ------------------------------- 引脚定义（这里修改） ------------------------------- */
// 定义D0引脚为GPIO16
const int pin_led_01 = 12;
const int pin_led_02 = 13;

void Scheduler_init(void); // 初始化任务调度器，必须放在setup中
void Scheduler_run(void);  // 任务调度器，必须放在loop中

/* ---------------------------------- 按键部分 ---------------------------------- */
struct Keys
{
    uint8_t key_sta;
    uint8_t judge_sta;
    uint8_t short_flag;
};

extern Keys key[3];
#endif _XU_SCHEDULE_H_
