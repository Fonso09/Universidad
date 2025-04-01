#include <stm32f7xx.h>

void I2C_SETUP();
void I2C1_Start();
void I2C1_Stop();
void I2C1_Write(int *w_buffer, int autoend, uint8_t NBYTES);
void I2C1_Address(uint8_t address, uint8_t RD_WRN, int autoend, uint8_t NBYTES);
void I2C1_Read(int *r_buffer, uint8_t size, int autoend);


void I2C_SETUP(){
    RCC->AHB1ENR |= 0x2;
    RCC->APB1ENR |= (1<<21); //I2C EN
    GPIOB->MODER |= (0xA<<16);// lEDs e I2C PB8(SCL) Y PB9(SDA) como alternante
    
    //Configuración PB8 y PB9 para I2C1
    GPIOB->OTYPER |= 0x300; //PB8 y 9 como open drain
    GPIOB->OSPEEDR |= 0xF0000; //PB8 y 9 como high speed
    GPIOB->PUPDR |= 0x50000;
    GPIOB->AFR[1] |= 0x44;

    //Reiniciar i2c?
    I2C1->CR1 |= (1<<0); //Habilita PE
    I2C1->CR1 &= ~(1<<0); //Deshabilita PE causando un reinicio
    //Configurar timing register del I2C
    //usaremos ejemplo en tabla 207 del manual ref a 16MHz para Standard_mode del i2c
    I2C1->TIMINGR |=(3<<28)|(0x4<<20)|(0x2<<16)|(0xF<<8)|(0x13); //PRESC, SCLDEL, SDADEL, SCLH y SCLL 
    I2C1->CR1 |= (1<<0); //Enable perif�rico. Tiene que ser despu�s de configurar el reloj
}

void I2C1_Start(){
    I2C1->CR2 |= (1<<13); //Da la indicaci�n de START
}

void I2C1_Address(uint8_t address, uint8_t RD_WRN, int autoend, uint8_t NBYTES){
    I2C1->CR2 &= ~(0xFF<<16); //Limpiar NBYTES
    I2C1->CR2 |= (NBYTES<<16)|(address);
    if(autoend){ //si es un 1 es pq se quiere que cierre solo
        I2C1->CR2 |= (1<<25);
    }else{ //Si es 0 es pq se da el stop manual
        I2C1->CR2 &= ~(1<<25); 
    }
    //RD_WRN es el bit 16 y define si se quiere leer o escribir
    if(RD_WRN){ //si es un 1 es pq se quiere leer  y se escribe as� el registro
        I2C1->CR2 |= (1<<10);
    }else{ //Si es 0 es pq se quiere escribir y se escribe as� el registro
        I2C1->CR2 &= ~(1<<10); 
    }
    //while(!(I2C1->ISR & (1<<6))); //TC indica que se trasnfirieron los datos
    //en el video usan addr pero aqu� es solo si la placa es esclavo entonces es con el TCR?
    //seg�n Figura 368 realmente no, se verifica evento de TXIS pero eso est� en I2C1_Write
}

void I2C1_Stop(){
    I2C1->CR2 |= (1<<14); //1 en Stop para generation despues del byte transferido
}

void I2C1_Write(int *w_buffer, int autoend, uint8_t NBYTES){
    //tx_data[] ya debe estar escrito previo a la funci�n
    for(int i = 0; i < NBYTES; i++){
        while(!(I2C1->ISR & (1<<1))); //TXIS indica cuando transmit data register est� vac�o
        //I2C1->TXDR = tx_data[i]; //Lo que se vaya a enviar   
        I2C1->TXDR = *(w_buffer + i);  
    }
    if(!autoend){
        while(!(I2C1->ISR & (1<<6))); //TCR indica que se trasnfirieron los datos
        //toca hacer m�s cosas que no entend�, creo que un STOP(bit 14 cr2)
    }
}

void I2C1_Read(int *r_buffer, uint8_t size, int autoend){
    for(int i = 0; i < size; i++){
        while(!(I2C1->ISR & (1<<2))); //RXNE avisa cuando RXDR ya no est� vac�o y se baja la flag ley�ndolo
        //rx_data[i] = I2C1->RXDR; 
        *(r_buffer + i) = I2C1->RXDR;
    }
    if(!autoend){
        while(!(I2C1->ISR & (1<<6))); //TC indica que se trasnfirieron los datos
        //toca hacer m�s cosas que no entend�, creo que un STOP(bit 14 cr2)
    }
    //el buffer es para que vaya leyendo los datos recividos y los vaya guardando
}



extern "C"{
    //Espacio para interrupciones a utilizar   
}

