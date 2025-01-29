#include <Arduino.h>
#include "ServoController.h"

ServoController::ServoController()
{

}

void ServoController::addServo(PinName pin)
{
    if(servosAdded < 4)
    {
        servoPins[servosAdded] = pin;
        servos[servosAdded].attach(pin);
        servosAdded++;
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

int ServoController::readServo(uint8_t index)
{
    return servos[index].read();
}

bool ServoController::servoAttached()
{
    return servos[0].attached();
}