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

#define IDLE_MODE 0
#define RC_MODE 1
#define K210 Serial1

#define HUMAN_SENSOR_PIN 4
#define LASER_PIN 22

#define WAIST_COEFFICIENT 0.1
#define TILT_COEFFICIENT 0.2

Servo waist_servo, tilt_servo;

// BaseServos base_servos(2, 3);
int get_mode()
{
  return IDLE_MODE; // todo: implement this function
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

void aim_target_1_itr(int target_x, int target_y, int canvas_width, int canvas_height)
{
  int current_waist_angle = waist_servo.read();
  int current_tilt_angle = tilt_servo.read();

  int delta_waist = -(target_x - canvas_width / 2) * WAIST_COEFFICIENT;
  int delta_tilt = -(target_y - canvas_height / 2) * TILT_COEFFICIENT;

  if (current_waist_angle + delta_waist > 180 || current_waist_angle + delta_waist < 0)
  {
    Serial.println("Waist angle out of range!");
    return;
  }
  if (current_tilt_angle + delta_tilt > 180 || current_tilt_angle + delta_tilt < 0)
  {
    Serial.println("Tilt angle out of range!");
    return;
  }

  waist_servo.write(current_waist_angle + delta_waist);
  tilt_servo.write(current_tilt_angle + delta_tilt);
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
  K210.begin(9600);

  waist_servo.attach(2);
  tilt_servo.attach(3);

  waist_servo.write(90);
  tilt_servo.write(90);
  pinMode(LASER_PIN, OUTPUT);
  pinMode(HUMAN_SENSOR_PIN, INPUT);
}

void loop()
{
  switch (get_mode())
  {
  case IDLE_MODE:
    if (read_human_sensor())
    {
      int times_not_found = 0;
      alarm();
      digitalWrite(LASER_PIN, HIGH);
      // K210.write("start");
      //  base_servos.set_tilt_angle(10);
      //  base_servos.set_waist_angle(10);
      //  base_servos.set_tilt_angle(0);
      //  base_servos.set_waist_angle(0);
      while (read_human_sensor())
      {
        // K210.write("get");
        if (K210.available())
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
              times_not_found = 0;
              waist_servo.write(90);
              tilt_servo.write(90);
              // 巡逻
            }
            continue;
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
