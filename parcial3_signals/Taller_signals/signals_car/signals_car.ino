#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <Adafruit_VL53L0X.h>

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

/*-------------Configuración VL53L0X-----------------*/

// Filtro de media móvil
const int N = 5;
float buffer[N];
int indiceBuffer = 0;
bool lleno = false;

unsigned long nextSendTime = 0;



/*-------------Configuración MQTT-----------------*/
// ⚠️ Reemplaza estos con los datos de tu red WiFi
const char* ssid = "PIPECAJICA";
const char* password = "dafe1961";

//const char* ssid = "APS_PUB_R";
//const char* password = "";

//const char* ssid = "Galaxy A318CD4";
//const char* password = "qbbb2551";

// Datos del broker
const char* mqtt_server = "64c2a81079224d5484342f5cdf8b1ea8.s1.eu.hivemq.cloud";
const int mqtt_port = 8883;
const char* mqtt_user = "signal_car";
const char* mqtt_pass = "Esp12345";

// Certificado raíz (Let's Encrypt ISRG Root X1)
const char* root_ca = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIDrzCCApegAwIBAgIQBMy5hCBlOv+zPb6O7KqsXzANBgkqhkiG9w0BAQsFADBh\n" \
"MQswCQYDVQQGEwJVUzETMBEGA1UEChMKRGln\nQ2VydCBJbmMxKDAmBgNVBAMTH0Rp\n" \
"Z2lDZXJ0IEdsb2JhbCBSb290IEcyMB4XDTIwMTEwNDAwMDAwMFoXDTM1MTEwMzIz\n" \
"NTk1OVowYTELMAkGA1UEBhMCVVMxEzARBgNVBAoTCkRpZ2ljZXJ0IEluYzEoMCYG\n" \
"A1UEAxMfRGlnaUNlcnQgR2xvYmFsIFJvb3QgRzIwggEiMA0GCSqGSIb3DQEBAQUA\n" \
"A4IBDwAwggEKAoIBAQDhfLa6+ACfXjNydlKBJv8mgVhKHjQ6Q+bbqKHCgWKP3yd/\n" \
"lhE2pDHK5j7zyJ26KrA4YQtGzHAnItOpUbtRZlVZZc+mbb5o2i8L/FtN9NKoWaDj\n" \
"JK3A1m/O9/N4Jc1MR4W8B4ntF2pPrzmX57kM+N0VFRHL08lA/oyKXUJ9RY0IFu/U\n" \
"zdyGSuTph9qZP35/X6fY8US8A0EAnUyt3rDRRgz4EYb7yFX3OTcy7j5W19Vkl5Kw\n" \
"AyquYY5J9Wc4+JbIvGSQKrhXvcgjsZUT+xtuO3lazptb/7b+Jmx/5aM9Q+Xl6jdQ\n" \
"nEM5caCEFm5+vJK5aPIgZQwKUKhVMTwq7IT9FoF3AgMBAAGjQjBAMA4GA1UdDwEB\n" \
"/wQEAwIBBjAPBgNVHRMBAf8EBTADAQH/MB0GA1UdDgQWBBSx0cJDBFQeY1P8LXN2\n" \
"3z6eYH9FODANBgkqhkiG9w0BAQsFAAOCAQEAZTDsDkKZUEgAN06fLCYZLZb9uvH4\n" \
"w1ga0FHRZzOfPQm8SPK2bQ5vZH5DORFw0ctX+aFD5n6DOI/0T8Ij8q6nvcD8Zs8p\n" \
"1ftmA9MAja99K4yYqMd9XN5kZp1P7qx09wEpzD7yfpJpBYlCVNULHqhHhHj0JYgx\n" \
"6A0QvKZB4NWkJ3G9PzYZcYVYYyG7MUMwfsBgNp0obKBP9BP28eF+nOEk3OTK4nLp\n" \
"qw8wWlO30QkgDBjNmF+dA4EcXq1jaPVzvZdHeWeNU17H8vMH0gT/RcW8vb15xQ4R\n" \
"pYm5n3mcVG+ymdZj0t+QkGp3xQjgzPq2zMnSMY1sKcK5gNfYZR3b0Efw==\n" \
"-----END CERTIFICATE-----\n";



WiFiClientSecure secureClient;
PubSubClient client(secureClient);


/*---------------Configuración motor-----------------*/
// Motor izquierdo
const int IN1 = 12;
const int IN2 = 13;

// Motor derecho
const int IN3 = 26;
const int IN4 = 27;

// Pines sensores IR digitales
const int IR_IZQ = 33;
const int IR_DER = 32;

bool seguirLinea = false;
bool buscando_objetos = false;
String seguidor_state = "ST";

/*---------Funciones -----------*/

void setup_wifi() {
  delay(10);
  Serial.println("Conectando a WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado");
}

void message_instructions(const String& message) {
  String cmd = message;
  cmd.trim();        // Elimina espacios en blanco
  cmd.toLowerCase(); // Convierte a minúsculas por robustez

  if (cmd == "p") {
    seguirLinea = !seguirLinea;
    Serial.printf("Seguidor: %d\n", seguirLinea);
    if(seguirLinea)
      client.publish("encyclopedia/seg", seguidor_state.c_str());
  } 
  else if (!seguirLinea) {
    if (cmd == "w") {
      adelante();
      Serial.println("Instrucción: avanzar");
    }
    else if (cmd == "s") {
      atras();
      Serial.println("Instrucción: retroceder");
    }
    else if (cmd == "a") {
      izquierda();
      Serial.println("Instrucción: girar izquierda");
    }
    else if (cmd == "d") {
      derecha();
      Serial.println("Instrucción: girar derecha");
    }
    else if (cmd == "stop") {
      parar();
      Serial.println("Instrucción: detener");
    }
    else {
      Serial.print("Instrucción no reconocida: ");
      Serial.println(cmd);
    }
  }else if (cmd == "o") {
      detectando_ando();
    }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensaje en [");
  Serial.print(topic);
  Serial.print("]: "); 

  String message;
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    message += (char)payload[i];
  }
  Serial.println();

  if (String(topic) != "encyclopedia/driving" || buscando_objetos) {
    return;  // Sale de la función si el tópico no es "driving"
  }

  
  message_instructions(message); // 👈 Llamada a la función de control

}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando a MQTT...");
    if (client.connect("ESP32Client", mqtt_user, mqtt_pass)) {
      Serial.println("Conectado");
      client.subscribe("encyclopedia/#");
    } else {
      Serial.print("Fallo, rc=");
      Serial.print(client.state());
      delay(5000);
    }
  }
}


/*---------Funciones motor-----------*/

void adelante() {
  Serial.println("AD");
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void atras() {
  Serial.println("AT");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void derecha() {
  Serial.println("DER");
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void izquierda() {
  Serial.println("IZQ");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void parar() {
  Serial.println("Tieso");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void rotating(){
  Serial.println("Rotando");
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

// Seguidor de linea

void SeguidorDeLinea() {
  int sensorIzq = digitalRead(IR_IZQ);
  int sensorDer = digitalRead(IR_DER);
  String prv_state = seguidor_state;
  Serial.printf("%d-%d", sensorIzq, sensorDer);
  if (sensorIzq == 0 && sensorDer == 0) {
    adelante();
    seguidor_state = "AD";
    //client.publish("encyclopedia/seg", "AD");
  } else if (sensorIzq == 1 && sensorDer == 0) {
    izquierda();
    seguidor_state = "IZ";
    //client.publish("encyclopedia/seg", "IZ");
  } else if (sensorIzq == 0 && sensorDer == 1) {
    derecha();
    seguidor_state = "DR";
    //client.publish("encyclopedia/seg", "DR");
  } else if (sensorIzq == 1 && sensorDer == 1) {
    //client.publish("encyclopedia/seg", "ST");
    parar();
    seguidor_state = "ST";
  }
  if(seguidor_state != prv_state)
    client.publish("encyclopedia/seg", seguidor_state.c_str());
}

//Sensor de distancia
float suavizar(float nuevaLectura) {
  buffer[indiceBuffer] = nuevaLectura;
  indiceBuffer = (indiceBuffer + 1) % N;

  if (indiceBuffer == 0) lleno = true;

  float suma = 0;
  int cantidad = lleno ? N : indiceBuffer;
  for (int i = 0; i < cantidad; i++) {
    suma += buffer[i];
  }

  return suma / cantidad;
}

// Convertir distancia (0–20 cm) a valor de DAC (0–255)
uint8_t cmAvoltaje(float cm) {
  cm = constrain(cm, 0, 20);
  return (uint8_t)(cm * (255.0 / 20.0));
}


//Midiendo por rotación
void detectando_ando() {
  unsigned long previousMillis = 0;
  const unsigned long interval = 72;
  int rotation_degree = 0;
  
  while (rotation_degree <= 360) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;

      VL53L0X_RangingMeasurementData_t medida;
      lox.rangingTest(&medida, false);

      float dist2send = -1;
      if (medida.RangeStatus != 4) {
        float distancia = medida.RangeMilliMeter / 10.0; // mm a cm
        float distanciaSuavizada = suavizar(distancia);
        if (distanciaSuavizada < 15) {
          Serial.printf("%.2f\n", distanciaSuavizada);
          dist2send = distanciaSuavizada;
        }
      }

      String mssg2send = String(rotation_degree) + "," + String(dist2send);
      client.publish("encyclopedia/obj", mssg2send.c_str());

      rotating();
      rotation_degree+=5;
    }
  }
  parar();
  client.publish("encyclopedia/obj", "0,-2");
  //nextSendTime = millis() + 5000; // Empezar conteo de 5 seg desde ahora
}


void setup() {
  Serial.begin(115200);
  Wire.begin(); // Usa GPIO21 (SDA) y GPIO22 (SCL) por defecto en ESP32

  if (!lox.begin()) {
    Serial.println("Fallo al iniciar el VL53L0X");
    while (1);
  }

  pinMode(2, OUTPUT);

  // Pines de motor como salida
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Pines sensores IR
  pinMode(IR_IZQ, INPUT);
  pinMode(IR_DER, INPUT);

  parar();
  setup_wifi();

  //secureClient.setCACert(root_ca);  // Establece el certificado
  secureClient.setInsecure();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
    digitalWrite(2, 0);
  }else
    digitalWrite(2, 1);
  client.loop();

  /*if (millis() >= nextSendTime) {
    client.publish("encyclopedia/driving", "o");
    nextSendTime = millis() + 5000;  // Reprograma el próximo envío
  }*/

  if (seguirLinea) {
    SeguidorDeLinea();
  }
}
