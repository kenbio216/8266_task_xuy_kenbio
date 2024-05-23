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

/* ---------------------------------- 类型定义 ---------------------------------- */
uint8_t task_num;

// 定义D0引脚为GPIO16
const int pinD0 = 16;

typedef struct
{
    void (*task_func)(void);
    uint16_t rate_ms;
    uint32_t last_run;
} scheduler_task_t;

/* --------------------------------- 任务函数定义 --------------------------------- */
static void led_blink(void)
{
    static uint8_t led_state = 0;
    if (led_state)
    {
        digitalWrite(pinD0, HIGH);
    }
    else
    {
        digitalWrite(pinD0, LOW);
    }
    led_state++;
    led_state %= 2;
}
static void uart1_test(void)
{
}

/* --------------------------------- 任务调度器 --------------------------------- */

static scheduler_task_t scheduler_task[] =
    {
        {led_blink, 500, 0},
        {uart1_test, 100, 0}};

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

void Scheduler_init(void)
{
    task_num = sizeof(scheduler_task) / sizeof(scheduler_task_t);
}

void setup()
{
    // 设置D0引脚为输出模式
    pinMode(pinD0, OUTPUT);
    Scheduler_init();
}

void loop()
{
    Scheduler_run();
}
