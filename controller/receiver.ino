#include <Arduino.h>

#define THROTTLE_PIN PB_4
#define ROLL_PIN  PB_5
#define PITCH_PIN PB_0_ALT1
#define YAW_PIN PB_1_ALT1

uint32_t throttleTimerChannel;
uint32_t rollTimerChannel;
uint32_t pitchTimerChannel;
uint32_t yawTimerChannel;

HardwareTimer *throttleTimer;
// HardwareTimer *rollTimer;
// HardwareTimer *pitchTimer;
// HardwareTimer *yawTimer;

unsigned long throttlePWMRiseTime = 0;
unsigned long rollPWMRiseTime = 0;
unsigned long pitchPWMRiseTime = 0;
unsigned long yawPWMRiseTime = 0;

bool throttleRisingEdge = true;
bool rollRisingEdge = true;
bool pitchRisingEdge = true;
bool yawRisingEdge = true;

unsigned long throttleStartTime = 0;
unsigned long throttlePWM = 0;

unsigned long rollStartTime = 0;
unsigned long rollPWM = 0;

unsigned long pitchStartTime = 0;
unsigned long pitchPWM = 0;

unsigned long yawStartTime = 0;
unsigned long yawPWM = 0;



void timerInterrupt(HardwareTimer* timer, uint32_t &timerChannel, PinName pinName, unsigned long &startTime, bool &isRisingEdge, unsigned long &pulseWidth) {

  if(isRisingEdge) {
    startTime = timer->getCaptureCompare(timerChannel);
    timer->setMode(timerChannel, TIMER_INPUT_CAPTURE_FALLING, pinName);
    timer->resume();
  } else {
    unsigned long endTime = timer->getCaptureCompare(timerChannel);

    if(endTime >= startTime) {
      pulseWidth = endTime - startTime;
    } else {
      pulseWidth = ((timer->getOverflow() - throttleStartTime) + endTime);
    }

    int scaleFactor = int(trunc(timer->getPrescaleFactor()/timer->getTimerClkFreq()));
    pulseWidth = pulseWidth*scaleFactor;
    timer->setMode(timerChannel, TIMER_INPUT_CAPTURE_RISING, pinName);
    timer->resume();
    Serial.println(pulseWidth);

  }

  isRisingEdge = !isRisingEdge;
  // uint8_t port = STM_PORT(pinName);
  // uint8_t pinIndex = STM_PIN(pinName);
  // Serial.print("Pin Name: P");
  // Serial.print(char('A' + port)); // Convert port number to 'A', 'B', 'C', etc.
  // Serial.println(pinIndex);

}

// float getPulsewidth(int id) {
//   switch(id) {
//     case 0:
//       // Throttle
//       float PWM = 
//       return 
//   }
// }

void throttleTimerInterrupt() {

  timerInterrupt(throttleTimer, throttleTimerChannel, THROTTLE_PIN, throttleStartTime, throttleRisingEdge, throttlePWM);
    // if(throttleRisingEdge) {    
    //     throttleStartTime = throttleTimer->getCaptureCompare(throttleTimerChannel);
    //     throttleTimer->setMode(throttleTimerChannel, TIMER_INPUT_CAPTURE_FALLING, THROTTLE_PIN);
    //     throttleTimer->resume();
    // }else {
    //     unsigned long endTime = throttleTimer->getCaptureCompare(throttleTimerChannel);

    //     if(endTime >= throttleStartTime) {
    //         throttlePWM = (endTime - throttleStartTime);
    //     } else {
    //         throttlePWM = ((throttleTimer->getOverflow() - throttleStartTime) + endTime);
    //     }
    //     throttleTimer->setMode(throttleTimerChannel, TIMER_INPUT_CAPTURE_RISING, THROTTLE_PIN);
    //     throttleTimer->resume();
    // }

    // throttleRisingEdge = !throttleRisingEdge;
    // // *(throttleTimer->getPrescaleFactor()/throttleTimer->getTimerClkFreq())
    // // Serial.println("Prescale factor");
    // // Serial.println(throttleTimer->getPrescaleFactor());
    // Serial.println(throttlePWM);
}

void rollTimerInterrupt() {
  timerInterrupt(throttleTimer, rollTimerChannel, ROLL_PIN, rollStartTime, rollRisingEdge, rollPWM);
}

void pitchTimerInterrupt() {
  timerInterrupt(throttleTimer, pitchTimerChannel, PITCH_PIN, pitchStartTime, pitchRisingEdge, pitchPWM);
}

void yawTimerInterrupt() {
  timerInterrupt(throttleTimer, yawTimerChannel, YAW_PIN, yawStartTime, yawRisingEdge, yawPWM);
}


void radioSetup() {

    // throttleTimerChannel = STM_PIN_CHANNEL(pinmap_function(THROTTLE_PIN, PinMap_PWM));
    Serial.println("Throttle start");
    throttleTimer = initializeTimer(THROTTLE_PIN);
    throttleTimerChannel = STM_PIN_CHANNEL(pinmap_function(THROTTLE_PIN, PinMap_PWM));
    throttleTimer->setMode(throttleTimerChannel, TIMER_INPUT_CAPTURE_RISING, THROTTLE_PIN);
    throttleTimer->attachInterrupt(throttleTimerChannel, throttleTimerInterrupt);
    // throttleTimer->resume();
    Serial.println("Roll start");

    // rollTimer = initializeTIMER(ROLL_PIN, rollTimerChannel);
    rollTimerChannel = STM_PIN_CHANNEL(pinmap_function(ROLL_PIN, PinMap_PWM));
    throttleTimer->setMode(rollTimerChannel, TIMER_INPUT_CAPTURE_RISING, ROLL_PIN);
    throttleTimer->attachInterrupt(rollTimerChannel, rollTimerInterrupt);
    // throttleTimer->resume();
    Serial.println("Pitch start");
    // pitchTimer = initializeTIMER(PITCH_PIN, pitchTimerChannel);
    pitchTimerChannel = STM_PIN_CHANNEL(pinmap_function(PITCH_PIN, PinMap_PWM));
    throttleTimer->setMode(pitchTimerChannel, TIMER_INPUT_CAPTURE_RISING, PITCH_PIN);
    throttleTimer->attachInterrupt(pitchTimerChannel, pitchTimerInterrupt);
    // throttleTimer->resume();
    Serial.println("Yaw start");
    // yawTimer = initializeTIMER(YAW_PIN, yawTimerChannel);
    yawTimerChannel = STM_PIN_CHANNEL(pinmap_function(YAW_PIN, PinMap_PWM));
    throttleTimer->setMode(yawTimerChannel, TIMER_INPUT_CAPTURE_RISING, YAW_PIN);
    throttleTimer->attachInterrupt(yawTimerChannel, yawTimerInterrupt);
    throttleTimer->resume();
    // Serial.println(throttleTimer->getTimerClkFreq());
    // Serial.println(throttleTimer->getPrescaleFactor());
}


HardwareTimer* initializeTimer(PinName pinName) {
    TIM_TypeDef *instance = (TIM_TypeDef *)pinmap_peripheral(pinName, PinMap_PWM);

    HardwareTimer *timer = new HardwareTimer(instance);
    // timer->setPrescaleFactor(108);
    timer->setOverflow(20000, MICROSEC_FORMAT);
    return timer;
}