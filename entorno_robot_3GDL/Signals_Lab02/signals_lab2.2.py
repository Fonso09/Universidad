import serial
import threading
import csv
import time
import tkinter as tk
from tkinter import messagebox
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from collections import deque

# Configurar puerto serie (ajustar según tu configuración)
puerto_serie = serial.Serial('COM13', 9600, timeout=1)

# Buffers de datos (últimos 200 valores)
buffer_size = 200
x_data = deque([0] * buffer_size, maxlen=buffer_size)
y_data = deque([0] * buffer_size, maxlen=buffer_size)
z_data = deque([0] * buffer_size, maxlen=buffer_size)
tiempo_data = deque([i * 0.05 for i in range(buffer_size)], maxlen=buffer_size)  # 50ms por muestra

# Bandera para controlar el cierre de la ventana
cerrar_programa = False

# Función para leer datos en un hilo
def leer_datos():
    global cerrar_programa
    while not cerrar_programa:
        try:
            linea = puerto_serie.readline()
            if linea:
                datos = linea.decode('utf-8', errors='ignore').strip()
                if ',' in datos:
                    try:
                        x_ser, y_ser, z_ser = map(float, datos.split(','))
                        x_data.append(x_ser*3)
                        y_data.append(y_ser*10)
                        z_data.append(z_ser*3)
                        tiempo_data.append(tiempo_data[-1] + 0.172)  # Incrementa 50ms (0.05s)
                    except ValueError:
                        print(f"Error de conversión: {datos}")
                else:
                    try:
                        z_ser = float(datos)
                        z_data.append(z_ser)
                        tiempo_data.append(tiempo_data[-1] + 0.05)
                    except ValueError:
                        print(f"Error de conversión: {datos}")  
        except Exception as e:
            print(f"Error en la lectura del puerto serie: {e}")

# Función de actualización de la gráfica
def actualizar_grafica(frame):
    plt.cla()  # Limpia la gráfica antes de actualizar

    plt.plot(tiempo_data, x_data, label="X", color="r")
    plt.plot(tiempo_data, y_data, label="Y", color="g")
    plt.plot(tiempo_data, z_data, label="Z", color="b")

    plt.xlabel("Tiempo (s)")
    plt.ylabel("Valor")
    plt.title("Datos del MPU6050 en Tiempo Real")
    plt.legend()
    plt.grid(True)
    plt.ylim(-3,3)

# Guardar datos en CSV al cerrar la ventana
def guardar_csv():
    with open("datos_mpu6050.csv", "w", newline="") as archivo_csv:
        escritor = csv.writer(archivo_csv)
        escritor.writerow(["Tiempo (s)", "X", "Y", "Z"])  # Encabezado
        for i in range(len(x_data)):
            escritor.writerow([tiempo_data[i], x_data[i], y_data[i], z_data[i]])
    print("Datos guardados en 'datos_mpu6050.csv'")

# Función para cerrar la ventana y guardar el CSV
def cerrar():
    global cerrar_programa
    cerrar_programa = True
    guardar_csv()
    root.destroy()

# Iniciar hilo de lectura
hilo_serial = threading.Thread(target=leer_datos, daemon=True)
hilo_serial.start()

# Crear ventana con Tkinter
root = tk.Tk()
root.protocol("WM_DELETE_WINDOW", cerrar)  # Llama a `cerrar` al cerrar la ventana
root.title("Gráfica en Tiempo Real")
root.geometry("300x100")
tk.Label(root, text="Cerrando la ventana se guardarán los datos.").pack(pady=20)

# Configurar la animación de la gráfica
fig = plt.figure()
ani = animation.FuncAnimation(fig, actualizar_grafica, interval=100)

# Iniciar la interfaz de usuario en un hilo separado
hilo_tk = threading.Thread(target=root.mainloop, daemon=True)
hilo_tk.start()

# Mostrar la gráfica en tiempo real
plt.show()
