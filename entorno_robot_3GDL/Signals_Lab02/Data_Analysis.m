close all
% Parámetros del filtro
Fs = 40;  % Frecuencia de muestreo en Hz
Fc = 0.6; % Frecuencia de corte en Hz
N = 10;    % Orden del filtro

% Cargar datos
filename = 'datos_mpu6050.csv'; 
data = readtable(filename);

% Extraer datos y normalizar el tiempo a 0
Tiempo = data{:,1};
Tiempo = Tiempo - Tiempo(1); % Hacer que el tiempo empiece desde 0
X = data{:,2}; 
Y = data{:,3}; 
Z = data{:,4}; 

% Normalizar frecuencia de corte (Nyquist = Fs/2)
Wn = Fc / (Fs / 2);

% Diseñar filtro Butterworth digital
[b, a] = butter(N, Wn, 'low');

% Filtrar señales X, Y, Z
X_filtrado = filtfilt(b, a, X);
Y_filtrado = filtfilt(b, a, Y);
Z_filtrado = filtfilt(b, a, Z);

% Graficar señales originales y filtradas
figure;
subplot(3,1,1);
plot(Tiempo, X, 'r--', 'DisplayName', 'X Original');
hold on;
plot(Tiempo, X_filtrado, 'r', 'DisplayName', 'X Filtrado');
xlabel('Tiempo (s)');
ylabel('X');
legend;
grid on;
title('Señal X - Original vs Filtrada');

subplot(3,1,2);
plot(Tiempo, Y, 'g--', 'DisplayName', 'Y Original');
hold on;
plot(Tiempo, Y_filtrado, 'g', 'DisplayName', 'Y Filtrado');
xlabel('Tiempo (s)');
ylabel('Y');
legend;
grid on;
title('Señal Y - Original vs Filtrada');

subplot(3,1,3);
plot(Tiempo, Z, 'b--', 'DisplayName', 'Z Original');
hold on;
plot(Tiempo, Z_filtrado, 'b', 'DisplayName', 'Z Filtrado');
xlabel('Tiempo (s)');
ylabel('Z');
legend;
grid on;
title('Señal Z - Original vs Filtrada');
hold off;

%%%%%%Sobremuestreo

% Coeficientes del ajuste de Fourier
a0 = -0.7863; a1 = -0.0039; b1 = 0.0026; a2 = -0.0042; b2 = 0.0141;
a3 = 0.0063; b3 = 0.0013; a4 = -0.0019; b4 = -0.0062; a5 = 0.0108;
b5 = -0.0009; a6 = 0.0096; b6 = -0.0041; a7 = 0.0038; b7 = 0.0086;
a8 = -0.0075; b8 = -0.0075; w = 5.5051;  % Frecuencia angular

% Intervalo de tiempo basado en los datos originales (ajustar según sea necesario)
t_start = 0;  % Tiempo inicial
t_end = 34.228;  % Tiempo final
fs = 20;  % Frecuencia de muestreo en Hz
t = linspace(t_start, t_end, fs*(t_end - t_start));  % Crear el tiempo ajustado

% Generar la señal ajustada utilizando los coeficientes de Fourier
signal = a0 + a1*cos(w*t) + b1*sin(w*t) + ...
    a2*cos(2*w*t) + b2*sin(2*w*t) + ...
    a3*cos(3*w*t) + b3*sin(3*w*t) + ...
    a4*cos(4*w*t) + b4*sin(4*w*t) + ...
    a5*cos(5*w*t) + b5*sin(5*w*t) + ...
    a6*cos(6*w*t) + b6*sin(6*w*t) + ...
    a7*cos(7*w*t) + b7*sin(7*w*t) + ...
    a8*cos(8*w*t) + b8*sin(8*w*t);

% Graficar la señal ajustada
figure;
plot(t, signal, 'b', 'LineWidth', 1.5);
title('Señal Ajustada con Coeficientes de Fourier (8 términos)');
xlabel('Tiempo (s)');
ylabel('Amplitud');
grid on;

%%%Filtrado señal sobremuestreada

% Parámetros
f_s = 20;  % Frecuencia de muestreo en Hz
f_0 = 0.876;  % Frecuencia fundamental en Hz
Q = 3;  % Factor de calidad (mayor Q = notch más estrecho)

% Normalización de las frecuencias
harmonics = 1:10;  % Primeros 10 armónicos
frequencies = f_0 * harmonics;  % f_0, 2f_0, 3f_0, ..., 10f_0
w = frequencies / (f_s / 2);  % Frecuencias normalizadas

% Crear los filtros notch para cada frecuencia armónica
b = cell(1, 10);  % Coeficientes de los filtros
a = cell(1, 10);  % Coeficientes de los filtros

% Generar y aplicar cada filtro notch
filtered_signal = signal;  % Inicializamos con la señal original
for i = 1:10
    [b{i}, a{i}] = iirnotch(w(i), w(i) / Q);
    filtered_signal = filter(b{i}, a{i}, filtered_signal);
end

% Graficar las señales
t = (0:length(signal)-1) / f_s;  % Tiempo en segundos
subplot(2,1,1);
plot(t, signal, 'b', 'DisplayName', 'Original');
hold on;
plot(t, filtered_signal, 'r', 'DisplayName', 'Filtrada');
title('Señal Original vs Filtrada');
xlabel('Tiempo (s)');
ylabel('Amplitud');
legend;

% Análisis en frecuencia
N = length(signal);
frequencies_axis = (0:N-1) * (f_s / N);
original_spectrum = abs(fft(signal) / N);
filtered_spectrum = abs(fft(filtered_signal) / N);

subplot(2,1,2);
plot(frequencies_axis, 20*log10(original_spectrum), 'b', 'DisplayName', 'Original');
hold on;
plot(frequencies_axis, 20*log10(filtered_spectrum), 'r', 'DisplayName', 'Filtrada');
title('Espectro de Potencia');
xlabel('Frecuencia (Hz)');
ylabel('Magnitud (dB)');
legend;
grid on;





%%% Filtrado Serial
% Parámetros del filtro
Fs = 5.81;  % Frecuencia de muestreo en Hz (corrige esto si es necesario)
f_0 = 0.87;  % Frecuencia fundamental en Hz
Q = 3;  % Factor de calidad (mayor Q = notch más estrecho)

% Cargar datos
filename = 'datos_mpu6050.csv'; 
data = readtable(filename);

% Extraer datos
Tiempo = data{:,1};  % Tiempos en segundos
X = data{:,2}; 
Y = data{:,3}; 
Z = data{:,4}; 

% Asegurar que el tiempo comience en cero
Tiempo = Tiempo - Tiempo(1);

% Calcular los armónicos de la frecuencia fundamental
harmonics = 1:5;  % Primeros 5 armónicos
frequencies = f_0 * harmonics;  % Frecuencias: f_0, 2f_0, 3f_0, ...

% Normalizar las frecuencias a la frecuencia de Nyquist (Fs / 2)
w = frequencies / (Fs / 2);  % Frecuencias normalizadas

% Verificar que las frecuencias normalizadas estén en el rango permitido
w(w >= 1) = 0.99; % Limitar valores fuera del rango

% Crear los filtros notch para cada armónico
b = cell(1, length(harmonics));
a = cell(1, length(harmonics));

for i = 1:length(harmonics)
    [b{i}, a{i}] = iirnotch(w(i), w(i) / Q);  % Filtro notch para cada armónico
end

% Filtrar las señales X, Y, Z
X_ntch_filt = X;
Y_ntch_filt = Y;
Z_ntch_filt = Z;

for i = 1:length(harmonics)
    X_ntch_filt = filter(b{i}, a{i}, X_ntch_filt);  
    Y_ntch_filt = filter(b{i}, a{i}, Y_ntch_filt);  
    Z_ntch_filt = filter(b{i}, a{i}, Z_ntch_filt);  
end

% Graficar señales originales y filtradas
figure;
subplot(3,1,1);
plot(Tiempo, X, 'r--', 'DisplayName', 'X Original');
hold on;
plot(Tiempo, X_ntch_filt, 'r', 'DisplayName', 'X Filtrado');
xlabel('Tiempo (s)');
ylabel('X');
legend;
grid on;
title('Señal X - Original vs Filtrada');

subplot(3,1,2);
plot(Tiempo, Y, 'g--', 'DisplayName', 'Y Original');
hold on;
plot(Tiempo, Y_ntch_filt, 'g', 'DisplayName', 'Y Filtrado');
xlabel('Tiempo (s)');
ylabel('Y');
legend;
grid on;
title('Señal Y - Original vs Filtrada');

subplot(3,1,3);
plot(Tiempo, Z, 'b--', 'DisplayName', 'Z Original');
hold on;
plot(Tiempo, Z_ntch_filt, 'b', 'DisplayName', 'Z Filtrado');
xlabel('Tiempo (s)');
ylabel('Z');
legend;
grid on;
title('Señal Z - Original vs Filtrada');
hold off;
