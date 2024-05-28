/***
 * @Author: xuyang
 * @Date: 2024-05-26 22:24:49
 * @LastEditors: xuyang
 * @LastEditTime: 2024-05-28 12:10:49
 * @FilePath: \8266_task_xuy_kenbio\mqtt_8266_big_task_esp32c3\Dai_tone.h
 * @Description:
 * @
 * @Copyright (c) 2024 by xuyang, All Rights Reserved
 */
#ifndef _DAI_TONE_H_
#define _DAI_TONE_H_

#include <Arduino.h>
#include <MD_Parola.h>
#include <MD_MAX72XX.h>
#include <SPI.h>

#include "Xu_schedule.h"

// 定义常量
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define DATA_PIN 3 // GPIO 13 - D7
#define CS_PIN 7   // GPIO 15 - D8
#define CLK_PIN 2  // GPIO 14 - D5

class DaiTone
{
public:
    DaiTone();
    void init();
    void toneShangChunShan();
    void toneDaYu();
    void toneYuanYuChou();

private:
    void displaySoundLevel(int level);

    uint8_t music_logo[8];
    char char_array[8];

    MD_Parola matrixDisplay;
    MD_MAX72XX mx;

    const int numColumns;
    const int tonepin;

    uint32_t current_time;
    uint8_t tone_ptr;
};

#endif // _DAI_TONE_H_
