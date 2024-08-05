/**
 * @file baseservos.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-08-05
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include<Servo.h>
#define MAX_TILT_ANGLE 170
#define MIN_TILT_ANGLE 10
#define MAX_WAIST_ANGLE 80
#define MIN_WAIST_ANGLE -80
#define WAIST_TIME_PER_DEGREE 5
#define TILT_TIME_PER_DEGREE 3
class BaseServos {
    public:
        BaseServos(int waist_pin, int tilt_pin);
        void set_waist_angle(int angle);//-90 to 90
        void set_tilt_angle(int angle);
        void incr_waist_angle(int angle);
        void incr_tilt_angle(int angle);
        void find_target_1_itr(int target_x, int target_y, int img_width, int img_height);
    private:
        Servo waist_servo, tilt_servo;
        double waist_ctrl_k=0.5;// per itr: waist_angle -= waist_ctrl_k*(target_x/img_width)
        double tilt_ctrl_k=0.5;// per itr: tilt_angle -= tilt_ctrl_k*(target_y/img_height)
        int curr_tilt_angle=0;
        int curr_waist_angle=0;
};