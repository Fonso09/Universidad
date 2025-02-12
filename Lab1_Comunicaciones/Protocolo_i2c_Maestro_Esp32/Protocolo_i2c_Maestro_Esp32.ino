#include "Wire.h"

#define I2C_DEV_ADDR 0x55
#define ADC_PIN 2 // Pin del ADC en el maestro

void setup() {
  Serial.begin(115200);
  Wire.begin();
}

void loop() {
  delay(10);

  // Leer ADC del maestro
  uint16_t adc_value = analogRead(ADC_PIN);
  //Serial.printf("ADC maestro: %u\n", adc_value);

  // Enviar el ADC al esclavo
  Wire.beginTransmission(I2C_DEV_ADDR);
  Wire.write((uint8_t*)&adc_value, sizeof(adc_value));
  uint8_t error = Wire.endTransmission(true);
  //Serial.printf("endTransmission: %u\n", error);

  // Solicitar la lectura del ADC del esclavo
  uint8_t bytesReceived = Wire.requestFrom(I2C_DEV_ADDR, sizeof(adc_value));
  if (bytesReceived == sizeof(adc_value)) {
    uint16_t slave_adc_value;
    Wire.readBytes((uint8_t*)&slave_adc_value, sizeof(slave_adc_value));
    Serial.printf("Valor recibido del esclavo: %u\n", slave_adc_value);
  }
}