import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import tkinter as tk
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg

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

def update(val):
    theta1 = slider1.get()
    theta2 = slider2.get()
    theta3 = slider3.get()
    plot_robot(theta1, theta2, theta3)

# Crear ventana Tkinter
root = tk.Tk()
root.title("Simulación 3D del Robot")

# Crear figura de Matplotlib
fig = plt.figure(figsize=(5, 5))
ax = fig.add_subplot(111, projection='3d')
canvas = FigureCanvasTkAgg(fig, master=root)
canvas.get_tk_widget().pack()

# Crear sliders
slider1 = tk.Scale(root, from_=-180, to=180, orient='horizontal', label='Theta1', command=update)
slider1.pack()
slider2 = tk.Scale(root, from_=-180, to=180, orient='horizontal', label='Theta2', command=update)
slider2.pack()
slider3 = tk.Scale(root, from_=-180, to=180, orient='horizontal', label='Theta3', command=update)
slider3.pack()

# Inicializar la gráfica
plot_robot(0, 0, 0)

# Ejecutar la ventana Tkinter
root.mainloop()
