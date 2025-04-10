#include <WiFi.h>
#include <PubSubClient.h>

// Configura tu WiFi
const char* ssid = "nombre red";
const char* password = "contraseña red";


const char* mqtt_server = "192.168.1.14"; //solamente se pone la ip del dispositivo de la comunicación, luego se coloca el puerto del MQTT

WiFiClient espClient;
PubSubClient client(espClient);

const char* topics[5] = {
  "sensor/temperatura",
  "sensor/humedad",
  "sensor/luz",
  "sensor/presion",
  "sensor/vibracion"
};

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Intentando conectar MQTT...");
    if (client.connect("ESP32Client")) {
      Serial.println("conectado");
    } else {
      Serial.print("falló, rc=");
      Serial.print(client.state());
      Serial.println(" intentando de nuevo en 5 segundos");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 2; j++) {
      int dato = random(0, 100); // dato aleatorio entre 0 y 99
      char mensaje[10];
      itoa(dato, mensaje, 10);
      client.publish(topics[i], mensaje);
      Serial.printf("Publicado en %s: %s\n", topics[i], mensaje);
      delay(500); 
    }
  }

 
  delay(5000); //5segs para siguiente msg
}
