#include <Arduino.h>
#include "PWMReader.h"

class RCReceiver {

    private:
        PinName _throttlePinName = PB_0_ALT1;
        PWMReader _throttlePin;
        // PinName _rollPin;

    public:
        RCReceiver();
        void initalizeReceiverPins();
        int getPWM();
};