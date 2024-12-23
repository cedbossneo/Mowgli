#include "imu/imu.h"
#include "imu/mpu6050.h"
#include "soft_i2c.h"
#include "main.h"
#include <math.h>

#define POSSIBLE_I2C_ADDRESSES {0x68, 0x69, 0x70, 0x71, 0x75}

#define MPU6050_SMPRT_DIV    0x19
#define MPU6050_CONFIG       0x1a
#define MPU6050_ACCEL_XOUT_H 0x3b
#define MPU6050_GYRO_XOUT_H  0x43
#define MPU6050_PWR_MGMT_1   0x6b
#define MPU6050_DPS_FACTOR (1/131.0)
#define MPU6050_G_FACTOR   (1/16384.0)

#define MPU6050_WHO_AM_I     0x75
#define MPU6500_WHO_AM_I     0x70
#define MPU9250_WHO_AM_I     0x69
#define MPU9255_WHO_AM_I     0x71

uint8_t detected_address = 0x00;  // Adresse de l'IMU détecté

/**
  * @brief  Test Device 
  * Teste si un périphérique MPU est présent à une adresse I2C donnée
  * 
  * @param  address L'adresse I2C du périphérique
  * @retval         0 -> test échoué, 1 -> test réussi, périphérique prêt à être utilisé
  */
uint8_t MPU6050_TestDevice(uint8_t address)
{
  uint8_t val;
  /* Test who am I */
  val = SW_I2C_UTIL_Read(address, MPU6050_WHO_AM_I);
  if (val == MPU9255_WHO_AM_I || val == MPU9250_WHO_AM_I || val == MPU6500_WHO_AM_I || val == 0x73) {
      return 1;
  }
  debug_printf("    > [MPU] - Error probing for (Gyro / Accelerometer) at I2C addr=0x%0x %x\r\n", address, val);
  return 0;
}

/**
  * @brief  Détecte et initialise tout périphérique MPU connecté aux adresses I2C connues
  */
void DetectAndInitMPUs(void) {
    uint8_t possible_addresses[] = POSSIBLE_I2C_ADDRESSES;
    uint8_t num_addresses = sizeof(possible_addresses) / sizeof(possible_addresses[0]);

    for (uint8_t i = 0; i < num_addresses; i++) {
        uint8_t address = possible_addresses[i];
        if (MPU6050_TestDevice(address)) {
            detected_address = address;  // Stocke l'adresse détectée
            MPU6050_Init(detected_address);
            debug_printf("MPU device detected and initialized at address 0x%x\r\n", detected_address);
            break; // Stoppe après avoir trouvé le premier périphérique MPU disponible
        } else {
            debug_printf("No MPU device found at address 0x%x\r\n", address);
        }
    }
}

void MPU6050_Init(uint8_t address)
{
  // Disable temperature sensor, use gyroscope clock
  SW_I2C_UTIL_WRITE(address, MPU6050_PWR_MGMT_1, 0b00001001);
  // Low pass filter 10 Hz
  SW_I2C_UTIL_WRITE(address, MPU6050_CONFIG, 0x5);
  // Sample rate divider 10 (=> 1 kHz/(9+1) = 100 Hz)
  SW_I2C_UTIL_WRITE(address, MPU6050_SMPRT_DIV, 9);
  debug_printf(" * MPU initialized at address 0x%x\r\n", address);
}

void MPU6050_ReadAccelerometerRaw(float *x, float *y, float *z)
{
    uint8_t accel_xyz[6];   // 2 bytes each
    SW_I2C_UTIL_Read_Multi(detected_address, MPU6050_ACCEL_XOUT_H, 6, (uint8_t*)&accel_xyz);

    *x =  (int16_t)(accel_xyz[0] << 8 | accel_xyz[1]) * MPU6050_G_FACTOR * MS2_PER_G;
    *y =  (int16_t)(accel_xyz[2] << 8 | accel_xyz[3]) * MPU6050_G_FACTOR * MS2_PER_G;
    *z =  (int16_t)(accel_xyz[4] << 8 | accel_xyz[5]) * MPU6050_G_FACTOR * MS2_PER_G;    
}

void MPU6050_ReadGyroRaw(float *x, float *y, float *z)
{
    uint8_t gyro_xyz[6];   // 2 bytes each
    SW_I2C_UTIL_Read_Multi(detected_address, MPU6050_GYRO_XOUT_H, 6, (uint8_t*)&gyro_xyz);
    
    *x = (int16_t)(gyro_xyz[0] << 8 | gyro_xyz[1]) * MPU6050_DPS_FACTOR * RAD_PER_G;
    *y = (int16_t)(gyro_xyz[2] << 8 | gyro_xyz[3]) * MPU6050_DPS_FACTOR * RAD_PER_G;
    *z = (int16_t)(gyro_xyz[4] << 8 | gyro_xyz[5]) * MPU6050_DPS_FACTOR * RAD_PER_G;    
}
