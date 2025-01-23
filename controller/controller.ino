#include <Wire.h>

#include "MPU6050Handler.h"
MPU6050Handler imu;

#include "ServoController.h"
ServoController servo;


// Define MPU pins
// Default pin in stm32f722zet6
// SCL PB8  Connected to MPU 6050
// SDA PB9  Connected to MPU 6050

// Define servo pins
#define SERVO1 PA0
#define SERVO2 PA1  // Subject to change
#define SERVO3 PA2  // Subject to change
#define SERVO4 PA3  // Subject to change

// Define ESC pins


// Define RC receiver pins

int angle = 0;

MPU6050_ADDR  0x68
WHO_AM_I 0x75
void setup() {
  // put your setup code here, to run once:
  // Serial.begin(9600);

  test_servo();

}

void loop() {
  // put your main code here, to run repeatedly:

  // imu.getIMUData();

}

void test_imu() {
  // Wire.begin();
  // Serial.println("Initializing");
  // imu.initializeIMU();
  // imu.calibrateIMU();
}

void check_whoami_register() {
  // Check WHO_AM_I register

  uint8_t whoAmI = readRegister(MPU6050_ADDR, WHO_AM_I);

  if (whoAmI == 0x68 || whoAmI == 0x69) {
    Serial.print("MPU6050 detected. WHO_AM_I = 0x");
    Serial.println(whoAmI, HEX);
  } else {
    Serial.print("Device not detected. WHO_AM_I = 0x");
    Serial.println(whoAmI, HEX);
  }
  
}

void test_servo() {
  Serial.begin(9600);
  pinMode(SERVO1, OUTPUT);
  servo.addServo(SERVO1);
  delay(100);
  Serial.print("Is Servo attached ");
  Serial.println(servo.servoAttached());
  servo.setAngle(0, 90);
}

void test_servo_loop() {
  servo.setAngle(0, angle);
  Serial.print("Angle set = ");
  Serial.println(angle);
  if(angle < 90){
    angle = angle + 2;
  }else {
    angle = 0;
  }
  delay(2000);
}

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