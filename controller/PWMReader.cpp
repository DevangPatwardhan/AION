#include <Arduino.h>

#include "PWMReader.h"

PWMReader::PWMReader() {}
PWMReader::PWMReader(PinName pinName, uint32_t min, uint32_t max)
: _pinName(pinName), _minPulseWidth(min), _maxPulseWidth(max) {
  initializeTimer();
}

void PWMReader::initializeTimer()
{
    Serial.println("PWM Reader class initialized");

    TIM_TypeDef *instance = (TIM_TypeDef *)pinmap_peripheral(_pinName, PinMap_PWM);
    _timerChannel = STM_PIN_CHANNEL(pinmap_function(_pinName, PinMap_PWM));
    _timer = new HardwareTimer(instance);

    _timer->setMode(_timerChannel, TIMER_INPUT_CAPTURE_RISING);
    
    _timer->attachInterrupt(_timerChannel, [this]() {timerInterruptHandler();});
    // _timer->attachInterrupt(_timerChannel, [this]() {Serial.println("Interrupted");});
    _timer->resume();

    Serial.println("PWM Reader class initialized end");
}

void PWMReader::timerInterruptHandler() {

    Serial.println("Handler Called");
    if(_is_rising_edge) {
      _startTime = _timer->getCaptureCompare(_timerChannel);
      // _timer->setMode(_timerChannel, TIMER_INPUT_CAPTURE_FALLING);
      // _timer->resume();

    } 
    //else {
    //   uint32_t endTime = _timer->getCaptureCompare(_timerChannel);

    //   if (endTime >= _startTime) {
    //     _pulseWidth = endTime - _startTime;
    //   } else {
    //     _pulseWidth = (_timer->getOverflow() - _startTime) + endTime;
    //   }
    //   _timer->setMode(_timerChannel, TIMER_INPUT_CAPTURE_RISING);
    // }

    _is_rising_edge = !_is_rising_edge;

}

int PWMReader::getPWM() {
    // Serial.println(_pulseWidth);
    return map(_pulseWidth, 0, 65535, _minPulseWidth, _maxPulseWidth);
}