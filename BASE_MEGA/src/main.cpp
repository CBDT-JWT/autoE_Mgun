/**
 * @file main.cpp
 * @author WeitaoJiang cbdt.topthu.org
 * @brief
 * @version 0.1
 * @date 2024-08-10
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <Arduino.h>
#include <Servo.h>
#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

#define IDLE_MODE 0
#define RC_MODE 1
#define DEBUG_MODE -1
#define K210 Serial1
bool with_K210 = false;
#define HUMAN_SENSOR_PIN 4
#define LASER_PIN 22
#define CHARGE_ENA_PIN 28
#define CHARGE_VOLTAGE_PIN A0

const int STAGE_CTRL_PINS[]={33,34,35,36,37,38};
const int INIT_PIN = 32;
const float aim_point_relative_x=0.5;
const float aim_point_relative_y=0.5;

Servo waist_servo, tilt_servo;

RF24 radio(7,8);
const byte address[6] = "00001";

//servo.write(servo.read()+ Kp * err + Ki * int + Kd * diff);
/*PID const configurations*/
//waist
const float Kp_waist = 0.3;
const float Ki_waist = 0.0;
const float Kd_waist = 0.05;
//tilt  
const float Kp_tilt = 0.4;
const float Ki_tilt = 0.0;
const float Kd_tilt = 0.0;

float int_waist = 0;
float diff_waist = 0;
float err_waist = 0;

float int_tilt = 0;
float diff_tilt = 0;
float err_tilt = 0;
/*PID functions*/
float PID_waist(float err_)
{ 
  
  int_waist += err_;
  diff_waist = err_ - err_waist;
  err_waist = err_;
  float ret =(Kp_waist * err_waist + Ki_waist * int_waist + Kd_waist * diff_waist)*90.0;
  if(ret > 10) return 10;
  if(ret < -10) return -10;
  return ret;
}
float PID_tilt(float err_)
{

  int_tilt += err_;
  diff_tilt = err_ - err_tilt;
  err_tilt = err_;
  return (Kp_tilt * err_tilt + Ki_tilt * int_tilt + Kd_tilt * diff_tilt)*90.0;
}

void aim_target_1_itr(int target_x, int target_y, int canvas_width, int canvas_height)
{
  int current_waist_angle = waist_servo.read();
  int current_tilt_angle = tilt_servo.read();

  float now_err_waist =- (target_x - canvas_width* aim_point_relative_x)/(float)canvas_width;
  float now_err_tilt =- (target_y - canvas_height* aim_point_relative_y)/(float)canvas_height;


  int delta_waist = PID_waist(now_err_waist);
  int delta_tilt = PID_tilt(now_err_tilt);
  
  waist_servo.write(current_waist_angle + delta_waist);
  tilt_servo.write(current_tilt_angle + delta_tilt);
  delay(20);
  waist_servo.write(waist_servo.read());
  tilt_servo.write(tilt_servo.read());//protections
}



int stage_delay_us_arr [] = {1000,10000,10000,10000,10000,10000};//delay after stage
long int stage_hold_us_arr [] = {1000000,100000,100000,100000,100000,100000}; //hold time

void fire(){
  digitalWrite(CHARGE_ENA_PIN,LOW);
  
  for(int i=0;i<6;i++){
    if(stage_hold_us_arr[i]==0)continue;
    Serial.println(i);
    digitalWrite(STAGE_CTRL_PINS[i], HIGH);
    delayMicroseconds(stage_hold_us_arr[i]);
    digitalWrite(STAGE_CTRL_PINS[i], LOW);
    delayMicroseconds(stage_delay_us_arr[i]);
  }
}



int get_mode()
{
  return DEBUG_MODE; // todo: implement this function
}

int read_human_sensor()
{
  return 1;
  if (digitalRead(HUMAN_SENSOR_PIN) == HIGH)
  {

    return 1;
  }
  return 0; // todo: implement this function
}


void alarm()
{
  // Serial.println("Human detected!");
  return; // todo: implement this function
}
void setup()
{
  // put your setup code here, to run once:

  Serial.begin(9600);
  Serial.println("Hello,world!");
  delay(1000);
  if(with_K210)K210.begin(9600);
  // if(radio.begin())Serial.println("radio begin success");
  // else Serial.println("radio begin failed");
  // radio.openReadingPipe(0, address);
  // radio.setPALevel(RF24_PA_MIN);
  // radio.startListening();
  waist_servo.attach(2);
  tilt_servo.attach(3);

  waist_servo.write(90);
  tilt_servo.write(90);
  delay(1000);
  pinMode(LASER_PIN, OUTPUT);
  pinMode(HUMAN_SENSOR_PIN, INPUT);
  pinMode(CHARGE_ENA_PIN, OUTPUT);
  pinMode(CHARGE_VOLTAGE_PIN, INPUT);
  for(int i=0;i<6;i++)
  {
    pinMode(STAGE_CTRL_PINS[i], OUTPUT); 
  }
  pinMode(INIT_PIN, OUTPUT);
  delay(1000);
  Serial.println("init...");
  delay(1000);
  digitalWrite(INIT_PIN, HIGH);
  delay(1000);
  digitalWrite(INIT_PIN, LOW);
}

void loop()
{ 
  
  switch (get_mode())
  {

  case DEBUG_MODE:
    if(analogRead(CHARGE_VOLTAGE_PIN)<650){
          digitalWrite(CHARGE_ENA_PIN, HIGH); 
        }else if (analogRead(CHARGE_VOLTAGE_PIN)>680){  
          digitalWrite(CHARGE_ENA_PIN, LOW);
        }
    Serial.print("charging!");
    Serial.println(analogRead(CHARGE_VOLTAGE_PIN));
    delay(200);
    if(Serial.available()){
      String command = Serial.readString();
      if(command.indexOf("fire")!=-1){
        Serial.println("got:fire!");
        fire();
      }
    }
  break;
  case IDLE_MODE:
    if (true)
    {
      int times_not_found = 0;
      alarm();
      digitalWrite(LASER_PIN, HIGH);
      while (read_human_sensor())
      { 
        /*charge and check*/
        if(analogRead(CHARGE_VOLTAGE_PIN)<650){
          digitalWrite(CHARGE_ENA_PIN, HIGH); 
        }else if (analogRead(CHARGE_VOLTAGE_PIN)>680){  
          digitalWrite(CHARGE_ENA_PIN, LOW);
        }
        Serial.print("charging:");
        Serial.println(analogRead(CHARGE_VOLTAGE_PIN));
        if(!with_K210)delay(1000);
        /*read from K210 and aim.*/
        // K210.write("get");
        if (with_K210&&K210.available())
        {
          String text = String(K210.readStringUntil('\n'));
          delay(10);
          Serial.println(text);
          if (text.indexOf("target not found") != -1)
          {
            times_not_found++;
            if (times_not_found > 10)
            {

              Serial.println("Target not found for 10 times, stop aiming.");
              /*set angle to (90,30)*/
              while (tilt_servo.read()>90){
                tilt_servo.write(tilt_servo.read()-1);
                delay(40);
              }
              while (tilt_servo.read()<90){
                tilt_servo.write(tilt_servo.read()+1);
                delay(40);
              }
              while (waist_servo.read()>30){
                waist_servo.write(waist_servo.read()-1);
                delay(40);
              }
              while (waist_servo.read()<30){
                waist_servo.write(waist_servo.read()+1);
                delay(40);
              }
              /*set pid vars to 0*/
              int_waist = 0;
              diff_waist = 0;
              err_waist = 0;
              int_tilt = 0;
              diff_tilt = 0;
              err_tilt = 0;

              int tmp_var_waist = -120;
              int tmp_var_tilt = 0;
              /*start searching for target
               waist: 30~150 150-(120~0)
                tilt: 90~120 120-(30~0)
              */
              while(true){
                text = String(K210.readStringUntil('\n'));
                Serial.println("searching for target,got: "+text);
                if(text.indexOf("aim:")!=-1){
                  int target_x, target_y, canvas_width, canvas_height, target_size;
                  sscanf(text.c_str(), "aim: %d %d canvas: %d %d size: %d", &target_x, &target_y, &canvas_width, &canvas_height, &target_size);
                  aim_target_1_itr(target_x, target_y, canvas_width, canvas_height);
                  times_not_found = 0;
                  break;
                }
                tilt_servo.write(120-abs(tmp_var_tilt));
                waist_servo.write(150-abs(tmp_var_waist));
                tmp_var_waist+=2;
                tmp_var_tilt+=1;
                if(tmp_var_waist>120){
                  tmp_var_waist = -120;
                }
                if(tmp_var_tilt>30){
                  tmp_var_tilt = -30;
                }
                delay(100);
            }
            continue;
          }}
          if (text.indexOf("aim:") != -1)
          {
            
            int target_x, target_y, canvas_width, canvas_height, target_size;
            sscanf(text.c_str(), "aim: %d %d canvas: %d %d size: %d", &target_x, &target_y, &canvas_width, &canvas_height, &target_size);
            aim_target_1_itr(target_x, target_y, canvas_width, canvas_height);
            times_not_found = 0;
          }
          // delay(50);
        }
      
      }
    }
    else
    {
      digitalWrite(LASER_PIN, LOW);
      K210.write("stop");
    }
    break;
  case RC_MODE:
    K210.write("stop");
    break;
  }
}
// put function definitions here:
