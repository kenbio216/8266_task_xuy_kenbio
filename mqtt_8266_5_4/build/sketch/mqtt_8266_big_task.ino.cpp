#include <Arduino.h>
#line 1 "E:\\win_code_git\\8266_task_xuy_kenbio\\mqtt_8266_big_task\\mqtt_8266_big_task.ino"
/*
 * @Author: xuyang
 * @Date: 2024-05-09 10:26:56
 * @LastEditors: xuyang
 * @LastEditTime: 2024-05-09 10:29:41
 * @FilePath: \8266_task_xuy_kenbio\mqtt_8266_big_task\mqtt_8266_big_task.ino
 * @Description: 
 * 
 * Copyright (c) 2024 by xuyang, All Rights Reserved 
 */
#include <MD_Parola.h>
#include <MD_MAX72XX.h>
#include <SPI.h>

// 定义硬件连接
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4

#define DATA_PIN D7 // DIN
#define CS_PIN D8   // CS
#define CLK_PIN D5  // CLK

MD_Parola matrixDisplay = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

#line 25 "E:\\win_code_git\\8266_task_xuy_kenbio\\mqtt_8266_big_task\\mqtt_8266_big_task.ino"
void setup();
#line 33 "E:\\win_code_git\\8266_task_xuy_kenbio\\mqtt_8266_big_task\\mqtt_8266_big_task.ino"
void loop();
#line 25 "E:\\win_code_git\\8266_task_xuy_kenbio\\mqtt_8266_big_task\\mqtt_8266_big_task.ino"
void setup()
{
    matrixDisplay.begin();
    matrixDisplay.setIntensity(0); // 亮度范围从 0 到 15
    matrixDisplay.displayClear();
    matrixDisplay.displayText("Hello", PA_CENTER, 100, 1000, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
}

void loop()
{
    if (matrixDisplay.displayAnimate())
    {
        matrixDisplay.displayReset();
    }
}

