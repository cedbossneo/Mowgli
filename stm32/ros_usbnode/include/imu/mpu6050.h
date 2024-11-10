#ifndef __MPU6050_H
#define __MPU6050_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * @brief Test for MPU-6050 at a specific I2C address
 * 
 * @param address I2C address of the device to test
 * @return zero if the device is not found, non-zero if the device is found
 */
uint8_t MPU6050_TestDevice(uint8_t address);

/**
  * @brief Initialize MPU-6050 at a specific I2C address
  * 
  * @param address I2C address of the device to initialize
  */
void MPU6050_Init(uint8_t address);

/**
  * @brief Reads the 3 accelerometer channels and stores them in *x,*y,*z
  * Units are m/s^2
  */
void MPU6050_ReadAccelerometerRaw(float *x, float *y, float *z);

/**
  * @brief Reads the 3 gyro channels and stores them in *x,*y,*z
  * Units are rad/sec
  */
void MPU6050_ReadGyroRaw(float *x, float *y, float *z);

#ifdef __cplusplus
}
#endif

#endif /* __MPU6050_H */
