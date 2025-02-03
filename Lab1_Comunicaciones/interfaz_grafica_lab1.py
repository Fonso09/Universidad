import serial
import threading
import tkinter as tk
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import collections
import time

# Configurar puerto serial
PORT = "COM8"  # Cambia según tu sistema
BAUDRATE = 115200
ser = serial.Serial(PORT, BAUDRATE, timeout=1)

# Buffer de datos
buffer_size = 1000
data_buffer = collections.deque([0] * buffer_size, maxlen=buffer_size)
lock = threading.Lock()
running = True  # Bandera para el hilo

def read_serial():
    """Lee datos del puerto serial en un hilo separado."""
    global running
    while running:
        try:
            line = ser.readline().decode(errors='ignore').strip()
            if line:
                with lock:
                    data_buffer.append(int(line))
        except ValueError:
            pass

def send_data(data):
    """Envía un carácter por el puerto serial."""
    ser.write(data.encode())
    print(f"Enviado: {data}")
    time.sleep(0.5)  # Esperar antes de abrir la gráfica
    open_graph()

def open_graph():
    """Abre una ventana con la gráfica en tiempo real."""
    fig, ax = plt.subplots()
    ax.set_ylim(0, 4095)
    ax.set_xlabel("Tiempo")
    ax.set_ylabel("Sensor")
    
    x_data = list(range(buffer_size))
    y_data = list(data_buffer)
    line, = ax.plot(x_data, y_data, 'r-')
    
    def update(frame):
        with lock:
            y_data = list(data_buffer)
            line.set_ydata(y_data)
        return line,
    
    ani = animation.FuncAnimation(fig, update, interval=50, blit=True)
    plt.show()

def close_program():
    """Cierra correctamente el programa."""
    global running
    running = False
    ser.close()
    root.destroy()

# Iniciar hilo de lectura serial
serial_thread = threading.Thread(target=read_serial, daemon=True)
serial_thread.start()

# Configurar interfaz gráfica
root = tk.Tk()
root.title("Control Serial")

buttons = {
    "I2C": "A",
    "SPI": "B",
    "BT": "C",
    "Serial": "D"
}

for text, cmd in buttons.items():
    btn = tk.Button(root, text=text, command=lambda c=cmd: send_data(c), height=2, width=10)
    btn.pack(pady=5)

tk.Button(root, text="Salir", command=close_program, height=2, width=10).pack(pady=5)

root.mainloop()
