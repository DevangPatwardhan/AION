#include <Arduino.h>

class BLDCMotor
{
    public:
        BLDCMotor(PinName motorPin);
        void setPWM(int pulseWidth);
        void stop();

    private:
        PinName _pinName;
        HardwareTimer* _timer;
        uint16_t _pwmFreq = 50;
        uint32_t _timerChannel;

};