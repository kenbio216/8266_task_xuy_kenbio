#include "Xu_schedule.h"

Scheduler *Scheduler::instance = nullptr;

// Key类实现
Key::Key(int pin) : pin(pin), key_sta(0), judge_sta(0), short_flag(0)
{
    pinMode(pin, INPUT_PULLUP);
}

void Key::scan()
{
    key_sta = digitalRead(pin);
    switch (judge_sta)
    {
    case 0:
        if (key_sta == 0)
            judge_sta = 1;
        break;
    case 1:
        if (key_sta == 0)
            judge_sta = 2;
        else
            judge_sta = 0;
        break;
    case 2:
        if (key_sta == 1)
        {
            judge_sta = 0;
            short_flag = 1;
        }
        break;
    }
}

bool Key::isShortPressed()
{
    if (short_flag)
    {
        short_flag = 0;
        return true;
    }
    return false;
}

// Task类实现
Task::Task(void (*task_func)(void), uint16_t rate_ms)
    : task_func(task_func), rate_ms(rate_ms), last_run(0) {}

bool Task::shouldRun(uint32_t now)
{
    return (now - last_run >= rate_ms);
}

void Task::run()
{
    task_func();
    last_run = millis();
}

// Scheduler类实现
Scheduler::Scheduler()
    : task_num(0), keys{Key(pin_button_1), Key(pin_button_2)}
{
    instance = this; // 用于在静态成员函数中访问非静态成员
    for (uint8_t i = 0; i < MAX_KEYS; i++)
    {
        keyEventHandlers[i] = nullptr;
    }
}

void Scheduler::init()
{
    Serial.begin(115200);
    pinMode(pin_led_01, OUTPUT);
    pinMode(pin_led_02, OUTPUT);
    pinMode(tonepin, OUTPUT);


    addTask(scanKeysTask, 10); // 添加按键扫描任务
}

void Scheduler::run()
{
    uint32_t now = millis();
    for (uint8_t i = 0; i < task_num; i++)
    {
        if (tasks[i]->shouldRun(now))
        {
            tasks[i]->run();
        }
    }
}

void Scheduler::addTask(void (*task_func)(void), uint16_t rate_ms)
{
    if (task_num < MAX_TASKS)
    { // 确保任务数量不超过最大任务数
        tasks[task_num] = new Task(task_func, rate_ms);
        task_num++;
    }
}

void Scheduler::addKeyEventHandler(uint8_t keyIndex, void (*handler)(void))
{
    if (keyIndex < MAX_KEYS)
    { // 确保按键索引不超过数组大小
        keyEventHandlers[keyIndex] = handler;
    }
}

void Scheduler::scanKeysTask()
{
    for (uint8_t i = 0; i < MAX_KEYS; i++)
    {
        instance->keys[i].scan();
        if (instance->keys[i].isShortPressed() && instance->keyEventHandlers[i] != nullptr)
        {
            instance->keyEventHandlers[i](); // 调用按键事件处理函数
        }
    }
}

