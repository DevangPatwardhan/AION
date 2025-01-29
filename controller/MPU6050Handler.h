

#ifndef MPUHandler_h
#define MPUHandler_h

#include "Arduino.h"

#include "src/MPU6050/I2Cdev.h"
#include "src/MPU6050/MPU6050.h"

/**
 * This class interfaces MPU 6050 to the main code
 */

class MPU6050Handler
{
public:

    MPU6050Handler(uint8_t addr=0x68);
    void setIMUScaleFactor();
    void initializeIMU();
    void calibrateIMU();
    void getIMUData();

private:
    /* data */
    float _acc[3];
    // float _acc_bias[3] = {0.03,-0.02,0.11};
    float _acc_bias[3] = {0,0,0};
    float _gyro[3];
    // float _gyro_bias[3] = {-3.46,1.37,0.67};
    float _gyro_bias[3] = {0,0,0};

    uint8_t _gyro_fs;
    float _gyro_scale_factor;
    uint8_t _acc_fs;
    float _acc_scale_factor;

    MPU6050 mpu;
};

#endif