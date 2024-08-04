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
Servo waist_servo, tilt_servo;
void setup() {
  waist_servo.attach(D1);
  tilt_servo.attach(D2);
  //Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {
    tilt_servo.write(140);
    delay(1000);
    tilt_servo.write(100);
    delay(1000);
    //waist_servo.writeMicroseconds(1000);
    //delay(1000);
    waist_servo.write(0);
    delay(500);
    waist_servo.write(180);
    delay(500);
    waist_servo.write(90);
}
