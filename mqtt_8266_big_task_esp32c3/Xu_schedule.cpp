/***
 * @Author: xuyang
 * @Date: 2024-05-23 08:20:45
 * @LastEditors: xuyang
 * @LastEditTime: 2024-05-23 08:21:24
 * @FilePath: \8266_task_xuy_kenbio\mqtt_8266_big_task_aux03\Xu_schedule.cpp
 * @Description:
 * @
 * @Copyright (c) 2024 by xuyang, All Rights Reserved
 */

#include "Xu_schedule.h"

/* ---------------------------------- 类型定义（不用改） ---------------------------------- */
uint8_t task_num;

typedef struct
{
    void (*task_func)(void);
    uint16_t rate_ms;
    uint32_t last_run;
} scheduler_task_t;

/* ---------------------------------- 按键部分 ---------------------------------- */

Keys key[2] = {0, 0, 0}; // 定义并初始化变量

static void key_scan(void)
{

    key[0].key_sta = digitalRead(pin_button_1);
    key[1].key_sta = digitalRead(pin_button_2);

    // if(key[0].key_sta == 0)
    // {
    //     delay(10);
    //     if(digitalRead(pin_button_1) == 0)
    //     {
    //         key[0].short_flag = 1;
    //         while(digitalRead(pin_button_1) == 0)
    //         {
    //         }
    //     }
    // }
    for (uint8_t i = 0; i < 2; i++)
    {
        switch (key[i].judge_sta)
        {
        case 0:
            if (key[i].key_sta == 0)
            {
                key[i].judge_sta = 1;
            }
            break;
        case 1:
            if (key[i].key_sta == 0)
                key[i].judge_sta = 2;
            else
                key[i].judge_sta = 0;
            break;
        case 2:
            if (key[i].key_sta == 1)
            {
                key[i].judge_sta = 0;
                key[i].short_flag = 1;
            }
            else
            {
                //?? woc，把这里注释掉按键就正常了。最奇怪的是明明换一套硬件不注释反而是更好使的
                // key[i].judge_sta = 0;
            }
        }
    }
}
/* --------------------------------- 任务函数定义(这里修改) --------------------------------- */
static void led_blink(void)
{
    static uint8_t led_state = 0;
    if (led_state)
    {
        digitalWrite(pin_led_02, HIGH);
        // Serial.println("led灯正常闪烁！");
    }
    else
    {
        digitalWrite(pin_led_02, LOW);
    }
    led_state++;
    led_state %= 2;
}
static void uart1_test(void)
{
}
/* -------------------------------------------------------------------------- */
/*                                     修改我                                    */
/* -------------------------------------------------------------------------- */
static scheduler_task_t scheduler_task[] =
    {
        {led_blink, 500, 0},
        {key_scan, 5, 0}};

/* -------------------------------------------------------------------------- */
/*                            会不断遍历函数指针，并运行可以运行的任务函数                            */
/* -------------------------------------------------------------------------- */
void Scheduler_run(void)
{
    for (int i = 0; i < task_num; i++)
    {
        uint32_t now_time = millis();
        if (now_time - scheduler_task[i].last_run >= scheduler_task[i].rate_ms)
        {
            scheduler_task[i].task_func();
            scheduler_task[i].last_run = now_time;
        }
    }
}

/* -------------------------------------------------------------------------- */
/*                                 记得在主循环中添加我                                 */
/* -------------------------------------------------------------------------- */
void Scheduler_init(void)
{
    Serial.begin(115200);
    pinMode(pin_button_1, INPUT_PULLUP); // 设置按钮1
    pinMode(pin_button_2, INPUT_PULLUP); // 设置按钮2
    pinMode(pin_led_01, OUTPUT);         // 打开一号灯
    pinMode(pin_led_02, OUTPUT);         // 打开二号灯
    pinMode(tonepin, OUTPUT);            // 打开蜂鸣器
    task_num = sizeof(scheduler_task) / sizeof(scheduler_task_t);
    // 如果你使用的是ESP32，下面的代码可以初始化LEDC
    ledcSetup(0, 1000, 8);     // 通道0，频率1000Hz，分辨率8位
    ledcAttachPin(tonepin, 0); // 将通道0与TONE_PIN关联
}
