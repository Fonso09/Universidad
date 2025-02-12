#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/spi_slave.h"
#include "esp_log.h"
#include "freertos/semphr.h"
#include "BluetoothSerial.h"
#include "driver/gpio.h"  // Biblioteca principal de GPIO
#include "soc/gpio_struct.h"  // Define la estructura GPIO para bajo nivel
 //libreria Dani
#include <HardwareSerial.h>
#include "Wire.h"

#define GPIO_MOSI 13
#define GPIO_MISO 12
#define GPIO_SCLK 14
#define GPIO_CS   15

#define PIN_ADC 34  

//defines Dani
#define I2C_DEV_ADDR 0x55
#define TX_PIN 17
#define RX_PIN 16
#define DE_RE_1 23
#define ADC_PIN 2

SemaphoreHandle_t timer_sem; //Inicializar el semáforo
hw_timer_t *timer = NULL; //Inicializar el semáforo
uint16_t valor_adc = 0;
uint16_t received_adc_value = 0;  // Último valor recibido del maestro
String modo = "2";
int int_modo = 2;

//I2C
void onRequest() {
  // Leer el ADC del esclavo

  valor_adc = analogRead(PIN_ADC); 
  // Enviar la lectura al maestro
  Wire.write((uint8_t*)&valor_adc, sizeof(valor_adc));
  //Serial.printf("Enviando ADC esclavo: %u\n", adc_value);
}

void onReceive(int len) {
  if (len >= sizeof(received_adc_value)) {
    Wire.readBytes((uint8_t*)&received_adc_value, sizeof(received_adc_value));
    //Serial.printf("Dato recibido del maestro: %u\n", received_adc_value);
  }
}

//RS485
HardwareSerial RS485(1);
String leer_RS485();
void enviar_RS485(String dato_ADC);

//SPI
char sendbuf[128] = {0};
char recvbuf[128] = "";

//BT
BluetoothSerial SerialBT;
String buffer = "";  // Buffer para almacenar los datos


//SPI
void SPI_Slave_SetUp();
void SPI_Slave_Sender();
void IRAM_ATTR onTimer();
void read_adc(void *parameter);
void adc_tm_setup();
void my_post_trans_cb(spi_slave_transaction_t *trans) {
    int x = 0;
}

//BT
void recibirDato();

void setup() {
    Serial.begin(115200);
    //I2C y RS485
    Wire.onReceive(onReceive);
    Wire.onRequest(onRequest);
    Wire.begin((uint8_t)I2C_DEV_ADDR);
    RS485.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN); //Inicializar RS485
    pinMode(DE_RE_1, OUTPUT); //Pin para poner RS485 como lectura o escritura
    pinMode(ADC_PIN, INPUT); //Pin que lee el sensor para enviar a la otra ESP
    digitalWrite(DE_RE_1, HIGH); // Inicializa el RS485 como modo lectura 
    //SPI
    SPI_Slave_SetUp();
    //BT
    SerialBT.begin("ESP32_Esclavo");  
    //Serial.println("Esperando datos del maestro...");
    pinMode(2, OUTPUT);
    pinMode(26, INPUT_PULLDOWN);
    pinMode(27, INPUT_PULLDOWN);
    adc_tm_setup();
}

void loop() {
  /*if (Serial.available()) {
    modo = Serial.readStringUntil('\n');
    Serial.println(modo);
  }*/
  //modo = gpio.
  enviar_RS485(String (valor_adc));
  int_modo = ((GPIO.in & (3<<26)) >> 26);
  //Serial.println(int_modo);
    if (int_modo == 0) {
        //I2C
        Serial.println(received_adc_value);
    } else if (int_modo == 1) {
        SPI_Slave_Sender();
    } else if (int_modo == 2) {
        recibirDato();
    } else if (int_modo == 3) {
        //Serial
    }

  //SPI_Slave_Sender();
  //recibirDato();  // Llamar la función de recepción en cada ciclo
  //Serial.printf("%04d\n", valor_adc);
  //delay(100);
}
void enviar_RS485(String dato_ADC){
    digitalWrite(DE_RE_1, HIGH);  // Cambiar a modo transmisión
    delay(10);
    RS485.println(dato_ADC);
    //Serial.println("Dato RS485 enviado");
    delay(10);
    //digitalWrite(DE_RE, LOW);   // Volver a modo recepción
}


void SPI_Slave_Sender(){
  spi_slave_transaction_t t;
  memset(&t, 0, sizeof(t));

  snprintf(sendbuf, sizeof(sendbuf), "%04d", valor_adc); // Formatea el valor ADC
  t.length = 4 * 8;
  t.tx_buffer = sendbuf;
  t.rx_buffer = recvbuf;

  esp_err_t ret = spi_slave_transmit(HSPI_HOST, &t, pdMS_TO_TICKS(500));
  if (ret == ESP_OK) {
      Serial.println(recvbuf);
  }
}
void SPI_Slave_SetUp(){
  spi_bus_config_t buscfg = {
        .mosi_io_num = GPIO_MOSI,
        .miso_io_num = GPIO_MISO,
        .sclk_io_num = GPIO_SCLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1
    };

  spi_slave_interface_config_t slvcfg = {
      .spics_io_num = GPIO_CS,
      .flags = 0,
      .queue_size = 3,
      .mode = 0,
      .post_trans_cb = my_post_trans_cb
  };

  spi_slave_initialize(HSPI_HOST, &buscfg, &slvcfg, SPI_DMA_CH_AUTO);
}

void adc_tm_setup(){
  timer_sem = xSemaphoreCreateBinary(); //Crear el Semáforo
  xTaskCreatePinnedToCore(
    read_adc,       // Función que define la tarea
    "Serial Call",   // Nombre descriptivo de la tarea
    2048,                 // Tamaño de la pila en palabras (8192 bytes)
    NULL,                 // Sin parámetros de entrada
    2,                    // Prioridad 2
    NULL,                 // No se guarda un identificador para luego modificar la tarea
    1                     // Ejecutar en el núcleo 1
  ); 

  timer = timerBegin(1000000); //Frecuencia de 1MHz
  timerAttachInterrupt(timer, &onTimer); //Timer y función asociadas a la interr
  timerAlarm(timer, 100000, true, 0); //Habilitar alarma
  //Timer, valor de conteo para alarma, reload y num de autoreloads (0 = unlimited)
}

void read_adc(void *parameter){
  while(true){
    if(xSemaphoreTake(timer_sem, portMAX_DELAY)){
      valor_adc = analogRead(PIN_ADC);  
    }
  }
}

//BT
void recibirDato() {
  digitalWrite(2, SerialBT.connected());
  while (SerialBT.available()) {
    char c = SerialBT.read();
    if (c == '\n') {  // Cuando llega el delimitador, enviar respuesta
      SerialBT.print(valor_adc);
      SerialBT.print("\n");  // Asegurar que el maestro recibe bien
      //Serial.print("Valor recibido: ");
      Serial.println(buffer);
      //Serial.printf("Valor enviado: %d\n", valor_adc);
      buffer = "";  // Limpiar el buffer
    } else {
      buffer += c;
    }
  }
}

//Interrupt
void IRAM_ATTR onTimer(){
  BaseType_t higherPriorityTaskWoken = pdFALSE;
  xSemaphoreGiveFromISR(timer_sem, &higherPriorityTaskWoken);
  portYIELD_FROM_ISR(higherPriorityTaskWoken);
}