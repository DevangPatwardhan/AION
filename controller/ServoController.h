#ifndef SERVO_CONTROLLER
#define SERVO_CONTROLLER


#include <Servo.h>

class ServoController 
{
    public:
        ServoController();
        void addServo(uint8_t pin);
        void setAngle(uint8_t index, int angle);
        bool servoAttached();

    private:
        const uint8_t totalServos = 4;
        uint8_t servoCount;
        Servo servos[4];
        int servoPins[4];

};

#endif