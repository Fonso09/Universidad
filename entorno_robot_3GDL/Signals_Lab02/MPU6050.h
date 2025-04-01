#include <stm32f7xx.h>
#include <stdio.h>
#include <cmath>
#include "I2C.h"

#define read 1
#define write 0

#define MPU6050_ADDR 0xD0

#define SMPLRT_DIV_REG 0x19
#define GYRO_CONFIG_REG 0x1B
#define ACCEL_CONFIG_REG 0x1C
#define ACCEL_XOUT_H_REG 0x3B
#define TEMP_OUT_H_REG 0x41
#define GYRO_XOUT_H_REG 0x43
#define PWR_MGMT_1_REG 0x6B
#define WHO_AM_I_REG 0x75
#define PI 3.14159265359
/*

int16_t Accel_X_RAW = 0;
int16_t Accel_Y_RAW = 0;
int16_t Accel_Z_RAW = 0;

int16_t Gyro_X_RAW = 0;
int16_t Gyro_Y_RAW = 0;
int16_t Gyro_Z_RAW = 0;

int16_t Tempt = 0;

float Ax, Ay, Az, Gx, Gy, Gz, Tmp;
float  gyr_offs_ready = 0;
float Gyr_offs[3];
float pitch = 0, roll = 0;
float kalm_pitch = 0, kalm_roll = 0, kalm_unc_pitch = 4, kalm_unc_roll = 4;
float kalm_output[2] = {0, 0};
char txt[50];

int tx_data[20]; //Se puede m�s pero por el momento no necesitamos tanto
int rx_data[20]; //Lo que se va a leer*/

void Gyr_Calib();
void MPU_Write(uint8_t address, int autoend, uint8_t NBYTES);
void MPU_Read(uint8_t address, int autoend, uint8_t NBYTES);
void MPU_Init();
void MPU_Read_Accel();
void MPU_Read_Gyro();
void MPU_Read_Temp();
float get_Pitch();
float get_Roll();
void get_Kalman_1d(float kalman_state, float kalman_uncertainty, float kalman_input, float kalman_measurement);
void get_Kalman_Roll();
float get_Acc_x();
float get_Acc_y();
float get_Acc_z();
float get_Gyr_x();
float get_Gyr_y();
float get_Gyr_z();
float get_Temp();
void get_Acc_array(float *acc_buffer);
void get_Gyr_array(float *gyr_buffer);
void get_Roll_Pitch_Yaw(double *buffer);