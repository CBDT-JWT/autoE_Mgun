/**
 * @file main.cpp
 * @author Weitao Jiang (cbdt.topthu.org)
 * @brief 
 * @version 0.1
 * @date 2024-08-13
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

/*pin macros*/
#define BUTTON_PIN 2
#define JOYSTICK_X A7
#define JOYSTICK_Y A6
#define JOYSTICK_BUTTON 3

/*oled macros*/
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

/*oled def*/
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

/*mpu6050 def*/
Adafruit_MPU6050 mpu;

/* radio def as transmitter*/
RF24 radio(7, 8);
const byte address[6] = "00001";


void setup() {
  /*init*/
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  pinMode(BUTTON_PIN, INPUT);
  pinMode(JOYSTICK_X, INPUT);
  pinMode(JOYSTICK_Y, INPUT);
  pinMode(JOYSTICK_BUTTON, INPUT);

  /*mpu6050 init*/
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");
  /*radio init*/
  if(radio.begin())Serial.println("Radio Found!");
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  
  /*show welcome img*/
  display.setCursor(0, 10);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.clearDisplay();
  display.println("Hello");
  display.display();
  delay(2000);
  
}

void loop() {
  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  float waist = atan2(a.acceleration.x, a.acceleration.z) * 180 / PI+90;
  Serial.print("Waist: ");
  Serial.println(waist);
  float tilt = atan(a.acceleration.y/a.acceleration.x) * 180 / PI;
  Serial.print("Tilt: ");
  Serial.println(tilt);
  const char text[] = "Hello World";
  if(radio.write(&text, sizeof(text)))Serial.println("send success");
  else Serial.println("send failed");
  delay(500);
  // put your main code here, to run repeatedly:
}
