import serial
import threading
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import tkinter as tk
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg

# Configura aquí tu puerto
puerto_serie = serial.Serial('COM14', 115200, timeout=1)

# Variables compartidas
x_target, y_target, z_target = 0, 0, 0
cerrar_programa = False
prevx, prevy, prevz = 0, 0, 0

# Longitudes del robot
l1 = 1
l2 = 1
l3 = 1

def forward_kinematics(theta1, theta2, theta3):
    x0, y0, z0 = 0, 0, 0

    x1 = l1 * np.cos(np.radians(theta1))
    y1 = l1 * np.sin(np.radians(theta1))
    z1 = 0

    x2 = x1 + l2 * np.cos(np.radians(theta1)) * np.cos(np.radians(theta2))
    y2 = y1 + l2 * np.sin(np.radians(theta1)) * np.cos(np.radians(theta2))
    z2 = l2 * np.sin(np.radians(theta2))

    x3 = x2 + l3 * np.cos(np.radians(theta1)) * np.cos(np.radians(theta2 + theta3))
    y3 = y2 + l3 * np.sin(np.radians(theta1)) * np.cos(np.radians(theta2 + theta3))
    z3 = z2 + l3 * np.sin(np.radians(theta2 + theta3))

    return [(x0, y0, z0), (x1, y1, z1), (x2, y2, z2), (x3, y3, z3)]

def inverse_kinematics(x, y, z):
    theta1 = np.degrees(np.arctan2(y, x))
    r = np.sqrt(x**2 + y**2)
    d = np.sqrt(r**2 + z**2)

    if d > (l2 + l3):
        return None  # Fuera del alcance

    cos_theta2 = (l2**2 + d**2 - l3**2) / (2 * l2 * d)
    theta2 = np.degrees(np.arctan2(z, r) + np.arccos(cos_theta2))

    cos_theta3 = (l2**2 + l3**2 - d**2) / (2 * l2 * l3)
    theta3 = np.degrees(np.arccos(cos_theta3)) - 180

    return theta1, theta2, theta3

def plot_robot(theta1, theta2, theta3):
    puntos = forward_kinematics(theta1, theta2, theta3)
    x, y, z = zip(*puntos)

    ax.clear()
    ax.quiver(0, 0, 0, 1, 0, 0, color='r')
    ax.quiver(0, 0, 0, 0, 1, 0, color='g')
    ax.quiver(0, 0, 0, 0, 0, 1, color='b')
    ax.plot(x, y, z, 'o-', color='k', linewidth=3)
    ax.set_xlim([-3, 3])
    ax.set_ylim([-3, 3])
    ax.set_zlim([-3, 3])
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')
    ax.set_title('Robot 3D controlado por puerto serial')
    canvas.draw()

def actualizar_robot():
    if (theta := inverse_kinematics(x_target, y_target, z_target)):
        plot_robot(*theta)
    root.after(100, actualizar_robot)  # Repite cada 100 ms

def leer_serial():
    global x_target, y_target, z_target, prevx, prevy, prevz 
    while not cerrar_programa:
        try:
            linea = puerto_serie.readline().decode('utf-8').strip()
            if ',' in linea:
                partes = linea.split(',')
                if len(partes) == 3:
                    x, y, z = map(float, partes)
                    x_target = prevx + (x)/(2)  # Ajusta escala si quieres
                    y_target = prevy + (y)/(2)
                    z_target = prevz + (z)/(2)
                    prevx= x_target
                    prevy= y_target
                    prevz= z_target
        except Exception as e:
            print("Error leyendo puerto:", e)

# Crear interfaz
root = tk.Tk()
root.title("Simulación 3D del Robot")

fig = plt.figure(figsize=(5, 5))
ax = fig.add_subplot(111, projection='3d')
canvas = FigureCanvasTkAgg(fig, master=root)
canvas.get_tk_widget().pack()

# Iniciar hilo de lectura serial
hilo_serial = threading.Thread(target=leer_serial, daemon=True)
hilo_serial.start()

# Actualizar robot periódicamente
actualizar_robot()

# Ejecutar interfaz
root.mainloop()
cerrar_programa = True
puerto_serie.close()
