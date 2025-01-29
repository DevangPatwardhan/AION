#include <Arduino.h>

// Refer HAL_TIM_MODULE_ENABLED section in PeripheralPins.c to find the pin name for corresponding timer
#define THROTTLE_PIN PB_4 //TIM3 CH1
#define ROLL_PIN  PB_6  //TIM4  CH1
#define PITCH_PIN PA_1 //TIM2 CH2
#define YAW_PIN PA_2_ALT1  //TIM5  CH3

uint32_t throttleTimerChannel;
uint32_t rollTimerChannel;
uint32_t pitchTimerChannel;
uint32_t yawTimerChannel;

HardwareTimer *throttleTimer;
HardwareTimer *rollTimer;
HardwareTimer *pitchTimer;
HardwareTimer *yawTimer;

unsigned long throttlePWMRiseTime = 0;
unsigned long rollPWMRiseTime = 0;
unsigned long pitchPWMRiseTime = 0;
unsigned long yawPWMRiseTime = 0;

bool throttleRisingEdge = true;
bool rollRisingEdge = true;
bool pitchRisingEdge = true;
bool yawRisingEdge = true;

unsigned long throttleStartTime = 0;
volatile unsigned long throttlePWM = 0;

unsigned long rollStartTime = 0;
unsigned long rollPWM = 0;

unsigned long pitchStartTime = 0;
unsigned long pitchPWM = 0;

unsigned long yawStartTime = 0;
unsigned long yawPWM = 0;


void timerInterrupt(HardwareTimer* timer, uint32_t &timerChannel, PinName pinName, unsigned long &startTime, bool &isRisingEdge, volatile unsigned long &pulseWidth) {
/**
 * Timer callback function that will be executed whenever the PWM pulse changes from
 * low to high or vice versa and calculated the pulsewidth
 * 
 * @param timer: Timer object which handles all timer related functions
 * @param timerChannel: specific channel of the timer used
 * @param pinName: Pin name
 * @param startTime: Last time when the pwm pulse width went high 
 * @param risingEdge: State maintained whether the current edge is a rising edge
 * @param pulseWidth: Calculated pulse width of the pwm signal
 */
  if(isRisingEdge) {
    // get the time when there is a rising edge
    startTime = timer->getCaptureCompare(timerChannel);
    // change the timer to watch out for the falling edge
    timer->setMode(timerChannel, TIMER_INPUT_CAPTURE_FALLING, pinName);
    timer->resume();
  } else {
    // get the time when there is a falling edge
    unsigned long endTime = timer->getCaptureCompare(timerChannel);

    if(endTime >= startTime) {
      pulseWidth = endTime - startTime;
    } else {
      // Incase the code did not get executed when there was a falling edge
      pulseWidth = ((timer->getOverflow() - throttleStartTime) + endTime);
    }

    // int scaleFactor = int(trunc(timer->getPrescaleFactor()/timer->getTimerClkFreq()));
    // pulseWidth = pulseWidth;
    timer->setMode(timerChannel, TIMER_INPUT_CAPTURE_RISING, pinName);
    timer->resume();
    Serial.print("Pulsewidth = ");
    Serial.println(pulseWidth);
    Serial.print("PWM = ");
    Serial.println((pulseWidth - 3272)/3437.0);

  }

  isRisingEdge = !isRisingEdge;
  // uint8_t port = STM_PORT(pinName);
  // uint8_t pinIndex = STM_PIN(pinName);
  // Serial.print("Pin Name: P");
  // Serial.print(char('A' + port)); // Convert port number to 'A', 'B', 'C', etc.
  // Serial.println(pinIndex);

}

void getPulsewidth() {
      // Throttle
      noInterrupts();
      float PWM = (throttlePWM - 3272)/3437.0;
      Serial.println("Main loop");
      Serial.print("Throttle PWM = ");
      Serial.println(throttlePWM);
      Serial.print("PWM = ");
      Serial.println(PWM);
      Serial.println("Main loop end");
      interrupts();
      // return throttlePWM;
}

void throttleTimerInterrupt() {
  timerInterrupt(throttleTimer, throttleTimerChannel, THROTTLE_PIN, throttleStartTime, throttleRisingEdge, throttlePWM);
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
    // attach the callback function that should be executed during timer event
    throttleTimer->attachInterrupt(throttleTimerChannel, throttleTimerInterrupt);
    throttleTimer->resume();

    Serial.println("Roll start");
    rollTimer = initializeTIMER(ROLL_PIN, rollTimerChannel);
    rollTimerChannel = STM_PIN_CHANNEL(pinmap_function(ROLL_PIN, PinMap_PWM));
    rollTimer->setMode(rollTimerChannel, TIMER_INPUT_CAPTURE_RISING, ROLL_PIN);
    rollTimer->attachInterrupt(rollTimerChannel, rollTimerInterrupt);
    rollTimer->resume();

    Serial.println("Pitch start");
    pitchTimer = initializeTIMER(PITCH_PIN, pitchTimerChannel);
    pitchTimerChannel = STM_PIN_CHANNEL(pinmap_function(PITCH_PIN, PinMap_PWM));
    pitchTimer->setMode(pitchTimerChannel, TIMER_INPUT_CAPTURE_RISING, PITCH_PIN);
    pitchTimer->attachInterrupt(pitchTimerChannel, pitchTimerInterrupt);
    pitchTimer->resume();

    Serial.println("Yaw start");
    yawTimer = initializeTIMER(YAW_PIN, yawTimerChannel);
    yawTimerChannel = STM_PIN_CHANNEL(pinmap_function(YAW_PIN, PinMap_PWM));
    yawTimer->setMode(yawTimerChannel, TIMER_INPUT_CAPTURE_RISING, YAW_PIN);
    yawTimer->attachInterrupt(yawTimerChannel, yawTimerInterrupt);
    yawTimer->resume();

    // Serial.println(throttleTimer->getTimerClkFreq());
    // Serial.println(throttleTimer->getPrescaleFactor());
    Serial.println("Radio initialized");
}


HardwareTimer* initializeTimer(PinName pinName) {
    TIM_TypeDef *instance = (TIM_TypeDef *)pinmap_peripheral(pinName, PinMap_PWM);

    HardwareTimer *timer = new HardwareTimer(instance);
    // set the timer to measure 50Hz pulses
    timer->setOverflow(20000, MICROSEC_FORMAT);
    return timer;
}