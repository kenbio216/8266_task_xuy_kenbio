#include "Dai_tone.h"
/* ------------------------------- 四合一点阵的引脚定义 ------------------------------- */
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4

#define DATA_PIN 3 // GPIO 13 - D7
#define CS_PIN 7   // GPIO 15 - D8
#define CLK_PIN 2  // GPIO 14 - D5

MD_Parola matrixDisplay = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);
MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

const int numColumns = MAX_DEVICES * 8; // 点阵模块的列数

/* -------------------------------------------------------------------------- */
/*                                    音调定义                                    */
/* -------------------------------------------------------------------------- */
/* ------------------------------- 蜂鸣器部分的音调关系 ------------------------------- */
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
/* ------------------------------ 四合一点阵部分的音调关系 ------------------------------ */
// 以下是上春山音的高低
#define aaNTLC6 0
#define aaNTC1 1
#define aaNTC2 2
#define aaNTC3 3
#define aaNTC5 5
#define aaNTC6 6
#define aaNTCL1 8

// 以下是大鱼音的高低
#define bbNTLC6 1
#define bbNTLC5 2
#define bbNTC1 3
#define bbNTC2 4
#define bbNTC3 5
#define bbNTC5 6
#define bbNTC6 7
#define bbNTCL1 8

// 以下是愿与愁的音
#define ccNTLC7 0
#define ccNTLC6 1
#define ccNTLC5 2
#define ccNTC1 3
#define ccNTC2 4
#define ccNTC3 5
#define ccNTC4 6
#define ccNTC5 6
#define ccNTC6 7
#define ccNTC7 7
#define ccNTL1 8

int start = 14;
int stop = 16;

/* -------------------------------------------------------------------------- */
/*                                   每一曲的音调                                   */
/* -------------------------------------------------------------------------- */
/* ------------------------------- 四合一点阵的音调数组 ------------------------------- */
int tune11[] =
    {
        // 以下是上春山的谱
        aaNTC5, aaNTC5, aaNTC5, aaNTC3, aaNTC2, aaNTC2, aaNTC3, aaNTC5,
        aaNTC3, aaNTC2, aaNTC1, aaNTC1, aaNTLC6, aaNTC1, aaNTC2, aaNTC6, aaNTC5, aaNTC5,
        aaNTC6, aaNTC5, aaNTC6, aaNTCL1, aaNTC6, aaNTC5, aaNTC3, aaNTC2, aaNTC3, aaNTC6, aaNTC5,
        aaNTC2, aaNTC3, aaNTC2, aaNTC1, aaNTLC6, aaNTC3, aaNTC2, aaNTC2,

        aaNTC5, aaNTC5, aaNTC5, aaNTC3, aaNTC2, aaNTC2, aaNTC3, aaNTC5,
        aaNTC3, aaNTC2, aaNTC1, aaNTC1, aaNTLC6, aaNTC1, aaNTC2, aaNTC6, aaNTC5, aaNTC5,
        aaNTC6, aaNTC5, aaNTC6, aaNTCL1, aaNTC6, aaNTC5, aaNTC3, aaNTC2, aaNTC3, aaNTC6, aaNTC5,
        aaNTC2, aaNTC3, aaNTC2, aaNTC1, aaNTLC6, aaNTC2, aaNTC1

};

int tune22[] = {
    // 以下是大鱼的谱
    bbNTLC6, bbNTC1, bbNTC1, bbNTC2, bbNTC2, bbNTC3, bbNTC3, bbNTC6, bbNTC5, bbNTC3, bbNTC2,
    bbNTLC6, bbNTC1, bbNTC1, bbNTC2, bbNTC2, bbNTC3, bbNTC3, bbNTLC6, bbNTLC5,
    bbNTLC6, bbNTC1, bbNTC1, bbNTC2, bbNTC2, bbNTC3, bbNTC3, bbNTC6, bbNTC5, bbNTC3, bbNTC2,
    bbNTC2, bbNTC3, bbNTLC6, bbNTC2, bbNTC3, bbNTLC6, bbNTLC5, bbNTLC6,

    bbNTLC6, bbNTC1, bbNTC2, bbNTC1, bbNTLC6, bbNTLC6, bbNTC1, bbNTC2, bbNTC1, bbNTC3,
    bbNTC3, bbNTC5, bbNTC6, bbNTC6, bbNTC5, bbNTC3, bbNTC2, bbNTC1, bbNTC2, bbNTC3,
    bbNTLC6, bbNTC1, bbNTC2, bbNTC1, bbNTLC6, bbNTLC6, bbNTC1, bbNTC2, bbNTC1, bbNTC3,
    bbNTC2, bbNTC3, bbNTLC6, bbNTC2, bbNTC3, bbNTLC6, bbNTLC5, bbNTLC6

};

int tune33[] = {
    // 以下是愿与愁的谱
    ccNTC5, ccNTC5, ccNTC3, ccNTC2, ccNTC1, ccNTC2, ccNTC3, ccNTLC6, ccNTC1,
    ccNTC1, ccNTLC7, ccNTLC5, ccNTLC5, ccNTLC6, ccNTC1,
    ccNTC5, ccNTC5, ccNTC3, ccNTC2, ccNTC1, ccNTC2, ccNTC3, ccNTLC6, ccNTC1,
    ccNTC2, ccNTC2, ccNTC1, ccNTC2, ccNTC3,
    ccNTC3, ccNTC4, ccNTC3, ccNTC1, ccNTLC6, ccNTC1, ccNTC3, ccNTC4, ccNTLC7,
    ccNTC1, ccNTLC7, ccNTLC5, ccNTLC5, ccNTLC6, ccNTC1,
    ccNTC5, ccNTC3, ccNTC2, ccNTC1, ccNTC2, ccNTC3, ccNTLC6, ccNTC1,
    ccNTC3, ccNTLC6, ccNTC2, ccNTC1,

    ccNTL1, ccNTL1, ccNTC7, ccNTC6, ccNTC5, ccNTC5, ccNTC6, ccNTC3, ccNTC5, ccNTC6, ccNTC3, ccNTC5, ccNTC6, ccNTC3, ccNTC5, ccNTC6, ccNTC3, ccNTC5, ccNTC4, ccNTC3,
    ccNTL1, ccNTL1, ccNTC7, ccNTC6, ccNTC5, ccNTC5, ccNTC6, ccNTC3, ccNTC5, ccNTC6, ccNTC3, ccNTC5, ccNTC6, ccNTC3, ccNTC5, ccNTC6, ccNTC3, ccNTC6,
    ccNTL1, ccNTL1, ccNTC7, ccNTC6, ccNTC5, ccNTC5, ccNTC6, ccNTC3, ccNTC5, ccNTC6, ccNTC3, ccNTC5, ccNTC6, ccNTC3, ccNTC5,
    ccNTC3, ccNTC3, ccNTC5, ccNTC3, ccNTC2, ccNTC1, ccNTLC7, ccNTC1, ccNTLC7, ccNTLC6,
    ccNTLC6, ccNTC3, ccNTC2, ccNTC1, ccNTLC7, ccNTLC5, ccNTLC6,
    ccNTC3, ccNTC2, ccNTC1, ccNTLC7, ccNTC1, ccNTLC7, ccNTLC6,
    ccNTLC6, ccNTC3, ccNTC2, ccNTC1, ccNTLC7, ccNTLC5, ccNTLC6

};
/* -------------------------------- 蜂鸣器的音调数组 -------------------------------- */
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

void displaySoundLevel(int level)
{
    mx.clear();
    for (int col = 0; col < numColumns; col++)
    {
        for (int row = 0; row < level; row++)
        {
            mx.setPoint(7 - row, col, true); // 从下到上绘制每列
        }
    }
}

void Dai_tone_init(void)
{
    matrixDisplay.begin();
    mx.begin();
    matrixDisplay.setIntensity(0); // 亮度范围从 0 到 15
    matrixDisplay.displayClear();
    mx.clear();
}

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
            int level = map(tune11[tone_ptr], 0, 8, 0, 8); // 将模拟值映射为点阵模块的行数（从0到8）
            displaySoundLevel(level);
            tone_ptr++;
            current_time = millis();
        }
    }
    else if (tone_ptr >= 74)
    {
        noTone(tonepin);
        tone_ptr = 0;
    }
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
