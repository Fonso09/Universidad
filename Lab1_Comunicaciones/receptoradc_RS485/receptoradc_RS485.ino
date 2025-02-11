#include <HardwareSerial.h>

#define TX_PIN 17
#define RX_PIN 16
#define DE_RE 23  // Control de dirección

HardwareSerial RS485(1);

void setup() {
    Serial.begin(115200);
    RS485.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN);

    pinMode(DE_RE, OUTPUT);
    digitalWrite(DE_RE, LOW);  // Inicia en modo recepción
}

void loop() {
    if (RS485.available()) {
        String adcValue = recibirMensaje();
        Serial.println(adcValue.toInt());
    }
}

// 🔹 Función para recibir mensajes
String recibirMensaje() {
    String msg = "";
    while (RS485.available()) {
        char c = RS485.read();
        if (c >= 32 || c == '\n') {  // Filtra caracteres basura
            msg += c;
        }
    }
    return msg;
}
