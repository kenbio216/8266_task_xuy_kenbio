#include <Arduino.h>
#line 1 "E:\\win_code_git\\8266_task_xuy_kenbio\\mqtt_8266_big_task\\mqtt_8266_big_task.ino"
#include <MD_Parola.h>
#include <MD_MAX72XX.h>
#include <SPI.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4

#define DATA_PIN   13  // GPIO 13 - D7
#define CS_PIN     15  // GPIO 15 - D8
#define CLK_PIN    14  // GPIO 14 - D5

#define MIC_PIN A0 // 麦克风输入引脚

MD_Parola matrixDisplay = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);
MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

const int numColumns = MAX_DEVICES * 8; // 点阵模块的列数

#line 19 "E:\\win_code_git\\8266_task_xuy_kenbio\\mqtt_8266_big_task\\mqtt_8266_big_task.ino"
void setup();
#line 27 "E:\\win_code_git\\8266_task_xuy_kenbio\\mqtt_8266_big_task\\mqtt_8266_big_task.ino"
void loop();
#line 34 "E:\\win_code_git\\8266_task_xuy_kenbio\\mqtt_8266_big_task\\mqtt_8266_big_task.ino"
void displaySoundLevel(int level);
#line 19 "E:\\win_code_git\\8266_task_xuy_kenbio\\mqtt_8266_big_task\\mqtt_8266_big_task.ino"
void setup() {
  matrixDisplay.begin();
  mx.begin();
  matrixDisplay.setIntensity(0); // 亮度范围从 0 到 15
  matrixDisplay.displayClear();
  mx.clear();
}

void loop() {
  int micValue = analogRead(MIC_PIN); // 读取麦克风模拟值
  int level = map(micValue, 0, 1023, 0, 8); // 将模拟值映射为点阵模块的行数（从0到8）
  
  displaySoundLevel(level);
}

void displaySoundLevel(int level) {
  mx.clear();
  for (int col = 0; col < numColumns; col++) {
    for (int row = 0; row < level; row++) {
      mx.setPoint(7 - row, col, true); // 从下到上绘制每列
    }
  }
}

