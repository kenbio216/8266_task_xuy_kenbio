#include <Arduino.h>
#line 1 "E:\\win_code_git\\8266_task_xuy_kenbio\\mqtt_8266_big_task_aux01\\mqtt_8266_big_task_aux01.ino"
/* FreqCount - Example with serial output
 * http://www.pjrc.com/teensy/td_libs_FreqCount.html
 *
 * This example code is in the public domain.
 */
#include <FreqCountESP.h>

void setup()
{
  int timerMs = 1000;
  FreqCountESP.begin(D0, timerMs);
  Serial.begin(57600);
}

void loop()
{
  if (FreqCountESP.available())
  {
    uint32_t frequency = FreqCountESP.read();
    Serial.println(frequency);
    if (frequency == 3000)
    {
    }
  }
  delay(50);
}

