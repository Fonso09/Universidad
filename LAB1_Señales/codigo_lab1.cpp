#include <stm32f7xx.h>
void RCC_SETUP();
void GPIO_SETUP();
void TIM_Config();

int main(){
		RCC_SETUP();
    GPIO_SETUP();
	TIM_Config();
	while(1){
		
	};
	
}
void RCC_SETUP(){
	RCC-> AHB1ENR |= 0xFF; //Activa los puertos de la A a la F
    RCC->APB1ENR |= (1<<18)|(1<<3)|(1<<2); //habilita el USART3, Habilita TIM5 y Habilita TIM4
    RCC->APB2ENR |= (1<<5); //Habilita el USART6
		RCC->APB2ENR|=(7<<8); // ENCENDEER EL ADC
		GPIOC->MODER|=0X3; //PC0
		ADC->CCR|=(2<<16); 
		//SetUp general features of ADC
		ADC1->CR2|=0X3;
		ADC1->CR1|=(0<<24);
		ADC1->SMPR1|=(1<<0);
		ADC1->SQR3=10;

}

void GPIO_SETUP(){
    GPIOC->MODER |= (0<<26); // PC13 es el boton de Usuario que sera el boton para el Paro de Emergencia, Input
    GPIOC->PUPDR |= (2<<26); // pull down el pin PC13

    GPIOE->MODER |= (1<<14)|(1<<16)|(1<<18)|(1<<20); //PE7, PE8, PE9, PE10 como salida para el control del puente H
    GPIOB->MODER |= (1<<14)|(1<<28)|(1<<18); //LEDS PRUEBA de la placa PB7 y PB14

    GPIOF->MODER |= (0<<26)|(0<<28); //PF13 y PF14 como entradas para el avanzado y retroceso manual
    GPIOF->PUPDR |= (2<<26)|(2<<28); //PF13 y PF14 pull down

    GPIOD->MODER |= 0xA0000;  //Pines a usar por el USART3 PD8(TX) y PD9(RX)
    GPIOD->AFR[1] |= 0x77; // Viendo el Datasheet se puede observar que el PD8 7 PD9 para USART3 se debe activar el AF7 y estan en la parte high del AFR

    //GPIOC->MODER |= (2<<22)|(2<<20);  //Pines a usar por el USART3 PC10(TX) y PC11(RX)
    //GPIOC->AFR[1] |= 0x7700; // Viendo el Datasheet se puede observar que el PC10 y PC11 para USART3 se debe activar el AF7 y estan en la parte high del AFR

    GPIOA->MODER |=(2<<0)|(2<<2)|(2<<4)|(2<<6); //Modo alternante para el TIM5|PWM, PA0, PA1, PA2, PA3
    GPIOA->AFR[0] |= 0x2222; //AF2 para el TIM5 de PA0, PA1, PA2 y PA3

    GPIOC->MODER |= (2<<12)|(2<<14); //USART6 PC6(TX) y PC7(RX)
    GPIOC->AFR[0] |= (0x88<<24); //USART6

    GPIOB->MODER |= (2<<16)|(2<<18); // PB8 para TIM4CH3 y PB9 para TIM4CH4
    GPIOB->AFR[1] |= 0x22; //AF2 del TIM4 para PB8 y PB9


}
void TIM_Config(){
    TIM5->PSC = 16-1;  // Para tener 50Hz
    TIM5->ARR = 1000-1; //ARR
    TIM5->DIER |= 0x1; //Habilita Interrupcion
    TIM5->EGR |= 0x1; //Reinicio del conteo
	TIM5->CCMR1|=0X6060; //Modo PWM Tim5  CH1 y 2 - Como salida 00 en CC1S
	TIM5->CCMR2|=0X6060; //Modo PWM Tim5  CH3 y 4 - Como salida 00 en CC1S
	TIM5->CCER|=(1<<0) | (1<<4)|(1<<8) | (1<<12);  //Habilitar salida de comparacion y captura
    TIM5->CR1|=0X1; //Habilitar conteo
	NVIC_EnableIRQ(TIM5_IRQn); //Por si queremos la interrupci?n que igual trabaja con el ARR

}

extern "C"{
    void TIM5_IRQHandler(void){
        TIM5->SR &=~(1UL<<0);
        GPIOB->ODR^=(1<<7);   
    }
}