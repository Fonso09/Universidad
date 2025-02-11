#include <HardwareSerial.h>
#define TX_PIN 17
#define RX_PIN 16
#define DE_RE_1 23
#define DE_RE_2 19  // este pin es para decirle a la otra esp que empiece a enviar datos para recibir. 
#define DE_RE_BTN 18 // lectura por si se activa o desactiva el DE_RE de la otra ESP
#define ADC_PIN 2

String modo_comunicacion = "";
int ADC_value =0;
bool req_enviar_RS485 = false; 

HardwareSerial RS485(1);
String leer_RS485();
void enviar_RS485(int dato_ADC);


void setup() {
  Serial.begin(115200); //Inicializar Serial con el PC
  RS485.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN); //Inicializar RS485
  pinMode(DE_RE_1, OUTPUT); //Pin para poner RS485 como lectura o escritura
  pinMode(DE_RE_2, OUTPUT); // pin para mandar a la otra esp con el rs485
  pinMode(ADC_PIN, INPUT); //Pin que lee el sensor para enviar a la otra ESP
  pinMode(DE_RE_BTN, INPUT); //Flag que manda la otra esp para que le envíen por RS485
  digitalWrite(DE_RE_1, LOW); // Inicializa el RS485 como modo lectura 
  

}

void loop() {
  /* 
  En Esta parte del codigo la idea es que lea lo que recibe del python para seleccionar de qué comunicación enviar los datos y lea el sensor continuamente y 
  */
  ADC_value = analogRead(ADC_PIN);
  //Serial.println(ADC_value);

  //si el RS485 de la otra esp pide que envie aquí se leera y enviará 
  req_enviar_RS485 = digitalRead(DE_RE_BTN);
  if(req_enviar_RS485){
    digitalWrite(DE_RE_1, HIGH); // cambia a modo transmisión
    enviar_RS485(ADC_value);
  } else if (!req_enviar_RS485){
    digitalWrite(DE_RE_1, LOW); // cambia a modo recepción
  }

  //Parte del código para enviar al python los datos recibidos
  if (Serial.available()>0) {
    modo_comunicacion = Serial.readStringUntil('\n');
  }
  //modo_comunicacion= "4";
  switch(modo_comunicacion.toInt()){
    case 1:
      //I2C
      break;
    case 2:
      //SPI
      break;
    case 3:
      //BT
      break;
    case 4:
      //SERIAL
      digitalWrite(DE_RE_2, HIGH); //Decirle a la otra ESP que te empiece a mandar por el RS485 los datos de su sensor
      digitalWrite(DE_RE_1, LOW); // Inicializa el RS485 de esta ESP32 como Lectura
      delay(100);
      if (RS485.available()) {
        String adcValue = leer_RS485();
        Serial.println(adcValue.toInt());
      }
      break;    
    }
}

void enviar_RS485(int dato_ADC){
    digitalWrite(DE_RE_1, HIGH);  // Cambiar a modo transmisión
    digitalWrite(DE_RE_2, LOW); //poner el DE_RE de la otra ESP como lectura
    delay(10);
    RS485.println(dato_ADC);
    Serial.println("Dato RS485 enviado");
    delay(10);
    //digitalWrite(DE_RE, LOW);   // Volver a modo recepción
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
  

