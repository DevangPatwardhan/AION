#include <Arduino.h>

#include "RCReceiver.h"

RCReceiver::RCReceiver() 
{

}

void RCReceiver::initalizeReceiverPins() 
{
    pin_function(_throttlePinName, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM3, 3, 0));
    _throttlePin = PWMReader(_throttlePinName);
    Serial.println("RC pins initialized");
}

int RCReceiver::getPWM()
{
   return _throttlePin.getPWM();
}

