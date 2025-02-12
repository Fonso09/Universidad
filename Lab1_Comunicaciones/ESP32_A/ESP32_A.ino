//ESP A va a ser la ESP que reciba siempre todos los datos.
#include <HardwareSerial.h>
#include "Wire.h"
#define I2C_DEV_ADDR 0x55
#define TX_PIN 17
#define RX_PIN 16
#define DE_RE_1 23
#define DE_RE_2 19  // este pin es para decirle a la otra esp que empiece a enviar datos para recibir. 
#define ADC_PIN 2

String modo_comunicacion = "";
uint16_t adc_value = 0;
uint16_t slave_adc_value=0;
uint8_t bytesReceived = 0;

HardwareSerial RS485(1);
String leer_RS485();



void setup() {
  Serial.begin(115200); //Inicializar Serial con el PC
  RS485.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN); //Inicializar RS485
  pinMode(DE_RE_1, OUTPUT); //Pin para poner RS485 como lectura o escritura
  pinMode(ADC_PIN, INPUT); //Pin que lee el sensor para enviar a la otra ESP
  digitalWrite(DE_RE_1, LOW); // Inicializa el RS485 como modo lectura 
  Wire.begin();
  

}

void loop() {
  /* 
  En Esta parte del codigo la idea es que lea lo que recibe del python para seleccionar de qué comunicación enviar los datos y lea el sensor continuamente y 
  */
  delay(10); //delay para el I2C
  adc_value = analogRead(ADC_PIN);
  //Serial.println(ADC_value);
  //envar ADC al esclavo del I2C
  Wire.beginTransmission(I2C_DEV_ADDR);
  Wire.write((uint8_t*)&adc_value, sizeof(adc_value));
  uint8_t error = Wire.endTransmission(true);
  // Solicitar la lectura del ADC del esclavo
  bytesReceived = Wire.requestFrom(I2C_DEV_ADDR, sizeof(adc_value));
  if (bytesReceived == sizeof(adc_value)) {
    Wire.readBytes((uint8_t*)&slave_adc_value, sizeof(slave_adc_value));
    //Serial.printf("Valor recibido del esclavo: %u\n", slave_adc_value);
  }

  //Parte del código para enviar al python los datos recibidos
  if (Serial.available()>0) {
    modo_comunicacion = Serial.readStringUntil('\n');
  }
  //modo_comunicacion= "4";
  switch(modo_comunicacion.toInt()){
    case 1:
      //I2C
      // Solicitar la lectura del ADC del esclavo
      if (bytesReceived == sizeof(adc_value)) {
        Serial.printf("Valor recibido del esclavo: %u\n", slave_adc_value);
      }
      break;
    case 2:
      //SPI
      break;
    case 3:
      //BT
      break;
    case 4:
      if(RS485.available()){
        String adcValue = leer_RS485();
        Serial.println(adcValue.toInt());
      }
      break;
    }
}

String leer_RS485(){
    String msg = "";
    while (RS485.available()) {
        char c = RS485.read();
        if (c >= 32 || c == '\n') {  // Filtra caracteres basura
            msg += c;
        }
    }
    return msg; 
}
  

