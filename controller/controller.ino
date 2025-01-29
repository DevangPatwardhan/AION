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

// extern  volatile unsigned long throttlePWM;


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
  // Just to check if PWM is getting generated
  // Connect PA0 port to PB7 the blue led will be glowing according to the PWM signal
  pinMode(PB7, INPUT);
  pin_function(PA_0, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM2, 1, 0));

  // setup for RC communication
  radioSetup();

  // code to test if PWM is working
  test_pwm_generation();
  
  // test_pwm_reception();

}

void loop() {
  // put your main code here, to run repeatedly:

  getPulsewidth();
  // noInterrupts();   
  // Serial.println("Main.ino");
  // Serial.println(throttlePWM);
  // interrupts();

  // imu.getIMUData();
  // test_servo_loop();

  delay(100);
}

void test_pwm_generation() {

  TIM_TypeDef* instance = (TIM_TypeDef *)pinmap_peripheral(PA_0, PinMap_PWM);
  uint32_t channel = 1;

  HardwareTimer *timer = new HardwareTimer(instance);

  timer->setPWM(channel, PA_0, 50, 10);

  // timer->resume();
}

void test_imu() {
  // Wire.begin();
  // Serial.println("Initializing");
  // imu.initializeIMU();
  // imu.calibrateIMU();
}

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