#define LED_PIN 2

unsigned long lastSendTime = 0;
const unsigned long interval = 2000; // 2 segundos

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Inicializar semilla aleatoria
  randomSeed(analogRead(0));
}

void loop() {
  // Enviar datos cada 2 segundos
  if (millis() - lastSendTime >= interval) {
    lastSendTime = millis();


    for (int i = 0; i < 5; i++) {
      int value = random(0, 181); // Entre 0 y 180

      // Imprimir siempre con 3 dígitos (rellena con ceros a la izquierda)
      if (value < 10) Serial.print("00");
      else if (value < 100) Serial.print("0");
      Serial.print(value);

      if (i < 4) Serial.print(","); // Separador
    }

    Serial.print("\n"); // Fin de línea
  }

  // Leer si llega algo por serial
  if (Serial.available() > 0) {
    String incoming = Serial.readStringUntil('\n'); // Leer lo que llega
    digitalWrite(LED_PIN, HIGH);
    delay(1000);
    digitalWrite(LED_PIN, LOW);
  }
}
