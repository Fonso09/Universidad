// ========= Configuración de sensores =========
const int sensorPins[8] = {14, 27, 26, 25, 33, 32, 35, 34};
int sensorValues[8];

// Ángulos por sensor (grados)
float sensorAngles[8] = {-5.26, -3.77, -2.26, -0.75, 0.75, 2.26, 3.77, 5.26};
const int IR = 13;
// Umbral para considerar detección de línea
const int UMBRAL = 3500;

// ========= Puente H / Motores =========
// Motor A (izquierdo)
const int IN1 = 18;
const int IN2 = 5;
const int ENA = 17; // PWM

// Motor B (derecho)
const int IN3 = 21;
const int IN4 = 22;
const int ENB = 23; // PWM

// Standby del puente (si aplica en tu driver)
const int STBY_PIN  = 4;   // Botón (opcional, no usado aquí)
const int STBY_CTRL = 19;  // Control standby del driver

// ========= Control =========
float PWM_Base = 100.0f;   // Velocidad base [0..255]
float Kp       = 1.0f;    // Ganancia proporcional
const int PWM_MIN = 0;
const int PWM_MAX = 255;

// ========= Captura (30 s) =========
const unsigned long DURACION_MS = 30000; // 30 s
const unsigned long TS_MS       = 50;    // periodo de muestreo (~20 Hz)
unsigned long t0 = 0;
bool capturaTerminada = false;

// ========= Estado auxiliar =========
float ultimoAngulo = 0.0f; // para continuar con el último ángulo si nadie supera el umbral

float calcularPosicionAngular() {
  long sumaPond = 0;
  long sumaVals = 0;

  for (int i = 0; i < 8; i++) {
    const int v = sensorValues[i];
    if (v > UMBRAL) {
      sumaPond += (long)v * sensorAngles[i];
      sumaVals += v;
    }
  }

  if (sumaVals == 0) {
    // Ningún sensor superó el umbral: mantener último ángulo (evita “saltos” a 0)
    return ultimoAngulo;
  }

  float ang = (float)sumaPond / (float)sumaVals; // grados
  ultimoAngulo = ang;
  return ang;
}

void setup() {
  Serial.begin(115200);

  // Sensores
  for (int i = 0; i < 8; i++) pinMode(sensorPins[i], INPUT);

  // Puente H
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT); pinMode(ENA, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT); pinMode(ENB, OUTPUT);
  pinMode(IR, OUTPUT);
  digitalWrite(IR, HIGH);

  pinMode(STBY_PIN, INPUT_PULLUP);
  pinMode(STBY_CTRL, OUTPUT);
  digitalWrite(STBY_CTRL, HIGH); // habilitar puente

  // Dirección hacia adelante por defecto
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);

  t0 = millis();

  // (Opcional) encabezado para depuración
  // Serial.println("PWM_A,PWM_B,Angulo");
}

void loop() {
  unsigned long t = millis();

  if (!capturaTerminada && (t - t0) <= DURACION_MS) {
    // ======= Medición de sensores =======
    for (int i = 0; i < 8; i++) sensorValues[i] = analogRead(sensorPins[i]);

    // ======= Ángulo (salida) =======
    float ang = calcularPosicionAngular(); // grados (negativo izq, positivo der)

    // ======= Control proporcional =======
    int pwmA = (int)(PWM_Base + Kp * ang); // izq más rápido si ang > 0
    int pwmB = (int)(PWM_Base - Kp * ang); // der más rápido si ang < 0
    pwmA = constrain(pwmA, PWM_MIN, PWM_MAX);
    pwmB = constrain(pwmB, PWM_MIN, PWM_MAX);

    // Aplicar PWM a motores
    analogWrite(ENA, pwmA);
    analogWrite(ENB, pwmB);

    // ======= Envío de datos =======
    Serial.print(pwmA); Serial.print(",");
    Serial.print(pwmB); Serial.print(",");
    Serial.println(ang, 2);

    delay(TS_MS); // periodo de muestreo
  }
  else if (!capturaTerminada) {
    // Fin de los 30 s: detener y avisar
    analogWrite(ENA, 0);
    analogWrite(ENB, 0);
    digitalWrite(STBY_CTRL, LOW); // deshabilitar puente (si aplica)
    Serial.println("FIN");
    capturaTerminada = true;
  }

  // Después de terminar, no hacemos nada más (motores apagados, no se envían datos)
}