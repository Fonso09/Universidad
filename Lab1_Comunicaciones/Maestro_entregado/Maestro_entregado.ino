#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "esp_log.h"
#include "driver/spi_master.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "BluetoothSerial.h"
#include "Wire.h"
#include <HardwareSerial.h>
#define I2C_DEV_ADDR 0x55
#define GPIO_MOSI 13
#define GPIO_MISO 12
#define GPIO_SCLK 14
#define GPIO_CS   15
#define TX_PIN 17
#define RX_PIN 16
#define DE_RE_1 23
#define PIN_ADC 34  

BluetoothSerial SerialBT;
String modo_comunicacion = "";
uint16_t adc_value = 0;
uint16_t slave_adc_value=0;
uint8_t bytesReceived = 0;
String modo = "2";
int int_modo = 2;
//ADC
SemaphoreHandle_t timer_sem; //Inicializar el semáforo
hw_timer_t *timer = NULL; //Inicializar el semáforo
int valor_adc = 0;

//RS485

HardwareSerial RS485(1);
String leer_RS485();

//SPI
spi_device_handle_t handle;
char recvbuf[128] = "";  
char sendbuf[128] = "";
volatile bool dataReady = false;  // Bandera para indicar datos nuevos

//SPI
void SPI_Master_SetUp();
void IRAM_ATTR spi_post_transaction_cb(spi_transaction_t *t);
void IRAM_ATTR onTimer();

//ADC
void read_adc(void *parameter);
void adc_tm_setup();
void SPI_Sender();

//BT
void BT_Master_SetUp();
void BT_Sender();

void setup() {
    Serial.begin(115200);
    Serial.println("ESP32 Maestro SPI");
    SPI_Master_SetUp();
    BT_Master_SetUp();
    adc_tm_setup();
    pinMode(2, OUTPUT);
    pinMode(26, OUTPUT);
    pinMode(27, OUTPUT);
    RS485.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN); //Inicializar RS485
    pinMode(DE_RE_1, OUTPUT); //Pin para poner RS485 como lectura o escritura
    digitalWrite(DE_RE_1, LOW); // Inicializa el RS485 como modo lectura 
    Wire.begin();
}

void loop() {   
    //SPI_Sender();
    //BT_sender();
  delay(10);
  //envar ADC al esclavo del I2C
  Wire.beginTransmission(I2C_DEV_ADDR);
  Wire.write((uint8_t*)&valor_adc, sizeof(valor_adc));
  uint8_t error = Wire.endTransmission(true);
  // Solicitar la lectura del ADC del esclavo
  bytesReceived = Wire.requestFrom(I2C_DEV_ADDR, sizeof(valor_adc));
  if (bytesReceived == sizeof(valor_adc)) {
    Wire.readBytes((uint8_t*)&slave_adc_value, sizeof(slave_adc_value));
    //Serial.printf("Valor recibido del esclavo: %u\n", slave_adc_value);
  }

  if (Serial.available()) {
    modo = Serial.readStringUntil('\n');
    //Serial.println(modo);
    int_modo = modo.toInt();
    digitalWrite(26, int_modo & (1<<0));
    digitalWrite(27, int_modo & (1<<1));
    delay(100);
  }
  
    if (int_modo == 0) {
        //I2C
        if (bytesReceived == sizeof(adc_value)) {
          //Serial.printf("Valor recibido del esclavo: %u\n", slave_adc_value);
          Serial.println(slave_adc_value);
        }

    } 
     else if (int_modo == 1) {
        SPI_Sender();
    } else if (int_modo == 2) {
        BT_Sender();
    } else if (int_modo == 3) {
        //Serial
        if(RS485.available()){
          String adcValue = leer_RS485();
          Serial.println(adcValue.toInt());
        }
    } 
  

}

//SPI
void SPI_Sender(){
  spi_transaction_t t;
    memset(&t, 0, sizeof(t));

    snprintf(sendbuf, sizeof(sendbuf), "%04d", valor_adc); // Formatea el valor ADC
    t.length = 4 * 8; 
    t.tx_buffer = sendbuf;
    t.rx_buffer = recvbuf;

    esp_err_t ret = spi_device_transmit(handle, &t);
    if (ret != ESP_OK) {
        Serial.printf("SPI transmit error: %d\n", ret);
    }

    if (dataReady) {
        //Serial.printf("Dato recibido: %s\n", recvbuf);
        Serial.println(recvbuf);
        dataReady = false;
    }
    
    //Serial.printf("%04d\n", valor_adc);
    delay(100);
}
void SPI_Master_SetUp(){
      spi_bus_config_t buscfg = {
        .mosi_io_num = GPIO_MOSI,
        .miso_io_num = GPIO_MISO,
        .sclk_io_num = GPIO_SCLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1
    };

    spi_device_interface_config_t devcfg = {
        .command_bits = 0,
        .address_bits = 0,
        .dummy_bits = 0,
        .mode = 0,
        .clock_speed_hz = 5000000,  
        .spics_io_num = GPIO_CS,
        .queue_size = 3,
        .post_cb = spi_post_transaction_cb  
    };

    spi_bus_initialize(HSPI_HOST, &buscfg, SPI_DMA_CH_AUTO);
    spi_bus_add_device(HSPI_HOST, &devcfg, &handle);
}

//BT
void BT_Master_SetUp(){
  SerialBT.begin("ESP32_Maestro", true);
  //48:E7:29:9F:2E:CA
  uint8_t address[] = {0x48, 0xE7, 0x29, 0x9F, 0x2E, 0xCA}; 
  
  if (SerialBT.connect(address)) {
    Serial.println("Conectado al esclavo!");
    digitalWrite(2, 1);
  } else {
    Serial.println("Fallo la conexión.");
    digitalWrite(2, 0);
    return;
  }
}

void BT_Sender(){
    //int valor_adc = analogRead(PIN_ADC);  
  SerialBT.print(valor_adc);  // Enviar sin salto de línea
  SerialBT.print("\n");      // Asegurar que el mensaje es completo
  //Serial.print("Valor enviado: ");
  //Serial.println(valor_adc);
  digitalWrite(2, SerialBT.connected());
  unsigned long startTime = millis();
  String respuesta = "";
  
  while (millis() - startTime < 1000) {  // Esperar hasta 1 segundo la respuesta
    if (SerialBT.available()) {  
      respuesta = SerialBT.readStringUntil('\n');  // Leer hasta el delimitador
      break;
    }
  }

  //Serial.print("Valor recibido: ");
  Serial.println(respuesta);
}

//ADC
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

void IRAM_ATTR spi_post_transaction_cb(spi_transaction_t *t) {
    if (t->rx_buffer != NULL) {
        memcpy(recvbuf, t->rx_buffer, t->length / 8);
        dataReady = true;
    }
}

void IRAM_ATTR onTimer(){
  BaseType_t higherPriorityTaskWoken = pdFALSE;
  xSemaphoreGiveFromISR(timer_sem, &higherPriorityTaskWoken);
  portYIELD_FROM_ISR(higherPriorityTaskWoken);
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
  
