import pandas as pd
import os

# Ruta completa de los archivos
file_ch1 = r"C:\Users\julio\Downloads\Universidad-main\Universidad-main\LAB1_Señales\Datos_osciloscopio_DAC\100Hz_con_10KHz\F0005CH1.CSV"
file_ch2 = r"C:\Users\julio\Downloads\Universidad-main\Universidad-main\LAB1_Señales\Datos_osciloscopio_DAC\100Hz_con_10KHz\F0005CH2.CSV"

# Verifica si los archivos existen
if not os.path.exists(file_ch1):
    print(f"❌ Archivo no encontrado: {file_ch1}")
if not os.path.exists(file_ch2):
    print(f"❌ Archivo no encontrado: {file_ch2}")

def load_oscilloscope_data(file_path):
    """Función para leer los datos del archivo CSV del osciloscopio"""
    with open(file_path, "r", encoding="utf-8") as file:
        lines = file.readlines()

    # Encontrar la primera línea con datos
    data_start_index = next(i for i, line in enumerate(lines) if line.strip().startswith(",,"))

    # Leer el CSV desde donde comienzan los datos
    df = pd.read_csv(file_path, skiprows=data_start_index, header=None, usecols=[3, 4], names=["Tiempo", "Voltaje"])
    
    return df

# Si los archivos existen, procesamos los datos
if os.path.exists(file_ch1) and os.path.exists(file_ch2):
    df_ch1 = load_oscilloscope_data(file_ch1)
    df_ch2 = load_oscilloscope_data(file_ch2)

    # Combinar los datos en un solo DataFrame
    df = pd.DataFrame({
        "Tiempo": df_ch1["Tiempo"],  # Tiempo es el mismo en ambos canales
        "Voltaje_CH1": df_ch1["Voltaje"],
        "Voltaje_CH2": df_ch2["Voltaje"]
    })

    # Guardar el archivo combinado para MATLAB
    output_file = r"C:\Users\julio\Downloads\datos_osc_matlab.csv"
    df.to_csv(output_file, index=False)

    print(f"✅ Datos de CH1 y CH2 exportados a: {output_file}")
else:
    print("⚠️ No se pudieron procesar los datos porque faltan archivos.")
