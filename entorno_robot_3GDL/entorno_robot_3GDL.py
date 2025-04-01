import serial
import threading
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import tkinter as tk
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg


# Configurar puerto serie
puerto_serie = serial.Serial('COM13', 9600, timeout=1)

# Variables de estado
xpos, ypos, zpos = 0, 0, 0
prevx, prevy, prevz = 0, 0, 0


cerrar_programa = False

def leer_datos():
    global xpos, ypos, zpos, prevx, prevy, prevz 
    while not cerrar_programa:
        try:
            linea = puerto_serie.readline()
            if linea:
                datos = linea.decode('utf-8', errors='ignore').strip()
                if ',' in datos:
                 
                        ax, ay, az = map(float, datos.split(','))  # Aceleraciones
                        ax=ax*5
                        ay=ay*7
                        az=az*5
                        print(ay)
                        #print('Lo que llega en y:', ay)
                        #print('Lo que llega en z:', az)
                        xpos = prevx +(ax*0.0144)/(2)
                        ypos=prevy +(ay*0.0144)/(2)
                        zpos= prevz +(az*0.0144)/(2)
                        #print(xpos)
                        
                        # Obtener ángulos e imprimirlos
                        theta1, theta2, theta3 = inverse_kinematics(xpos, ypos, zpos)
                        plot_robot(theta1, theta2, theta3)
                        prevx= xpos
                        prevy= ypos
                        prevz= zpos



        except Exception as e:
            print(f"Error en la lectura del puerto serie: {e}")

hilo_serial = threading.Thread(target=leer_datos, daemon=True)
hilo_serial.start()

def forward_kinematics(theta1, theta2, theta3, l1=1, l2=1, l3=1):
    """Calcula la posición de los eslabones en 3D mediante cinemática directa."""
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

def inverse_kinematics(x, y, z, l1=1, l2=1, l3=1):
    """Calcula los ángulos de las articulaciones para alcanzar la posición deseada."""
    theta1 = np.degrees(np.arctan2(y, x))
    r = np.sqrt(x**2 + y**2)
    d = np.sqrt(r**2 + z**2)
    
    if d > (l2 + l3):
        raise ValueError("El punto está fuera del alcance del robot.")
    
    cos_theta2 = (l2**2 + d**2 - l3**2) / (2 * l2 * d)
    theta2 = np.degrees(np.arctan2(z, r) + np.arccos(cos_theta2))
    
    cos_theta3 = (l2**2 + l3**2 - d**2) / (2 * l2 * l3)
    theta3 = np.degrees(np.arccos(cos_theta3)) - 180
    
    return theta1, theta2, theta3

def plot_robot(theta1, theta2, theta3):
    """Dibuja el robot en 3D con los ángulos dados."""
    points = forward_kinematics(theta1, theta2, theta3)
    x, y, z = zip(*points)
    
    ax.clear()
    
    # Dibujar el sistema de coordenadas
    ax.quiver(0, 0, 0, 1, 0, 0, color='r', linewidth=2)
    ax.quiver(0, 0, 0, 0, 1, 0, color='g', linewidth=2)
    ax.quiver(0, 0, 0, 0, 0, 1, color='b', linewidth=2)
    
    # Dibujar los eslabones
    ax.plot(x, y, z, 'o-', markersize=8, linewidth=3, color='k')
    
    # Configurar límites y etiquetas
    ax.set_xlim([-3, 3])
    ax.set_ylim([-3, 3])
    ax.set_zlim([-3, 3])
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')
    ax.set_title('Robot de 3 eslabones con 3 GDL')
    
    canvas.draw()

# Crear ventana Tkinter
root = tk.Tk()
root.title("Simulación 3D del Robot")

# Crear figura de Matplotlib
fig = plt.figure(figsize=(5, 5))
ax = fig.add_subplot(111, projection='3d')
canvas = FigureCanvasTkAgg(fig, master=root)
canvas.get_tk_widget().pack()

# Inicializar la gráfica
plot_robot(0, 0, 0)

# Ejecutar la ventana Tkinter
root.mainloop()
