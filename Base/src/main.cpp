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

#include"baseservos.cpp"

#define MODE_IDLE 0
#define MODE_AIM_PERSON 1
#define MODE_AIM_TARGET 2
#define MODE_MPU 3
#define MODE_DEBUG 4
typedef int mode_t;

mode_t mode = MODE_IDLE;
BaseServos base(D1,D2);
void setup() {
  
  Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {
    switch(mode){
        case MODE_IDLE:
            break;
        case MODE_AIM_PERSON:
            break;
        case MODE_AIM_TARGET:
            break;
        case MODE_MPU:
            break;
        case MODE_DEBUG:
            break;
    }
}
