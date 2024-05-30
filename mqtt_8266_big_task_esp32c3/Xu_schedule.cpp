#include "Xu_schedule.h"

Scheduler *Scheduler::instance = nullptr; // 初始化静态成员变量

// Key类实现
Key::Key(int pin) : pin(pin), key_sta(0), judge_sta(0), short_flag(0), long_flag(0), long_flag_aux(0) // 使用成员初始化列表对一些私有参数进行初始化
{
    pinMode(pin, INPUT_PULLUP); // 使用构造函数进行一些默认操作，这里没有再初始化其他的参数，而是设置了按键的引脚模式
}

void Key::scan() // 按键扫描函数，灵感来自于蓝桥杯的定时器扫描按键
{
    key_sta = digitalRead(pin);
    switch (judge_sta)
    {
    case 0:
        if (key_sta == 0)
        {
            judge_sta = 1;
            timer = 0;
            long_flag_aux = 1;
        }

        break;
    case 1:
        if (key_sta == 0)
            judge_sta = 2;
        else
            judge_sta = 0;
        break;
    case 2:
        if (key_sta == 1 && timer < 80)
        {
            judge_sta = 0;
            short_flag = 1;
        }
        else if (key_sta == 1 && timer > 80)
        {
            judge_sta = 0;
        }
        else
        {
            timer++;
        }
        if (timer > 100 && key_sta == 0 && long_flag_aux == 1)
        {
            long_flag = 1;
            long_flag_aux = 0;
        }
        break;
    }
}

bool Key::is_short_pressed() // 判断按键是否短按，顺便清理短按标志位
{
    if (short_flag)
    {
        short_flag = 0;
        return true;
    }
    return false;
}

bool Key::is_long_pressed() // 类似同上
{
    if (long_flag)
    {
        long_flag = 0;
        return true;
    }
    return false;
}

// Task类实现
Task::Task(void (*task_func)(void), uint16_t rate_ms)
    : task_func(task_func), rate_ms(rate_ms), last_run(0) {} // 没啥好说的，最简单的一个

bool Task::is_should_run(uint32_t now)
{
    return (now - last_run >= rate_ms);
}

void Task::run()
{
    task_func();
    last_run = millis();
}

// Scheduler类实现
Scheduler::Scheduler()                                        // 主要是有两个数组，一个是任务数组，一个是按键数组
    : task_num(0), keys{Key(pin_button_1), Key(pin_button_2)} // 初始化任务数量为0，初始化按键数组
{
    instance = this;                       // 用于在静态成员函数中访问非静态成员
    for (uint8_t i = 0; i < MAX_KEYS; i++) // 逐个初始化按键事件处理函数数组
    {
        key_event_handers_arr[i] = nullptr;
        key_event_handers_long_press_arr[i] = nullptr;
    }
}

void Scheduler::init() // 打开波特率通信，开启引脚模式，添加默认的按键扫描任务
{
    Serial.begin(115200);
    pinMode(pin_led_01, OUTPUT);
    pinMode(pin_led_02, OUTPUT);
    pinMode(tonepin, OUTPUT);

    add_task(scan_keys_task, 10); // 添加按键扫描任务
}

void Scheduler::run() // 每隔一段时间就运行一次任务
{
    uint32_t now = millis();
    for (uint8_t i = 0; i < task_num; i++)
    {
        if (tasks[i]->is_should_run(now))
        {
            tasks[i]->run();
        }
    }
}

void Scheduler::add_task(void (*task_func)(void), uint16_t rate_ms) // 使用C++的new函数来为一个类数组添加新的成员
{
    if (task_num < MAX_TASKS)
    { // 确保任务数量不超过最大任务数
        tasks[task_num] = new Task(task_func, rate_ms);
        task_num++;
    }
}

void Scheduler::add_key_event_handler(uint8_t keyIndex, void (*handler)(void)) // 方便在ino文件中直接写处理函数
{
    if (keyIndex < MAX_KEYS)
    { // 确保按键索引不超过数组大小
        key_event_handers_arr[keyIndex] = handler;
    }
}

void Scheduler::add_key_event_long_press_handler(uint8_t keyIndex, void (*handler)(void)) // 同上
{
    if (keyIndex < MAX_KEYS)
    { // 确保按键索引不超过数组大小
        key_event_handers_long_press_arr[keyIndex] = handler;
    }
}

void Scheduler::scan_keys_task() // 作为按键扫描函数，用于检测按键是否被按下，如果按下，调用对应的事件处理函数，也就是所谓的hander句柄
{
    for (uint8_t i = 0; i < MAX_KEYS; i++)
    {
        instance->keys[i].scan();
        if (instance->keys[i].is_short_pressed() && instance->key_event_handers_arr[i] != nullptr) // 按键短按了，就执行短按对应的事件处理函数
        {
            instance->key_event_handers_arr[i](); // 调用按键事件处理函数
            Serial.printf("short press on key %d\n", i + 1);
        }
        if (instance->keys[i].is_long_pressed() && instance->key_event_handers_long_press_arr[i] != nullptr) // 按键长按了，就执行短按对应的事件处理函数
        {
            instance->key_event_handers_long_press_arr[i](); // 调用长按事件处理函数
            Serial.printf("Long press on key %d\n", i + 1);
        }
    }
}
