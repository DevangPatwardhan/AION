#include <Arduino.h>

class PWMReader
{

    public:
        PWMReader();
        PWMReader(PinName pinName, uint32_t min=1000, uint32_t max=2000);
        void initializeTimer();
        void timerInterruptHandler();
        int getPWM();


    private:
        PinName _pinName;
        HardwareTimer* _timer;
        uint32_t _timerChannel;
        uint32_t _minPulseWidth;
        uint32_t _maxPulseWidth;

        uint32_t _startTime = 0;
        uint32_t _pulseWidth = 0;
        bool _is_rising_edge = true;

};
