// Pines de los sensores
const int sensorPins[8] = {32,33,25,26,27,14,12,13};
////////////////////////
unsigned long t0;
unsigned long contador = 0;
// Variables globales
float receivedValue = 0;
bool dataReceived = false;
unsigned long startTime = 0;
// ========= Configuración de sensores =========
int sensorValues[8];
const int irLEDPin = 2;
int k=10; 
int ref = 0; 
int start=0;
float ang=0;
const int umbral=4000;
// Ángulos por sensor (grados) - Corregidos para simetría perfecta
float sensorAngles[8] = {-11.0, -7.86, -4.71, -1.57, 1.57, 4.71, 7.86, 11.0};
// Umbral para considerar detección de línea
// ========= Puente H / Motores =========
// Motor A (izquierdo)
const int IN1 = 21;
const int IN2 = 22;
const int ENA = 23; // PWM
// Motor B (derecho)
const int IN3 = 18;
const int IN4 = 5;
const int ENB = 17; // PWM
// Standby del puente (si aplica en tu driver)
const int STBY_PIN  = 34;   // Botón (opcional, no usado aquí)
const int STBY_CTRL = 19;  // Control standby del driver
int s=0;
float ultimoAngulo = 0.0f;
// ========= Estado auxiliar =========

float calcularPosicionAngular() {
  // Primero verificamos si algún sensor detecta la línea
  bool lineaDetectada = false;
  for (int i = 0; i < 8; i++) {
    if (sensorValues[i] > umbral) {
      lineaDetectada = true;
      break;
    }
  }
  
  // Si no hay detección, mantener el último ángulo
  if (!lineaDetectada) {
    return ultimoAngulo;
  }
  
  // Calcular posición ponderada
  float sumaPonderada = 0.0;
  float sumaValores = 0.0;
  
  for (int i = 0; i < 8; i++) {
    if (sensorValues[i] > umbral) {
      // Normalizar el valor del sensor (restar el umbral)
      float valorNormalizado = (float)(sensorValues[i] - umbral);
      sumaPonderada += valorNormalizado * sensorAngles[i];
      sumaValores += valorNormalizado;
    }
  }
  
  // Calcular ángulo promedio ponderado
  if (sumaValores > 0) {
    ultimoAngulo = sumaPonderada / sumaValores;
  }
  
  // Filtro simple para suavizar pequeñas variaciones cerca de 0
  if (abs(ultimoAngulo) < 0.05) {
    ultimoAngulo = 0.0;
  }
  
  return ultimoAngulo;
}

void setup() {
  Serial.begin(115200);
  t0 = millis();
  // Sensores
  for (int i = 0; i < 8; i++) pinMode(sensorPins[i], INPUT);
  pinMode(irLEDPin, OUTPUT);
  digitalWrite(irLEDPin, HIGH);
  // Puente H
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT); pinMode(ENA, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT); pinMode(ENB, OUTPUT);
  pinMode(STBY_PIN, INPUT_PULLUP);
  pinMode(STBY_CTRL, OUTPUT);
  digitalWrite(STBY_CTRL, HIGH); // habilitar puente
  // Dirección hacia adelante por defecto
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  // (Opcional) encabezado para depuración
  Serial.println("PWM,Angulo,Estado,S0,S1,S2,S3,S4,S5,S6,S7");
}

void loop() {
  
  //contador++;
  //if (millis() - t0 >= 1000) {
  //  Serial.print("Frecuencia de muestreo = ");
  //  Serial.print(contador);
  //  Serial.println(" Hz");
    
  //  contador = 0;
  //  t0       = millis();
  //}
  if (s==0)
  {
    delay (2000);
    s++;
  }
  // 2) Sólo calcular / enviar si ya se recibió una referencia
    if (Serial.available() > 0) {
    // Lee la línea completa hasta el salto de línea
    String input = Serial.readStringUntil('\n');
    // Convierte a entero
    ref = input.toInt();
    }
        // ======= Medición de sensores =======
    // Limitar PWM máximo
    // ======= Medici0ón de sensores =======
    for (int i = 0; i < 8; i++) sensorValues[i] = analogRead(sensorPins[i]);
    
      // Referencia = 0 grados (centro)
    
    // ======= Ángulo (salida) =======
    float ang = calcularPosicionAngular(); // grados (negativo izq, positivo der)
    
    // Calcular error y PWM
    float error = ref - ang;  // Error: positivo = girar a la izquierda, negativo = girar a la derecha
    int pwm = abs(k * error);
    if (pwm > 255) pwm = 255;
    if (error<-0.05 || error>0.05){
    for (int i = 0; i < 8; i++) sensorValues[i] = analogRead(sensorPins[i]);
    
      // Referencia = 0 grados (centro)
    
    // ======= Ángulo (salida) =======
    ang = calcularPosicionAngular(); // grados (negativo izq, positivo der)
    
    // Calcular error y PWM
    error = ref - ang;  // Error: positivo = girar a la izquierda, negativo = girar a la derecha
    pwm = abs(k * error);
    if (pwm > 255) pwm = 255;
    if ((ref-ang)<0){
      digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
      analogWrite(ENA, pwm);
      digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
      analogWrite(ENB, pwm);
    }
    if ((ref-ang)>0){
      digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
      analogWrite(ENB, pwm);
      digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
      analogWrite(ENA, pwm);
    }
    Serial.print(millis());
      Serial.print(",");
      Serial.println(ang);
    delay (2);    // ======= Envío de datos =======
    }
    //Serial.print(pwm); Serial.print(",");
    //Serial.print(ang);Serial.print(",");
    //Serial.print(s);Serial.print(",");
    //for (int i = 0; i < 8; i++) {
    //  Serial.print(sensorValues[i]);
    //  Serial.print(" ");
    //}
  
  //Serial.println();
  //delay(20);
  // Después de terminar, no hacemos nada más (motores apagados, no se envían datos)
}