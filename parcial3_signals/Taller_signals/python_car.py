import time
import threading
import keyboard  # pip install keyboard
import paho.mqtt.client as paho
from paho import mqtt

activo = True
last_pressed = set()

# setting callbacks for different events to see if it works, print the message etc.
def on_connect(client, userdata, flags, rc, properties=None):
    print("CONNACK received with code %s." % rc)

# with this callback you can see if your publish was successful
def on_publish(client, userdata, mid, properties=None):
    print("mid: " + str(mid))

# print which topic was subscribed to
def on_subscribe(client, userdata, mid, granted_qos, properties=None):
    print("Subscribed: " + str(mid) + " " + str(granted_qos))

# función para publicar el comando
def publish_command(command):
    client.publish("encyclopedia/driving", payload=command, qos=1)
    print(f"Enviado: {command}")

# cuando reciba el mensaje final del escaneo, inicia el temporizador
def on_message(client, userdata, msg):
    payload = msg.payload.decode()
    print(msg.topic + " " + str(msg.qos) + " " + payload)

    if msg.topic == "encyclopedia/obj" and payload == "0,-2":
        print("⏱️ Mensaje de fin de escaneo recibido. Iniciando temporizador de 5 segundos...")
        threading.Timer(5.0, lambda: publish_command("o")).start()

# Configuración del cliente MQTT
client = paho.Client(client_id="test", userdata=None, protocol=paho.MQTTv5)
client.on_connect = on_connect
client.on_subscribe = on_subscribe
client.on_message = on_message
client.on_publish = on_publish

# Seguridad y conexión
client.tls_set(tls_version=mqtt.client.ssl.PROTOCOL_TLS)
client.username_pw_set("python_car", "Esp12345")
client.connect("64c2a81079224d5484342f5cdf8b1ea8.s1.eu.hivemq.cloud", 8883)

# Suscripción
client.subscribe("encyclopedia/#", qos=1)

# Arranque del loop MQTT
client.loop_start()


# Asignar acciones a las teclas
key_map = {
    'w': 'adelante',
    'a': 'izquierda',
    's': 'atras',
    'd': 'derecha',
    'p': 'modo',
    'o': 'detecta objetos'
}

try:
    publish_command('o')
    while True:
        if keyboard.is_pressed('°'):
            print("Control desactivado.")
            break

        for key in key_map:
            if keyboard.is_pressed(key) and key not in last_pressed and activo:
                publish_command(key)
                #publish_command(key_map[key])
                last_pressed.add(key)

            if not keyboard.is_pressed(key) and key in last_pressed:
                publish_command("stop")
                last_pressed.remove(key)

        time.sleep(0.05)

except KeyboardInterrupt:
    print("Interrumpido por el usuario.")
finally:
    client.loop_stop()
    client.disconnect()