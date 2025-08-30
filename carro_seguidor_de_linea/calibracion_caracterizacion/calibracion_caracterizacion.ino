#include <Arduino.h>
#include <QTRSensors.h>

#define LEDON_PIN 5

// Pines de los 8 sensores (ADC)
const uint8_t QTR_PINS[8] = {32, 33, 34, 35, 36, 39, 25, 26};

// Crear objeto QTR
QTRSensors qtr;

uint16_t raw[8];
uint16_t cal[8];

void setup() {
  Serial.begin(115200);

  // Configurar tipo y pines
  qtr.setTypeAnalog();
  qtr.setSensorPins(QTR_PINS, 8);
  qtr.setEmitterPin(LEDON_PIN);

  // Calibración
  delay(500);
  Serial.println("Calibrando...");
  for (int i = 0; i < 400; i++) {
    qtr.calibrate();
    delay(5);
  }
  Serial.println("Calibracion lista.");
}

void loop() {
  // Lecturas crudas (0–4095 en ESP32 ADC)
  qtr.read(raw);

  // Lecturas calibradas (0–1000)
  qtr.readCalibrated(cal);

  Serial.print("RAW:\t");
  for (int i = 0; i < 8; i++) {
    Serial.print(raw[i]);
    Serial.print(i < 7 ? '\t' : '\n');
  }

  Serial.print("CAL:\t");
  for (int i = 0; i < 8; i++) {
    Serial.print(cal[i]);
    Serial.print(i < 7 ? '\t' : '\n');
  }
  Serial.println("---");

  delay(100);
}
