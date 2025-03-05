#include <stm32f7xx.h>
void RCC_SETUP();
void GPIO_SETUP();
void TIM_SETUP();


int main(){
    void RCC_SETUP();
    void GPIO_SETUP();
    void TIM_SETUP();

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
}
void TIM_SETUP(){
    // se activara el TIM2 para el modo captura
    TIM2->CCMR1 |= (1<<0);
    
}


extern "C"{

}