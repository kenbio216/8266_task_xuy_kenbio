/***
 * @Author: xuyang
 * @Date: 2024-05-26 22:24:49
 * @LastEditors: xuyang
 * @LastEditTime: 2024-05-30 22:08:06
 * @FilePath: \8266_task_xuy_kenbio\mqtt_8266_big_task_esp32c3\Xu_schedule.h
 * @Description:
 * 这是一份风格比较激进的代码。本来是用C写的好好的，突然脑子一抽，为什么不用C++改写呢？
 * 所以，本来是简单的定时器按键扫描+简单的任务指针配合任务调度器实现低延迟的类时间片调度效果，
 * 但是，但是，这也太没意思了吧？如果大家都这么写，那么也太无聊了。
 * 所以就添加了一点点的OOP编程，耗费了三天的心血，把任务调度器和按键扫描封装成类，然后在ino文件中调用。
 * 这样，代码看起来更加简洁，也更加容易理解。
 * 或许吧。。。。。。。2333
 * @
 * @Copyright (c) 2024 by xuyang, All Rights Reserved
 */

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
class Key // 创造了一个按键类。用按键对应连线的引脚作为传入参数，初始化按键
{
public: // 只有三个函数，一个是最重要的检测按键是否被扫描，其他的是封装了清除按键标志位的判断函数，方便通过直接调用函数的方式检测按键
    Key(int pin);
    void scan();
    bool is_short_pressed();
    bool is_long_pressed();

private: // pin用来传入参数，初始化按键引脚，其他的都是按键扫描需要用到的变量
    int pin;
    uint8_t key_sta;
    uint8_t judge_sta;
    uint8_t short_flag;
    uint8_t long_flag;
    uint8_t long_flag_aux; // 辅助防止长按一直触发
    uint32_t timer;
};

// Task类声明
class Task // 创造了一个任务类，一方面与任务调度器配合，另一方便提供函数指针，方便直接调用函数句柄，可以在ino文件中直接写任务函数
{
public:                                              // 难以理解的部分是为了提供调用便利和增加封装性。实际上，它的作用就是每隔rate_ms毫秒就执行一次传入的函数task_func
    Task(void (*task_func)(void), uint16_t rate_ms); // 类的构造函数，提供了传入参数的形式，请参考C++基础
    void run();                                      // 用来让函数指针指向自己写的函数，然后调用这个函数。也就是借助函数指针，隐藏了调用函数的细节，只需要调用run函数就可以了
    bool is_should_run(uint32_t now);                // 用来判断任务是否应该运行，如果任务的运行时间到了，就返回true，否则返回false

private:
    void (*task_func)(void); // 函数指针，为了方便调用我们自己写的任务函数，任务函数名字可以自己取，函数指针直接指向它就可以了，非常方便
    uint16_t rate_ms;        // 这两货是用来根据millis的值计算函数是否应该运行的，确保每隔rate_ms就运行一次
    uint32_t last_run;
};

// Scheduler类声明
class Scheduler // 一个精华部分就是————任务调度器类
{
public:
    Scheduler();                                                         // 构造函数不需要任何值
    void init();                                                         // 初始化函数，根据具体情况改写，放在setup里面
    void run();                                                          // 放在loop里面，运行is_should_run条件为真的任务task
    void add_task(void (*task_func)(void), uint16_t rate_ms);            // 用来添加任务的，也就是让函数指针指向我们自己写的函数，方便调用
    void add_key_event_handler(uint8_t keyIndex, void (*handler)(void)); // 这是我想要把按键扫描函数直接封装在任务调度器内部，所以加了这个。
    void add_key_event_long_press_handler(uint8_t keyIndex, void (*handler)(void)); // 同上，这是长按键的

private:
    Task *tasks[MAX_TASKS];                                   // 任务数组
    uint8_t task_num;                                         // 任务数量，自动计算
    Key keys[MAX_KEYS];                                       // 按键类数组，我是因为想要把按键放在任务调度器内才这样写的
    void (*key_event_handers_arr[MAX_KEYS])(void);            // 按键事件处理函数数组，方便用数组去遍历可以运行的按键事件处理函数
    void (*key_event_handers_long_press_arr[MAX_KEYS])(void); // 同上，用于长按部分
    static void scan_keys_task();                             // 利用数组来遍历
    static Scheduler *instance;                               // 用来为静态对象访问非静态成员变量提供方便。
    // 因为scan_keys_task是静态函数，所以不能直接访问非静态成员变量比如说this指针，所以不能直接访问this下面的成员类可key，
    // 所以需要这个指针
    // 哈哈哈哈哈才不是因为我手贱想试试static的作用，不然不会这么精(ma)彩(fan)
};

#endif // _XU_SCHEDULE_H_
