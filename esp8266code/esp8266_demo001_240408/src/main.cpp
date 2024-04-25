/*** 
 * @Author: xuyang
 * @Date: 2024-04-08 21:56:23
 * @LastEditors: xuyang
 * @LastEditTime: 2024-04-08 22:06:54
 * @FilePath: \esp8266_demo001_240408\src\main.cpp
 * @Description: 
 * @
 * @Copyright (c) 2024 by xuyang, All Rights Reserved 
 */
#include <Arduino.h>
#include <HardwareSerial.h>

// put function declarations here:
int myFunction(int, int);

void setup() {
  // put your setup code here, to run once:
  int result = myFunction(2, 3);
  Serial.begin(9600);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Hello, world!");
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}