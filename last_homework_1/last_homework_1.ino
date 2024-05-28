#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Ticker.h>
#include <Adafruit_NeoPixel.h>
#include <arduinoFFT.h>
#include <MD_MAX72xx.h>
#include <MD_Parola.h>
#include <SPI.h>
#include <math.h>

/* --------------------------------- 四合一点阵定义 -------------------------------- */
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define DATA_PIN 0
#define CS_PIN 4
#define CLK_PIN 5
MD_Parola matrixDisplay = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);
MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);
const int numColumns = MAX_DEVICES * 8; // 点阵模块的列数
double t;
float waveformPeriod = 1.0;    // 矩形波周期（秒）
float waveformAmplitude = 0.5; // 矩形波幅值（单位：伏特）
// 定义傅里叶级数的阶数
const int numHarmonics = 10;
// 傅里叶级数的系数数组
float coefficients[numHarmonics];
float fourierValue;
int col = 0;
/* ---------------------------------- 氛围灯定义 --------------------------------- */
#define PIN 13
#define NUMPIXELS 4
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

/* ---------------------------------- 按键定义 ---------------------------------- */
#define bottun_study 16
#define bottun_party 12
#define bottun_trip 14

/* -------------------------------- 蜂鸣器定义 -------------------------------- */
#define buzzer 12

/* ---------------------------------- 卡农定义 ---------------------------------- */
#define NOTE_B0 31
#define NOTE_C1 33
#define NOTE_CS1 35
#define NOTE_D1 37
#define NOTE_DS1 39
#define NOTE_E1 41
#define NOTE_F1 44
#define NOTE_FS1 46
#define NOTE_G1 49
#define NOTE_GS1 52
#define NOTE_A1 55
#define NOTE_AS1 58
#define NOTE_B1 62
#define NOTE_C2 65
#define NOTE_CS2 69
#define NOTE_D2 73
#define NOTE_DS2 78
#define NOTE_E2 82
#define NOTE_F2 87
#define NOTE_FS2 93
#define NOTE_G2 98
#define NOTE_GS2 104
#define NOTE_A2 110
#define NOTE_AS2 117
#define NOTE_B2 123
#define NOTE_C3 131
#define NOTE_CS3 139
#define NOTE_D3 147
#define NOTE_DS3 156
#define NOTE_E3 165
#define NOTE_F3 175
#define NOTE_FS3 185
#define NOTE_G3 196
#define NOTE_GS3 208
#define NOTE_A3 220
#define NOTE_AS3 233
#define NOTE_B3 247
#define NOTE_C4 262
#define NOTE_CS4 277
#define NOTE_D4 294
#define NOTE_DS4 311
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_FS4 370
#define NOTE_G4 392
#define NOTE_GS4 415
#define NOTE_A4 440
#define NOTE_AS4 466
#define NOTE_B4 494
#define NOTE_C5 523
#define NOTE_CS5 554
#define NOTE_D5 587
#define NOTE_DS5 622
#define NOTE_E5 659
#define NOTE_F5 698
#define NOTE_FS5 740
#define NOTE_G5 784
#define NOTE_GS5 831
#define NOTE_A5 880
#define NOTE_AS5 932
#define NOTE_B5 988
#define NOTE_C6 1047
#define NOTE_CS6 1109
#define NOTE_D6 1175
#define NOTE_DS6 1245
#define NOTE_E6 1319
#define NOTE_F6 1397
#define NOTE_FS6 1480
#define NOTE_G6 1568
#define NOTE_GS6 1661
#define NOTE_A6 1760
#define NOTE_AS6 1865
#define NOTE_B6 1976
#define NOTE_C7 2093
#define NOTE_CS7 2217
#define NOTE_D7 2349
#define NOTE_DS7 2489
#define NOTE_E7 2637
#define NOTE_F7 2794
#define NOTE_FS7 2960
#define NOTE_G7 3136
#define NOTE_GS7 3322
#define NOTE_A7 3520
#define NOTE_AS7 3729
#define NOTE_B7 3951
#define NOTE_C8 4186
#define NOTE_CS8 4435
#define NOTE_D8 4699
#define NOTE_DS8 4978
/* --------------------------------- 大鱼定义 --------------------------------- */
#define NTC0 -1
#define NTC1 262
#define NTC2 294
#define NTC3 330
#define NTC4 350
#define NTC5 393
#define NTC6 441
#define NTC7 495
#define NTCL5 196
#define NTCL6 221
#define NTCL7 248
#define NTCH1 525
/* ---------------------------------- 欢乐斗地主定义 --------------------------------- */
#define NOTE_B0 31
#define NOTE_C1 33
#define NOTE_CS1 35
#define NOTE_D1 37
#define NOTE_DS1 39
#define NOTE_E1 41
#define NOTE_F1 44
#define NOTE_FS1 46
#define NOTE_G1 49
#define NOTE_GS1 52
#define NOTE_A1 55
#define NOTE_AS1 58
#define NOTE_B1 62
#define NOTE_C2 65
#define NOTE_CS2 69
#define NOTE_D2 73
#define NOTE_DS2 78
#define NOTE_E2 82
#define NOTE_F2 87
#define NOTE_FS2 93
#define NOTE_G2 98
#define NOTE_GS2 104
#define NOTE_A2 110
#define NOTE_AS2 117
#define NOTE_B2 123
#define NOTE_C3 131
#define NOTE_CS3 139
#define NOTE_D3 147
#define NOTE_DS3 156
#define NOTE_E3 165
#define NOTE_F3 175
#define NOTE_FS3 185
#define NOTE_G3 196
#define NOTE_GS3 208
#define NOTE_A3 220
#define NOTE_AS3 233
#define NOTE_B3 247
#define NOTE_C4 262
#define NOTE_CS4 277
#define NOTE_D4 294
#define NOTE_DS4 311
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_FS4 370
#define NOTE_G4 392
#define NOTE_GS4 415
#define NOTE_A4 440
#define NOTE_AS4 466
#define NOTE_B4 494
#define NOTE_C5 523
#define NOTE_CS5 554
#define NOTE_D5 587
#define NOTE_DS5 622
#define NOTE_E5 659
#define NOTE_F5 698
#define NOTE_FS5 740
#define NOTE_G5 784
#define NOTE_GS5 831
#define NOTE_A5 880
#define NOTE_AS5 932
#define NOTE_B5 988
#define NOTE_C6 1047
#define NOTE_CS6 1109
#define NOTE_D6 1175
#define NOTE_DS6 1245
#define NOTE_E6 1319
#define NOTE_F6 1397
#define NOTE_FS6 1480
#define NOTE_G6 1568
#define NOTE_GS6 1661
#define NOTE_A6 1760
#define NOTE_AS6 1865
#define NOTE_B6 1976
#define NOTE_C7 2093
#define NOTE_CS7 2217
#define NOTE_D7 2349
#define NOTE_DS7 2489
#define NOTE_E7 2637
#define NOTE_F7 2794
#define NOTE_FS7 2960
#define NOTE_G7 3136
#define NOTE_GS7 3322
#define NOTE_A7 3520
#define NOTE_AS7 3729
#define NOTE_B7 3951
#define NOTE_C8 4186
#define NOTE_CS8 4435
#define NOTE_D8 4699
#define NOTE_DS8 4978
/* ---------------------------------- 卡农赋值 ---------------------------------- */
int tune_kanong[] =
    {
        NOTE_G5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_E5, NOTE_F5,
        NOTE_G5, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5,
        NOTE_E5, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_E4, NOTE_F4,
        NOTE_G4, NOTE_A4, NOTE_G4, NOTE_F4, NOTE_G4, NOTE_E4, NOTE_F4, NOTE_G4,
        NOTE_F4, NOTE_A4, NOTE_G4, NOTE_F4, NOTE_E4, NOTE_D4,
        NOTE_E4, NOTE_D4, NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4,
        NOTE_F4, NOTE_A4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5,
        NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5};
float durt_kanong[] =
    {
        1, 0.5, 0.5, 1, 0.5, 0.5,
        0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
        1, 0.5, 0.5, 1, 0.5, 0.5,
        0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
        1, 0.5, 0.5, 1, 0.5, 0.5,
        0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
        1, 0.5, 0.5, 1, 0.5, 0.5,
        0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5};
int length_kanong;
/* --------------------------------- 大鱼赋值 --------------------------------- */
int tune_dayu[] =
    {
        NTC3,
        NTC2,
        NTC3,
        NTC6,
        NTC3,
        NTC2,
        NTC3,
        NTC7,
        NTC3,
        NTC2,
        NTC3,
        NTCH1,
        NTC7,
        NTC5,
        NTC3,
        NTC2,
        NTC3,
        NTC6,
        NTC3,
        NTC2,
        NTC3,
        NTC7,
        NTC5,
        NTC2,
        NTCL7,
        NTC3,
        NTC2,
        NTC3,
        NTC6,
        NTC3,
        NTC2,
        NTC3,
        NTC7,
        NTC3,
        NTC2,
        NTC3,
        NTCH1,
        NTC7,
        NTC5,
        NTC2,
        NTC3,
        NTCL6,
        NTC2,
        NTC3,
        NTCL6,
        NTCL5,
        NTCL6,
        NTC1,
        NTCL7,

        NTCL6,
        NTC1,
        NTC1,
        NTC2,
        NTC2,
        NTC3,
        NTC3,
        NTC5,
        NTC6,
        NTC5,
        NTC3,
        NTC2,
        NTCL6,
        NTC1,
        NTC1,
        NTC2,
        NTC2,
        NTC3,
        NTC3,
        NTCL6,
        NTCL5,
        NTCL6,
        NTC1,
        NTC1,
        NTC2,
        NTC2,
        NTC3,
        NTC3,
        NTC5,
        NTC6,
        NTC5,
        NTC3,
        NTC2,
        NTC2,
        NTC3,
        NTCL6,
        NTC2,
        NTC3,
        NTCL6,
        NTCL5,
        NTCL6,

        NTCL6,
        NTC1,
        NTC2,
        NTC1,
        NTCL6,
        NTCL6,
        NTC1,
        NTC2,
        NTC1,
        NTC3,
        NTC3,
        NTC5,
        NTC6,
        NTC6,
        NTC5,
        NTC3,
        NTC2,
        NTC1,
        NTC2,
        NTC3,
        NTCL6,
        NTC1,
        NTC2,
        NTC1,
        NTCL6,
        NTCL6,
        NTC1,
        NTC2,
        NTC1,
        NTC3,
        NTC2,
        NTC3,
        NTCL6,
        NTC2,
        NTC3,
        NTCL6,
        NTCL5,
        NTCL6,

        NTC3,
        NTC5,
        NTCH1,
        NTC7,
        NTC3,
        NTC3,
        NTC2,
        NTC1,
        NTC1,
        NTC2,
        NTC3,
        NTC3,
        NTC2,
        NTC1,
        NTC6,
        NTCH1,
        NTC7,
        NTC6,
        NTC5,
        NTC2,
        NTC3,
        NTC3,
        NTC5,
        NTCH1,
        NTC7,
        NTC3,
        NTC3,
        NTC2,
        NTC1,
        NTC1,
        NTC2,
        NTC3,
        NTC2,
        NTC3,
        NTCL6,
        NTC2,
        NTC3,
        NTCL6,
        NTCL5,
        NTCL6,
};
int durt_dayu[] =
    {
        250,
        250,
        250,
        250,
        250,
        250,
        250,
        250,
        250,
        250,
        250,
        250,
        500,
        500,
        250,
        250,
        250,
        250,
        250,
        250,
        250,
        250,
        500,
        500,
        1000,
        250,
        250,
        250,
        250,
        250,
        250,
        250,
        250,
        250,
        250,
        250,
        250,
        500,
        500,
        250,
        250,
        500,
        250,
        250,
        250,
        250,
        500,
        500,
        1000,

        250,
        250,
        250,
        250,
        250,
        250,
        250,
        125,
        125,
        750,
        250,
        1000,
        250,
        250,
        250,
        250,
        250,
        250,
        500,
        500,
        1500,
        250,
        250,
        250,
        250,
        250,
        250,
        250,
        125,
        125,
        750,
        250,
        1000,
        250,
        250,
        500,
        250,
        250,
        250,
        250,
        1500,

        250,
        250,
        750,
        250,
        500,
        250,
        250,
        750,
        250,
        500,
        250,
        250,
        500,
        250,
        250,
        250,
        250,
        500,
        500,
        1000,
        250,
        250,
        875,
        125,
        500,
        250,
        250,
        500,
        500,
        1000,
        250,
        250,
        500,
        250,
        250,
        250,
        250,
        1500,

        250,
        250,
        750,
        250,
        500,
        250,
        250,
        500,
        250,
        250,
        500,
        250,
        250,
        500,
        250,
        250,
        250,
        250,
        250,
        250,
        1500,
        250,
        250,
        750,
        250,
        500,
        250,
        250,
        500,
        250,
        250,
        1000,
        250,
        250,
        500,
        250,
        250,
        250,
        250,
        2000,

};
int length_dayu;
/* ---------------------------------- 欢乐斗地主 --------------------------------- */
short tune_huanle[] = {
    NOTE_G4, NOTE_G4, NOTE_A4, NOTE_G4, NOTE_E4, NOTE_G4, NOTE_C4,
    NOTE_C4, NOTE_D4, NOTE_C4, NOTE_A3, NOTE_G3, NOTE_A3, NOTE_A3,
    NOTE_G3, NOTE_A3, NOTE_C4, NOTE_D4, NOTE_C4, NOTE_D4, NOTE_E4,
    NOTE_G4, NOTE_A4, NOTE_G4, NOTE_E4, NOTE_D4, NOTE_G4, NOTE_G4,
    NOTE_A4, NOTE_G4, NOTE_E4, NOTE_G4, NOTE_D4, NOTE_D4, NOTE_E4,
    NOTE_D4, NOTE_C4, NOTE_A3, NOTE_G3, NOTE_A3, NOTE_C4, NOTE_D4,
    NOTE_E4, NOTE_A4, NOTE_E4, NOTE_G4, NOTE_D4, NOTE_E4, NOTE_D4,
    NOTE_A3, NOTE_C4, NOTE_A4, NOTE_A4, NOTE_G4, NOTE_E4, NOTE_G4,
    NOTE_A4, NOTE_E4, NOTE_G4, NOTE_G4, NOTE_A4, NOTE_G4, NOTE_E4,
    NOTE_G4, NOTE_D4, NOTE_D4, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_D4,
    NOTE_C4, NOTE_A3, NOTE_G3, NOTE_D4, NOTE_C4, NOTE_E4, NOTE_A4,
    NOTE_A4, NOTE_G4, NOTE_E4, NOTE_G4, NOTE_A4, NOTE_E4, NOTE_G4,
    NOTE_G4, NOTE_A4, NOTE_G4, NOTE_E4, NOTE_G4, NOTE_D4, NOTE_D4,
    NOTE_E4, NOTE_D4, NOTE_C4, NOTE_D4, NOTE_C4, NOTE_A3, NOTE_G3,
    NOTE_C4, NOTE_C4, 0, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_C4, NOTE_G3,
    NOTE_A3, NOTE_C4, NOTE_G4, NOTE_G4, NOTE_G4, NOTE_E3, NOTE_G4,
    NOTE_G3, NOTE_G3, NOTE_A3, NOTE_C4, NOTE_G3, NOTE_A3, NOTE_E4,
    NOTE_D4, NOTE_D4, NOTE_D4, NOTE_C4, NOTE_D4, NOTE_E4, NOTE_E4,
    NOTE_D4, NOTE_E4, NOTE_G4, NOTE_A4, NOTE_C5, NOTE_A4, NOTE_G4,
    NOTE_A4, NOTE_G4, NOTE_E4, NOTE_D4, NOTE_A4, NOTE_E4, NOTE_D4,
    NOTE_G4, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_C4, NOTE_A3, NOTE_C4,
    NOTE_G3, NOTE_G3, NOTE_A3, NOTE_C4, NOTE_G3, NOTE_A3, NOTE_C4,
    NOTE_G4, NOTE_G4, NOTE_G4, NOTE_E4, NOTE_G4, NOTE_G3, NOTE_G3,
    NOTE_A3, NOTE_C4, NOTE_E4, NOTE_D4, NOTE_E4, NOTE_D4, NOTE_C4,
    NOTE_D4, NOTE_E4, NOTE_E4, NOTE_D4, NOTE_E4, NOTE_G4, NOTE_A4,
    NOTE_C5, NOTE_A4, NOTE_G4, NOTE_A4, NOTE_G4, NOTE_E4, NOTE_D4,
    NOTE_D4, NOTE_D4, NOTE_E4, NOTE_G4, NOTE_G3, NOTE_E4, NOTE_D4,
    NOTE_C4, NOTE_D4, NOTE_E4, NOTE_D4, NOTE_E4, NOTE_G4, NOTE_A4,
    NOTE_D5, NOTE_C5, NOTE_G4, NOTE_C5, NOTE_E4, NOTE_E4, NOTE_D4,
    NOTE_C4, NOTE_C4, NOTE_A3, NOTE_D4, NOTE_E4, NOTE_D4, NOTE_E4,
    NOTE_G3, NOTE_A3, NOTE_A3, NOTE_G3, NOTE_A3, NOTE_C4, NOTE_G4,
    NOTE_A4, NOTE_E4, NOTE_G4, NOTE_D4, NOTE_E4, NOTE_E4, NOTE_D4,
    NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_C5, NOTE_A4,
    NOTE_G4, NOTE_E4, NOTE_D4, NOTE_D4, NOTE_E4, NOTE_G4, NOTE_G3,
    NOTE_D4, NOTE_E4, NOTE_D4, NOTE_E4, NOTE_C4, NOTE_E4, NOTE_E4,
    NOTE_D4, NOTE_E4, NOTE_G4, NOTE_A4, NOTE_C5, NOTE_A4, NOTE_G4,
    NOTE_A4, NOTE_G4, NOTE_E4, NOTE_D4, NOTE_D4, NOTE_E4, NOTE_G4,
    NOTE_G3, NOTE_D4, NOTE_E4, NOTE_D4, NOTE_E4, NOTE_C4, NOTE_D4,
    NOTE_D4, NOTE_D4, NOTE_E4, NOTE_G4, NOTE_G4, NOTE_A4, NOTE_C5,
    NOTE_A4, NOTE_C5, NOTE_A3, NOTE_A3, NOTE_C4, NOTE_A3, NOTE_G3,
    NOTE_A3, NOTE_C4, NOTE_D4, NOTE_E4, NOTE_G4, NOTE_G4, NOTE_A4,
    NOTE_G4, NOTE_D4, NOTE_E4, NOTE_A3, NOTE_A3, NOTE_C4, NOTE_A3,
    NOTE_G3, NOTE_A3, NOTE_C4, NOTE_D4, NOTE_E4, NOTE_D4, NOTE_D4,
    NOTE_E4, NOTE_D4, NOTE_C4, NOTE_D4, NOTE_A3, NOTE_A3, NOTE_C4,
    NOTE_A3, NOTE_G3, NOTE_A3, NOTE_C4, NOTE_D4, NOTE_A4, NOTE_G4,
    NOTE_G4, NOTE_A4, NOTE_G4, NOTE_D4, NOTE_E4, NOTE_E4, NOTE_G4,
    NOTE_A4, NOTE_A4, NOTE_G4, NOTE_E4, NOTE_A4, NOTE_A3, NOTE_A3,
    NOTE_G3, NOTE_A3, NOTE_C4, 0, 0, NOTE_E4, NOTE_E4, NOTE_D4,
    NOTE_E4, NOTE_A4, 0, 0, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_A4,
    NOTE_G4, NOTE_D4, NOTE_E4, NOTE_A3, NOTE_A3, NOTE_G3, NOTE_A3,
    NOTE_C4, 0, 0, NOTE_E4, NOTE_E4, NOTE_D4, NOTE_E4, NOTE_A4, 0, 0,
    NOTE_A4, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_G4, NOTE_E4, NOTE_A4,
    NOTE_E4, NOTE_D4, NOTE_E4, NOTE_G4, NOTE_C5, NOTE_B4, NOTE_G4, NOTE_A4};
byte durt_huanle[] = {
    8, 16, 16, 8, 8, 2, 8, 16, 16, 8, 8, 2, 8, 16, 16, 8, 8, 8,
    8, 8, 8, 16 / 3, 16, 8, 8, 2, 8, 16, 16, 8, 8, 2, 8, 16,
    16, 8, 8, 2, 8, 8, 8, 8, 8, 8, 8, 8, 16 / 3, 16, 8, 8, 2, 8,
    16, 16, 8, 8, 8 / 3, 8, 8, 16, 16, 8, 8, 2, 8, 16, 16, 8, 8, 8,
    8, 8, 8, 8 / 3, 8, 2, 8, 16, 16, 8, 8, 8 / 3, 8, 8, 16, 16, 8,
    8, 2, 8, 16, 16, 8, 8, 8, 8, 8, 8, 8 / 3, 8 / 3, 4, 8, 16, 16,
    16, 16, 16, 16, 16, 16, 16, 16, 4, 8, 16, 16, 16, 16, 16, 16, 16,
    16, 16, 16, 4, 8, 16, 16, 8, 8, 16, 16, 16, 16, 8, 16, 16, 8, 16, 16,
    16, 16, 16, 16, 8, 16, 16, 4, 8, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
    4, 8, 16, 16, 8, 8, 16, 16, 16, 16, 4, 8, 16, 16, 8, 8, 16, 16, 16, 16,
    8, 16, 16, 16, 16, 16, 16, 8, 8, 8, 8, 4, 32 / 3, 32, 16, 16, 16, 16, 8,
    8, 8, 4, 8, 16, 16, 8, 16, 16, 16, 16, 16, 16, 4, 8, 16, 16, 8, 8, 16, 16, 16,
    16, 4, 8, 16, 16, 8, 8, 16, 16, 16, 16, 8, 16, 16, 8, 16, 16, 8, 16, 16, 8, 8, 16,
    16, 16, 16, 4, 8, 16, 16, 8, 8, 16, 16, 16, 16, 8, 16, 16, 8, 16, 16, 8, 8, 16, 16,
    16, 16, 4, 16, 16, 16, 16, 8, 16, 16, 8, 8, 4, 8, 16, 16, 8, 8, 8, 8, 8, 8, 8, 16, 16,
    8, 8, 2, 8, 16, 16, 8, 8, 8, 8, 8, 8, 8, 16, 16, 8, 8, 2, 8, 16, 16, 8, 8, 8, 8, 8, 8, 8,
    16, 16, 8, 8, 8 / 3, 16, 16, 8 / 3, 8, 4, 4, 1 / 2, 8, 16, 16, 8, 8, 4, 4, 8, 16, 16, 8, 8,
    4, 4, 16 / 3, 16, 8, 8, 4, 4, 1 / 2, 8, 16, 16, 8, 8, 4, 4, 8, 16, 16, 8, 8, 4,
    4, 16 / 3, 16, 8, 8, 4, 4, 1 / 2, 16 / 3, 16, 8, 8, 8, 4, 8, 1 / 2};
int length_huanle;

/* ---------------------------------- 按键定义 ---------------------------------- */
Ticker time0;
typedef struct KEY
{
  bool short_flag;
  uint8_t state;
  uint8_t judge;
} keys;
keys key[3] = {0};
/* ----------------------------------- 联网 ----------------------------------- */
const char *ssid = "Mi11LE";                 // 替换自己的WIFI
const char *password = "Gqw415319";          // 替换自己的WIFI
const char *mqtt_server = "192.168.129.220"; // 替换自己的MQTT服务器IP地址

/* ----------------------------- 下面是一系列变量定义及初始化 ----------------------------- */
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0; // 用来保存上一次接收TOPIC的时间
char msg[100];    // 用来存放ESP8266将要PUBLISH到服务器的消息
int value = 0;    // 这个变量假装自己是温度值，在后面的程序中每秒钟累计加1
int key_flag = 1; // 这个变量用来和KEY端口读过来的值做比较

/* -------------------------- 让ESP8266开发板能够接入WiFi网络 ------------------------- */
void setup_wifi()
{
  delay(10);
  Serial.println();
  Serial.print("童卓 2022030233 罗子丹 2022020584 Connecting to ");
  Serial.println(ssid); // 串口显示提示文字Connecting to ssid

  WiFi.begin(ssid, password); // 开始连接ssid网络

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  } // 如果未连接上WiFi，串口每半分钟打印一个.

  randomSeed(micros()); // 根据当前时间产生随机数种子

  Serial.println("");
  Serial.println("童卓 2022030233 罗子丹 2022020584 WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP()); // 如果连接上WiFi，串口显示WiFi connected以及ESP8266开发板的IP地址
}

// 处理MQTT服务器虚拟客户端PUBLISH的TOPIC
void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("童卓 2022030233 罗子丹 2022020584 Message arrived [");
  Serial.print(topic);
  Serial.print("] "); // 串口显示Message arrived [topic]，主题（topic）是led1或者distance
  // 解决完如何识别进来的是谁的信息

  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  } // payload是具体的消息，比如Hello World；length是消息长度；payload是个字符串，因此这里将字符串的每个字符依次打印出来
  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1')
  {
    key[0].short_flag = 1;
  } // 读取payload的第1个字符，如果这个字符是1，就开BUILTIN_LED灯
  if ((char)payload[0] == '2')
  {
    key[1].short_flag = 1;
  } // 读取payload的第1个字符，如果这个字符是0，就关BUILTIN_LED灯
  if ((char)payload[0] == '3')
  {
    key[2].short_flag = 1;
  } // 读取payload的第1个字符，如果这个字符是0，就关BUILTIN_LED灯
}

// 让ESP8266开发板能够接入MQTT服务器
void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("童卓 2022030233 罗子丹 2022020584 Attempting MQTT connection..."); // 如果ESP8266开发板还没有连上MQTT服务器，串口输出Attempting MQTT connection...
    // Create a random client ID
    String clientId = "ESP8266Client-"; // 给ESP8266开发板随机生成一个客户端ID号，在它连上MQTT服务器后，我们在MQTT服务器网页上能够查看到
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str()))
    {
      Serial.println("童卓 2022030233 罗子丹 2022020584 connected"); // 如果ESP8266开发板连上了MQTT服务器，串口输出connected
      // Once connected, publish an announcement...
      client.publish("outTopic2", "hello world"); // 同时ESP8266开发板向MQTT服务器PUBLISH一个主题为outTopic内容为hello world的消息
      // ... and resubscribe
      client.subscribe("inTopic2"); // 同时ESP8266开发板从MQTT服务器SUBSCRIBE一个主题inTopic2
    }
    else
    {
      Serial.print("童卓 2022030233 罗子丹 2022020584 failed, rc="); // 如果ESP8266开发板还是没有连上了MQTT服务器，串口输出failed, rc=
      Serial.print(client.state());                                  // 串口输出错误代码
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

// 初始化傅里叶级数的系数
void initCoefficients()
{
  for (int n = 1; n <= numHarmonics; ++n)
  {
    if (n % 2 == 0)
    {                            // 如果是偶数项
      coefficients[n - 1] = 0.0; // 偶次谐波的系数为0
    }
    else
    {                                                               // 如果是奇数项
      coefficients[n - 1] = (4.0 / (n * M_PI)) * waveformAmplitude; // 计算奇次谐波的系数
    }
  }
}

// 计算傅里叶级数的和，考虑周期性
// 计算带有相位偏移的傅里叶级数的和
float calculateFourierSeries(float time)
{
  float phaseOffset = random(0, 50);
  float normalizedTime = fmod(time, waveformPeriod); // 将时间归一化到周期内
  float result = 0.0;
  for (int n = 0; n < numHarmonics; ++n)
  {
    result += coefficients[n] * sin(2.0 * M_PI * (2 * n + 1) * normalizedTime / waveformPeriod + phaseOffset);
  }
  return result;
}

void displaySoundLevel()
{
  mx.clear();
  for (t = waveformPeriod / 32; t < waveformPeriod; t = t + waveformPeriod / 32)
  {
    col++;
    fourierValue = calculateFourierSeries(t) * 10;
    if (fourierValue <= 0)
    {
      fourierValue = -fourierValue;
    }
    for (int row = 0; row < fourierValue; row++)
    {
      if (fourierValue >= 8)
      {
        fourierValue = 8;
      }
      mx.setPoint(7 - row, col, true); // 从下到上绘制每列
    }
  }
  col = 0;
}

void Time0_PeriodElapsedCallback()
{
  key[0].state = digitalRead(bottun_study);
  key[1].state = digitalRead(bottun_party);
  key[2].state = digitalRead(bottun_trip);
  for (int i = 0; i <= 2; i++)
  {
    switch (key[i].judge)
    {
    case 0:
      if (key[i].state == 1)
      {
        key[i].judge = 1;
      }
      break;
    case 1:
      if (key[i].state == 1)
        key[i].judge = 2;
      else
        key[i].judge = 0;
      break;
    case 2:
      if (key[i].state == 0)
      {
        key[i].short_flag = 1; // 短按键识别
        key[i].judge = 0;
      }
      break;
    }
  }
}

void task_study()
{
  pixels.setBrightness(2);
  pixels.setPixelColor(0, 127, 255, 212);
  pixels.setPixelColor(1, 127, 255, 212);
  pixels.setPixelColor(2, 127, 255, 212);
  pixels.setPixelColor(3, 127, 255, 212);
  pixels.show();
  length_dayu = sizeof(tune_dayu) / sizeof(tune_dayu[0]);
  Serial.println("study");
  for (int i = 0; i < length_dayu; i++)
  {
    if (key[1].short_flag == 1 | key[2].short_flag == 1)
    {
      break;
    }
    tone(buzzer, tune_dayu[i]);
    delay(durt_dayu[i]);
    noTone(buzzer);

    waveformPeriod = durt_dayu[i] / 10; // 矩形波周期（秒）
    for (t = waveformPeriod / 32; t < waveformPeriod; t = t + waveformPeriod / 32)
    {
      Serial.print("大鱼：");
      fourierValue = calculateFourierSeries(t) * 10;
      Serial.println(fourierValue);
    }
    displaySoundLevel();
  }
}

void task_party()
{
  pixels.setBrightness(20);
  pixels.setPixelColor(0, 61, 145, 64);
  pixels.setPixelColor(1, 176, 23, 31);
  pixels.setPixelColor(2, 25, 25, 112);
  pixels.setPixelColor(3, 0, 199, 140);
  pixels.show();
  length_huanle = sizeof(tune_huanle) / sizeof(tune_huanle[0]);
  Serial.println("party");
  for (int thisNote = 0; thisNote < length_huanle; thisNote++)
  {
    if (key[0].short_flag == 1 | key[2].short_flag == 1)
    {
      break;
    }
    int noteDuration = 1.5 * 1000 / durt_huanle[thisNote];
    tone(buzzer, tune_huanle[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30; // 响完一个音后的停顿时间
    delay(pauseBetweenNotes);
    noTone(buzzer);

    waveformPeriod = pauseBetweenNotes / 10; // 矩形波周期（秒）
    for (t = waveformPeriod / 32; t < waveformPeriod; t = t + waveformPeriod / 32)
    {
      Serial.print("欢乐：");
      fourierValue = calculateFourierSeries(t) * 10;
      Serial.println(fourierValue);
    }
    displaySoundLevel();
  }
}

void task_trip()
{
  pixels.setBrightness(2);
  pixels.setPixelColor(0, 61, 145, 64);
  pixels.setPixelColor(1, 176, 23, 31);
  pixels.setPixelColor(2, 25, 25, 112);
  pixels.setPixelColor(3, 0, 199, 140);
  pixels.show();
  length_kanong = sizeof(tune_kanong) / sizeof(tune_kanong[0]);
  Serial.println("trip");
  for (int i = 0; i < length_kanong; i++)
  {
    if (key[1].short_flag == 1 | key[0].short_flag == 1)
    {
      break;
    }
    tone(buzzer, tune_kanong[i]);
    delay(400 * durt_kanong[i]);
    noTone(buzzer);

    waveformPeriod = 400 * durt_kanong[i] / 10; // 矩形波周期（秒）
    for (t = waveformPeriod / 32; t < waveformPeriod; t = t + waveformPeriod / 32)
    {
      Serial.print("卡农：");
      fourierValue = calculateFourierSeries(t) * 10;
      Serial.println(fourierValue);
    }
    displaySoundLevel();
  }
}

// Arduino初始化命令，写在setup函数中的命令只执行1次
void setup()
{
  // 按键初始化
  pinMode(bottun_study, INPUT);
  pinMode(bottun_party, INPUT);
  pinMode(bottun_trip, INPUT);
  // 蜂鸣器初始化
  pinMode(buzzer, OUTPUT);
  // 初始化串口波特率为115200
  Serial.begin(115200);
  pixels.begin();
  /* ---------------------------------- 四合一点阵 --------------------------------- */
  matrixDisplay.begin();
  mx.begin();
  matrixDisplay.setIntensity(0); // 亮度范围从 0 到 15
  matrixDisplay.displayClear();
  mx.clear();
  initCoefficients();
  /* ---------------------------------- 中断的设置 --------------------------------- */
  time0.attach_ms(20, Time0_PeriodElapsedCallback);
  // 以下依次调用连接WiFi、设置MQTT服务器、回调（接收并处理来自MQTT服务器的消息）、连接MQTT服务器等函数
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  reconnect();
  // // 如果没有成功订阅主题为led内容为0的消息，串口输出订阅失败;尝试重新订阅！，并在0.3秒后重新订阅
  // // 以下为接受函数名字的名称定义
  // // 订阅开发板2的指示灯，接受信息
  while (!client.subscribe("2led", 0))
  {
    Serial.println("童卓 2022030233 罗子丹 2022020584 订阅失败;尝试重新订阅！");
    client.subscribe("2led", 0);
    delay(300);
  }
  // 如果成功订阅主题led，串口输出订阅成功~~~
  Serial.println("童卓 2022030233 罗子丹 2022020584 2led订阅成功~~~");
  // 订阅开发板2的亮度传感器数值信息,接受信息
  while (!client.subscribe("distance", 0))
  {
    Serial.println("童卓 2022030233 罗子丹 2022020584 订阅失败;尝试重新订阅！");
    client.subscribe("distance", 0);
    delay(300);
  }
  // 如果成功订阅主题led，串口输出订阅成功~~~
  Serial.println("童卓 2022030233 罗子丹 2022020584 distance订阅成功~~~");
}

// Arduino循环执行命令，写在loop函数中的命令循环执行
void loop()
{
  // 如果ESP8266开发板没有连接上MQTT服务器，重新建立连接
  // if (!client.connected())
  // {
  //   reconnect();
  // }
  // client.loop();
  if (1)
  {
    task_study();
    key[0].short_flag = 0;
  }
  if (key[1].short_flag == 1)
  {
    task_party();
    key[1].short_flag = 0;
  }
  if (key[2].short_flag == 1)
  {
    task_trip();
    key[2].short_flag = 0;
  }
}