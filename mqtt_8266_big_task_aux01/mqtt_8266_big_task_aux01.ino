#include "arduinoFFT.h"

const int analogPin = 34;                 // GPIO34
const int sampleSize = 256;               // FFT采样大小
const float amplificationFactor = 1000.0; // 信号变化量放大倍数
const int offset = 350;                   // 偏置量

// FFT相关变量
ArduinoFFT<double> FFT; // 指定模板类型为 double
double vReal[sampleSize];
double vImag[sampleSize];

// 卡尔曼滤波器变量
float Q = 0.1; // 过程噪声协方差
float R = 10;  // 测量噪声协方差
float P = 1, K = 0;
float X = 0; // 估计的初始值

void setup()
{
    Serial.begin(115200);
    pinMode(analogPin, INPUT);
}
double peakFrequency = 0;

void loop()
{
    for (int i = 0; i < sampleSize; i++)
    {
        // 读取原始模拟信号
        int rawValue = analogRead(analogPin);

        // 处理信号
        float processedValue = processSignal(rawValue);
        float filteredValue = kalmanFilter(processedValue);
        Serial.print(filteredValue);
        Serial.print(",");
        Serial.println(peakFrequency);
        // 填充FFT输入数组
        vReal[i] = filteredValue;
        vImag[i] = 0;

        delay(1); // 短延时，防止过度采样
    }

    // 执行FFT
    FFT.windowing(vReal, sampleSize, FFTWindow::Hamming, FFTDirection::Forward);
    FFT.compute(vReal, vImag, sampleSize, FFTDirection::Forward);
    FFT.complexToMagnitude(vReal, vImag, sampleSize);

    // 找到峰值频率
    peakFrequency = FFT.majorPeak(vReal, sampleSize, 5000.0);

    // 输出峰值频率
    // Serial.print("Peak Frequency: ");
    // Serial.println(peakFrequency);
}

// 处理信号函数：偏置调整和变化量放大
float processSignal(int rawValue)
{
    // 信号偏置调整
    float biasedValue = rawValue - offset;

    // 放大信号变化量
    float amplifiedValue = biasedValue * amplificationFactor;

    return amplifiedValue;
}

// 卡尔曼滤波函数
float kalmanFilter(float value)
{
    // 预测更新
    P = P + Q;

    // 计算卡尔曼增益
    K = P / (P + R);

    // 测量更新
    X = X + K * (value - X);
    P = (1 - K) * P;

    return X; // 返回滤波后的值
}
