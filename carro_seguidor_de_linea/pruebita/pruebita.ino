// ========= CONFIGURACIÓN DE SENSORES =========
const int sensorPins[8] = {14, 27, 26, 25, 33, 32, 35, 34};
int sensorValues[8];
const int IR_LED = 13; 
const int UMBRAL = 2000;  
const int sensorAngles[8] = {-45, -30, -15, -5, 5, 15, 30, 45};

// ========= CONFIGURACIÓN DE MOTORES =========
// Motor A (izquierdo)
#define IN1 18
#define IN2 5
#define ENA 17  // PWM

// Motor B (derecho)
#define IN3 21
#define IN4 22
#define ENB 23  // PWM

// Standby
#define STBY 19

// PWM ESP32
const int canalA = 0;
const int canalB = 1;
const int freqPWM = 1000;
const int resolPWM = 8; // 0-255

// ========= VARIABLES GLOBALES =========
float referencia = 0;   // Ángulo objetivo recibido por serial
float angulo = 0;       // Ángulo detectado actual
bool nuevaRef = false;  // Bandera de nueva referencia

// ========= FUNCIONES =========
void moverMotores(int velA, int velB) {
  // Configurar direcciones
  if (velA > 0) { digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH); }
  else if (velA < 0) { digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW); }
  else { digitalWrite(IN1, LOW); digitalWrite(IN2, LOW); }

  if (velB > 0) { digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW); }
  else if (velB < 0) { digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH); }
  else { digitalWrite(IN3, LOW); digitalWrite(IN4, LOW); }

  ledcWrite(canalA, abs(velA));
  ledcWrite(canalB, abs(velB));
}

void detenerMotores() {
  moverMotores(0, 0);
}

// ========= SETUP =========
void setup() {
  Serial.begin(115200);

  // Sensores
  for (int i = 0; i < 8; i++) pinMode(sensorPins[i], INPUT);
  pinMode(IR_LED, OUTPUT);
  digitalWrite(IR_LED, HIGH);

  // Motores
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(STBY, OUTPUT);
  digitalWrite(STBY, HIGH);

  ledcSetup(canalA, freqPWM, resolPWM);
  ledcAttachPin(ENA, canalA);
  ledcSetup(canalB, freqPWM, resolPWM);
  ledcAttachPin(ENB, canalB);

  Serial.println("Sistema iniciado. Enviar ángulo de referencia por serial.");
}

// ========= LOOP =========
void loop() {
  // Leer sensores y calcular ángulo
  int sumAngles = 0;
  int activeCount = 0;

  for (int i = 0; i < 8; i++) {
    sensorValues[i] = analogRead(sensorPins[i]);
    if (sensorValues[i] > UMBRAL) {
      sumAngles += sensorAngles[i];
      activeCount++;
    }
  }

  if (activeCount > 0) {
    angulo = (float)sumAngles / activeCount;
  } else {
    angulo = 999; // sin línea detectada
  }

  // Mostrar siempre el ángulo
  Serial.print("Ángulo detectado: ");
  if (angulo == 999) Serial.println("Sin línea");
  else Serial.println(angulo);

  // Revisar si llegó un nuevo ángulo por serial
  if (Serial.available() > 0) {
    referencia = Serial.parseFloat();
    nuevaRef = true;
    Serial.print("Nueva referencia recibida: ");
    Serial.println(referencia);
  }

  // Control simple de motores
  if (nuevaRef && angulo != 999) {
    float error = referencia - angulo;

    if (abs(error) <= 3) {
      detenerMotores();
      Serial.println("Ángulo alcanzado, motores detenidos.");
      nuevaRef = false;
    } else {
      int velocidadBase = 130; // PWM (0-255)
      if (error > 0) {
        // Necesita girar a la derecha
        moverMotores(velocidadBase, -velocidadBase);
        Serial.println("Girando derecha...");
      } else {
        // Necesita girar a la izquierda
        moverMotores(-velocidadBase, velocidadBase);
        Serial.println("Girando izquierda...");
      }
    }
  } else {
    detenerMotores();
  }

  delay(100);
}
