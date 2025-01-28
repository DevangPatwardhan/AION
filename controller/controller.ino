#include <Wire.h>
#include <Arduino.h>

#include "MPU6050Handler.h"
MPU6050Handler imu;

#include "ServoController.h"
ServoController servo;

#include "RCReceiver.h"
// Define RC receiver pins
RCReceiver receiver;

#include "BLDCMotor.h"
// Define ESC pins
#define MOTOR1  PB_6
#define MOTOR2  PB_7
#define MOTOR3  PB_8
#define MOTOR4  PB_9
BLDCMotor motor1(MOTOR1);
BLDCMotor motor2(MOTOR2);
BLDCMotor motor3(MOTOR3);
BLDCMotor motor4(MOTOR4);


// Define MPU pins
// Default pin in stm32f722zet6
// SCL PB8  Connected to MPU 6050
// SDA PB9  Connected to MPU 6050

// Define servo pins
#define SERVO1 PA10
// #define SERVO2 PA1  // Subject to change
// #define SERVO3 PA2  // Subject to change
// #define SERVO4 PA3  // Subject to change
// #define MPU6050_ADDR  0x68
// #define WHO_AM_I 0x75



int angle = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  Serial.println("Setup function");
  pinMode(PB7, INPUT);
  // pinMode(PA0, OUTPUT);

  pin_function(PA_0, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM2, 1, 0));

  radioSetup();
  test_pwm_generation();
  
  // test_pwm_reception();

}

void loop() {
  // put your main code here, to run repeatedly:

  // int pwm = 
  // motor1.setPWM(pwm)

  // imu.getIMUData();
  // test_servo_loop();

  // delay(100);

  // digitalWrite(LED, HIGH);  // turn the LED on (HIGH is the voltage level)
  // delay(1000);                      // wait for a second
  // digitalWrite(LED, LOW);   // turn the LED off by making the voltage LOW
  // delay(1000);    
}

void test_pwm_generation() {

  TIM_TypeDef* instance = (TIM_TypeDef *)pinmap_peripheral(PA_0, PinMap_PWM);
  uint32_t channel = 1;

  HardwareTimer *timer = new HardwareTimer(instance);

  // timer->setOverflow(1, HERTZ_FORMAT);

  // timer->setMode(channel, TIMER_OUTPUT_COMPARE_PWM1, PA_0);

  // timer->setCaptureCompare(channel, 50, PERCENT_COMPARE_FORMAT);
  timer->setPWM(channel, PA_0, 50, 10);

  // timer->resume();
}

// void test_pwm_reception() {
  
//   TIM_TypeDef* instance = (TIM_TypeDef *)pinmap_peripheral(PB_0_ALT1, PinMap_PWM);
//   uint32_t channel = STM_PIN_CHANNEL(pinmap_function(PB_0_ALT1, PinMap_PWM));
//   Serial.println(channel);
//   // channel = 3;

//   HardwareTimer *timer = new HardwareTimer(instance);

//   // timer->setOverflow(1, HERTZ_FORMAT);

//   timer->setMode(channel, TIMER_INPUT_CAPTURE_RISING, PB_0_ALT1);

//   // uint32_t PrescalerFactor = 1;
//   // MyTim->setPrescaleFactor(PrescalerFactor);
//   // timer->setOverflow(0x10000);
//   timer->attachInterrupt(channel, InputCapture_IT_callback);
//   timer->resume();
// }

void test_imu() {
  // Wire.begin();
  // Serial.println("Initializing");
  // imu.initializeIMU();
  // imu.calibrateIMU();
}

// void check_whoami_register() {
//   // Check WHO_AM_I register

//   uint8_t whoAmI = readRegister(MPU6050_ADDR, WHO_AM_I);

//   if (whoAmI == 0x68 || whoAmI == 0x69) {
//     Serial.print("MPU6050 detected. WHO_AM_I = 0x");
//     Serial.println(whoAmI, HEX);
//   } else {
//     Serial.print("Device not detected. WHO_AM_I = 0x");
//     Serial.println(whoAmI, HEX);
//   }
// }

// void test_servo() {
//   Serial.begin(9600);
//   servo.addServo(SERVO1);
//   pinMode(SERVO1, OUTPUT);
//   delay(100);
//   Serial.print("Is Servo attached ");
//   Serial.println(servo.servoAttached());
//   servo.setAngle(0, 0);
// }

// void test_servo_loop() {
//   servo.setAngle(0, angle);
//   Serial.print("Angle set = ");
//   Serial.println(angle);

//   int a = servo.readServo(0);
//   Serial.print("Angle read = ");
//   Serial.println(a);
//   if(angle < 90){
//     angle = angle + 5;
//   }else {
//     angle = 0;
//   }
//   delay(2000);
// }

void i2cScanner() {
    
    Serial.println("\nScanning I2C bus...");
    byte error, address;
    int deviceCount = 0;

    for(address = 1; address < 127; address++) {
        Wire.beginTransmission(address);
        error = Wire.endTransmission(false);

        if (error == 0) {
            Serial.print("I2C device found at address 0x");
            if (address < 16) {
                Serial.print("0");
            }
            Serial.print(address, HEX);
            Serial.println();
            deviceCount++;
        }
    }

    if (deviceCount == 0) {
        Serial.println("No I2C devices found!");
    } else {
        Serial.print("Found ");
        Serial.print(deviceCount);
        Serial.println(" device(s)");
    }
}