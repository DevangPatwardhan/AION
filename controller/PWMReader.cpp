#include <Arduino.h>

#include "PWMReader.h"

PWMReader::PWMReader(uint8_t pin, TIM_TypeDef* timer, uint32_t timerChannel, uint32_t min, uint32_t max)
: _inputPin(pin), _timer(timer), _timerChannel(timerChannel), _minPulseWidth(min), _maxPulseWidth(max) {}

void PWMReader::initializeTimer()
{
    pinMode(_inputPin, INPUT);
    HardwareTimer *Timer = new HardwareTimer(_timer);

    Timer->setMode(_timerChannel, TIMER_INPUT_CAPTURE_RISING);
    Timer->attachInterrupt([this]() { this->timerInterruptHandler(); });
    Timer->resume();
}

void PWMReader::timerInterruptHandler() {
    static uint32_t lastCaptureTime = 0;
    uint32_t currentCaptureTime = _timer->CCR1;
    
    if (currentCaptureTime > lastCaptureTime) {
      _pulseWidth = currentCaptureTime - lastCaptureTime;
    } else {
      _pulseWidth = (0xFFFF - lastCaptureTime) + currentCaptureTime;
    }
    
    lastCaptureTime = currentCaptureTime;
    _newPulseReceived = true;
}

int PWMReader::getPWM() {
    if (_newPulseReceived) {
      _newPulseReceived = false;
      return map(_pulseWidth, 0, 65535, _minPulseWidth, _maxPulseWidth);
    }
    return -1;  // No new pulse
}