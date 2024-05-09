# 1 "E:\\win_code_git\\8266_task_xuy_kenbio\\mqtt_8266_big_task_aux01\\mqtt_8266_big_task_aux01.ino"
/* FreqCount - Example with serial output

 * http://www.pjrc.com/teensy/td_libs_FreqCount.html

 *

 * This example code is in the public domain.

 */
# 6 "E:\\win_code_git\\8266_task_xuy_kenbio\\mqtt_8266_big_task_aux01\\mqtt_8266_big_task_aux01.ino"
# 7 "E:\\win_code_git\\8266_task_xuy_kenbio\\mqtt_8266_big_task_aux01\\mqtt_8266_big_task_aux01.ino" 2

void setup() {
  Serial.begin(115200);
  FreqCount.begin(1000);
}

void loop() {
  if (FreqCount.available()) {
    unsigned long count = FreqCount.read();
    Serial.println(count);
  }
}
