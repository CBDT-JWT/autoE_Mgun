#include<Servo.h>

class BaseServos {
    public:
        BaseServos(int waist_pin, int tilt_pin);
        void set_waist_speed(int speed);
        void set_tilt_angle(int angle);
        void renew_waist_err(double err);
        void renew_tilt_err(double err);
    private:
        Servo waist_servo, tilt_servo;
        double waist_servo_err_p, tilt_servo_err_p;
        double waist_servo_err_i, tilt_servo_err_i;
        double waist_servo_err_d, tilt_servo_err_d;

        int waist_servo_last_err, tilt_servo_last_err;
        int waist_serbo_curr_err, tilt_servo_curr_err;
        int waist_servo_err_int, tilt_servo_err_int;
};