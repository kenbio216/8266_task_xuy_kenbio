/***
 * @Author: xuyang
 * @Date: 2024-05-29 10:48:44
 * @LastEditors: xuyang
 * @LastEditTime: 2024-05-29 10:49:13
 * @FilePath: \8266_task_xuy_kenbio\mqtt_8266_big_task_esp32c3\SignalProcessing.cpp
 * @Description:
 * @
 * @Copyright (c) 2024 by xuyang, All Rights Reserved
 */
/***
 * @Author: xuyang
 * @Date: 2024-05-29 10:48:44
 * @LastEditors: xuyang
 * @LastEditTime: 2024-05-29 10:49:04
 * @FilePath: \8266_task_xuy_kenbio\mqtt_8266_big_task_esp32c3\SignalProcessing.cpp
 * @Description:
 * @
 * @Copyright (c) 2024 by xuyang, All Rights Reserved
 */
// SignalProcessing.c

#include "SignalProcessing.h"
#include "Dai_tone.h"
// #include <SPI.h>

// FFT相关参数
double vReal[samples];
double vImag[samples];

// arduinoFFT FFT = arduinoFFT(vReal, vImag, samples, samplingFrequency);
ArduinoFFT<double> FFT = ArduinoFFT<double>(vReal, vImag, samples, samplingFrequency);


// MD_MAX72XX相关参数
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define DATA_PIN 3 // GPIO 13 - D7
#define CS_PIN 7   // GPIO 15 - D8
#define CLK_PIN 2  // GPIO 14 - D5

// MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

// 卡尔曼滤波器参数
float Q = 0.1; // 过程噪声协方差
float R = 0.1; // 测量噪声协方差
float P = 1.0; // 估计误差协方差
float K = 0.0; // 卡尔曼增益
float X = 0.0; // 估计值

void initDisplay()
{
    mx.begin();
    mx.control(MD_MAX72XX::INTENSITY, 5); // 设置亮度等级，范围从0到15
}

void initFFT()
{
    // FFT初始化可以在这里进行，但当前示例不需要额外初始化
}

float kalmanFilter(float measurement)
{
    P = P + Q;
    K = P / (P + R);
    X = X + K * (measurement - X);
    P = (1 - K) * P;
    return X;
}

void processSignal(int micValue)
{
    static uint16_t sampleIndex = 0;
    double filteredValue = kalmanFilter(micValue); // 使用卡尔曼滤波器进行滤波
    vReal[sampleIndex] = filteredValue;            // 将滤波后的值存入vReal数组
    vImag[sampleIndex] = 0.0;                      // 将虚部设为0
    sampleIndex++;

    if (sampleIndex >= samples)
    {
        sampleIndex = 0; // 重置索引以进行下一次采样

        // 执行FFT
        FFT.windowing(FFTWindow::Hamming, FFTDirection::Forward); // 使用汉明窗函数
        FFT.compute(FFTDirection::Forward);                       // 进行FFT计算
        FFT.complexToMagnitude();                                 // 计算幅值

        // 显示结果
        displayFFTResult();
    }
}

void displayFFTResult()
{
    // 清除显示屏
    mx.clear();

    // 可视化FFT结果
    for (uint16_t i = 2; i < samples / 2; i++)
    {                                                 // 忽略第一个和第二个样本（直流和低频分量）
        int barHeight = map(vReal[i], 0, 1024, 0, 7); // 将FFT幅值映射到0-7的范围
        barHeight = constrain(barHeight, 0, 7);
        for (int y = 0; y <= barHeight; y++)
        {
            mx.setPoint(y, i - 2, true); // 绘制频谱柱状图
        }
    }

    // 刷新显示
    mx.update();
}
