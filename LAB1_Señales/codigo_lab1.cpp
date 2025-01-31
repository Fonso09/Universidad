
#include <stm32f7xx.h>
#include <stdio.h>
void RCC_SETUP();
void GPIO_SETUP();
void TIM_Config();
void ADC_SETUP();
void DAC_SETUP();
void USART_SETUP();
void USART3_SendChar(int value);
void Serial_buffer(char *send_txt);
float map(int value, int fromLow, int fromHigh, int toLow, int toHigh);
int send = 0, adc = 0;
float adc_mapped = 0;
char tx_txt[10];
int main(){
	RCC_SETUP();
    ADC_SETUP();
    DAC_SETUP();
    GPIO_SETUP();
	TIM_Config();
    USART_SETUP();

	while(1){
        ADC1->CR2 |= (1<<30); // INICIO CONVERSION
        while((ADC1->SR & (1<<1)) == 0){ADC1->SR = 0;} 
        adc = ADC1->DR;
        adc_mapped = map(adc,0,4096,0,100);

	    if(send == 1){
//		    ADC1->CR2 |= (1<<30); // INICIO CONVERSION
//          while((ADC1->SR & (1<<1)) == 0){ADC1->SR = 0;} 
//          adc = ADC1->DR;
            send = 0;
            //sprintf(tx_txt, "%d\n", adc);
            //Serial_buffer(tx_txt);

			//DAC->DHR12R2 = salida;

        }
		
	};
	
}
float map(int value, int fromLow, int fromHigh, int toLow, int toHigh) {
    if(value < fromLow) return toLow;
    if(value > fromHigh) return toHigh;
    return (value - fromLow) * (toHigh - toLow) / (fromHigh - fromLow) + toLow;
}
void Serial_buffer(char *send_txt){
    while(*send_txt != '\n'){
        USART3_SendChar(*send_txt);
        send_txt++;
        //Delay_ms(8);
    }
    USART3_SendChar(10);
}

void USART3_SendChar(int value){
    USART3->TDR = value; //Se escribe lo que se va a enviar
    while(!(USART3->ISR & USART_ISR_TXE)); //A la posici?n 7
    //No sale de la funci?n hasta que indica que envi? el dato
}
void RCC_SETUP(){
	RCC-> AHB1ENR |= 0xFF; //Activa los puertos de la A a la F
    RCC->APB1ENR |= (1<<18)|(1<<3)|(1<<2)|(1<<29); //habilita el USART3, Habilita TIM5, Habilita TIM4 y habilita el DAC
    RCC->APB2ENR |= (1<<5)|(7<<8); //Habilita el USART6 Y ADC 1,2 y 3
}

void GPIO_SETUP(){
    GPIOC->MODER |= 0X3; // ADC para PC0

    GPIOB->MODER |= (1<<14)|(1<<28)|(1<<18); //LEDS PRUEBA de la placa PB7 y PB14

    GPIOD->MODER |= 0xA0000;  //Pines a usar por el USART3 PD8(TX) y PD9(RX)
    GPIOD->AFR[1] |= 0x77; // Viendo el Datasheet se puede observar que el PD8 7 PD9 para USART3 se debe activar el AF7 y estan en la parte high del AFR

    GPIOA->MODER |=(2<<0)|(2<<2)|(2<<4)|(2<<6); //Modo alternante para el TIM5|PWM, PA0, PA1, PA2, PA3
    GPIOA->MODER |= 0xC00; //DAC PA5 canal 2    
    GPIOA->AFR[0] |= 0x2222; //AF2 para el TIM5 de PA0, PA1, PA2 y PA3

    GPIOB->MODER |= (2<<16)|(2<<18); // PB8 para TIM4CH3 y PB9 para TIM4CH4
    GPIOB->AFR[1] |= 0x22; //AF2 del TIM4 para PB8 y PB9

}
void TIM_Config(){
    TIM4->PSC = 16-1;  // Para tener 10KHz
    TIM4->ARR = 100-1; //ARR Para tener 10KHz
    TIM4->DIER |= 0x1; //Habilita Interrupcion
    TIM4->EGR |= 0x1; //Reinicio del conteo
    TIM4->CR1|=0X1; //Habilitar conteo
	NVIC_EnableIRQ(TIM4_IRQn); //Por si queremos la interrupcion que igual trabaja con el ARR

    TIM5->PSC = 32-1;  // Para tener 5KHz, que es x10 la frecuencia de muestreo
    TIM5->ARR = 100-1; //ARR, Para tener 5KHz, que es x10 la frecuencia de muestreo
    TIM5->DIER |= 0x1; //Habilita Interrupcion
    TIM5->EGR |= 0x1; //Reinicio del conteo
	TIM5->CCMR1|=0X6060; //Modo PWM Tim5  CH1 y 2 - Como salida 00 en CC1S
	TIM5->CCMR2|=0X6060; //Modo PWM Tim5  CH3 y 4 - Como salida 00 en CC1S
	TIM5->CCER|=(1<<0) | (1<<4)|(1<<8) | (1<<12);  //Habilitar salida de comparacion y captura
    TIM5->CR1|=0X1; //Habilitar conteo
	NVIC_EnableIRQ(TIM5_IRQn); //Por si queremos la interrupci?n que igual trabaja con el ARR
}

void ADC_SETUP(){   
    ADC->CCR|=(2<<16); 
    ADC1->CR2 |= 0x3; // Activa el Conversor A/D y la conversion continua
    ADC1->CR1 |=  (0<<24); //12bit de resolicion
    ADC1->SMPR1|=(1<<0); // como el PC0 tiene el ADC1 en el canal 10 se configura en este, se ponen 15 ciclos que es lo que se necesita cuando se usa resolucion de 12 bits.
    ADC1->SQR3=10; 
}

void DAC_SETUP(){
    DAC->CR |= (1<<0) |(1<<16); //DAC CHANEL 1, 2 ENABLE
}
void USART_SETUP(){
    USART3->BRR |= 139; //BAUD RATE 115200
    //USART3->CR1 |= ((0xD) | (0<<15));
    USART3->CR1 |= ((0xD) | (0<<15)); //Pal oversampling que va al baud rate
}

extern "C"{
    void TIM4_IRQHandler(void){
        TIM4->SR &=~(1UL<<0);
        GPIOB->ODR^=(1<<7); //Blink el Led usuario
        DAC->DHR12R2 = adc;
        TIM5->CCR1 = (int)adc_mapped; //PA0
        send = 1;

    }
    void TIM5_IRQHandler(void){
        TIM5->SR &=~(1UL<<0);
        //GPIOB->ODR^=(1<<7);   
    }
}