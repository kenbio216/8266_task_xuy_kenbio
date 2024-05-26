#ifndef _XU_SCHEDULE_H_
#define _XU_SCHEDULE_H_

#include <Arduino.h> 
/* ------------------------------- 引脚定义（这里修改） ------------------------------- */
// 定义D0引脚为GPIO16
const int pin_led_blue = 2;
const int pin_led_red = 0;

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
