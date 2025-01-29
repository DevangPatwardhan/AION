#ifndef SERVO_CONTROLLER
#define SERVO_CONTROLLER


#include <Servo.h>

class ServoController 
{
    public:
        // One PWM signal will control the tilt angle of 4 servos
        PinName SERVO1_PIN = PA_0;

        ServoController();
        void addServo(PinName pin);
        void setAngle(uint8_t index, int angle);
        bool servoAttached();
        int readServo(uint8_t index);

    private:
        const uint8_t totalServos = 4;      // No of servos intended to attach
        uint8_t servosAdded;
        Servo servos[4];
        PinName servoPins[4];

};

#endif