#include <WiFi.h>
#include <HTTPClient.h>

int incremento = 0;
const char* ssid = "nombre wifi";
const char* password = "contraseña wifi";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando...");
  }
  Serial.println("Conectado a WiFi");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    String mensaje = "holas n, " + String(incremento++);
    Serial.print("Enviando: ");
    Serial.println(mensaje);

    http.begin("http://192.168.1.52:8080/recibir.php");
    http.addHeader("Content-Type", "text/plain");

    int httpResponseCode = http.POST(mensaje);
    Serial.print("Código de respuesta: ");
    Serial.println(httpResponseCode);

    http.end();
  } else {
    Serial.println("WiFi no conectado");
  }

  delay(1000);
}
