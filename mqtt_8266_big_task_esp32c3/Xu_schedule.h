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

const uint8_t MAX_TASKS = 10; // 任务的最大数量
const uint8_t MAX_KEYS = 2;   // 按键的最大数量

// Key类声明
class Key {
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
class Task {
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
class Scheduler {
public:
    Scheduler();
    void init();
    void run();
    void addTask(void (*task_func)(void), uint16_t rate_ms);
    void addKeyEventHandler(uint8_t keyIndex, void (*handler)(void));
private:
    Task* tasks[MAX_TASKS]; // 任务数组
    uint8_t task_num;
    Key keys[MAX_KEYS];
    void (*keyEventHandlers[MAX_KEYS])(void); // 按键事件处理函数数组
    static void scanKeysTask();
    static Scheduler* instance;
};

#endif // _XU_SCHEDULE_H_
