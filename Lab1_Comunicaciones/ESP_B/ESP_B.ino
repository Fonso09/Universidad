//ESP B se dedicará exclusivamente a enviar los datos 
#include <HardwareSerial.h>
#include "Wire.h"
#define I2C_DEV_ADDR 0x55
#define TX_PIN 17
#define RX_PIN 16
#define DE_RE_1 23
#define DE_RE_2 19  // este pin es para decirle a la otra esp que empiece a enviar datos para recibir. 
#define DE_RE_BTN 18 // lectura por si se activa o desactiva el DE_RE de la otra ESP
#define ADC_PIN 2

String modo_comunicacion = "";
int ADC_value =0;
bool req_enviar_RS485 = false; 
uint16_t received_adc_value = 0;  // Último valor recibido del maestro
uint16_t adc_value = 0;
void onRequest() {
  // Leer el ADC del esclavo
  adc_value = analogRead(ADC_PIN);
  
  // Enviar la lectura al maestro
  Wire.write((uint8_t*)&adc_value, sizeof(adc_value));
  //Serial.printf("Enviando ADC esclavo: %u\n", adc_value);
}
void onReceive(int len) {
  if (len >= sizeof(received_adc_value)) {
    Wire.readBytes((uint8_t*)&received_adc_value, sizeof(received_adc_value));
    //Serial.printf("Dato recibido del maestro: %u\n", received_adc_value);
  }
}
HardwareSerial RS485(1);
String leer_RS485();
void enviar_RS485(String dato_ADC);


void setup() {
  Serial.begin(115200); //Inicializar Serial con el PC
  Wire.onReceive(onReceive);
  Wire.onRequest(onRequest);
  Wire.begin((uint8_t)I2C_DEV_ADDR);
  RS485.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN); //Inicializar RS485
  pinMode(DE_RE_1, OUTPUT); //Pin para poner RS485 como lectura o escritura
  pinMode(ADC_PIN, INPUT); //Pin que lee el sensor para enviar a la otra ESP
  digitalWrite(DE_RE_1, HIGH); // Inicializa el RS485 como modo lectura 
  

}

void loop() {
  /* 
  En Esta parte del codigo la idea es que lea lo que recibe del python para seleccionar de qué comunicación enviar los datos y lea el sensor continuamente y 
  */
  ADC_value = analogRead(ADC_PIN);
  //Serial.println(ADC_value);
  enviar_RS485(String (ADC_value));
  //Parte del código para enviar al python los datos recibidos
  if (Serial.available()>0) {
    modo_comunicacion = Serial.readStringUntil('\n');
  }
  //modo_comunicacion= "4";
  switch(modo_comunicacion.toInt()){
    case 1:
      //I2C
      Serial.printf("Dato recibido del maestro: %u\n", received_adc_value);
      break;
    case 2:
      //SPI
      break;
    case 3:
      //BT
      break;
    case 4:
      //SERIAL
      break;
    }

}

void enviar_RS485(String dato_ADC){
    digitalWrite(DE_RE_1, HIGH);  // Cambiar a modo transmisión
    delay(10);
    RS485.println(dato_ADC);
    Serial.println("Dato RS485 enviado");
    delay(10);
    //digitalWrite(DE_RE, LOW);   // Volver a modo recepción
}

  

