#include <Arduino.h>

#include "Oneshot125Motor.h"

// Constructor
Oneshot125Motor::OneShot125Motor(uint8_t motorPin, TIM_TypeDef* timerInstance = TIM2) {
    _motorPin = motorPin;
    
    // Initialize pin
    pinMode(_motorPin, OUTPUT);
    
    // Create hardware timer
    _timer = new HardwareTimer(timerInstance);
    
    // Configure timer for Oneshot125
    _timer->setMode(1, TIMER_OUTPUT_COMPARE_PWM1, _motorPin);
    _timer->setOverflow(400, HERTZ_FORMAT);
    _timer->setCaptureCompare(1, 0, PERCENT_COMPARE_FORMAT);
    _timer->resume();
}

// Set motor speed (0-100%)
void Oneshot125Motor::setSpeed(int speed) {
    int pulseWidth = mapSpeedToPulseWidth(speed);
    analogWrite(_motorPin, map(pulseWidth, 0, 400, 0, 255));
}

// Validate and map speed to Oneshot125 pulse width
int Oneshot125Motor::mapSpeedToPulseWidth(int speed) {
    speed = constrain(speed, 0, 100);
    return map(speed, 0, 100, 125, 250);
}


// Emergency stop
void Oneshot125Motor::stop() {
    setSpeed(0);
}
