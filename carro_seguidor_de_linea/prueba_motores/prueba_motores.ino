// ========= Sensores =========
const int sensorPins[8] = {14, 27, 26, 25, 33, 32, 35, 34};
int sensorValues[8];

// Ángulos en grados de cada sensor
float sensorAngles[8] = {-5.26, -3.77, -2.26, -0.75, 0.75, 2.26, 3.77, 5.26};

// Umbral de detección (ajústalo según pruebas)
const int UMBRAL = 3500;

// ========= Motores (puente H) =========
const int AIN1 = 18;
const int AIN2 = 5;
const int ENA = 17; // PWM motor A Izquierdo

const int BIN1 = 21;
const int BIN2 = 22;
const int ENB = 23; // PWM motor B Derecho

const int STBY_CTRL = 19; // habilitar driver

// ========= Control =========
float REF = 0.0f;       // Inicia en 0
float Kp  = 32.0f;      
const int PWM_MAX = 255;
const int PWM_MIN = 0;

float ultimoAngulo = 0.0f;

// Tiempo inicial
unsigned long tiempoInicio;

// ========= Función: cálculo ángulo =========
float calcularPosicionAngular() {
  long sumaPond = 0;
  long sumaVals = 0;

  for (int i = 0; i < 8; i++) {
    int v = analogRead(sensorPins[i]);
    sensorValues[i] = v;  
    if (v > UMBRAL) {
      sumaPond += (long)v * sensorAngles[i];
      sumaVals += v;
    }
  }

  if (sumaVals == 0) {
    return ultimoAngulo; 
  }

  float ang = (float)sumaPond / (float)sumaVals;
  ultimoAngulo = ang;
  return ang;
}

// ========= Setup =========
void setup() {
  Serial.begin(115200);

  for (int i = 0; i < 8; i++) pinMode(sensorPins[i], INPUT);

  pinMode(AIN1, OUTPUT); pinMode(AIN2, OUTPUT); pinMode(ENA, OUTPUT);
  pinMode(BIN1, OUTPUT); pinMode(BIN2, OUTPUT); pinMode(ENB, OUTPUT);
  pinMode(STBY_CTRL, OUTPUT);

  digitalWrite(STBY_CTRL, HIGH); 
  digitalWrite(AIN1, HIGH); digitalWrite(AIN2, LOW); 
  digitalWrite(BIN1, HIGH); digitalWrite(BIN2, LOW);

  tiempoInicio = millis();  // guarda el tiempo de inicio

}

// ========= Loop =========
void loop() {
  // ====== Cambio automático de referencia ======
  if ((millis() - tiempoInicio) > 6000) {  
    REF = 3.77f;  
  }

  // Leer sensores y calcular ángulo
  float ang = calcularPosicionAngular();

  // Error con respecto a referencia
  float error = REF - ang;

  // Control proporcional
  int pwm = (int)(Kp * abs(error));
  pwm = constrain(pwm, PWM_MIN, PWM_MAX);

  int pwmIzq = 0;
  int pwmDer = 0;

  // ====== Dirección según signo del error ======
  if (error > 0) {
    // Línea hacia la derecha -> gira a la derecha
    pwmIzq = pwm;   // motor izquierdo encendido
    pwmDer = 0;     // motor derecho apagado
  } else if (error < 0) {
    // Línea hacia la izquierda -> gira a la izquierda
    pwmIzq = 0;     // motor izquierdo apagado
    pwmDer = pwm;   // motor derecho encendido
  } else {
    // centrado
    pwmIzq = pwm;
    pwmDer = pwm;
  }

  // Aplicar PWM
  analogWrite(ENA, pwmIzq);
  analogWrite(ENB, pwmDer);

  // ====== Debug serial ======
  Serial.print(millis());
  Serial.print(" | ");
  Serial.print("REF: "); Serial.print(REF, 2);
  Serial.print(" | ANG: "); Serial.print(ang, 2);
  //Serial.print(" | ERR: "); Serial.print(error, 2);
  //Serial.print(" | PWM_I: "); Serial.print(pwmIzq);
  //Serial.print(" | PWM_D: "); Serial.print(pwmDer);
  //Serial.print(" | SENSORES: ");
  //for (int i = 0; i < 8; i++) {
    //Serial.print(sensorValues[i]);
  //  if (i < 7) Serial.print(",");
  //}
  Serial.println();
  //delay(50);
}