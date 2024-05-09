# 1 "E:\\win_code_git\\8266_task_xuy_kenbio\\mqtt_8266_big_task\\mqtt_8266_big_task.ino"
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
# 11 "E:\\win_code_git\\8266_task_xuy_kenbio\\mqtt_8266_big_task\\mqtt_8266_big_task.ino"
# 12 "E:\\win_code_git\\8266_task_xuy_kenbio\\mqtt_8266_big_task\\mqtt_8266_big_task.ino" 2

# 14 "E:\\win_code_git\\8266_task_xuy_kenbio\\mqtt_8266_big_task\\mqtt_8266_big_task.ino" 2

// 定义硬件连接







MD_Parola matrixDisplay = MD_Parola(MD_MAX72XX::FC16_HW, D7 /* DIN*/, D5 /* CLK*/, D8 /* CS*/, 4);

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
