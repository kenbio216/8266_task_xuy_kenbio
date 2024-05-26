#include "Dai_tone.h"

/* -------------------------------------------------------------------------- */
/*                                    音调定义                                    */
/* -------------------------------------------------------------------------- */
// 以下是上春山的音
#define aNTLC6 278
#define aNTC1 330
#define aNTC2 380
#define aNTC3 415
#define aNTC5 495
#define aNTC6 556
#define aNTCL1 661
// 以下是大鱼的音
#define bNTLC6 220
#define bNTLC5 196
#define bNTC1 262
#define bNTC2 292
#define bNTC3 330
#define bNTC5 393
#define bNTC6 441
#define bNTCL1 661
// 以下是愿与愁的音
#define cNTLC7 371
#define cNTLC6 330
#define cNTLC5 294
#define cNTC1 393
#define cNTC2 451
#define cNTC3 495
#define cNTC4 530
#define cNTC5 595
#define cNTC6 661
#define cNTC7 742
#define cNTL1 786

int start = 14;
int stop = 16;

/* -------------------------------------------------------------------------- */
/*                                   每一曲的音调                                   */
/* -------------------------------------------------------------------------- */

int tune1[] =
    {
        // 以下是上春山的谱
        aNTC5, aNTC5, aNTC5, aNTC3, aNTC2, aNTC2, aNTC3, aNTC5,
        aNTC3, aNTC2, aNTC1, aNTC1, aNTLC6, aNTC1, aNTC2, aNTC6, aNTC5, aNTC5,
        aNTC6, aNTC5, aNTC6, aNTCL1, aNTC6, aNTC5, aNTC3, aNTC2, aNTC3, aNTC6, aNTC5,
        aNTC2, aNTC3, aNTC2, aNTC1, aNTLC6, aNTC3, aNTC2, aNTC2,

        aNTC5, aNTC5, aNTC5, aNTC3, aNTC2, aNTC2, aNTC3, aNTC5,
        aNTC3, aNTC2, aNTC1, aNTC1, aNTLC6, aNTC1, aNTC2, aNTC6, aNTC5, aNTC5,
        aNTC6, aNTC5, aNTC6, aNTCL1, aNTC6, aNTC5, aNTC3, aNTC2, aNTC3, aNTC6, aNTC5,
        aNTC2, aNTC3, aNTC2, aNTC1, aNTLC6, aNTC2, aNTC1};

int tune2[] = {
    // 以下是大鱼的谱
    bNTLC6, bNTC1, bNTC1, bNTC2, bNTC2, bNTC3, bNTC3, bNTC6, bNTC5, bNTC3, bNTC2,
    bNTLC6, bNTC1, bNTC1, bNTC2, bNTC2, bNTC3, bNTC3, bNTLC6, bNTLC5,
    bNTLC6, bNTC1, bNTC1, bNTC2, bNTC2, bNTC3, bNTC3, bNTC6, bNTC5, bNTC3, bNTC2,
    bNTC2, bNTC3, bNTLC6, bNTC2, bNTC3, bNTLC6, bNTLC5, bNTLC6,

    bNTLC6, bNTC1, bNTC2, bNTC1, bNTLC6, bNTLC6, bNTC1, bNTC2, bNTC1, bNTC3,
    bNTC3, bNTC5, bNTC6, bNTC6, bNTC5, bNTC3, bNTC2, bNTC1, bNTC2, bNTC3,
    bNTLC6, bNTC1, bNTC2, bNTC1, bNTLC6, bNTLC6, bNTC1, bNTC2, bNTC1, bNTC3,
    bNTC2, bNTC3, bNTLC6, bNTC2, bNTC3, bNTLC6, bNTLC5, bNTLC6};

int tune3[] = {
    // 以下是愿与愁的谱
    cNTC5, cNTC5, cNTC3, cNTC2, cNTC1, cNTC2, cNTC3, cNTLC6, cNTC1,
    cNTC1, cNTLC7, cNTLC5, cNTLC5, cNTLC6, cNTC1,
    cNTC5, cNTC5, cNTC3, cNTC2, cNTC1, cNTC2, cNTC3, cNTLC6, cNTC1,
    cNTC2, cNTC2, cNTC1, cNTC2, cNTC3,
    cNTC3, cNTC4, cNTC3, cNTC1, cNTLC6, cNTC1, cNTC3, cNTC4, cNTLC7,
    cNTC1, cNTLC7, cNTLC5, cNTLC5, cNTLC6, cNTC1,
    cNTC5, cNTC3, cNTC2, cNTC1, cNTC2, cNTC3, cNTLC6, cNTC1,
    cNTC3, cNTLC6, cNTC2, cNTC1,

    cNTL1, cNTL1, cNTC7, cNTC6, cNTC5, cNTC5, cNTC6, cNTC3, cNTC5, cNTC6, cNTC3, cNTC5, cNTC6, cNTC3, cNTC5, cNTC6, cNTC3, cNTC5, cNTC4, cNTC3,
    cNTL1, cNTL1, cNTC7, cNTC6, cNTC5, cNTC5, cNTC6, cNTC3, cNTC5, cNTC6, cNTC3, cNTC5, cNTC6, cNTC3, cNTC5, cNTC6, cNTC3, cNTC6,
    cNTL1, cNTL1, cNTC7, cNTC6, cNTC5, cNTC5, cNTC6, cNTC3, cNTC5, cNTC6, cNTC3, cNTC5, cNTC6, cNTC3, cNTC5,
    cNTC3, cNTC3, cNTC5, cNTC3, cNTC2, cNTC1, cNTLC7, cNTC1, cNTLC7, cNTLC6,
    cNTLC6, cNTC3, cNTC2, cNTC1, cNTLC7, cNTLC5, cNTLC6,
    cNTC3, cNTC2, cNTC1, cNTLC7, cNTC1, cNTLC7, cNTLC6,
    cNTLC6, cNTC3, cNTC2, cNTC1, cNTLC7, cNTLC5, cNTLC6};

/* -------------------------------------------------------------------------- */
/*                                  每一曲的延迟时间                                  */
/* -------------------------------------------------------------------------- */
int durt1[] =
    {
        // 以下是上春山的节奏
        500, 500, 500, 250, 250, 725, 250, 500,
        250, 250, 500, 250, 250, 500, 500, 750, 250, 1000,
        500, 250, 250, 500, 250, 250, 500, 250, 250, 500, 500,
        250, 250, 250, 250, 500, 250, 250, 2000,

        500, 500, 500, 250, 250, 725, 250, 500,
        250, 250, 500, 250, 250, 500, 500, 750, 250, 1000,
        500, 250, 250, 500, 250, 250, 500, 250, 250, 500, 500,
        250, 250, 250, 250, 500, 250, 250, 2000};

int durt2[] = {
    // 以下是大鱼的节奏
    250, 250, 250, 250, 250, 250, 250, 250, 750, 500, 1100,
    250, 250, 250, 250, 250, 250, 500, 375, 1100,
    250, 250, 250, 250, 250, 250, 250, 250, 750, 500, 1000,
    250, 250, 500, 250, 250, 250, 250, 1500,

    250, 250, 750, 500, 500, 250, 250, 750, 500, 500,
    250, 250, 500, 250, 250, 250, 250, 500, 375, 1000,
    250, 250, 750, 200, 625, 250, 250, 500, 500, 1000,
    250, 250, 500, 250, 250, 250, 250, 1000};

int durt3[] = {
    // 以下是愿与愁的节奏
    250, 250, 250, 250, 250, 250, 250, 250, 1000,
    250, 250, 250, 250, 375, 1000,
    250, 250, 250, 250, 250, 250, 250, 250, 1000,
    250, 250, 250, 250, 1000,
    250, 250, 250, 250, 250, 250, 250, 250, 1000,
    250, 250, 250, 250, 375, 1000,
    250, 250, 250, 250, 250, 250, 250, 1000,
    375, 500, 500, 1000,

    250, 250, 250, 250, 250, 250, 250, 250, 375, 250, 250, 375, 250, 250, 375, 250, 250, 375, 250, 725,
    250, 250, 250, 250, 250, 250, 250, 250, 375, 250, 250, 375, 250, 250, 375, 250, 375, 725,
    250, 250, 250, 250, 250, 250, 250, 250, 375, 250, 250, 375, 250, 250, 725,
    375, 250, 500, 250, 500, 250, 500, 250, 250, 725,
    250, 250, 500, 250, 500, 250, 725,
    250, 500, 250, 500, 250, 375, 725,
    250, 250, 500, 250, 500, 250, 725};

int length1 = sizeof(tune1) / sizeof(tune1[0]);
int length2 = sizeof(tune2) / sizeof(tune2[0]);
int length3 = sizeof(tune3) / sizeof(tune3[0]);

void tone_shang_chun_shan(void)
{
    static uint32_t current_time;
    static uint8_t tone_ptr;
    if (tone_ptr < 74)
    {
        if (millis() - current_time > durt1[tone_ptr - 1])
        {
            noTone(tonepin);
            tone(tonepin, tune1[tone_ptr]);
            tone_ptr++;
            current_time = millis();
        }
    }
    else
        noTone(tonepin);
}

void tone_da_yu(void)
{
    static uint32_t current_time;
    static uint8_t tone_ptr;
    if (tone_ptr < 77)
    {
        if (millis() - current_time > durt2[tone_ptr - 1])
        {
            noTone(tonepin);
            tone(tonepin, tune2[tone_ptr]);
            tone_ptr++;
            current_time = millis();
        }
    }
    else
        noTone(tonepin);
}

void tone_yuan_yu_chou(void)
{
    static uint32_t current_time;
    static uint8_t tone_ptr;
    if (tone_ptr < 141)
    {
        if (millis() - current_time > durt3[tone_ptr - 1])
        {
            noTone(tonepin);
            tone(tonepin, tune3[tone_ptr]);
            tone_ptr++;
            current_time = millis();
        }
    }
    else
        noTone(tonepin);
}

// 上春山第一个唱

// // 大鱼第二个唱
// for (int x = 0; x < length2; x++)
// {
//     tone(tonepin, tune2[x]);
//     delay(durt2[x]);
//     noTone(tonepin);
//     if (digitalRead(stop) == 1)
//         break;
// }

// // 愿与愁第三个唱
// for (int x = 0; x < length3; x++)
// {
//     tone(tonepin, tune3[x]);
//     delay(durt3[x]);
//     noTone(tonepin);
//     if (digitalRead(stop) == 1)
//         break;
// }