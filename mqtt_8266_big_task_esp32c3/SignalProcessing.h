// SignalProcessing.h

#ifndef SIGNALPROCESSING_H
#define SIGNALPROCESSING_H

#include <arduinoFFT.h>
#include <MD_MAX72xx.h>

// FFT相关参数
const uint16_t samples = 64; // FFT样本数量（必须是2的幂）
const double samplingFrequency = 1000; // 采样频率

extern double vReal[samples];
extern double vImag[samples];
extern ArduinoFFT<double> FFT; // 使用模板参数double

// MD_MAX72XX相关参数
extern MD_MAX72XX mx;

// 卡尔曼滤波器参数
extern float Q;
extern float R;
extern float P;
extern float K;
extern float X;

// 函数声明
void initDisplay();
void initFFT();
float kalmanFilter(float measurement);
void processSignal(int micValue);
void displayFFTResult();

#endif // SIGNALPROCESSING_H
