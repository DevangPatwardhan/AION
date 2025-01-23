
#include <Arduino.h>
#include <Wire.h>

#include "MPU6050Handler.h"

MPU6050Handler::MPU6050Handler(uint8_t addr): mpu(addr) {
    setIMUScaleFactor();
}

void MPU6050Handler::setIMUScaleFactor() {
    /**
     * Set the scale factor and full scale range of the accelerometer and gyro sensors
     * Uncomment based on the sensor accuracy used
     */

    _acc_fs = MPU6050_ACCEL_FS_2;
    _acc_scale_factor = 16384.0;

    // _acc_fs = MPU6050_ACCEL_FS_4
    // _acc_scale_factor = 8192.0;

    // _acc_fs = MPU6050_ACCEL_FS_8
    // _acc_scale_factor = 4096.0;

    // _acc_fs = MPU6050_ACCEL_FS_16
    // _acc_scale_factor = 2048.0;

    _gyro_fs = MPU6050_GYRO_FS_250;
    _gyro_scale_factor = 131.0;

    // _gyro_fs = MPU6050_GYRO_FS_500;
    // _gyro_scale_factor = 65.5;

    // _gyro_fs = MPU6050_GYRO_FS_1000;
    // _gyro_scale_factor = 32.8;

    // _gyro_fs = MPU6050_GYRO_FS_2000;
    // _gyro_scale_factor = 16.4;

}

void MPU6050Handler::initializeIMU() {
    /**
    * Initializes the MPU6050 and tests the connection
    */

    Wire.begin();
    // Wire.setClock(1000000);     //Note this is 2.5 times the spec sheet 400 kHz max

    mpu.initialize();

    if(!mpu.testConnection()) {
        Serial.println("MPU 6050 initialization unsuccessful");
        Serial.println("Check MPU 6050 wiring");

        // Pause the execution here
        while(1){}
    }else {
      Serial.println("MPU 6050 initialized successfully");
    }

    mpu.setFullScaleAccelRange(_acc_fs);
    mpu.setFullScaleGyroRange(_gyro_fs);
}

void MPU6050Handler::calibrateIMU() {
    // Computes IMU accelerometer and gyro error on startup. Note: vehicle should be powered up on flat surface
    /*
    * IMU_data = value + bias_error + random_noise
    * random_noise will be average to zero
    * This function computes the bias error which will later be used to correct the values obtained from the sensor
    */
    int16_t AcX,AcY,AcZ,GyX,GyY,GyZ,MgX,MgY,MgZ;

    //Read IMU values 12000 times
    int c = 0;
    while (c < 12000) {
        mpu.getMotion6(&AcX, &AcY, &AcZ, &GyX, &GyY, &GyZ);

    _acc_bias[0]  += AcX / _acc_scale_factor;
    _acc_bias[1]  += AcY / _acc_scale_factor;
    _acc_bias[2]  += AcZ / _acc_scale_factor;
    _gyro_bias[0] += GyX / _gyro_scale_factor;
    _gyro_bias[1] += GyY / _gyro_scale_factor;
    _gyro_bias[2] += GyZ / _gyro_scale_factor;

    c++;
    }
    //Divide the sum by 12000 to get the error value
    _acc_bias[0]  = _acc_bias[0] / c;
    _acc_bias[1]  = _acc_bias[1] / c;
    _acc_bias[2]  = _acc_bias[2] / c - 1.0;
    _gyro_bias[0] = _gyro_bias[0] / c;
    _gyro_bias[1] = _gyro_bias[1] / c;
    _gyro_bias[2] = _gyro_bias[2] / c;

    Serial.print("float AccErrorX = ");
    Serial.print(_acc_bias[0]);
    Serial.println(";");
    Serial.print("float AccErrorY = ");
    Serial.print(_acc_bias[1]);
    Serial.println(";");
    Serial.print("float AccErrorZ = ");
    Serial.print(_acc_bias[2]);
    Serial.println(";");

    Serial.print("float GyroErrorX = ");
    Serial.print(_gyro_bias[0]);
    Serial.println(";");
    Serial.print("float GyroErrorY = ");
    Serial.print(_gyro_bias[1]);
    Serial.println(";");
    Serial.print("float GyroErrorZ = ");
    Serial.print(_gyro_bias[2]);
    Serial.println(";");

    Serial.println("Paste these values in user specified variables section and comment out calculate_IMU_error() in void setup.");
}

void MPU6050Handler::getIMUData() {
    int16_t AcX, AcY, AcZ, GyX, GyY, GyZ;

    mpu.getMotion6(&AcX, &AcY, &AcZ, &GyX, &GyY, &GyZ);

    _acc[0] = (AcX / _acc_scale_factor) + _acc_bias[0];
    _acc[1] = (AcY / _acc_scale_factor) + _acc_bias[1];
    _acc[3] = (AcZ / _acc_scale_factor) + _acc_bias[2];

    _gyro[0] = (GyX / _gyro_scale_factor) + _gyro_bias[0];
    _gyro[1] = (GyY / _gyro_scale_factor) + _gyro_bias[1];
    _gyro[2] = (GyZ / _gyro_scale_factor) + _gyro_bias[2];

    Serial.print("AccX:");
    Serial.print(_acc[0]);
    Serial.print("AccY:");
    Serial.print(_acc[1]);
    Serial.print("AccZ:");
    Serial.println(_acc[2]);

    Serial.print("GyroX:");
    Serial.println(_gyro[0]);
    Serial.print("GyroY:");
    Serial.println(_gyro[1]);
    Serial.print("GyroZ:");
    Serial.println(_gyro[2]);
}



