/**
 * @file baseservos.cpp
 * @author Weitao Jiang(www.topthu.org)
 * @brief functions of servo control.
 * @version 0.1
 * @date 2024-08-05
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include"baseservos.h"

BaseServos::BaseServos(int waist_pin, int tilt_pin) {
    waist_servo.attach(waist_pin);
    tilt_servo.attach(tilt_pin);
    curr_tilt_angle = 0;
    curr_waist_angle = 0;
    waist_servo.write(90);
    tilt_servo.write(90);
}

void BaseServos::set_waist_angle(int angle) {
    
    if(angle>MAX_WAIST_ANGLE) angle = MAX_WAIST_ANGLE;
    if(angle<MIN_WAIST_ANGLE) angle = MIN_WAIST_ANGLE;
    angle+=90;
    if(curr_waist_angle+90<angle)for(int i=curr_waist_angle+90; i<angle; i+=1) {
        waist_servo.write(i);
        delay(WAIST_TIME_PER_DEGREE);
    }
    else{
        for(int i=curr_waist_angle+90; i>angle; i-=1) {
            waist_servo.write(i);
            delay(WAIST_TIME_PER_DEGREE);
        }
    }
    curr_waist_angle = angle-90;
}

void BaseServos::set_tilt_angle(int angle) {
    angle = 90+angle;
    if(angle>MAX_TILT_ANGLE) angle = MAX_TILT_ANGLE;
    if(angle<MIN_TILT_ANGLE) angle = MIN_TILT_ANGLE;
    if(curr_tilt_angle+90<angle)for(int i=curr_tilt_angle+90; i<angle; i+=1) {
        tilt_servo.write(i);
        delay(TILT_TIME_PER_DEGREE);
    }
    else{
        for(int i=curr_tilt_angle+90; i>angle; i-=1) {
            tilt_servo.write(i);
            delay(TILT_TIME_PER_DEGREE);
        }
    }
    curr_tilt_angle = angle-90;
}
void BaseServos::incr_waist_angle(int angle) {
    set_waist_angle(curr_waist_angle+angle);
}
void BaseServos::incr_tilt_angle(int angle) {
    set_tilt_angle(curr_tilt_angle+angle);
}
void BaseServos::find_target_1_itr(int target_x, int target_y, int img_width, int img_height) {
    incr_waist_angle(-waist_ctrl_k*(target_x/img_width));
    incr_tilt_angle(-tilt_ctrl_k*(target_y/img_height));
}