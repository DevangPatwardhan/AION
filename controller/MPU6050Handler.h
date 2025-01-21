

#ifndef MPUHandler_h
#define MPUHandler_h

#include "Arduino.h"

#include "src/MPU6050/I2Cdev.h"
#include "src/MPU6050/MPU6050.h"


class MPU6050Handler
{
public:

    MPU6050Handler();
    void setIMUScaleFactor();
    void initializeIMU();
    void calibrateIMU();
    void getIMUData();

private:
    /* data */
    float _acc[3];
    float _acc_bias[3];
    float _gyro[3];
    float _gyro_bias[3];

    uint8_t _gyro_fs;
    float _gyro_scale_factor;
    uint8_t _acc_fs;
    float _acc_scale_factor;

    MPU6050 mpu;
};

#endif