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
#define IDLE_MODE 2
#define RC_MODE 0
#define AIM_MODE 1
#define DEBUG_MODE -1
#define K210 Serial2
#define ESP8266 Serial1

bool with_K210 = true;
bool Use_RC = true;

#define LASER_PIN 22
#define CHARGE_ENA_PIN 37
#define CHARGE_VOLTAGE_PIN A0

#define ANALOG_TO_VOLTAGE_MULTIPLIER (300 / 800.0)
typedef int switchmode_t;
switchmode_t mode = IDLE_MODE;

const int STAGE_CTRL_PINS[] = {38, 40, 42, 44, 46, 48};
const int INIT_PIN = 32;
const float aim_point_relative_x = 0.45;
const float aim_point_relative_y = 0.5;

Servo waist_servo, tilt_servo, load_servo;

// servo.write(servo.read()+ Kp * err + Ki * int + Kd * diff);
/*PID const configurations*/
// waist
const float Kp_waist = 0.3;
const float Ki_waist = 0.0;
const float Kd_waist = 0.05;
// tilt
const float Kp_tilt = 0.4;
const float Ki_tilt = 0.0;
const float Kd_tilt = 0.1;

float int_waist = 0;
float diff_waist = 0;
float err_waist = 0;

float int_tilt = 0;
float diff_tilt = 0;
float err_tilt = 0;

// servo angle ctrl
int load_angle = 100;
int load_rest_angle = 10;

struct Cmd
{
  switchmode_t mode;
  int joy_x;
  int joy_y;
  int joy_btn_pressed;
} cmd;
/*PID functions*/
float PID_waist(float err_)
{

  int_waist += err_;
  diff_waist = err_ - err_waist;
  err_waist = err_;
  float ret = (Kp_waist * err_waist + Ki_waist * int_waist + Kd_waist * diff_waist) * 90.0;
  if (ret > 10)
    return 10;
  if (ret < -10)
    return -10;
  return ret;
}
float PID_tilt(float err_)
{

  int_tilt += err_;
  diff_tilt = err_ - err_tilt;
  err_tilt = err_;
  return (Kp_tilt * err_tilt + Ki_tilt * int_tilt + Kd_tilt * diff_tilt) * 80.0;
}

void aim_target_1_itr(int target_x, int target_y, int canvas_width, int canvas_height)
{
  int current_waist_angle = waist_servo.read();
  int current_tilt_angle = tilt_servo.read();

  float now_err_waist = -(target_x - canvas_width * aim_point_relative_x) / (float)canvas_width;
  float now_err_tilt = -(target_y - canvas_height * aim_point_relative_y) / (float)canvas_height;

  int delta_waist = PID_waist(now_err_waist);
  int delta_tilt = PID_tilt(now_err_tilt);

  waist_servo.write(current_waist_angle + delta_waist);
  tilt_servo.write(current_tilt_angle + delta_tilt);
  delay(20);
  waist_servo.write(waist_servo.read());
  tilt_servo.write(tilt_servo.read()); // protections
}

/*fire related funcs*/

int VSETlo = 335;
int VSEThi = VSETlo + 3; // voltage set points, sadly our zvs cant pid

int stage_delay_us_arr[] = {0, 0, 0, 0, 0, 0};                // delay after stage
long int stage_hold_us_arr[] = {1750, 800, 600, 420, 100, 0}; // hold time

void fire()
{
  digitalWrite(CHARGE_ENA_PIN, LOW);
  load_servo.write(load_rest_angle);
  delay(200);
  load_servo.write(load_angle);
  delay(200);
  for (int i = 0; i < 6; i++)
  {
    if (stage_hold_us_arr[i] == 0)
      continue;
    delayMicroseconds(stage_delay_us_arr[i]);
    digitalWrite(STAGE_CTRL_PINS[i], HIGH);
    delayMicroseconds(stage_hold_us_arr[i]);
    digitalWrite(STAGE_CTRL_PINS[i], LOW);
  }
  load_servo.write(load_rest_angle);
}

int get_voltage()
{
  return analogRead(CHARGE_VOLTAGE_PIN) * ANALOG_TO_VOLTAGE_MULTIPLIER;
}

void update_charger()
{
  // if (Use_RC)
  // {
  //   ESP8266.print("Voltage:");
  //   ESP8266.print(get_voltage());
  //   ESP8266.println(",Charging:1,Active:1");
  // }
  if (get_voltage() < VSETlo)
  {
    digitalWrite(CHARGE_ENA_PIN, HIGH);
  }
  else if (get_voltage() > VSEThi)
  {
    digitalWrite(CHARGE_ENA_PIN, LOW);
  }
  Serial.print("charging:");
  Serial.println(get_voltage());
  delay(100);
}
/*ctrl related funcs*/

float JOY_X_TO_WAIST = 10.0 / 4096.0;
float JOY_Y_TO_TILT = 5.0 / 4096.0;
int read_human_sensor()
{
  return 1;
}

void alarm()
{
  // Serial.println("Human detected!");
  return; // todo: implement this function
}

/* MAIN */
void setup()
{
  // put your setup code here, to run once:

  Serial.begin(9600);
  Serial.println("Hello,world!");
  delay(1000);
  if (with_K210)
    K210.begin(9600);
  if (Use_RC)
    ESP8266.begin(9600);
  waist_servo.attach(7);
  waist_servo.write(155);
  tilt_servo.attach(3);
  tilt_servo.write(92);
  load_servo.attach(2);

  load_servo.write(load_rest_angle);
  delay(1000);
  pinMode(LASER_PIN, OUTPUT);
  pinMode(CHARGE_ENA_PIN, OUTPUT);
  pinMode(CHARGE_VOLTAGE_PIN, INPUT);
  for (int i = 0; i < 6; i++)
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
  update_charger();
  if (Use_RC && ESP8266.available())
  {
    String command = ESP8266.readStringUntil('\n');
    Serial.print("ESP says:");
    Serial.println(command);
    cmd.joy_btn_pressed = 0;
    sscanf(command.c_str(), "Mode:%d,JoyX:%d,JoyY:%d,JoyBtnPressed:%d", &cmd.mode, &cmd.joy_x, &cmd.joy_y, &cmd.joy_btn_pressed);
  }
  if (Use_RC == false)
  {
    cmd.mode = DEBUG_MODE;
  }
  switch (cmd.mode)
  {

  case DEBUG_MODE:

    delay(200);
    if (Serial.available())
    {
      String command = Serial.readString();
      if (command.indexOf("fire") != -1)
      {
        Serial.println("got:fire!");
        fire();
      }
    }
    break;
  case AIM_MODE:
    if (true)
    {
      int times_not_found = 0;
      alarm();
      digitalWrite(LASER_PIN, HIGH);
      while (read_human_sensor())
      {
        if (Use_RC && ESP8266.available())
        {
          String command = ESP8266.readStringUntil('\n');
          Serial.print("ESP says:");
          Serial.println(command);
          cmd.joy_btn_pressed = 0;
          sscanf(command.c_str(), "Mode:%d,JoyX:%d,JoyY:%d,JoyBtnPressed:%d", &cmd.mode, &cmd.joy_x, &cmd.joy_y, &cmd.joy_btn_pressed);
          if (cmd.mode == RC_MODE)
          {
            break;
          }
        }
        /*charge and check*/
        update_charger();
        if (!with_K210)
          delay(1000);
        /*read from K210 and aim.*/
        // K210.write("get");
        if (with_K210 && K210.available())
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

              while (tilt_servo.read() > 90)
              {
                tilt_servo.write(tilt_servo.read() - 1);
                delay(40);
              }
              while (tilt_servo.read() < 90)
              {
                tilt_servo.write(tilt_servo.read() + 1);
                delay(40);
              }
              while (waist_servo.read() > 155)
              {
                waist_servo.write(waist_servo.read() - 1);
                delay(40);
              }
              while (waist_servo.read() < 155)
              {
                waist_servo.write(waist_servo.read() + 1);
                delay(40);
              }
              /*set pid vars to 0*/
              int_waist = 0;
              diff_waist = 0;
              err_waist = 0;
              int_tilt = 0;
              diff_tilt = 0;
              err_tilt = 0;

              int tmp_var_waist = -20;
              int tmp_var_tilt = -10;
              /*start searching for target
               waist: 140~160 160-(20~0)
                tilt: 90~120 120-(30~0)
              */
              //(140,160)
              while (true)
              {
                if (Use_RC && ESP8266.available())
                {
                  String command = ESP8266.readStringUntil('\n');
                  Serial.print("ESP says:");
                  Serial.println(command);
                  cmd.joy_btn_pressed = 0;
                  cmd.joy_btn_pressed=0;
                  sscanf(command.c_str(), "Mode:%d,JoyX:%d,JoyY:%d,JoyBtnPressed:%d", &cmd.mode, &cmd.joy_x, &cmd.joy_y, &cmd.joy_btn_pressed);
                  if(cmd.joy_btn_pressed)
                  {
                    if (get_voltage() > VSETlo)
                    {
                      fire();
                    }
                  }
                  if (cmd.mode == RC_MODE)
                  {
                    break;
                  }
                }
                update_charger();
                text = String(K210.readStringUntil('\n'));
                Serial.println("searching for target,got: " + text);
                if (text.indexOf("aim:") != -1)
                {
                  int target_x, target_y, canvas_width, canvas_height, target_size;
                  sscanf(text.c_str(), "aim: %d %d canvas: %d %d size: %d", &target_x, &target_y, &canvas_width, &canvas_height, &target_size);
                  aim_target_1_itr(target_x, target_y, canvas_width, canvas_height);
                  times_not_found = 0;
                  break;
                }
                tilt_servo.write(100 - abs(tmp_var_tilt));
                waist_servo.write(175 - abs(tmp_var_waist));
                tmp_var_waist += 1;
                tmp_var_tilt += 1;
                if (tmp_var_waist > 20)
                {
                  tmp_var_waist = -20;
                }
                if (tmp_var_tilt > 10)
                {
                  tmp_var_tilt = -10;
                }
                delay(100);
              }
              continue;
            }
          }
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
    break;
  case RC_MODE:
    if (cmd.joy_btn_pressed)
    {
      if (get_voltage() > VSETlo)
      {
        fire();
      }
    }

    break;
  }
}
// put function definitions here:
