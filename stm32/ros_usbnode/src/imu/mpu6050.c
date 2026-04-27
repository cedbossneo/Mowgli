#include "imu/imu.h"
#include "imu/mpu6050.h"
#include "soft_i2c.h"
#include "main.h"
#include <math.h>

#define MPU6500_WHO_AM_I     0x70
#define MPU9250_WHO_AM_I     0x71
#define MPU9255_WHO_AM_I     0x73

#define MPU6050_DPS_FACTOR (1/131.0)
#define MPU6050_G_FACTOR   (1/16384.0)

#ifndef DISABLE_MPU6050

/**
  * @brief  Test Device 
  * Perform any tests possible before actually enabling and using the device,
  * for example check the i2c address and whoami registers if existing  
  *
  * @retval          0 -> test failed 1-> test ok, good to init and use
  */
uint8_t MPU6050_TestDevice(void)
{
  uint8_t  val;
  /* Test who am I */
  val = SW_I2C_UTIL_Read(MPU6050_DEFAULT_ADDRESS, MPU6050_RA_WHO_AM_I);
  if (val == MPU6500_WHO_AM_I ||  val == MPU6050_DEFAULT_ADDRESS || val == MPU9255_WHO_AM_I || val == MPU9250_WHO_AM_I) return 1;
  debug_printf("    > [MPU-6050] - Error probing for (Gyro / Accelerometer) at I2C addr=0x%0x %x\r\n", MPU6050_DEFAULT_ADDRESS,val);
  return 0;
}

void MPU6050_Init(void)
{
  /**
   * The MPU-60X0 can be put into Low Power Mode by setting CYCLE to 1 in the Power Management 1 register (Register 107). In this mode, the device will power off all devices except for the primary I2C interface, waking only the accelerometer at fixed intervals to take a single measurement. The frequency of wake-ups can be configured with LP_WAKE_CTRL as shown below:
   *
   * LP_WAKE_CTRL | Wake-up Frequency
   * -------------+------------------
   * 0            | 1.25 Hz
   * 1            | 2.5 Hz
   * 2            | 20 Hz
   * 3            | 40 Hz
   */

  /** PWR_MGMT_1 register :
  SLEEP to 0 : sleep mode disabled
  CYCLE to 1 : low power mode with period wake up enabled
  TMP_DIS to 1 : disable temperature sensor
  CLKSEL[1:0] to 0 : use oscillator clock
  */
  SW_I2C_UTIL_WRITE(MPU6050_DEFAULT_ADDRESS, MPU6050_RA_PWR_MGMT_1, 0b00101000);

  /** PWR_MGMT_2 register
  LP_WAKE_CTRL[7:6] set to 2 (20Hz). Specifies to wake up at 20 Hz during low power mode
  STBY_XA When set to 1, this bit puts the X axis accelerometer into standby mode.
  STBY_YA When set to 1, this bit puts the Y axis accelerometer into standby mode.
  STBY_ZA When set to 1, this bit puts the Z axis accelerometer into standby mode.
  STBY_XG When set to 1, this bit puts the X axis gyroscope into standby mode.
  STBY_YG When set to 1, this bit puts the Y axis gyroscope into standby mode.
  STBY_ZG When set to 1, this bit puts the Z axis gyroscope into standby mode.
  */
  SW_I2C_UTIL_WRITE(MPU6050_DEFAULT_ADDRESS, MPU6050_RA_PWR_MGMT_2, 0b10111111);

  /** CONFIG register :
  DLPF_CFG[2:0] set to 5 : Low pass filter 10 Hz
  */
  SW_I2C_UTIL_WRITE(MPU6050_DEFAULT_ADDRESS, MPU6050_RA_CONFIG, 0x5);

  /** SMPLRT_DIV register :
   SMPLRT_DIV[7:0] set to 9 : sample rate divider 10 (=> 1 kHz/(9+1) = 100 Hz)
  */
  SW_I2C_UTIL_WRITE(MPU6050_DEFAULT_ADDRESS, MPU6050_RA_SMPLRT_DIV, 9);

  // ACCEL_CONFIG
  // GYRO_CONFIG
  // We don't touch the default configuration: 250°/s, +/- 2g

  debug_printf(" * MPU 6050 initialized\r\n");
}

/**
  * @brief  Reads the 3 accelerometer channels and stores them in *x,*y,*z
  * units are m/s^2
  */
void MPU6050_ReadAccelerometerRaw(float *x, float *y, float *z)
{
    uint8_t accel_xyz[6];   // 2 bytes each

    SW_I2C_UTIL_Read_Multi(MPU6050_DEFAULT_ADDRESS, MPU6050_RA_ACCEL_XOUT_H, 6, (uint8_t*)&accel_xyz);

    *x =  (int16_t)(accel_xyz[0] << 8 | accel_xyz[1]) * MPU6050_G_FACTOR * MS2_PER_G;
    *y =  (int16_t)(accel_xyz[2] << 8 | accel_xyz[3]) * MPU6050_G_FACTOR * MS2_PER_G;
    *z =  (int16_t)(accel_xyz[4] << 8 | accel_xyz[5]) * MPU6050_G_FACTOR * MS2_PER_G;    
}

/**
  * @brief  Reads the 3 gyro channels and stores them in *x,*y,*z
  * units are rad/sec
  */
void MPU6050_ReadGyroRaw(float *x, float *y, float *z)
{
    uint8_t gyro_xyz[6];   // 2 bytes each

    SW_I2C_UTIL_Read_Multi(MPU6050_DEFAULT_ADDRESS, MPU6050_RA_GYRO_XOUT_H, 6, (uint8_t*)&gyro_xyz);
    
    *x = (int16_t)(gyro_xyz[0] << 8 | gyro_xyz[1]) * MPU6050_DPS_FACTOR * RAD_PER_G;
    *y = (int16_t)(gyro_xyz[2] << 8 | gyro_xyz[3]) * MPU6050_DPS_FACTOR * RAD_PER_G;
    *z = (int16_t)(gyro_xyz[4] << 8 | gyro_xyz[5]) * MPU6050_DPS_FACTOR * RAD_PER_G;    
}

#endif
