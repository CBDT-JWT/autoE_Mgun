/*********************************************************************************
 * File Name     : main.cpp
 * Author        : Weitao Jiang
 * Date          : Aug. 4th, 2024
 * Description   : Main file for the esp-8266 on base.
 * Version       : 1.0
 * Functions     : 
 * - control base angle via Servo Motor#TODO
 * - connect to nano (for shooting) via wire #todo
 * - - note: maybe use an esp8266 instead
 * - control zvs charger #TODO
 * - connect to K210 camera via wifi #TODO
 * - connect to ESP32-cam via wifi #TODO
**********************************************************************************/

#include<Arduino.h>
#include<Servo.h>
void setup() {
  
  pinMode(LED_BUILTIN, OUTPUT);  // Initialize the LED_BUILTIN pin as an output
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BUILTIN, LOW);  
  delay(1000);                      
  digitalWrite(LED_BUILTIN, HIGH); 
  delay(2000);                      
}
