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
    tone_yuan_yu_chou();
}