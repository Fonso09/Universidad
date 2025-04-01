import serial
import time

# Configuración del puerto serie (ajustar según tu configuración)
puerto_serie = serial.Serial('COM5', 1000000, timeout=1)  # Usa la velocidad de baudios adecuada

# Variables para almacenar el tiempo de los mensajes
ultimo_tiempo = None

# Función para leer y calcular el intervalo de tiempo
def leer_datos():
    global ultimo_tiempo
    while True:
        try:
            # Lee una línea de datos
            linea = puerto_serie.readline()
            if linea:
                # Convierte la línea a string y elimina saltos de línea
                datos = linea.decode('utf-8', errors='ignore').strip()
                
                # Captura el tiempo actual
                tiempo_actual = time.time()

                if ultimo_tiempo is not None:
                    # Calcula el intervalo de tiempo entre el último y el actual
                    intervalo = tiempo_actual - ultimo_tiempo
                    print(f"Intervalo de tiempo: {intervalo:.5f} segundos")
                else:
                    print("Primer mensaje recibido.")
                
                # Actualiza el último tiempo
                ultimo_tiempo = tiempo_actual
        except Exception as e:
            print(f"Error en la lectura del puerto serie: {e}")

# Ejecutar la función para leer los datos
leer_datos()
