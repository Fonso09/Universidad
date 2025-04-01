#include <stm32f7xx.h>
#include <stdio.h>
#include <arm_math.h>
#include <math.h>  // Para usar sin() y fmod


//FILTROS  
#define PI 3.14159265359
#define FIR_LENGTH 101
#define IIR_STAGES 25
#define SAMPLE_FREQUENCY 5000 // 500Hz
#define SIGNAL_FREQUENCY 200  // Frecuencia de la se�al a ingresar

float xd = 0;

/////Filtros FIR
// Variables de la se�al
float32_t t = 0;  // Tiempo de la se�al
float32_t original_signal = 0;  // Se�al original
float32_t filtered_signal = 0;  // Se�al filtrada
float32_t filtered_HP_signal = 0;  // Se�al filtrada
float32_t filtered_BP_signal = 0;  // Se�al filtrada

///FILTRO PASABAJOS
// Definir los coeficientes del filtro FIR (ejemplo: 101 coeficientes)
float32_t fir_coef[FIR_LENGTH] = {
		    0.0060112,  0.0060734,  0.0059844,  0.0057394,  0.0053369,  0.0047791,  
    0.0040719,  0.0032251,  0.0022523,  0.0011704, -1.8820e-18, -0.0012353,  
   -0.0025091, -0.0037929, -0.0050562, -0.0062673, -0.0073940, -0.0084040,  
   -0.0092655, -0.0099481, -0.0104230, -0.0106660, -0.0106520, -0.0103630,  
   -0.0097836, -0.0089043, -0.0077194, -0.0062286, -0.0044372, -0.0023557,  
    1.9383e-18,  0.0026087,  0.0054444,  0.0084764,  0.0116700,  0.0149860,  
    0.0183830,  0.0218180,  0.0252450,  0.0286160,  0.0318870,  0.0350090,  
    0.0379410,  0.0406380,  0.0430610,  0.0451770,  0.0469520,  0.0483610,  
    0.0493820,  0.0500020,  0.0502090,  0.0500020,  0.0493820,  0.0483610,  
    0.0469520,  0.0451770,  0.0430610,  0.0406380,  0.0379410,  0.0350090,  
    0.0318870,  0.0286160,  0.0252450,  0.0218180,  0.0183830,  0.0149860,  
    0.0116700,  0.0084764,  0.0054444,  0.0026087,  1.9383e-18, -0.0023557,  
   -0.0044372, -0.0062286, -0.0077194, -0.0089043, -0.0097836, -0.0103630,  
   -0.0106520, -0.0106660, -0.0104230, -0.0099481, -0.0092655, -0.0084040,  
   -0.0073940, -0.0062673, -0.0050562, -0.0037929, -0.0025091, -0.0012353,  
   -1.8820e-18,  0.0011704,  0.0022523,  0.0032251,  0.0040719,  0.0047791,  
    0.0053369,  0.0057394,  0.0059844,  0.0060734,  0.0060112  

};



// Declaraci�n de variables para CMSIS FIR
float32_t fir_state[FIR_LENGTH];  // Buffer de estado
arm_fir_instance_f32 fir_instance; // Estructura de filtro
float32_t fir_in, fir_out;

//FILTRO PASA ALTOS
// Definir los coeficientes del filtro FIR (ejemplo: 101 coeficientes)
float32_t fir_HP_coef[FIR_LENGTH] = {
	 -0.0060218, -0.0060841, -0.0059950, -0.0057496, -0.0053463, -0.0047875,  
   -0.0040791, -0.0032308, -0.0022562, -0.0011724, -3.5822e-17,  0.0012375,  
    0.0025135,  0.0037996,  0.0050651,  0.0062784,  0.0074071,  0.0084188,  
    0.0092818,  0.0099657,  0.0104420,  0.0106840,  0.0106700,  0.0103810,  
    0.0098009,  0.0089200,  0.0077330,  0.0062396,  0.0044450,  0.0023599,  
   -4.0776e-17, -0.0026133, -0.0054541, -0.0084914, -0.0116900, -0.0150120,  
   -0.0184160, -0.0218570, -0.0252890, -0.0286670, -0.0319430, -0.0350710,  
   -0.0380080, -0.0407090, -0.0431380, -0.0452560, -0.0470350, -0.0484460,  
   -0.0494700, -0.0500900,  0.9556600, -0.0500900, -0.0494700, -0.0484460,  
   -0.0470350, -0.0452560, -0.0431380, -0.0407090, -0.0380080, -0.0350710,  
   -0.0319430, -0.0286670, -0.0252890, -0.0218570, -0.0184160, -0.0150120,  
   -0.0116900, -0.0084914, -0.0054541, -0.0026133, -4.0776e-17,  0.0023599,  
    0.0044450,  0.0062396,  0.0077330,  0.0089200,  0.0098009,  0.0103810,  
    0.0106700,  0.0106840,  0.0104420,  0.0099657,  0.0092818,  0.0084188,  
    0.0074071,  0.0062784,  0.0050651,  0.0037996,  0.0025135,  0.0012375,  
   -3.5822e-17, -0.0011724, -0.0022562, -0.0032308, -0.0040791, -0.0047875,  
   -0.0053463, -0.0057496, -0.0059950, -0.0060841, -0.0060218  

 
};




// Declaraci�n de variables para CMSIS FIR
float32_t fir_HP_state[FIR_LENGTH];  // Buffer de estado
arm_fir_instance_f32 fir_HP_instance; // Estructura de filtro
float32_t fir_HP_in, fir_HP_out;


//FILTRO PASA BANDAS

// Definir los coeficientes del filtro FIR (ejemplo: 101 coeficientes)
float32_t fir_BP_coef[FIR_LENGTH] = {
   -1.4044e-18,  0.0005976,  0.0013817,  0.0023326,  0.0034206,  0.0046064,  
    0.0058426,  0.0070752,  0.0082452,  0.0092915,  0.0101530,  0.0107700,  
    0.0110910,  0.0110680,  0.0106670,  0.0098634,  0.0086477,  0.0070251,  
    0.0050161,  0.0026572, -3.2876e-18, -0.0028898, -0.0059344, -0.0090455,  
   -0.0121280, -0.0150810, -0.0178050, -0.0202020, -0.0221790, -0.0236540,  
   -0.0245580, -0.0248360, -0.0244520, -0.0233910, -0.0216570, -0.0192760,  
   -0.0162970, -0.0127860, -0.0088290, -0.0045288, -2.9799e-18,  0.0046331,  
    0.0092405,  0.0136910,  0.0178560,  0.0216130,  0.0248530,  0.0274790,  
    0.0294130,  0.0305970,  0.0309960,  0.0305970,  0.0294130,  0.0274790,  
    0.0248530,  0.0216130,  0.0178560,  0.0136910,  0.0092405,  0.0046331,  
   -2.9799e-18, -0.0045288, -0.0088290, -0.0127860, -0.0162970, -0.0192760,  
   -0.0216570, -0.0233910, -0.0244520, -0.0248360, -0.0245580, -0.0236540,  
   -0.0221790, -0.0202020, -0.0178050, -0.0150810, -0.0121280, -0.0090455,  
   -0.0059344, -0.0028898, -3.2876e-18,  0.0026572,  0.0050161,  0.0070251,  
    0.0086477,  0.0098634,  0.0106670,  0.0110680,  0.0110910,  0.0107700,  
    0.0101530,  0.0092915,  0.0082452,  0.0070752,  0.0058426,  0.0046064,  
    0.0034206,  0.0023326,  0.0013817,  0.0005976, -1.4044e-18  
 
};



// Declaraci�n de variables para CMSIS FIR
float32_t fir_BP_state[FIR_LENGTH];  // Buffer de estado
arm_fir_instance_f32 fir_BP_instance; // Estructura de filtro
float32_t fir_BP_in, fir_BP_out;


//Funciones filtros FIR
float32_t LowPass_Filter(float32_t signal);
float32_t HighPass_Filter(float32_t signal);
float32_t BandPass_Filter(float32_t signal);


/////Filtros FIR
//FILTRO PASA BAJO
float32_t iir_coeffs[IIR_STAGES][5] = {
{ 0.006126f, 0.012251f, 0.006126f, 1.965718f, -0.990221f },
    { 0.006067f, 0.012133f, 0.006067f, 1.946718f, -0.970984f },
    { 0.006009f, 0.012018f, 0.006009f, 1.928191f, -0.952226f },
    { 0.005953f, 0.011905f, 0.005953f, 1.910191f, -0.934002f },
    { 0.005898f, 0.011797f, 0.005898f, 1.892769f, -0.916363f },
    { 0.005846f, 0.011692f, 0.005846f, 1.875969f, -0.899353f },
    { 0.005796f, 0.011592f, 0.005796f, 1.859830f, -0.883013f },
    { 0.005748f, 0.011495f, 0.005748f, 1.844389f, -0.867379f },
    { 0.005702f, 0.011404f, 0.005702f, 1.829677f, -0.852484f },
    { 0.005658f, 0.011317f, 0.005658f, 1.815722f, -0.838355f },
    { 0.005617f, 0.011234f, 0.005617f, 1.802549f, -0.825018f },
    { 0.005579f, 0.011157f, 0.005579f, 1.790179f, -0.812494f },
    { 0.005543f, 0.011085f, 0.005543f, 1.778632f, -0.800803f },
    { 0.005509f, 0.011019f, 0.005509f, 1.767923f, -0.789960f },
    { 0.005479f, 0.010957f, 0.005479f, 1.758067f, -0.779981f },
    { 0.005451f, 0.010901f, 0.005451f, 1.749075f, -0.770877f },
    { 0.005425f, 0.010851f, 0.005425f, 1.740957f, -0.762659f },
    { 0.005403f, 0.010806f, 0.005403f, 1.733723f, -0.755334f },
    { 0.005383f, 0.010766f, 0.005383f, 1.727379f, -0.748911f },
    { 0.005366f, 0.010732f, 0.005366f, 1.721932f, -0.743396f },
    { 0.005352f, 0.010704f, 0.005352f, 1.717386f, -0.738793f },
    { 0.005341f, 0.010681f, 0.005341f, 1.713744f, -0.735106f },
    { 0.005332f, 0.010664f, 0.005332f, 1.711011f, -0.732339f },
    { 0.005326f, 0.010653f, 0.005326f, 1.709187f, -0.730493f },
    { 0.005323f, 0.010647f, 0.005323f, 1.708275f, -0.729569f },
};

//Declaracion variables para CMSIS IIR

//Filter State (4 por cada stage)
float32_t iir_state[IIR_STAGES*4];

float32_t iir_in, iir_out;

//Estructura del Filtro
arm_biquad_cascade_df2T_instance_f32 iirFilter;

float32_t iir_filtered_signal;

//FILTRO PASA ALTOS
float32_t iir_HP_coeffs[IIR_STAGES][5] = {
		{ 0.988985f, -1.977969f, 0.988985f, 1.965718f, -0.990221f },
    { 0.979425f, -1.958851f, 0.979425f, 1.946718f, -0.970984f },
    { 0.970104f, -1.940208f, 0.970104f, 1.928191f, -0.952226f },
    { 0.961048f, -1.922097f, 0.961048f, 1.910191f, -0.934002f },
    { 0.952283f, -1.904566f, 0.952283f, 1.892769f, -0.916363f },
    { 0.943830f, -1.887661f, 0.943830f, 1.875969f, -0.899353f },
    { 0.935711f, -1.871421f, 0.935711f, 1.859830f, -0.883013f },
    { 0.927942f, -1.855884f, 0.927942f, 1.844389f, -0.867379f },
    { 0.920540f, -1.841080f, 0.920540f, 1.829677f, -0.852484f },
    { 0.913519f, -1.827038f, 0.913519f, 1.815722f, -0.838355f },
    { 0.906892f, -1.813783f, 0.906892f, 1.802549f, -0.825018f },
    { 0.900668f, -1.801337f, 0.900668f, 1.790179f, -0.812494f },
    { 0.894859f, -1.789717f, 0.894859f, 1.778632f, -0.800803f },
    { 0.889471f, -1.778942f, 0.889471f, 1.767923f, -0.789960f },
    { 0.884512f, -1.769024f, 0.884512f, 1.758067f, -0.779981f },
    { 0.879988f, -1.759976f, 0.879988f, 1.749075f, -0.770877f },
    { 0.875904f, -1.751808f, 0.875904f, 1.740957f, -0.762659f },
    { 0.872264f, -1.744529f, 0.872264f, 1.733723f, -0.755334f },
    { 0.869073f, -1.738145f, 0.869073f, 1.727379f, -0.748911f },
    { 0.866332f, -1.732664f, 0.866332f, 1.721932f, -0.743396f },
    { 0.864045f, -1.728090f, 0.864045f, 1.717386f, -0.738793f },
    { 0.862213f, -1.724425f, 0.862213f, 1.713744f, -0.735106f },
    { 0.860837f, -1.721675f, 0.860837f, 1.711011f, -0.732339f },
    { 0.859920f, -1.719840f, 0.859920f, 1.709187f, -0.730493f },
    { 0.859461f, -1.718922f, 0.859461f, 1.708275f, -0.729569f },
};

//Declaracion variables para CMSIS IIR

//Filter State (4 por cada stage)
float32_t iir_HP_state[IIR_STAGES*4];

float32_t iir_HP_in, iir_HP_out;

//Estructura del Filtro
arm_biquad_cascade_df2T_instance_f32 iir_HP_Filter;

float32_t iir_filtered_HP_signal;

//FILTRO PASA BANDA
float32_t iir_BP_coeffs[IIR_STAGES][5] = {
    { 0.056319f, 0.000000f, -0.056319f, 1.945240f, -0.990431f },
    { 0.056319f, 0.000000f, -0.056319f, 1.985365f, -0.995453f },
    { 0.055930f, 0.000000f, -0.055930f, 1.927467f, -0.971797f },
    { 0.055930f, 0.000000f, -0.055930f, 1.976259f, -0.986401f },
    { 0.055558f, 0.000000f, -0.055558f, 1.911113f, -0.954191f },
    { 0.055558f, 0.000000f, -0.055558f, 1.967076f, -0.977374f },
    { 0.055208f, 0.000000f, -0.055208f, 1.896526f, -0.938000f },
    { 0.055208f, 0.000000f, -0.055208f, 1.957778f, -0.968340f },
    { 0.054883f, 0.000000f, -0.054883f, 1.884003f, -0.923565f },
    { 0.054883f, 0.000000f, -0.054883f, 1.948333f, -0.959276f },
    { 0.054589f, 0.000000f, -0.054589f, 1.873785f, -0.911180f },
    { 0.054589f, 0.000000f, -0.054589f, 1.938722f, -0.950175f },
    { 0.054328f, 0.000000f, -0.054328f, 1.866057f, -0.901086f },
    { 0.054328f, 0.000000f, -0.054328f, 1.928943f, -0.941053f },
    { 0.054103f, 0.000000f, -0.054103f, 1.860936f, -0.893462f },
    { 0.054103f, 0.000000f, -0.054103f, 1.919027f, -0.931961f },
    { 0.053915f, 0.000000f, -0.053915f, 1.858465f, -0.888418f },
    { 0.053915f, 0.000000f, -0.053915f, 1.909049f, -0.922997f },
    { 0.053768f, 0.000000f, -0.053768f, 1.899149f, -0.914325f },
    { 0.053768f, 0.000000f, -0.053768f, 1.858595f, -0.885973f },
    { 0.053662f, 0.000000f, -0.053662f, 1.889546f, -0.906186f },
    { 0.053662f, 0.000000f, -0.053662f, 1.861174f, -0.886044f },
    { 0.053598f, 0.000000f, -0.053598f, 1.880547f, -0.898900f },
    { 0.053598f, 0.000000f, -0.053598f, 1.865940f, -0.888437f },
    { 0.053576f, 0.000000f, -0.053576f, 1.872537f, -0.892848f },
};

//Declaracion variables para CMSIS IIR

//Filter State (4 por cada stage)
float32_t iir_BP_state[IIR_STAGES*4];

float32_t iir_BP_in, iir_BP_out;

//Estructura del Filtro
arm_biquad_cascade_df2T_instance_f32 iir_BP_Filter;

float32_t iir_filtered_BP_signal;

//Funciones filtros IIR
float32_t LowPass_Filter_IIR(float32_t signal);
float32_t HighPass_Filter_IIR(float32_t signal);
float32_t BandPass_Filter_IIR(float32_t signal);

////RECREACIÓN DE LA SEÑAL
void Clock_PLL(int M, int N, int P, int Q);
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
    Clock_PLL(16, 192, 0, 4); //Para 96 MHz
    // Inicializa el filtro FIR
    arm_fir_init_f32(&fir_instance, FIR_LENGTH, fir_coef, fir_state, 1);
    arm_fir_init_f32(&fir_HP_instance, FIR_LENGTH, fir_HP_coef, fir_HP_state, 1);
    arm_fir_init_f32(&fir_BP_instance, FIR_LENGTH, fir_BP_coef, fir_BP_state, 1);
    arm_biquad_cascade_df2T_init_f32(&iirFilter, IIR_STAGES, &iir_coeffs[0][0], &iir_state[0]);
    arm_biquad_cascade_df2T_init_f32(&iir_HP_Filter, IIR_STAGES, &iir_HP_coeffs[0][0], &iir_HP_state[0]);
    arm_biquad_cascade_df2T_init_f32(&iir_BP_Filter, IIR_STAGES, &iir_BP_coeffs[0][0], &iir_BP_state[0]);

    RCC_SETUP();
    ADC_SETUP();
    DAC_SETUP();
    GPIO_SETUP();
    TIM_Config();
    USART_SETUP();
    ADC1->CR2 |= (1<<30); // INICIO CONVERSION
	while(1){
        //ADC1->CR2 |= (1<<30); // INICIO CONVERSION
        //while((ADC1->SR & (1<<1)) == 0){ADC1->SR = 0;} 
        //adc = ADC1->DR;
        adc_mapped = map((int)filtered_BP_signal+2047,0,4096,0,100);

        if(send == 1){
            //ADC1->CR2 |= (1<<30); // INICIO CONVERSION
            //while((ADC1->SR & (1<<1)) == 0){ADC1->SR = 0;} 
            //adc = ADC1->DR;
            send = 0;
            sprintf(tx_txt, "%d\n", adc);
            Serial_buffer(tx_txt);

			//DAC->DHR12R2 = salida;

        }
		
	};
	
}

////FILTROS FIR
float32_t LowPass_Filter(float32_t signal){
    fir_in = signal;
    arm_fir_f32(&fir_instance, &fir_in, &fir_out, 1);
    return fir_out;  // Se�al filtrada
}

float32_t HighPass_Filter(float32_t signal){
    fir_HP_in = signal;
    arm_fir_f32(&fir_HP_instance, &fir_HP_in, &fir_HP_out, 1);
    return fir_HP_out;  // Se�al filtrada
}

float32_t BandPass_Filter(float32_t signal){
    fir_BP_in = signal;
    arm_fir_f32(&fir_BP_instance, &fir_BP_in, &fir_BP_out, 1);
    return fir_BP_out;  // Se�al filtrada
}

////FILTROS IIR
float32_t LowPass_Filter_IIR(float32_t signal){
    iir_in = signal;
    arm_biquad_cascade_df2T_f32(&iirFilter, &iir_in, &iir_out, 1);
    return iir_out;
}

float32_t HighPass_Filter_IIR(float32_t signal){
    iir_HP_in = signal;
    arm_biquad_cascade_df2T_f32(&iir_HP_Filter, &iir_HP_in, &iir_HP_out, 1);
    return iir_HP_out;//23.075;
}

float32_t BandPass_Filter_IIR(float32_t signal){
    iir_BP_in = signal;
    arm_biquad_cascade_df2T_f32(&iir_BP_Filter, &iir_BP_in, &iir_BP_out, 1);
    return iir_BP_out;
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
    RCC->APB1ENR |= (1<<18)|(1<<5)|(1<<3)|(1<<2)|(1<<29); //habilita el USART3, Habilita TIM7, Habilita TIM5, Habilita TIM4 y habilita el DAC
    RCC->APB2ENR |= (1<<5)|(7<<8); //Habilita el USART6 Y ADC 1,2 y 3
}

void GPIO_SETUP(){
    GPIOC->MODER |= 0X3; // ADC para PC0

    GPIOB->MODER |= (1<<14)|(1<<28)|(1<<18)|(1<<2)|(1<<0); //LEDS PRUEBA de la placa PB7 y PB14

    GPIOD->MODER |= 0xA0000;  //Pines a usar por el USART3 PD8(TX) y PD9(RX)
    GPIOD->AFR[1] |= 0x77; // Viendo el Datasheet se puede observar que el PD8 7 PD9 para USART3 se debe activar el AF7 y estan en la parte high del AFR

    GPIOA->MODER |=(2<<0)|(2<<2)|(2<<4)|(2<<6); //Modo alternante para el TIM5|PWM, PA0, PA1, PA2, PA3
    GPIOA->MODER |= 0xC00; //DAC PA5 canal 2    
    GPIOA->AFR[0] |= 0x2222; //AF2 para el TIM5 de PA0, PA1, PA2 y PA3

    GPIOB->MODER |= (2<<16)|(2<<18); // PB8 para TIM4CH3 y PB9 para TIM4CH4
    GPIOB->AFR[1] |= 0x22; //AF2 del TIM4 para PB8 y PB9

}
void TIM_Config(){
    TIM4->PSC = 96-1;  // Para tener 10KHz
    TIM4->ARR = 200-1; //ARR Para tener 10KHz
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

    TIM7->ARR = 2000 - 1;  // Configurar el temporizador para la frecuencia de muestreo (500 Hz)
    TIM7->PSC = 16 - 1;  // Configurar el prescaler para generar una frecuencia de 1 MHz
    TIM7->DIER |= 0x1;  // Habilitar interrupciones de TIM7
    TIM7->EGR |= 0x1;  // Reiniciar el temporizador
    TIM7->CR1 |= 0x1;  // Habilitar el temporizador
    NVIC_EnableIRQ(TIM7_IRQn);  // Habilitar la interrupci�n para TIM7

}

void ADC_SETUP(){   
    ADC->CCR|=(2<<16); 
    ADC1->CR2 |= 0x3; // Activa el Conversor A/D y la conversion continua
    ADC1->CR1 |=  (0<<24)|(1<<5); //12bit de resolicion, EOCIE
    ADC1->SMPR1|=(1<<0); // como el PC0 tiene el ADC1 en el canal 10 se configura en este, se ponen 15 ciclos que es lo que se necesita cuando se usa resolucion de 12 bits.
    ADC1->SQR3=10; 
	NVIC_EnableIRQ(ADC_IRQn); //Por si queremos la interrupci?n que igual trabaja con el ARR
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
    void TIM7_IRQHandler(void) {
        // Limpia la bandera de la interrupci�n
        TIM7->SR &= ~(1UL << 0);
        
        // Generar una se�al de 500 Hz (sinusoide) y pasarla por el filtro
        original_signal = 10.0*sin(2 * PI * t) + sin(2 * PI * SIGNAL_FREQUENCY * t); // Se�al de 500 Hz
        xd = sin(2 * PI * SIGNAL_FREQUENCY * t);
        t = fmod(t + 1.0 / SAMPLE_FREQUENCY, 1);  // Incrementar el tiempo (muestreo)
        // Filtrado FIR usando CMSIS-DSP
        /*fir_in = original_signal;
        fir_HP_in = original_signal;
        fir_BP_in = original_signal;
        arm_fir_f32(&fir_instance, &fir_in, &fir_out, 1);
        filtered_signal = fir_out;  // Se�al filtrada
        arm_fir_f32(&fir_HP_instance, &fir_HP_in, &fir_HP_out, 1);
        filtered_HP_signal = fir_HP_out;  // Se�al filtrada
        arm_fir_f32(&fir_BP_instance, &fir_BP_in, &fir_BP_out, 1);
        filtered_BP_signal = fir_BP_out;  // Se�al filtrada
        */
        //filtered_signal = LowPass_Filter(original_signal);
        //filtered_HP_signal = HighPass_Filter(original_signal);
        //filtered_BP_signal = BandPass_Filter(original_signal);


        //FIltrado IIR usando CMSIS-DSP
        /*
        iir_in = original_signal;
        arm_biquad_cascade_df2T_f32(&iirFilter, &iir_in, &iir_out, 1);
        iir_filtered_signal = iir_out;//23.075;

        iir_HP_in = original_signal;
        arm_biquad_cascade_df2T_f32(&iir_HP_Filter, &iir_HP_in, &iir_HP_out, 1);
        iir_filtered_HP_signal = iir_HP_out;//23.075;

        iir_BP_in = original_signal;
        arm_biquad_cascade_df2T_f32(&iir_BP_Filter, &iir_BP_in, &iir_BP_out, 1);
        iir_filtered_BP_signal = iir_BP_out;//23.075;
        */
        /*
        iir_filtered_signal = LowPass_Filter_IIR(original_signal);
        iir_filtered_HP_signal = HighPass_Filter_IIR(original_signal);
        iir_filtered_BP_signal = BandPass_Filter_IIR(original_signal);
        */
    }
    
    void TIM4_IRQHandler(void){
        TIM4->SR &=~(1UL<<0);
        GPIOB->ODR^=(1<<7);
        GPIOB->ODR^=(3<<1); //Blink el Led usuario
        //DAC->DHR12R2 = adc;
        //float32_t adc_32 = (float32_t)adc;
        //filtered_signal = LowPass_Filter(adc);
        //filtered_HP_signal = HighPass_Filter(adc);
        filtered_BP_signal = BandPass_Filter(adc);

        //iir_filtered_signal = LowPass_Filter_IIR(adc);
        //iir_filtered_HP_signal = HighPass_Filter_IIR(adc);
        //iir_filtered_BP_signal = BandPass_Filter_IIR(adc);

        //DAC->DHR12R2 = (int)filtered_signal;
        //DAC->DHR12R2 = (int)filtered_HP_signal + 2047;
        //DAC->DHR12R2 = (int)filtered_BP_signal + 2047;

        //DAC->DHR12R2 = (int)iir_filtered_signal;
        //DAC->DHR12R2 = (int)iir_filtered_HP_signal + 2047;
        DAC->DHR12R2 = (int)filtered_BP_signal + 2047;
        TIM5->CCR1 = (int)adc_mapped; //PA0
        send = 1;

    }
    void TIM5_IRQHandler(void){
        TIM5->SR &=~(1UL<<0);
        //GPIOB->ODR^=(1<<7);   
    }
		void ADC_IRQHandler(void){
        adc = ADC1->DR;   
    }
}

void Clock_PLL(int M, int N, int P, int Q){
    // 1. Activar la fuente de reloj
        //RCC->CR|=1<<16; //Ese es HSE
        //while((RCC->CR & 1<<17)==0);
        
    //2. Habilitar regulador PWREN
    RCC->APB1ENR |= 1<<28;
        
    //3. Pilas con la latencia	
    FLASH->ACR |= 1<<9 | 0x5;
        
    //4. Ojito con la frecuencia de los perifericos
        //RCC->CFGR |= RCC_CFGR_PPRE2_DIV4 | RCC_CFGR_PPRE1_DIV4;
        //Mantenerlo en 0 para que no lo dividan
        
    // 5. Nos metemos con PLL, M, N, P y Q
    //Como tiene un valor inicial, para que no moleste lo borramos. 
    RCC->PLLCFGR &= ~(0xF<<24); //Limpiar el valor de PLLQ
    RCC->PLLCFGR &= ~(0x1<<22); //Limpiar el valor de PLLSRC
    RCC->PLLCFGR &= ~(0x3<<16); //Limpiar el valor de PLLP
    RCC->PLLCFGR &= ~(0x1FF<<6); //Limpiar el valor de PLLN
    RCC->PLLCFGR &= ~(0x3F<<0); //Limpiar el valor de PLLM

    RCC->PLLCFGR |= M<< 0| N << 6| P<< 16| Q<<24 | (0<<22); 

    // 6. Encendemos  PLL
    RCC->CR|= 1<<24;
    while((RCC->CR & 1<<25)==0);	
    RCC->CFGR |= 0x2;
    while((RCC->CFGR & 3<<2) != RCC_CFGR_SWS_PLL);
}