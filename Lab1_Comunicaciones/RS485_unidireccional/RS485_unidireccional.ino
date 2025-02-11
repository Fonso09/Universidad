#include <HardwareSerial.h>
#define TX_PIN 17
#define RX_PIN 16
#define DE_RE_1 23
#define ADC_PIN 2

void enviar_RS485(int dato_ADC);
void Recibir_R485();

int ADC_value =0;
HardwareSerial RS485(1);
void setup() {
  Serial.begin(115200); //Inicializar Serial con el PC
  RS485.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN); //Inicializar RS485
  pinMode(DE_RE_1, OUTPUT); //Pin para poner RS485 como lectura o escritura
  pinMode(ADC_PIN, INPUT); //Pin que lee el sensor para enviar a la otra ESP
  digitalWrite(DE_RE_1, LOW); // Inicializa el RS485 como modo lectura 
  
}

void loop() {
  ADC_value = analogRead(ADC_PIN);
  enviar_RS485(ADC_value);
  Serial.println(ADC_value);
  delay(5);

}
void enviar_RS485(int dato_ADC){
    digitalWrite(DE_RE_1, HIGH);  // Cambiar a modo transmisión
    delay(10);
    RS485.println(dato_ADC);
    delay(10);
    //digitalWrite(DE_RE, LOW);   // Volver a modo recepción
}
