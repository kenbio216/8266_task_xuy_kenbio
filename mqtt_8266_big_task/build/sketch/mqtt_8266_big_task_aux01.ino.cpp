#include <Arduino.h>
#line 1 "E:\\win_code_git\\8266_task_xuy_kenbio\\mqtt_8266_big_task_aux01\\mqtt_8266_big_task_aux01.ino"
#include <arduinoFFT.h> // 引入ArduinoFFT库

const uint16_t samples = 128; // 设置采样数量（必须是2的幂，如64, 128, 256）
const double samplingFrequency = 5000; // 设置采样频率（Hz）

double vReal[samples]; // 用于存储采集的真实信号
double vImag[samples]; // 用于存储虚部信号（初始化为0）

ArduinoFFT<double> FFT = ArduinoFFT<double>(vReal, vImag, samples, samplingFrequency);

const int analogPin = A0; // 定义模拟量引脚

#line 13 "E:\\win_code_git\\8266_task_xuy_kenbio\\mqtt_8266_big_task_aux01\\mqtt_8266_big_task_aux01.ino"
void setup();
#line 19 "E:\\win_code_git\\8266_task_xuy_kenbio\\mqtt_8266_big_task_aux01\\mqtt_8266_big_task_aux01.ino"
void loop();
#line 13 "E:\\win_code_git\\8266_task_xuy_kenbio\\mqtt_8266_big_task_aux01\\mqtt_8266_big_task_aux01.ino"
void setup() {
    Serial.begin(115200); // 设置串口通讯波特率
    while (!Serial);
    Serial.println("Ready");
}

void loop() {
    // 读取模拟信号
    for (uint16_t i = 0; i < samples; i++) {
        vReal[i] = analogRead(analogPin);
        vImag[i] = 0; // FFT要求虚部信号
        delayMicroseconds((1.0 / samplingFrequency) * 1000000); // 控制采样频率
    }

    // 将原始数据发送到VOFA+
    for (uint16_t i = 0; i < samples; i++) {
        // Serial.print(vReal[i]);
        if (i < samples - 1) {
            // Serial.print(",");
        }
    }
    // Serial.println(); // 添加换行符分隔每一组数据

    // 执行FFT
    FFT.windowing(FFTWindow::Hamming, FFTDirection::Forward); // 应用窗口函数
    FFT.compute(FFTDirection::Forward); // 计算FFT
    FFT.complexToMagnitude(); // 将结果转换为幅度

    // 将FFT数据发送到VOFA+
    for (uint16_t i = 0; i < (samples / 2); i++) {
        // Serial.print(vReal[i]); // 输出每个频率的幅度
        if (i < (samples / 2) - 1) {
            // Serial.print(",");
        }
    }
    // Serial.println(); // 添加换行符分隔每一组数据

    // 输出主要频率
    double dominantFrequency = FFT.majorPeak();
    // Serial.print("Dominant frequency: ");
    Serial.println(dominantFrequency, 6);

    // delay(1000); // 延迟1秒
}

