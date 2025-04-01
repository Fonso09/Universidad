#include <stm32f7xx.h>

void I2C_SETUP();
void I2C1_Start();
void I2C1_Stop();
void I2C1_Write(int *w_buffer, int autoend, uint8_t NBYTES);
void I2C1_Address(uint8_t address, uint8_t RD_WRN, int autoend, uint8_t NBYTES);
void I2C1_Read(int *r_buffer, uint8_t size, int autoend);

