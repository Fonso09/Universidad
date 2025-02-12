#include "Wire.h"

#define I2C_DEV_ADDR 0x55
#define ADC_PIN 2  // Pin del ADC en el esclavo

uint16_t received_adc_value = 0;  // Último valor recibido del maestro

void onRequest() {
  // Leer el ADC del esclavo
  uint16_t adc_value = analogRead(ADC_PIN);
  
  // Enviar la lectura al maestro
  Wire.write((uint8_t*)&adc_value, sizeof(adc_value));
  //Serial.printf("Enviando ADC esclavo: %u\n", adc_value);
}

void onReceive(int len) {
  if (len >= sizeof(received_adc_value)) {
    Wire.readBytes((uint8_t*)&received_adc_value, sizeof(received_adc_value));
    Serial.printf("Dato recibido del maestro: %u\n", received_adc_value);
  }
}

void setup() {
  Serial.begin(115200);
  Wire.onReceive(onReceive);
  Wire.onRequest(onRequest);
  Wire.begin((uint8_t)I2C_DEV_ADDR);
}

void loop() {
  delay(100);
}