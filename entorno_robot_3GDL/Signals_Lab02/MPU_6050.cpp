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

int16_t Accel_X_RAW = 0;
int16_t Accel_Y_RAW = 0;
int16_t Accel_Z_RAW = 0;

int16_t Gyro_X_RAW = 0;
int16_t Gyro_Y_RAW = 0;
int16_t Gyro_Z_RAW = 0;

int16_t Tempt = 0;

float Ax, Ay, Az, Gx, Gy, Gz, Tmp;
float Gx_offs = 0, Gy_offs = 0, Gz_offs = 0, gyr_offs_ready = 0;
float Gyr_offs[3];
double gx = 0, gy = 0, gz = 0, ax, ay, az;
//float pitch = 0, roll = 0;
//float kalm_pitch = 0, kalm_roll = 0, kalm_unc_pitch = 4, kalm_unc_roll = 4;
//float kalm_output[2] = {0, 0};

int tx_data[20]; //Se puede m�s pero por el momento no necesitamos tanto
int rx_data[20]; //Lo que se va a leer

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

void Gyr_Calib(){
    for(int i = 0; i < 2000; i++){
        get_Gyr_array(Gyr_offs);
        Gx_offs += Gyr_offs[0];
        Gy_offs += Gyr_offs[1];
        Gz_offs += Gyr_offs[2];
    }
    Gx_offs /= 2000;
    Gy_offs /= 2000;
    Gz_offs /= 2000;
    gyr_offs_ready = 1;
}

void MPU_Write(uint8_t address, int autoend, uint8_t NBYTES){
    I2C1_Address(MPU6050_ADDR, write, autoend, NBYTES);
    I2C1_Start(); //Se debe dar a START una vez se haya configurado CR1
    I2C1_Write(tx_data, autoend, NBYTES);
    if(!autoend){
        I2C1_Stop();
    }
}

void MPU_Read(uint8_t address, int autoend, uint8_t NBYTES){
    I2C1_Address(MPU6050_ADDR, write, 0, 1);
    I2C1_Start(); //Se debe dar a START una vez se haya configurado CR1
    I2C1_Write(tx_data, 0, 1);
    I2C1_Address(MPU6050_ADDR, read, autoend, NBYTES);
    I2C1_Start(); //Se debe dar a START una vez se haya configurado CR1
    I2C1_Read(rx_data, NBYTES, autoend);
    if(!autoend){
        I2C1_Stop();
    }
}

void MPU_Init(){
    tx_data[0] = WHO_AM_I_REG;
    MPU_Read(MPU6050_ADDR, 1, 1);
    if(rx_data[0] == 104){ //se devuelve 0x68 so todo se reconoce bien
        
        // power management register 0X6B we should write all 0's to wake the sensor up
        tx_data[0] = PWR_MGMT_1_REG;
        tx_data[1] = 0;
        MPU_Write(MPU6050_ADDR, 1, 2);

        // Set DATA RATE of 1KHz by writing SMPLRT_DIV regist with 0x07
        // 1kHz / (div + 1) = FREQ  
        tx_data[0] = SMPLRT_DIV_REG;
        tx_data[1] = 93; // 0x07;
        MPU_Write(MPU6050_ADDR, 1, 2);

		// Set accelerometer configuration in ACCEL_CONFIG Register
		// XA_ST=0,YA_ST=0,ZA_ST=0, FS_SEL=0 -> ? 2g
        tx_data[0] = ACCEL_CONFIG_REG;
        tx_data[1] = 0x00;
        MPU_Write(MPU6050_ADDR, 1, 2);

		// Set Gyroscopic configuration in GYRO_CONFIG Register
		// XG_ST=0,YG_ST=0,ZG_ST=0, FS_SEL=0 -> ? 250 ?/s
        tx_data[0] = GYRO_CONFIG_REG;
        tx_data[1] = 0xE0;
        MPU_Write(MPU6050_ADDR, 1, 2);

        tx_data[0] = GYRO_CONFIG_REG;
        tx_data[1] = 0x00;
        MPU_Write(MPU6050_ADDR, 1, 2);

    }
    Gyr_Calib();
}

void MPU_Read_Accel(void)
{	
	// Read 6 BYTES of data starting from ACCEL_XOUT_H register
    tx_data[0] = ACCEL_XOUT_H_REG;
	MPU_Read (MPU6050_ADDR, 1, 6);

	Accel_X_RAW = (int16_t)(rx_data[0] << 8 | rx_data [1]);
	Accel_Y_RAW = (int16_t)(rx_data[2] << 8 | rx_data [3]);
	Accel_Z_RAW = (int16_t)(rx_data[4] << 8 | rx_data [5]);

	/*** convert the RAW values into acceleration in 'g'
	     we have to divide according to the Full scale value set in FS_SEL
	     I have configured FS_SEL = 0. So I am dividing by 16384.0
	     for more details check ACCEL_CONFIG Register              ****/

	Ax = Accel_X_RAW/16384.0;
	Ay = Accel_Y_RAW/16384.0;
	Az = Accel_Z_RAW/16384.0;
}

void MPU_Read_Gyro(void)
{	
	// Read 6 BYTES of data starting from ACCEL_XOUT_H register
    tx_data[0] = GYRO_XOUT_H_REG;
	MPU_Read (MPU6050_ADDR, 1, 6);

	Gyro_X_RAW = (int16_t)(rx_data[0] << 8 | rx_data [1]);
	Gyro_Y_RAW = (int16_t)(rx_data[2] << 8 | rx_data [3]);
	Gyro_Z_RAW = (int16_t)(rx_data[4] << 8 | rx_data [5]);

	/*** convert the RAW values into acceleration in 'g'
	     we have to divide according to the Full scale value set in FS_SEL
	     I have configured FS_SEL = 0. So I am dividing by 16384.0
	     for more details check ACCEL_CONFIG Register              ****/

	Gx = Gyro_X_RAW/131.0 - (Gx_offs * gyr_offs_ready);
	Gy = Gyro_Y_RAW/131.0 - (Gy_offs * gyr_offs_ready);
	Gz = Gyro_Z_RAW/131.0 - (Gz_offs * gyr_offs_ready);
}

void MPU_Read_Temp(void)
{	
	// Read 6 BYTES of data starting from ACCEL_XOUT_H register
    tx_data[0] = TEMP_OUT_H_REG;
	MPU_Read (MPU6050_ADDR, 1, 2);

	Tempt = (int16_t)(rx_data[0] << 8 | rx_data [1]);

	/*** convert the RAW values into acceleration in 'g'
	     we have to divide according to the Full scale value set in FS_SEL
	     I have configured FS_SEL = 0. So I am dividing by 16384.0
	     for more details check ACCEL_CONFIG Register              ****/

	Tmp = Tempt/340.0 + 36.53;

}

float get_Temp(){
    return Tmp;
}

float get_Acc_x(){
    MPU_Read_Accel();
    return Ax;
}

float get_Acc_y(){
    MPU_Read_Accel();
    return Ay;
}

float get_Acc_z(){
    MPU_Read_Accel();
    return Az;
}

float get_Gyr_x(){
    MPU_Read_Gyro();
    return Gx;
}

float get_Gyr_y(){
    MPU_Read_Gyro();
    return Gy;
}

float get_Gyr_z(){
    MPU_Read_Gyro();
    return Gz;
}

void get_Acc_array(float *acc_buffer){
    MPU_Read_Accel();
    *acc_buffer = Ax;
    *(acc_buffer + 1) = Ay; 
    *(acc_buffer + 2) = Az;
}

void get_Gyr_array(float *gyr_buffer){
    MPU_Read_Gyro();
    *gyr_buffer = Gx;
    *(gyr_buffer + 1) = Gy; 
    *(gyr_buffer + 2) = Gz;
}

float get_Pitch(){
    MPU_Read_Accel();
    return (atan((-1*Ax)/sqrt(Ay*Ay + Az*Az))) * (180.0/PI);
}

float get_Roll(){
    MPU_Read_Accel();
    return (atan(Ay/sqrt(Ax*Ax + Az*Az))) * (180.0/PI);
}

void get_Roll_Pitch_Yaw(double *buffer){
    MPU_Read_Accel();
    MPU_Read_Gyro();
    double roll = (atan((-1*Ax)/sqrt(Ay*Ay + Az*Az))) * (180.0/PI); 
    double pitch = (atan(Ay/sqrt(Ax*Ax + Az*Az))) * (180.0/PI);
    *buffer = *buffer + Gx*(1/1000);
    *(buffer + 1) = *(buffer + 1) - Gy*(1/1000);
    *(buffer + 2) = *(buffer + 2) + Gz*3/1000;
    *buffer = *buffer * 0.96 + roll * 0.04;
    *(buffer + 1) = *(buffer + 1) * 0.96 + pitch * 0.04;
}

void get_Kalman_1d(float kalman_state, float kalman_uncertainty, float kalman_input, float kalman_measurement){
    kalman_state = kalman_state + 0.004*kalman_input;
    kalman_uncertainty = kalman_uncertainty + 0.004*0.004*4*4;
    float kalman_gain = kalman_uncertainty*1/(kalman_uncertainty + 9);
    kalman_state = kalman_state +  kalman_gain*(kalman_measurement - kalman_state);
    kalman_uncertainty = (1 - kalman_state)*kalman_uncertainty;
    //kalm_output[0] = kalman_state;
    //kalm_output[1] = kalman_uncertainty;
}


