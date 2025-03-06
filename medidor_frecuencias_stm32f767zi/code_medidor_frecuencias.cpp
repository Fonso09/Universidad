#include <stm32f7xx.h>
void RCC_SETUP();
void GPIO_SETUP();
void TIM_SETUP();

volatile uint32_t prev_captura = 0;
volatile uint32_t frecuencia = 0; 

int main(){
    RCC_SETUP();
    GPIO_SETUP();
    TIM_SETUP();

    while(1){

    }
}

void RCC_SETUP(){
    RCC->AHB1ENR |= 0xFF; //Activa los puertos de la A a la F
    RCC->APB1ENR |= (1<<0); //Habilita TIM2, el que se usara para poner en modo captura y medir la frecuencia
}
void GPIO_SETUP(){
    GPIOA->MODER |= (2<<0); // se coloca en modo Alternante el PA0 el cual sera el encargado de leer para el TIM2
    GPIOA->AFR[0] |= (1<<0); // AF1 para PA0 para TIM2 Channel 1
    GPIOB->MODER |= (1<<14);// LED de prueba PB7
}
void TIM_SETUP(){
    TIM2->PSC = 8 - 1;  // Prescaler para que cuente a 1 MHz (16 MHz / 16)
    TIM2->ARR = 0xFFFFFFFF; // Máximo valor de conteo

    //TIM2->CCMR1 &= ~(3 << 0); // Limpiar CC1S
    TIM2->CCMR1 |= (1 << 0); // Configurar CH1 en modo Input Capture (CC1S = 01)
    TIM2->CCER |= (1<<0); // Habilitar captura en CH1
    TIM2->CCER &=~ (1<<1);
    TIM2->DIER |= (1<<1); // Habilitar interrupción por captura
    TIM2->CR1 |= 0x1; // Habilitar el Timer

    NVIC_EnableIRQ(TIM2_IRQn);
    
}


extern "C"{
    void TIM2_IRQHandler(void){
            TIM2->SR &=~(1<<1);
            GPIOB->ODR^=(1<<7); //Blink el Led usuario
            uint32_t now_captura = TIM2->CCR1;
            uint32_t periodo = now_captura - prev_captura;
            prev_captura = now_captura;
            frecuencia = 16000000 / periodo; // Calcular frecuencia en Hz
        
        
    }
}




