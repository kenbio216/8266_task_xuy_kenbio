#include "Xu_schedule.h"

// Key类实现
Key::Key(int pin) : pin(pin), key_sta(0), judge_sta(0), short_flag(0) {
    pinMode(pin, INPUT_PULLUP);
}

void Key::scan() {
    key_sta = digitalRead(pin);
    switch (judge_sta) {
    case 0:
        if (key_sta == 0) judge_sta = 1;
        break;
    case 1:
        if (key_sta == 0) judge_sta = 2;
        else judge_sta = 0;
        break;
    case 2:
        if (key_sta == 1) {
            judge_sta = 0;
            short_flag = 1;
        }
        break;
    }
}

bool Key::isShortPressed() {
    if (short_flag) {
        short_flag = 0;
        return true;
    }
    return false;
}

// Task类实现
Task::Task(void (*task_func)(void), uint16_t rate_ms) 
    : task_func(task_func), rate_ms(rate_ms), last_run(0) {}

bool Task::shouldRun(uint32_t now) {
    return (now - last_run >= rate_ms);
}

void Task::run() {
    task_func();
    last_run = millis();
}

// Scheduler类实现
Scheduler::Scheduler() 
    : task_num(0), keys{Key(pin_button_1), Key(pin_button_2)} {}

void Scheduler::init() {
    Serial.begin(115200);
    pinMode(pin_led_01, OUTPUT);
    pinMode(pin_led_02, OUTPUT);
    pinMode(tonepin, OUTPUT);

    // 添加任务
    addTask(led_blink1, 1000);
    addTask(led_blink2, 500);
}

void Scheduler::run() {
    uint32_t now = millis();
    for (uint8_t i = 0; i < task_num; i++) {
        if (tasks[i]->shouldRun(now)) {
            tasks[i]->run();
        }
    }
    for (uint8_t i = 0; i < 2; i++) {
        keys[i].scan();
        if (keys[i].isShortPressed()) {
            // 处理按键短按事件
        }
    }
}

void Scheduler::addTask(void (*task_func)(void), uint16_t rate_ms) {
    if (task_num < 10) { // 确保任务数量不超过10
        tasks[task_num] = new Task(task_func, rate_ms);
        task_num++;
    }
}

// led_blink1 和 led_blink2 函数实现
static inline void led_blink1() {
    digitalWrite(pin_led_01, !digitalRead(pin_led_01));
}

static inline void led_blink2() {
    digitalWrite(pin_led_02, !digitalRead(pin_led_02));
}
