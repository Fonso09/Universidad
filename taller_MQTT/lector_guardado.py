import mysql.connector
import paho.mqtt.client as mqtt
from datetime import datetime

# conexión a la base de datos:
conn = mysql.connector.connect(
    host="localhost",    #se coloca esto porque tenemos abierto el puerto del contenedor de la base de datos
    user="fonso",  #se coloca el user de la base de datos
    password="fonso1",
    database="mqtt_data" 
)
cursor = conn.cursor()

#guardar el mensaje que se capturó del mqtt broker
def save_to_db(topic, payload):
    cursor.execute("INSERT INTO mqtt_msg (topic, payload) VALUES (%s, %s)",(topic, payload))
    conn.commit()
#esto es para que cuando le llegue un mensajes por mqtt guarde en la db
def on_msg(client, userdata, msg):
    print(f"REcibido -> Topic: {msg.topic}, Payload: {msg.payload.decode()}")
    save_to_db(msg.topic, msg.payload.decode())

client = mqtt.Client()
client.on_message = on_msg

client.connect("localhost", 1883)
client.subscribe("#")

client.loop_forever()