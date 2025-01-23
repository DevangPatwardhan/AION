#include <Arduino.h>
#include "ServoController.h"

ServoController::ServoController()
{

}

void ServoController::addServo(uint8_t pin)
{
    if(servoCount < 4)
    {
        servoPins[servoCount] = pin;
        servos[servoCount].attach(pin);
        servoCount++;
    } else {
        Serial.println("Max servo limit reached");
    }
}

void ServoController::setAngle(uint8_t index, int angle)
{
    if(index >= 4 || index <0){
        Serial.println("Wrong index");
        return;
    }

    servos[index].write(angle);

}

bool ServoController::servoAttached()
{
    return servos[0].attached();
}