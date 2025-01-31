import serial
import threading
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import collections
import pandas as pd
import time

# Configurar puerto serial
ser = serial.Serial('COM13', 115200, timeout=1)

# Tamaño del buffer
buffer_size = 6000  # Almacenamos hasta 6000 datos
display_size = 1000  # Solo mostramos los últimos 1000

# Buffers para almacenar datos
data_buffer = collections.deque([0] * buffer_size, maxlen=buffer_size)
lock = threading.Lock()
running = True  # Bandera para detener el hilo

def read_serial():
    """Función para leer datos del puerto serial en un hilo separado."""
    global running
    while running:
        try:
            line = ser.readline()
            line = line.decode(errors='ignore').strip()  # Ignorar bytes no decodificables
            if line:
                voltage = int(line)
                with lock:
                    data_buffer.append(voltage)
        except ValueError:
            pass  # Ignorar líneas con datos no convertibles
        except Exception as e:
            print(f"Error en la lectura serial: {e}")

def save_data():
    """Guardar los datos en un archivo CSV para MATLAB."""
    with lock:
        datos = list(data_buffer)
    df = pd.DataFrame({"Muestra": range(len(datos)), "ADC": datos})
    df.to_csv("datos_adc.csv", index=False, sep=";")  # Guardar CSV con separador ";"
    print("Datos guardados en 'datos_adc.csv'.")

# Iniciar hilo de lectura serial
serial_thread = threading.Thread(target=read_serial, daemon=True)
serial_thread.start()

# Configurar gráfico
fig, ax = plt.subplots()
ax.set_ylim(0, 4095)  # Limitar valores de voltaje del ADC
ax.set_xlabel("N muestra")  # Etiqueta en X
ax.set_ylabel("ADC")        # Etiqueta en Y

x_data = list(range(display_size))
y_data = list(data_buffer)[-display_size:]  # Solo los últimos 1000 valores
line, = ax.plot(x_data, y_data, 'r-', animated=True)

def update(frame):
    with lock:
        y_data = list(data_buffer)[-display_size:]  # Últimos 1000 datos
        line.set_ydata(y_data)
    return line,

# Evita advertencia de FuncAnimation agregando save_count
ani = animation.FuncAnimation(fig, update, interval=1, blit=True, save_count=1000)

def close_program(event):
    """Función para cerrar correctamente el programa."""
    global running
    running = False  # Detener hilo
    time.sleep(0.5)  # Esperar a que termine la lectura
    save_data()  # Guardar datos en CSV
    ser.close()  # Cerrar puerto serial
    print("Programa cerrado.")
    plt.close(fig)  # Cerrar gráfica

# Agregar botón de stop
fig.canvas.mpl_connect('close_event', close_program)
plt.show()
