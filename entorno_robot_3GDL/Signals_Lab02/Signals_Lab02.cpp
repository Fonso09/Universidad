#include <stm32f7xx.h>
#include <stdio.h>
#include <cmath>
#include <arm_math.h>
#include <math.h>  // Para usar sin() y fmod
//#include "LCD.H"
#include "I2C.h"
#include "MPU6050.h"

#define FREQ 30 // sample freq in Hz
#define FIR_LENGTH 21

volatile uint32_t tm_ms = 0; 
volatile int usart_ind = 0;
float acc[3], gyr[3];
int x_ang = 0, y_ang = 0, y_z_ang = 0, x_z_ang = 0, x_ang_offs = 0, y_ang_offs = 0;
int begin_repet = 0, exerc_ct = 0;
float pitch = 0, roll = 0, yaw = 0;
float aplha = 0.98;
float kalm_pitch = 0, kalm_roll = 0, kalm_unc_pitch = 4, kalm_unc_roll = 4;
//float ax = 0, ay = 0, az = 0;
//double gx = 0, gy = 0, gz = 0, ax, ay, az;
double gyrX = 0, gyrY = 0, gyrZ = 0;
double dt = 0;
float kalm_output[2] = {0, 0};
double roll_pitch_yaw[3] = {0, 0, 0};
char txt[50];
char rx_txt[20];

float accx = 0, accy = 0, accz = 0;


//FILTRO PASA BAJOS
float32_t t = 0;  // Tiempo de la se�al
float32_t original_signal = 0;  // Se�al original
float32_t filtered_signal1 = 0;  // Se�al filtrada 1
float32_t filtered_signal2 = 0;  // Se�al filtrada 2
float32_t filtered_signal3 = 0;  // Se�al filtrada 3

float32_t filtered_HP_signal = 0;  // Se�al filtrada
float32_t filtered_BP_signal = 0;  // Se�al filtrada

///FILTRO PASABAJOS
// Definir los coeficientes del filtro FIR (ejemplo: 101 coeficientes)
float32_t fir_coef[FIR_LENGTH] = {
    0.018433, 0.021522, 0.027203, 0.034641, 0.0429, 
    0.051142, 0.058717, 0.065145, 0.070054, 0.073142, 
    0.074197, 0.073142, 0.070054, 0.065145, 0.058717, 
    0.051142, 0.0429, 0.034641, 0.027203, 0.021522, 
    0.018433
};




// Declaraci�n de variables para CMSIS FIR
float32_t fir_state[FIR_LENGTH];  // Buffer de estado
arm_fir_instance_f32 fir_instance; // Estructura de filtro
float32_t fir_in, fir_out;

void SysTick_Init(void);                   //funciones para los delays
void SysTick_Wait(uint32_t n);
void SysTick_Wait1ms(uint32_t delay);
void SysTick_Wait1us(uint32_t delay);
void Ms_Counter();
void Delay_ms(uint16_t ms);

void RCC_SETUP();
void USART_SETUP();
void GPIO_SETUP();
void EXTI_SETUP();
void TIM_SETUP();
void USART3_SendChar(int value);	
void Serial_buffer(char *send_txt);
void Serial();
void Gyr_Calib();
//void LCD();
void Angulos();
int X_Offset();
int Y_Offset();
void Contador_Ejercicio();
void Usart_Funct();
float32_t LowPass_Filter(float32_t signal);

int main(){
    arm_fir_init_f32(&fir_instance, FIR_LENGTH, fir_coef, fir_state, 1);
    SysTick_Init(); //Configurado con interrupciones para que contara cada 1ms
    NVIC_EnableIRQ(SysTick_IRQn);
    RCC_SETUP();  
    GPIO_SETUP();
    USART_SETUP();
    TIM_SETUP();
    I2C_SETUP();
    MPU_Init(); 
    //LCD_Init();
    Ms_Counter(); //Tener cuidado con poner la interruopci�n m�s corta que el proceso de obtenci�n de datos
    Delay_ms(5);
    TIM5->DIER |= 0x1; //Habilita Interrupcion
    NVIC_EnableIRQ(TIM5_IRQn);

    while(1){
        Usart_Funct();
        float32_t signal1 = (float32_t) acc[0];
        float32_t signal2 = (float32_t) acc[1];
        float32_t signal3 = (float32_t) acc[2];
        filtered_signal1 = LowPass_Filter(signal1);
        filtered_signal2 = LowPass_Filter(signal2);
        filtered_signal3 = LowPass_Filter(signal3);
			//Delay_ms(50);
			//Delay_ms(1);
			
//        get_Kalman_1d(kalm_pitch, kalm_unc_pitch, Gy, pitch);
//        kalm_pitch = kalm_output[0];
//        kalm_unc_pitch = kalm_output[1];
//        get_Kalman_1d(kalm_roll, kalm_unc_roll, Gx, roll);
//        kalm_roll = kalm_output[0];
//        kalm_unc_roll = kalm_output[1];
    }
    
}

void Usart_Funct(){
//    float rad_roll = roll_pitch_yaw[0]/(180/PI), rad_pitch =  roll_pitch_yaw[1]/(180/PI), rad_yaw = roll_pitch_yaw[2]/(180/PI);
//    //sprintf(txt, "%.2f,%.2f,%.2f\n", acc[0], acc[1], acc[2]);
//	sprintf(txt, "%.2f\n", acc[2]);
    Serial_buffer(txt);
//    for(int i = 0; i <= 18; i++){
//        USART3_SendChar(txt[i]);
//        if(txt[i] == 10){i = 20;}
//        SysTick_Wait1ms(5);
//    }
}
void Contador_Ejercicio(){
    if(y_z_ang < -65){
        begin_repet = 1;
    }
    if(y_z_ang > 75 && begin_repet){
        begin_repet = 0;
        exerc_ct++;
    }
}

void Angulos(){
    x_ang_offs = X_Offset();
    y_ang_offs = Y_Offset();
    x_ang = -1*(atan(acc[0]/acc[1])*(180/PI) + x_ang_offs);
    y_ang = -1*(atan(acc[1]/acc[0])*(180/PI) + y_ang_offs);
    y_z_ang = atan(acc[1]/acc[2])*(180/PI);
}

int X_Offset(){
    if(acc[0] < 0 && acc[1] < 0){
        return -180;
    }else if(acc[0] > 0 && acc[1] < 0){
        return 180;
    }
    return -1;
}

int Y_Offset(){
    if(acc[0] > 0 && acc[1] > 0){
        return -180;
    }else if(acc[0] > 0 && acc[1] < 0){
        return 180;
    }
    return 0;
}


void Serial(){
    sprintf(txt, "Hola\n");
    USART3_SendChar(txt[usart_ind]);
}
void Serial_buffer(char *send_txt){
    while(*send_txt != '\n'){
        USART3_SendChar(*send_txt);
        send_txt++;
        Delay_ms(10);
        //SysTick_Wait1ms(5);
    }
    USART3_SendChar(10);
}

float32_t LowPass_Filter(float32_t signal){
    fir_in = signal;
    arm_fir_f32(&fir_instance, &fir_in, &fir_out, 1);
    return fir_out;  // Se�al filtrada
}

void RCC_SETUP(){
    RCC->AHB1ENR |= 0xA;
    RCC->APB1ENR |= (1<<21) | (1<<18); //I2C EN y USART3 EN
    RCC->APB2ENR |= (1<<14); //EXTI EN
    RCC->APB1ENR |= (1<<3)|(1<<2); //Habilita Tim 5 y 4
}

void GPIO_SETUP(){
    GPIOB->MODER |= (1<<0)|(1<<2)|(1<<28)|(1<<14)|(0xA<<16);// lEDs e I2C PB8(SCL) Y PB9(SDA) como alternante
    GPIOD->MODER |= 0xA0000; //Para USART3 PD8(TX) Y PD9(RX) como alternante
    GPIOB->OTYPER |= 0x300; //PB8 y 9 como open drain
    GPIOB->OSPEEDR |= 0xF0000; //PB8 y 9 como high speed
    GPIOB->PUPDR |= 0x50000;
    GPIOB->AFR[1] |= 0x44;
    GPIOD->AFR[1] |= 0x77; //Su respectiva funci?n alternante, se encuentra en mapa AF
}

void USART_SETUP(){
    USART3->BRR |= 0x113; //BAUD RATE 115200
    //USART3->CR1 |= ((0xD) | (0<<15));
    USART3->CR1 |= ((0x2D) | (1<<15)); //Pal oversampling que va al baud rate
    //USART EN, Reciever y Transmiter EN, RX INTERRUPT EN
    NVIC_EnableIRQ(USART3_IRQn); //nombre en tabla del NVIC
}

void USART3_SendChar(int value){
    USART3->TDR = value; //Se escribe lo que se va a enviar
    while(!(USART3->ISR & USART_ISR_TXE)); //A la posici?n 7
    //No sale de la funci?n hasta que indica que envi? el dato
}

void SysTick_Init(void) {
    SysTick->LOAD = 0xFFFFFF; //Cargar el valor en el que se da el overload
    SysTick->CTRL = 0x00000005; //Habilita systick, interrupci?n (bit 1) desactivada, y la fuente de reloj
}
void SysTick_Wait(uint32_t n) {
    SysTick->LOAD = n - 1;
    SysTick->VAL = 0;
    SysTick->CTRL &= ~(1<<1); //Desahibilitamos la interrupci?n
    while ((SysTick->CTRL & 0x00010000) == 0); //Se ponga en alto la flag del overload
}
void SysTick_Wait1ms(uint32_t delay) {
    for (uint32_t i = 0; i < delay; i++) {
        SysTick_Wait(16000);
    }
}
void SysTick_Wait1us(uint32_t delay) {
    for (uint32_t i = 0; i < delay; i++) {
        SysTick_Wait(16);
    }
}
void Ms_Counter(){
    dt = 0;
    SysTick->LOAD = 63999;
    SysTick->VAL = 0;
    SysTick->CTRL |= (1<<1); //Habilitamos la interrupci?n
    //while ((SysTick->CTRL & 0x00010000) == 0);
}

void TIM_SETUP(){

	RCC->APB1ENR|=0x18; //Prender Timer 6 y 5
	TIM6->ARR = 2000-1; //ARR
	TIM6->PSC = 8-1; //PSC Para tener 1 kHz
    TIM6->EGR|=0x1; //Reinicio del conteo
	TIM6->CR1 |= 0x1; //Habilitar conteo
	
    TIM4->ARR = 2000-1; //ARR
	TIM4->PSC = 8-1; //PSC Para tener 500 Hz
    TIM4->EGR|=0x1; //Reinicio del conteo
	TIM4->CR1 |= 0x1; //Habilitar conteo
	TIM4->DIER |= 0x1; // habilita interruupción

    TIM5->PSC = 1600-1;  // Para tener 500Hz
    TIM5->ARR = 20-1; //ARR Para tener 500Hz
    //TIM5->DIER |= 0x1; //Habilita Interrupcion
    TIM5->EGR |= 0x1; //Reinicio del conteo
    TIM5->CR1|=0X1; //Habilitar conteo

	//NVIC_EnableIRQ(TIM5_IRQn); //Por si queremos la interrupcion que igual trabaja con el ARR

	NVIC_EnableIRQ(TIM4_IRQn);
	
	
	
	while (!(TIM6->SR & (1<<0)));  // UIF: Update interrupt flag..  This bit is set by hardware when the registers are updated
}

void Delay_ms(uint16_t ms){
    TIM6->CNT = 0;
    for(int i = 0; i < ms; i++){ 
        while(!(TIM6->SR & (1<<0)));
        TIM6->SR &=~(1UL<<0);
    }
    
}

extern "C"{
    void USART3_IRQHandler(void){

		while (USART3->ISR & USART_ISR_RXNE){
            char rx = (char)(USART3->RDR & 0xFF);
            switch(rx) //rx is the variable to compare
            {
            case 'S': //Task for char 'M';
                GPIOB->BSRR |= (1<<0);
                break;
            case 'N': //Task for char 'N';	
                GPIOB->BSRR |= (1<<7);
                break;
            default:
                GPIOB->ODR ^= (1<<14);
                break;
            }
			
		}	
		 	
	}
	void TIM4_IRQHandler(void){
        TIM4->SR &=~(1UL<<0);
        GPIOB->ODR^=(3<<0); //Bascula cada 500Hz
		float rad_roll = roll_pitch_yaw[0]/(180/PI), rad_pitch =  roll_pitch_yaw[1]/(180/PI), rad_yaw = roll_pitch_yaw[2]/(180/PI);
        
    //sprintf(txt, "%.2f,%.2f,%.2f\n", acc[0], acc[1], acc[2]);
	//sprintf(txt, "%.2f\n", acc[2]);
    sprintf(txt, "%.2f,%.2f,0\n", acc[2], filtered_signal);
    }
    void TIM5_IRQHandler(void){
        TIM5->SR &=~(1UL<<0);
        get_Acc_array(acc);
    }

    void SysTick_Handler(void) {
        //dt++;
        dt = 1;
        //get_Roll_Pitch_Yaw(roll_pitch_yaw);
			//get_Acc_array(acc);
			//GPIOB->ODR^=(3<<0); //Bascula cada 500Hz
        /*get_Acc_array(acc);
        get_Gyr_array(gyr);
        pitch = get_Pitch();
        roll = get_Roll();
        gx = gx + gyr[0]*(1/1000);
        gy = gy - gyr[1]*(1/1000);
        gz = gz + get_Gyr_z()*3/1000;
        gx = gx * 0.96 + pitch * 0.04;
        gy = gy * 0.96 + roll * 0.04;*/
	}
    
}