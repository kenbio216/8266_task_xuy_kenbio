# 1 "E:\\win_code_git\\8266_task_xuy_kenbio\\mqtt_8266_big_task\\mqtt_8266_big_task.ino"
# 2 "E:\\win_code_git\\8266_task_xuy_kenbio\\mqtt_8266_big_task\\mqtt_8266_big_task.ino" 2

# 4 "E:\\win_code_git\\8266_task_xuy_kenbio\\mqtt_8266_big_task\\mqtt_8266_big_task.ino" 2
# 14 "E:\\win_code_git\\8266_task_xuy_kenbio\\mqtt_8266_big_task\\mqtt_8266_big_task.ino"
MD_Parola matrixDisplay = MD_Parola(MD_MAX72XX::FC16_HW, 13 /* GPIO 13 - D7*/, 14 /* GPIO 14 - D5*/, 15 /* GPIO 15 - D8*/, 4);
MD_MAX72XX mx = MD_MAX72XX(MD_MAX72XX::FC16_HW, 13 /* GPIO 13 - D7*/, 14 /* GPIO 14 - D5*/, 15 /* GPIO 15 - D8*/, 4);

const int numColumns = 4 * 8; // 点阵模块的列数

void setup() {
  matrixDisplay.begin();
  mx.begin();
  matrixDisplay.setIntensity(0); // 亮度范围从 0 到 15
  matrixDisplay.displayClear();
  mx.clear();
}

void loop() {
  int micValue = analogRead(A0 /* 麦克风输入引脚*/); // 读取麦克风模拟值
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
