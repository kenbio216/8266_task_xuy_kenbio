/*
 * @Author: xuyang
 * @Date: 2024-05-26 22:24:39
 * @LastEditors: xuyang
 * @LastEditTime: 2024-05-31 23:56:39
 * @FilePath: \8266_task_xuy_kenbio\mqtt_8266_big_task_esp32c3\mqtt_8266_big_task_esp32c3.ino
 * @Description:
 *
 * Copyright (c) 2024 by xuyang, All Rights Reserved
 */
// TODO 一份好的代码注释，可以让你少写很多的文档，因为代码本身就是最好的文档
// TODO 有感而发，好的代码有三个特点：1，运行延迟低卡bug少，比如任务调度器
// TODO  2，可读性高简洁明了，比如OOP编程 3，分工明确，没有无效定义，函数见名知意，比如这个代码

#include "Xu_schedule.h"
#include "Dai_tone.h"

#include "SignalProcessing.h"

/* -------------------------------------------------------------------------- */
/*                                   点灯科技部分                                   */
/* -------------------------------------------------------------------------- */
#define BLINKER_PRINT Serial
#define BLINKER_WIFI

#include <Blinker.h>

char auth[] = "92ee9b0c302e";
char ssid[] = "kenbio";
char pswd[] = "123456xuy";

// 新建组件对象
BlinkerButton Button1("btn-abc");
BlinkerNumber Number1("num-abc");

/* -------------------------------------------------------------------------- */
/*                                   任务调度器部分                                  */
/* -------------------------------------------------------------------------- */
Scheduler scheduler;
uint8_t mode_flag = 0;
uint8_t tone_flag = 0;
uint8_t blinker_flag = 0;
/* -------------------------------------------------------------------------- */
/*                                   用户写的函数                                   */
/* -------------------------------------------------------------------------- */
// 定义 RGB LED 引脚
const int redPin = 19;
const int greenPin = 18;
const int bluePin = 12;

enum
{
    SHANG_CHUN_SHAN = 0,
    DA_YU,
    YUAN_YU_CHOU,
    TONE_STOP
};

// 颜色变化函数
void setColor(int redValue, int greenValue, int blueValue)
{
    analogWrite(redPin, redValue);
    analogWrite(greenPin, greenValue);
    analogWrite(bluePin, blueValue);
}

// 炫彩效果函数
void rainbowEffect()
{
    static uint8_t hue = 0;
    hue++;
    uint8_t redValue = sin(hue * 0.1) * 127 + 128;
    uint8_t greenValue = sin(hue * 0.1 + 2) * 127 + 128;
    uint8_t blueValue = sin(hue * 0.1 + 4) * 127 + 128;
    setColor(redValue, greenValue, blueValue);
}

void rgb_light_task()
{
    if (mode_flag == FREE)
    {
        switch (tone_flag)
        {
        case SHANG_CHUN_SHAN:
            setColor(255, 0, 0); // 红色
            break;
        case DA_YU:
            setColor(0, 200, 0); // 绿色
            break;
        case YUAN_YU_CHOU:
            setColor(0, 0, 100); // 蓝色
            break;
        case TONE_STOP:
            setColor(0, 0, 0); // 关闭
            break;
        }
    }
    else if (mode_flag != FREE)
    {
        rainbowEffect(); // 炫彩效果
    }
    // case FREE:
    //     break;
    // case MICROSOUND:
    //     setColor(255, 255, 0); // 黄色
    //     break;
}

void uart_task()
{
    // 依次打印tone_flag,mode_flag,blinker_flag
    Serial.print("tone_flag:");
    Serial.println(tone_flag);
    Serial.print("mode_flag:");
    Serial.println(mode_flag);
    Serial.print("blinker_flag:");
    Serial.println(blinker_flag);
}

void tone_task()
{
    switch (tone_flag)
    {
    case SHANG_CHUN_SHAN:
        tone_shang_chun_shan();
        break;
    case DA_YU:
        tone_da_yu();
        break;
    case YUAN_YU_CHOU:
        tone_yuan_yu_chou();
        break;
    case TONE_STOP:
        noTone(tonepin);
        break;
    }
}

void microsound_func()
{
    int micValue = 0;
    switch (mode_flag)
    {
    case MICROSOUND:
        micValue = analogRead(MIC_PIN); // 读取麦克风的模拟值
        processSignal(micValue);        // 处理信号并显示
        break;
    case FREE:
        break;
    }
}
/* -------------------------------- 按键处理函数的部分 ------------------------------- */
void button1_short_press_func()
{
    Serial.println("Button 1 pressed!");
    if (tone_flag == SHANG_CHUN_SHAN)
    {
        tone_flag = DA_YU;
    }
    else if (tone_flag == DA_YU)
    {
        tone_flag = YUAN_YU_CHOU;
    }
    else if (tone_flag == YUAN_YU_CHOU)
    {
        tone_flag = SHANG_CHUN_SHAN;
    }
}

void button2_short_press_func()
{
    Serial.println("Button 2 pressed!");
    static uint8_t flag = 0;
    if (tone_flag != TONE_STOP)
    {
        flag = tone_flag;
        tone_flag = TONE_STOP;
    }
    else if (tone_flag == TONE_STOP)
    {
        tone_flag = flag;
    }
}
void button1_long_press_func()
{
    Serial.println("Button 1 long pressed!");
    mode_flag++;
    mode_flag = mode_flag % 2;
}

void button2_long_press_func()
{
    Serial.println("Button 2 long pressed!");
    blinker_flag++;
    blinker_flag = blinker_flag % 2;
}

/* ---------------------- led_blink1 和 led_blink2 函数部分，方便测试 ---------------------- */
static inline void led_blink1()
{
    digitalWrite(pin_led_01, !digitalRead(pin_led_01));
}

static inline void led_blink2()
{
    digitalWrite(pin_led_02, !digitalRead(pin_led_02));
}

/* -------------------------------------------------------------------------- */
/*                             MQTT，点灯科技的按键回调函数部分                             */
/* -------------------------------------------------------------------------- */
void button1_callback(const String &state)
{
    BLINKER_LOG("get button state: ", state);
    if (tone_flag == SHANG_CHUN_SHAN)
    {
        tone_flag = DA_YU;
    }
    else if (tone_flag == DA_YU)
    {
        tone_flag = YUAN_YU_CHOU;
    }
    else if (tone_flag == YUAN_YU_CHOU)
    {
        tone_flag = SHANG_CHUN_SHAN;
    }
    Serial.println("点灯科技测试成功");
}

// 如果未绑定的组件被触发，则会执行其中内容
void dataRead(const String &data)
{
    static int counter = 0;
    BLINKER_LOG("Blinker readString: ", data);
    counter++;
    Number1.print(counter);
}

/* -------------------------------------------------------------------------- */
/*                               setup和loop函数部分                               */
/* -------------------------------------------------------------------------- */
void setup()
{
    /* ----------------------------------- 初始化 ---------------------------------- */
    xu_signal_init();
    Dai_tone_init();
    scheduler.init();
    /* -------------------------------- 添加按键的回调函数 ------------------------------- */
    scheduler.add_key_event_handler(0, button1_short_press_func);
    scheduler.add_key_event_long_press_handler(0, button1_long_press_func);
    scheduler.add_key_event_handler(1, button2_short_press_func);
    scheduler.add_key_event_long_press_handler(1, button2_long_press_func);

    // 添加用户的任务和初始化

    mode_flag = FREE;
    tone_flag = DA_YU;

    scheduler.add_task(led_blink1, 200);
    scheduler.add_task(tone_task, 10);
    scheduler.add_task(microsound_func, 1);
    scheduler.add_task(uart_task, 100);
    scheduler.add_task(led_blink2, 100);
    scheduler.add_task(rgb_light_task, 10);

    // 额外添加的RGB彩灯
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);
    //     // 设置 PWM 频率和通道
    // ledcSetup(0, 5000, 8); // 通道 0, 5kHz, 8-bit 分辨率
    // ledcSetup(1, 5000, 8);
    // ledcSetup(2, 5000, 8);

    //   // 初始化定时器中断
    //     hw_timer_t *timer = timerBegin(0, 80, true); // 80 分频，APB 时钟为 80MHz
    //     timerAttachInterrupt(timer, &taskScheduler, true);
    //     timerAlarmWrite(timer, taskInterval * 1000, true); // 每 taskInterval 毫秒触发一次中断
    //     timerAlarmEnable(timer);
    /* ------------------------------- MQTT,点灯科技部分 ------------------------------ */
    // 初始化blinker
#if defined(BLINKER_PRINT)
    BLINKER_DEBUG.stream(BLINKER_PRINT);
#endif
    // 初始化blinker
    Blinker.begin(auth, ssid, pswd);
    Blinker.attachData(dataRead);
    Button1.attach(button1_callback);

    // scheduler.add_task([](){ Blinker.run(); }, 700);
}

void loop()
{

    // 延迟测试，调试用的
    // static uint32_t timer;
    // Serial.print(millis() - timer);
    // Serial.println("ms");
    // timer = millis();
    /* ------------------------------- 任务调度器的核心函数 ------------------------------- */
    scheduler.run();
    /* ------------------------------ 点灯科技，MQTT的部分 ------------------------------ */
    if (blinker_flag)
    {
        Blinker.run();
    }

    // switch (mode_flag)
    // {
    // case 0:
    //     tone_shang_chun_shan();
    //     break;
    // case 1:
    //     tone_da_yu();
    //     break;
    // case 2:
    //     tone_yuan_yu_chou();
    //     break;
    // case 3:
    //     int micValue = analogRead(MIC_PIN); // 读取麦克风的模拟值
    //     processSignal(micValue);            // 处理信号并显示
    //     break;
    // }
}
