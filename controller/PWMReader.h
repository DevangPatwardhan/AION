#include <Arduino.h>

class PWMReader
{

    public:
        PWMReader(uint8_t pin, TIM_TypeDef* timer, uint32_t timerChannel, uint32_t min, uint32_t max);
        void initializeTimer();
        void timerInterruptHandler();
        int getPWM();

    private:
        uint8_t _inputPin;
        TIM_TypeDef* _timer;
        uint32_t _timerChannel;
        uint32_t _minPulseWidth;
        uint32_t _maxPulseWidth;

        volatile uint32_t _pulseWidth;
        volatile bool _newPulseReceived;
};
