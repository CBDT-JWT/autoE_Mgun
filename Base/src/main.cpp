/*********************************************************************************
 * File Name     : main.cpp
 * Author        : Weitao Jiang
 * Date          : Aug. 4th, 2024
 * Description   : Main file for the esp-8266 on base.
 * Version       : 1.0
 * Functions     : 
 * - control base angle via Servo Motor #DONE
 * - connect to nano (for shooting) via wire #todo
 * - - note: maybe use an esp8266 instead
 * - control zvs charger #TODO
 * - connect to K210 camera via wifi #TODO
 * - connect to ESP32-cam via wifi #TODO
 * - Status machine #TODO
**********************************************************************************/

#include<Arduino.h>
#include<ESP8266WiFi.h>
#include"baseservos.cpp"

/*macros related to mode switching*/
#define MODE_IDLE 0
#define MODE_AIM_PERSON 1
#define MODE_AIM_TARGET 2
#define MODE_MPU 3
#define MODE_DEBUG 4
typedef int MODE_t;
MODE_t mode = MODE_IDLE;

/*wifi config definitions*/
IPAddress local_IP(192,168,4,22);
IPAddress gateway(192,168,4,9);
IPAddress subnet(255,255,255,0);

BaseServos base(D1,D2);

void IDLE_MODE(){
    Serial.printf("Stations connected = %d\n", WiFi.softAPgetStationNum());
    delay(3000);
}

void AIM_PERSON_MODE(){
}

void AIM_TARGET_MODE(){
}

void MPU_MODE(){
}

void DEBUG_MODE(){

}


void setup() { 
    Serial.begin(9600);
    if(true){//TODO: some button or shit to tell the base :either to set up ap or connect to wifi. 
    Serial.println();

    Serial.print("Setting soft-AP configuration ... ");
    Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");

    Serial.print("Setting soft-AP ... ");
    Serial.println(WiFi.softAP("BASE","475201314!") ? "Ready" : "Failed!");

    Serial.print("Soft-AP IP address = ");
    Serial.println(WiFi.softAPIP());
    }else{
        //TODO: connect to wifi, eg. the r1c at my dorm
    }
}

void loop() {
    switch(mode){
        case MODE_IDLE:
            IDLE_MODE();
            break;
        case MODE_AIM_PERSON:
            AIM_PERSON_MODE();
            break;
        case MODE_AIM_TARGET:
            AIM_TARGET_MODE();
            break;
        case MODE_MPU:
            MPU_MODE();
            break;
        case MODE_DEBUG:
            DEBUG_MODE();
            break;
    }
}
