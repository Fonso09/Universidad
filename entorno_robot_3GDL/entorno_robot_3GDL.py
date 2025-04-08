import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import tkinter as tk
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg

# Cinemática directa

def forward_kinematics(theta1, theta2, theta3, l1=1, l2=1, l3=1):
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

# Cinemática inversa

def inverse_kinematics(x, y, z, l1=1, l2=1, l3=1):
    theta1 = np.degrees(np.arctan2(y, x))
    r = np.sqrt(x**2 + y**2)
    d = np.sqrt(r**2 + z**2)

    if d > (l2 + l3):
        d = l2 + l3  # limitar alcance

    cos_theta2 = (l2**2 + d**2 - l3**2) / (2 * l2 * d)
    cos_theta2 = np.clip(cos_theta2, -1.0, 1.0)
    theta2 = np.degrees(np.arctan2(z, r) + np.arccos(cos_theta2))

    cos_theta3 = (l2**2 + l3**2 - d**2) / (2 * l2 * l3)
    cos_theta3 = np.clip(cos_theta3, -1.0, 1.0)
    theta3 = np.degrees(np.arccos(cos_theta3)) - 180

    return theta1, theta2, theta3

# Visualización

def plot_robot(x, y, z):
    theta1, theta2, theta3 = inverse_kinematics(x, y, z)
    points = forward_kinematics(theta1, theta2, theta3)
    xp, yp, zp = zip(*points)

    ax.clear()

    ax.quiver(0, 0, 0, 1, 0, 0, color='r', linewidth=2)
    ax.quiver(0, 0, 0, 0, 1, 0, color='g', linewidth=2)
    ax.quiver(0, 0, 0, 0, 0, 1, color='b', linewidth=2)

    ax.plot(xp, yp, zp, 'o-', markersize=8, linewidth=3, color='k')

    ax.set_xlim([-3, 3])
    ax.set_ylim([-3, 3])
    ax.set_zlim([-3, 3])
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')
    ax.set_title('Robot de 3 eslabones con control por punto final')

    canvas.draw()

# Actualizar con sliders

def update(val):
    x = x_slider.get()
    y = y_slider.get()
    z = z_slider.get()
    plot_robot(x, y, z)

# Interfaz gráfica

root = tk.Tk()
root.title("Control de punto final del robot 3D")

fig = plt.figure(figsize=(5, 5))
ax = fig.add_subplot(111, projection='3d')
canvas = FigureCanvasTkAgg(fig, master=root)
canvas.get_tk_widget().pack()

x_slider = tk.Scale(root, from_=-2, to=2, resolution=0.1, orient='horizontal', label='X', command=update)
x_slider.pack()
y_slider = tk.Scale(root, from_=-2, to=2, resolution=0.1, orient='horizontal', label='Y', command=update)
y_slider.pack()
z_slider = tk.Scale(root, from_=-2, to=2, resolution=0.1, orient='horizontal', label='Z', command=update)
z_slider.pack()

plot_robot(0, 0, 0)

root.mainloop()
