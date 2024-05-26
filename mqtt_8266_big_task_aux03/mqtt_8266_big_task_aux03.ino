#include "Xu_schedule.h"
#include "Dai_tone.h"

void setup()
{
    pinMode(pin_led_blue, OUTPUT); // 打开小蓝灯
    pinMode(pin_led_red, OUTPUT);  // 打开小红灯
    pinMode(tonepin, OUTPUT);      // 打开蜂鸣器
    Scheduler_init();
}

void loop()
{
    Scheduler_run();
    // tone_shang_chun_shan();
    // tone_da_yu();
    if (key[0].short_flag)
    {
        digitalWrite(pin_led_blue, HIGH);
        delay(1000);
        digitalWrite(pin_led_blue, LOW);
        key[0].short_flag = 0;
    }
    if (key[1].short_flag) //按键2被按下就播放音乐
    {
        tone_yuan_yu_chou();
        key[1].short_flag = 0;
    }
}