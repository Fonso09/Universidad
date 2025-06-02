import pygame
import threading
import math
import time
import paho.mqtt.client as paho
from paho import mqtt

# === CONFIGURA ESTOS DATOS DE HIVE MQ CLOUD ===
HIVEMQ_CLUSTER = "64c2a81079224d5484342f5cdf8b1ea8.s1.eu.hivemq.cloud"
HIVEMQ_USER = "fonsito"
HIVEMQ_PASS = "Esp123456"

# MQTT topics
TOPIC_SEG = "encyclopedia/seg"
TOPIC_MANUAL = "encyclopedia/driving"
TOPIC_OBJ = "encyclopedia/obj"

# Estado del carro
car = {
    "x": 1100,
    "y": 1700,
    "angle": 0,
    "velocity": 0.5,
    "last_cmd": "ST",  # Puede ser: AD, IZ, DR, ST, o W/A/S/D
    "last_obj_time": 0,
    "scanning": False,
}
obstacles = []
trail = []
MAX_TRAIL = 100
OBJ_TIMEOUT = 1.5  # segundos

# MQTT callbacks
def on_connect(client, userdata, flags, rc, properties=None):
    print("Conectado con código:", rc)
    client.subscribe("encyclopedia/#", qos=1)

def on_message(client, userdata, msg):
    topic = msg.topic
    payload = msg.payload.decode()
    print(f"[{topic}] {payload}")  # DEBUG: muestra todo lo recibido

    if topic == TOPIC_SEG:
        if not car["scanning"]:
            car["last_cmd"] = payload

    elif topic == TOPIC_MANUAL:
        if payload == "o":
            car["scanning"] = True
            print("⏺ Iniciando escaneo...")
        elif not car["scanning"]:
            if payload =="stop":
                car["last_cmd"]= "ST"
            if payload in ['w', 'a', 's', 'd']:
                car["last_cmd"] = payload.upper()

    elif topic == TOPIC_OBJ:
        try:
            degree_str, dist_str = payload.split(",")
            degree = float(degree_str)
            dist = float(dist_str)

            car["angle"] = degree
            car["last_obj_time"] = time.time()
            car["scanning"] = True

            if dist != -1:
                angle_rad = math.radians(degree)
                dx = dist * 10 * math.cos(angle_rad)
                dy = dist * 10 * math.sin(angle_rad)
                obstacles.append((car["x"] + dx, car["y"] - dy))

        except Exception as e:
            print("❌ Error procesando obj:", e)

# Movimiento del carro
def update_car():
    if car["scanning"] and (time.time() - car["last_obj_time"]) > OBJ_TIMEOUT:
        car["scanning"] = False
        print("✅ Escaneo finalizado")

    if not car["scanning"]:
        angle_rad = math.radians(car["angle"])
        cmd = car["last_cmd"]

        if cmd in ["AD", "W"]:
            car["x"] += car["velocity"] * math.cos(angle_rad)
            car["y"] -= car["velocity"] * math.sin(angle_rad)
        elif cmd in ["IZ", "A"]:
            car["angle"] = (car["angle"] + 1) % 360
        elif cmd in ["DR", "D"]:
            car["angle"] = (car["angle"] - 1) % 360
        elif cmd == "S":
            car["x"] -= car["velocity"] * math.cos(angle_rad)
            car["y"] += car["velocity"] * math.sin(angle_rad)

# Hilo MQTT
def mqtt_thread():
    client = paho.Client(client_id="", protocol=paho.MQTTv5)
    client.on_connect = on_connect
    client.on_message = on_message
    client.tls_set(tls_version=mqtt.client.ssl.PROTOCOL_TLS)
    client.username_pw_set(HIVEMQ_USER, HIVEMQ_PASS)
    client.connect(HIVEMQ_CLUSTER, 8883)
    client.loop_forever()

# Interfaz con pygame
def main():
    pygame.init()
    screen = pygame.display.set_mode((3000, 3000))
    pygame.display.set_caption("Vista del Carro Seguidor")
    clock = pygame.time.Clock()

    threading.Thread(target=mqtt_thread, daemon=True).start()

    trail_timer = time.time()
    running = True

    while running:
        screen.fill((30, 30, 30))
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False

        update_car()

        # Cola del recorrido
        if time.time() - trail_timer > 0.1:
            trail.append((car["x"], car["y"]))
            if len(trail) > MAX_TRAIL:
                trail.pop(0)
            trail_timer = time.time()
        if len(trail) > 1:
            pygame.draw.lines(screen, (0, 255, 0), False, trail, 2)

        # Obstáculos
        for ox, oy in obstacles:
            pygame.draw.rect(screen, (255, 0, 0), pygame.Rect(ox - 5, oy - 5, 10, 10))

        # Carro
        surf = pygame.Surface((20, 10))
        surf.fill((0, 200, 255))
        rotated = pygame.transform.rotate(surf, car["angle"])
        rect = rotated.get_rect(center=(car["x"], car["y"]))
        screen.blit(rotated, rect.topleft)

        # Estado
        font = pygame.font.SysFont(None, 24)
        state = "ESCANEANDO" if car["scanning"] else "SIGUIENDO / MANUAL"
        text = font.render(f"Estado: {state}", True, (255, 255, 255))
        screen.blit(text, (10, 10))

        pygame.display.flip()
        clock.tick(30)

    pygame.quit()

if __name__ == "__main__":
    main()
