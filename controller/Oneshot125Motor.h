#include <Arduino.h>

class Oneshot125Motor
{
private:
    uint8_t _motorPin;
    HardwareTimer* _timer;

    int mapSpeedToPulseWidth(int speed);
public:
    Oneshot125Motor(/* args */);
    void setSpeed(int speed);
    void stop();
};