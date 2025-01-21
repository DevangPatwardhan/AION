#include <Wire.h>

// #include "src/MPU6050/MPU6050.h"
// MPU6050 imu();
#include "MPU6050Handler.h"
MPU6050Handler imu;

// Pin mapping
// SCL PB8  Connected to MPU 6050
// SDA PB9  Connected to MPU 6050

void setup() {
  // put your setup code here, to run once:
  // Serial.begin(115200);

  // // i2cScanner();
  // Wire.begin();
  // imu.setDeviceID(52);
  // imu.initialize();
  // bool conn = imu.testConnection();
  // Serial.println("Initializing");
  // Serial.println(conn);

  // Serial.begin(115200);
  // Wire.begin();
  // Wire.setSCL(PB8);
  // Wire.setSDA(PB9);
  // Wire.setClock(400000); // Set I2C clock speed to 400kHz
  
  // // Wake up MPU6050
  // Wire.beginTransmission(MPU6050_ADDR);
  // Wire.write(PWR_MGMT_1);
  // Wire.write(0x00);
  // Wire.endTransmission(true);
  
  // // Verify device ID
  // Wire.beginTransmission(MPU6050_ADDR);
  // Wire.write(WHO_AM_I);
  // Wire.endTransmission(false);
  // Wire.requestFrom(MPU6050_ADDR, 1, true);
  // byte whoAmI = Wire.read();
  
  // if(whoAmI != 0x68) {
  //   Serial.println("MPU6050 not found!");
  //   Serial.println(whoAmI, HEX);
  //   while(1);
  // }
  
  // Serial.println("MPU6050 initialized successfully");
}

void loop() {
  // put your main code here, to run repeatedly:

  // mpu.getIMUData();
  delay(1000);

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