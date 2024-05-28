#ifndef _XU_SCHEDULE_H_
#define _XU_SCHEDULE_H_

#include <Arduino.h>
#include <MD_Parola.h>
#include <MD_MAX72XX.h>
#include <SPI.h>

// 引脚定义
const int pin_led_01 = 12; // D4灯
const int pin_led_02 = 13; // D5灯
const int tonepin = 6;
#define DATA_PIN 3 // GPIO 13 - D7
#define CS_PIN 7   // GPIO 15 - D8
#define CLK_PIN 2  // GPIO 14 - D5
const int pin_button_1 = 0;
const int pin_button_2 = 1;

// Key类声明
class Key
{
public:
    Key(int pin);
    void scan();
    bool isShortPressed();

private:
    int pin;
    uint8_t key_sta;
    uint8_t judge_sta;
    uint8_t short_flag;
};

// Task类声明
class Task
{
public:
    Task(void (*task_func)(void), uint16_t rate_ms);
    void run();
    bool shouldRun(uint32_t now);

private:
    void (*task_func)(void);
    uint16_t rate_ms;
    uint32_t last_run;
};

// Scheduler类声明
class Scheduler
{
public:
    Scheduler();
    void init();
    void run();
    void addTask(void (*task_func)(void), uint16_t rate_ms);

private:
    Task *tasks[10]; // 假设最多有10个任务
    uint8_t task_num;
    Key keys[2];
};

// 任务函数声明
static inline void led_blink1();
static inline void led_blink2();

#endif _XU_SCHEDULE_H_
