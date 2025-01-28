#include <Arduino.h>

#include "BLDCMotor.h"

// Constructor
BLDCMotor::BLDCMotor(PinName motorPin) {
    _pinName = motorPin;
    
    // Initialize pin
    // pinMode(_pinName, OUTPUT);
    
    // Create hardware timer
    TIM_TypeDef *instance = (TIM_TypeDef *)pinmap_peripheral(_pinName, PinMap_PWM);
    _timer = new HardwareTimer(instance);
    _timerChannel = STM_PIN_CHANNEL(pinmap_function(_pinName, PinMap_PWM));
}

// Set motor speed (0-100%)
void BLDCMotor::setPWM(int pulseWidth) {
    // pulsewidth in percentage
    _timer->setPWM(_timerChannel, _pinName, _pwmFreq, pulseWidth);
}

// Emergency stop
void BLDCMotor::stop() {
    setPWM(0);
}
