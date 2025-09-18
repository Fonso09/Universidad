function robotControlGUI()
    % ===============================================
    % CONFIGURACIÓN DE COMUNICACIÓN SERIAL (AJUSTABLES)
    % ===============================================
    % Configuración del puerto serial
    SERIAL_CONFIG = struct();
    SERIAL_CONFIG.PORT = 'COM4';           % Cambia por tu puerto (COM1, COM3, etc. en Windows; /dev/ttyUSB0 en Linux)
    SERIAL_CONFIG.BAUD_RATE = 128000;        % Velocidad de transmisión
    SERIAL_CONFIG.SEPARATOR = ',';         % Separador entre ángulos
    SERIAL_CONFIG.TERMINATOR = 'F';       % Terminador de línea (LF = \n)
    SERIAL_CONFIG.ENABLED = true;          % Habilitar/deshabilitar comunicación serial
    SERIAL_CONFIG.DEBUG_MODE = false;       % Mostrar datos en consola aunque no haya conexión
    SERIAL_CONFIG.AUTO_CONNECT = true;    % No conectar automáticamente al inicio
    
    % Variable para el objeto serial
    serialObj = [];
    
    % Variables para almacenar datos de potenciómetros
    potValues = zeros(5,1);  % Valores de los potenciómetros
    lastPotUpdate = datetime('now'); % Última actualización
    % ===============================================
    
    % ===============================================
    % CONFIGURACIÓN DE OFFSETS PARA CADA ARTICULACIÓN (AJUSTABLES)
    % ===============================================
    % Offsets en grados para cada articulación (solo afectan etiquetas y envío serial)
    JOINT_OFFSETS = [0, 90, 0, 0, 0]; % Offset en grados para cada joint [J1, J2, J3, J4, J5]
    % ===============================================
    
    % ===============================================
    % CONFIGURACIÓN DE LÍMITES DE LA GRÁFICA (AJUSTABLES)
    % ===============================================
    % Límites del espacio de trabajo del robot
    PLOT_LIMITS.X_MIN = -2.5;
    PLOT_LIMITS.X_MAX = 3.5;
    PLOT_LIMITS.Y_MIN = -2.5;
    PLOT_LIMITS.Y_MAX = 3.5;
    PLOT_LIMITS.Z_MIN = 0;
    PLOT_LIMITS.Z_MAX = 4.0;
    
    % Espaciado de la grilla (para las etiquetas de ángulos)
    PLOT_LIMITS.GRID_SPACING = 0.5;
    % ===============================================
    
    % ===============================================
    % CONFIGURACIÓN DE LÍMITES DE ARTICULACIONES (AJUSTABLES)
    % ===============================================
    % Límites angulares para cada articulación (en radianes)
    JOINT_LIMITS = struct();
    
    % Joint 1: Rotación base (0° a 165°)
    JOINT_LIMITS.MIN(1) = 0;            % 0°
    JOINT_LIMITS.MAX(1) = (165/180)*pi;           % 165°
    
    % Joint 2: Hombro (0° a 180°)
    JOINT_LIMITS.MIN(2) = -pi/2;            % 0°
    JOINT_LIMITS.MAX(2) = pi/2;           % 180°
    
    % Joint 3: Codo (0° a 180°)
    JOINT_LIMITS.MIN(3) = 0;            % 0°
    JOINT_LIMITS.MAX(3) = deg2rad(125);           % 180°
    
    % Joint 4: Muñeca pitch (0° a 180°)
    JOINT_LIMITS.MIN(4) = 0;            % 0°
    JOINT_LIMITS.MAX(4) = deg2rad(150);           % 180°
    
    % Joint 5: Muñeca roll (0° a 180°)
    JOINT_LIMITS.MIN(5) = 0;            % 0°
    JOINT_LIMITS.MAX(5) = pi;           % 180°
    % ===============================================
    
    % ===============================================
    % PARÁMETROS DH DEL ROBOT CORREGIDOS (AJUSTADOS SEGÚN ESPECIFICACIONES)
    % ===============================================
    % Parámetros Denavit-Hartenberg [a, alpha, d, theta_offset]
    % MODIFICADO: Actualizadas las distancias según especificaciones reales
    DH_PARAMS = [
        0,     pi/2,   0.05,   0;      % Joint 1: d = 11.5cm = 0.115m (S0 a S1)
        0.045, 0,      0,       -pi/2;  % Joint 2: a = 12.5cm = 0.125m (S1 a S2)  
        0.125, 0,      0,       0;      % Joint 3: a = 12.5cm = 0.125m (S2 a S3)
        0,     pi/2,   0.125,   0;      % Joint 4: d = 13.5cm = 0.135m (S3 a S4)
        0,     0,      0.13,     0       % Joint 5: d = 10cm = 0.1m (estimado para end effector)
    ];
    % ===============================================
    
    % Cargar el robot
    robot = importrobot('Brazzzo.urdf');
    
    % Configurar el formato de datos para usar vectores en lugar de estructuras
    robot.DataFormat = 'column';
    
    % Crear la figura principal - EXPANDIDA para incluir panel MTH
    fig = figure('Name', 'Control de Brazo Robótico con MTH y Euler', 'NumberTitle', 'off', ...
                 'Position', [50, 50, 2000, 800], 'Resize', 'on');
    
    % Reorganizar paneles con nuevo layout
    % Panel de controles (izquierda)
    controlPanel = uipanel('Parent', fig, 'Position', [0, 0, 0.2, 1], ...
                          'Title', 'Control de Articulaciones', 'FontSize', 12);
    
    % Panel del robot (centro-izquierda)
    robotPanel = uipanel('Parent', fig, 'Position', [0.2, 0, 0.3, 1], ...
                        'Title', 'Visualización del Robot', 'FontSize', 12);
    
    % Panel de potenciómetros (centro-derecha)
    potPanel = uipanel('Parent', fig, 'Position', [0.5, 0, 0.2, 1], ...
                      'Title', 'Lectura de Potenciómetros', 'FontSize', 12, ...
                      'BackgroundColor', [0.95, 0.95, 1]);
    
    % Panel de MTH y Euler (derecha)
    mthPanel = uipanel('Parent', fig, 'Position', [0.7, 0, 0.3, 1], ...
                      'Title', 'Matrices MTH y Ángulos de Euler', 'FontSize', 12, ...
                      'BackgroundColor', [1, 0.98, 0.9]);
    
    % Crear los ejes para la visualización del robot
    ax = axes('Parent', robotPanel, 'Position', [0.1, 0.1, 0.85, 0.8]);
    
    % Configurar la visualización inicial
    show(robot, 'Parent', ax, 'Visuals', 'on');
    
    % Aplicar los límites configurados
    setPlotLimits();
    
    % Configurar la vista y grilla
    axis(ax, 'equal');
    view(ax, 3);
    grid(ax, 'on');
    
    % Variables para almacenar los valores de las articulaciones
    jointValues = zeros(5,1); % Vector columna para los valores de las 5 articulaciones
    
    % Variables para almacenar MTH
    currentMTH = cell(6,1); % MTH desde SC{0} a SC{5}
    currentEuler = zeros(6,3); % Ángulos de Euler para cada MTH
    
    % ===============================================
    % CREACIÓN DE CONTROLES ORIGINALES
    % ===============================================
    
    % Crear sliders y etiquetas para cada articulación
    sliders = gobjects(5, 1);
    labels = gobjects(5, 1);
    valueLabels = gobjects(5, 1);
    angleDegreeLabels = gobjects(5, 1);
    
    % Configuración para cada slider
    sliderHeight = 0.12;
    sliderSpacing = 0.15;
    startY = 0.85;
    
    for i = 1:5
        % Posición Y para cada slider
        yPos = startY - (i-1) * sliderSpacing;
        
        % Etiqueta del joint con información de límites
        labels(i) = uicontrol('Parent', controlPanel, 'Style', 'text', ...
                             'Position', [10, yPos*800-10, 170, 25], ...
                             'String', sprintf('Joint %d: [%.0f° to %.0f°]', i, ...
                                             rad2deg(JOINT_LIMITS.MIN(i))+JOINT_OFFSETS(i), ...
                                             rad2deg(JOINT_LIMITS.MAX(i))+JOINT_OFFSETS(i)), ...
                             'FontSize', 9, 'FontWeight', 'bold', ...
                             'HorizontalAlignment', 'left');
        
        % Slider con actualización continua y límites personalizados
        sliders(i) = uicontrol('Parent', controlPanel, 'Style', 'slider', ...
                              'Position', [10, yPos*800-40, 200, 20], ...
                              'Min', JOINT_LIMITS.MIN(i), 'Max', JOINT_LIMITS.MAX(i), 'Value', 0, ...
                              'SliderStep', [0.01, 0.1]);
        
        % Configurar el evento de actualización continua
        addlistener(sliders(i), 'ContinuousValueChange', ...
                   @(src, evt) updateRobot(i, src.Value));
        
        % También mantener el callback normal para asegurar la actualización final
        set(sliders(i), 'Callback', @(src, evt) updateRobot(i, src.Value));
        
        % Etiqueta del valor en radianes
        valueLabels(i) = uicontrol('Parent', controlPanel, 'Style', 'text', ...
                                  'Position', [220, yPos*800-45, 80, 15], ...
                                  'String', '0.00 rad', ...
                                  'FontSize', 9, ...
                                  'HorizontalAlignment', 'left');
        
        % Etiqueta del valor en grados
        angleDegreeLabels(i) = uicontrol('Parent', controlPanel, 'Style', 'text', ...
                                        'Position', [220, yPos*800-30, 80, 15], ...
                                        'String', '0.0°', ...
                                        'FontSize', 9, ...
                                        'HorizontalAlignment', 'left', ...
                                        'ForegroundColor', [0.2, 0.2, 0.8]);
    end
    
    % Botones de control
    uicontrol('Parent', controlPanel, 'Style', 'pushbutton', ...
              'Position', [10, 70, 60, 25], ...
              'String', 'Reset', 'FontSize', 9, ...
              'Callback', @resetJoints);
    
    uicontrol('Parent', controlPanel, 'Style', 'pushbutton', ...
              'Position', [80, 70, 60, 25], ...
              'String', 'Random', 'FontSize', 9, ...
              'Callback', @randomConfig);
    
    uicontrol('Parent', controlPanel, 'Style', 'pushbutton', ...
              'Position', [150, 70, 70, 25], ...
              'String', 'Vista Home', 'FontSize', 8, ...
              'Callback', @resetView);
    
    % Botón para conectar/desconectar serial
    serialButton = uicontrol('Parent', controlPanel, 'Style', 'pushbutton', ...
              'Position', [10, 40, 100, 25], ...
              'String', 'Conectar Serial', 'FontSize', 9, ...
              'Callback', @toggleSerial);
    
    % Indicador de estado serial
    serialStatus = uicontrol('Parent', controlPanel, 'Style', 'text', ...
              'Position', [120, 40, 120, 25], ...
              'String', 'Serial: Desconectado', ...
              'FontSize', 9, ...
              'HorizontalAlignment', 'left', ...
              'ForegroundColor', [0.8, 0.2, 0.2]);
    
    % ===============================================
    % CREACIÓN DEL PANEL DE POTENCIÓMETROS (ORIGINAL)
    % ===============================================
    
    % Etiquetas para los potenciómetros
    potLabels = gobjects(5, 1);
    potValueLabels = gobjects(5, 1);
    
    % Título del panel de potenciómetros
    uicontrol('Parent', potPanel, 'Style', 'text', ...
              'Position', [20, 750, 320, 25], ...
              'String', 'Valores de Potenciómetros', ...
              'FontSize', 12, 'FontWeight', 'bold', ...
              'HorizontalAlignment', 'center', ...
              'BackgroundColor', [0.95, 0.95, 1]);
    
    % Crear etiquetas para cada potenciómetro
    for i = 1:5
        yPos = 650 - (i-1) * 90;
        
        % Etiqueta del potenciómetro
        potLabels(i) = uicontrol('Parent', potPanel, 'Style', 'text', ...
                                'Position', [20, yPos, 150, 25], ...
                                'String', sprintf('Potenciómetro %d:', i), ...
                                'FontSize', 11, 'FontWeight', 'bold', ...
                                'HorizontalAlignment', 'left', ...
                                'BackgroundColor', [0.95, 0.95, 1]);
        
        % Valor del potenciómetro
        potValueLabels(i) = uicontrol('Parent', potPanel, 'Style', 'text', ...
                                     'Position', [20, yPos-40, 320, 36], ...
                                     'String', 'Sin datos - Esperando...', ...
                                     'FontSize', 14, 'FontWeight', 'bold', ...
                                     'HorizontalAlignment', 'center', ...
                                     'BackgroundColor', [1, 1, 1], ...
                                     'ForegroundColor', [0.5, 0.5, 0.5]);
    end
    
    % Indicador de estado de recepción
    receptionStatus = uicontrol('Parent', potPanel, 'Style', 'text', ...
                               'Position', [20, 80, 320, 20], ...
                               'String', 'Estado: Sin datos recibidos', ...
                               'FontSize', 10, ...
                               'HorizontalAlignment', 'center', ...
                               'BackgroundColor', [0.95, 0.95, 1], ...
                               'ForegroundColor', [0.8, 0.4, 0]);
    
    % ===============================================
    % NUEVO: CREACIÓN DEL PANEL DE MTH Y EULER
    % ===============================================
    
    % Título principal del panel MTH
    uicontrol('Parent', mthPanel, 'Style', 'text', ...
              'Position', [10, 760, 580, 30], ...
              'String', 'Matrices de Transformación Homogénea y Ángulos de Euler', ...
              'FontSize', 13, 'FontWeight', 'bold', ...
              'HorizontalAlignment', 'center', ...
              'BackgroundColor', [1, 0.98, 0.9], ...
              'ForegroundColor', [0.2, 0.2, 0.8]);
    
    % Crear pestañas para cada MTH
    mthTabs = gobjects(6, 1);
    mthDisplays = gobjects(6, 1);
    eulerDisplays = gobjects(6, 1);
    positionDisplays = gobjects(6, 1);
    
    % Selector de MTH (combo box)
    uicontrol('Parent', mthPanel, 'Style', 'text', ...
              'Position', [10, 720, 100, 20], ...
              'String', 'Seleccionar MTH:', ...
              'FontSize', 10, 'FontWeight', 'bold', ...
              'HorizontalAlignment', 'left', ...
              'BackgroundColor', [1, 0.98, 0.9]);
    
    mthSelector = uicontrol('Parent', mthPanel, 'Style', 'popup', ...
                           'Position', [120, 720, 150, 25], ...
                           'String', {'SC{0}→SC{1}', 'SC{0}→SC{2}', 'SC{0}→SC{3}', ...
                                     'SC{0}→SC{4}', 'SC{0}→SC{5}', 'SC{0}→EF (Final)'}, ...
                           'FontSize', 10, ...
                           'Value', 6, ... % Por defecto mostrar la MTH final
                           'Callback', @updateMTHDisplay);
    
    % Botón para actualizar todas las MTH
    uicontrol('Parent', mthPanel, 'Style', 'pushbutton', ...
              'Position', [290, 720, 120, 25], ...
              'String', 'Actualizar MTH', ...
              'FontSize', 9, ...
              'Callback', @calculateAllMTH);
    
    % Área de visualización de la matriz MTH
    uicontrol('Parent', mthPanel, 'Style', 'text', ...
              'Position', [10, 680, 580, 20], ...
              'String', 'Matriz de Transformación Homogénea:', ...
              'FontSize', 11, 'FontWeight', 'bold', ...
              'HorizontalAlignment', 'left', ...
              'BackgroundColor', [1, 0.98, 0.9]);
    
    % Display de la matriz MTH (usando listbox para formato monospace)
    mthDisplay = uicontrol('Parent', mthPanel, 'Style', 'listbox', ...
                          'Position', [10, 520, 580, 150], ...
                          'String', {'Esperando cálculo de MTH...'}, ...
                          'FontSize', 9, ...
                          'FontName', 'Courier New', ...
                          'BackgroundColor', [0.95, 1, 0.95]);
    
    % Área de información de posición y orientación
    uicontrol('Parent', mthPanel, 'Style', 'text', ...
              'Position', [10, 490, 280, 20], ...
              'String', 'Posición (x, y, z):', ...
              'FontSize', 10, 'FontWeight', 'bold', ...
              'HorizontalAlignment', 'left', ...
              'BackgroundColor', [1, 0.98, 0.9]);
    
    positionDisplay = uicontrol('Parent', mthPanel, 'Style', 'text', ...
                               'Position', [10, 460, 280, 25], ...
                               'String', 'X: 0.000  Y: 0.000  Z: 0.000', ...
                               'FontSize', 10, ...
                               'HorizontalAlignment', 'center', ...
                               'BackgroundColor', [0.9, 1, 0.9], ...
                               'ForegroundColor', [0.2, 0.6, 0.2]);
    
    % Área de ángulos de Euler
    uicontrol('Parent', mthPanel, 'Style', 'text', ...
              'Position', [310, 490, 280, 20], ...
              'String', 'Ángulos de Euler (ZYZ):', ...
              'FontSize', 10, 'FontWeight', 'bold', ...
              'HorizontalAlignment', 'left', ...
              'BackgroundColor', [1, 0.98, 0.9]);
    
    eulerDisplay = uicontrol('Parent', mthPanel, 'Style', 'text', ...
                            'Position', [310, 460, 280, 25], ...
                            'String', 'φ: 0.0°  θ: 0.0°  ψ: 0.0°', ...
                            'FontSize', 10, ...
                            'HorizontalAlignment', 'center', ...
                            'BackgroundColor', [0.9, 0.9, 1], ...
                            'ForegroundColor', [0.2, 0.2, 0.8]);
    
    % Tabla resumen de todas las MTH
    uicontrol('Parent', mthPanel, 'Style', 'text', ...
              'Position', [10, 430, 580, 20], ...
              'String', 'Resumen de Todas las Transformaciones:', ...
              'FontSize', 11, 'FontWeight', 'bold', ...
              'HorizontalAlignment', 'left', ...
              'BackgroundColor', [1, 0.98, 0.9]);
    
    % Tabla con información resumida
    summaryTable = uitable('Parent', mthPanel, ...
                          'Position', [10, 20, 580, 400], ...
                          'ColumnName', {'MTH', 'X (m)', 'Y (m)', 'Z (m)', 'Roll (°)', 'Pitch (°)', 'Yaw (°)'}, ...
                          'ColumnWidth', {80, 70, 70, 70, 75, 75, 75}, ...
                          'Data', cell(6, 7), ...
                          'RowName', [], ...
                          'FontSize', 9);
    
    % ===============================================
    % FUNCIONES DE CÁLCULO DE MTH Y EULER
    % ===============================================
    
    % Función para calcular la MTH usando parámetros DH
    function T = calculateDH_MTH(a, alpha, d, theta)
        % Matriz de transformación homogénea usando parámetros DH
        T = [cos(theta), -sin(theta)*cos(alpha),  sin(theta)*sin(alpha), a*cos(theta);
             sin(theta),  cos(theta)*cos(alpha), -cos(theta)*sin(alpha), a*sin(theta);
             0,           sin(alpha),             cos(alpha),            d;
             0,           0,                      0,                     1];
    end
    
    % Función para calcular todas las MTH del robot
    function calculateAllMTH(~, ~)
        try
            % Matriz identidad inicial
            T_cumulative = eye(4);
            
            % Calcular MTH para cada joint
            for i = 1:5
                % Obtener parámetros DH para el joint i
                a = DH_PARAMS(i, 1);
                alpha = DH_PARAMS(i, 2);
                d = DH_PARAMS(i, 3);
                theta_offset = DH_PARAMS(i, 4);
                
                % Ángulo actual del joint
                theta = jointValues(i) + theta_offset;
                
                % Calcular MTH individual para este joint
                T_individual = calculateDH_MTH(a, alpha, d, theta);
                
                % Acumular transformación
                T_cumulative = T_cumulative * T_individual;
                
                % Almacenar la MTH acumulativa
                currentMTH{i} = T_cumulative;
                
                % Calcular ángulos de Euler (ZYZ convention)
                currentEuler(i, :) = rotm2eul(T_cumulative(1:3, 1:3), 'ZYZ');
            end
            
            % MTH final (igual a la del joint 5)
            currentMTH{6} = currentMTH{5};
            currentEuler(6, :) = currentEuler(5, :);
            
            % Actualizar displays
            updateMTHDisplay();
            updateSummaryTable();
            
        catch ME
            fprintf('Error calculando MTH: %s\n', ME.message);
        end
    end
    
    % Función para actualizar el display de MTH seleccionada
    function updateMTHDisplay(~, ~)
        try
            selectedIndex = get(mthSelector, 'Value');
            
            if selectedIndex <= length(currentMTH) && ~isempty(currentMTH{selectedIndex})
                T = currentMTH{selectedIndex};
                
                % Formatear la matriz para display
                mthStrings = cell(6, 1);
                mthStrings{1} = sprintf('┌                                           ┐');
                
                for row = 1:4
                    if row <= 3
                        mthStrings{row+1} = sprintf('│ %8.4f %8.4f %8.4f %8.4f │', ...
                                                   T(row, 1), T(row, 2), T(row, 3), T(row, 4));
                    else
                        mthStrings{row+1} = sprintf('│ %8.0f %8.0f %8.0f %8.0f │', ...
                                                   T(row, 1), T(row, 2), T(row, 3), T(row, 4));
                    end
                end
                
                mthStrings{6} = sprintf('└                                           ┘');
                
                set(mthDisplay, 'String', mthStrings);
                
                % Actualizar información de posición
                position = T(1:3, 4);
                set(positionDisplay, 'String', sprintf('X: %.3f  Y: %.3f  Z: %.3f', ...
                                                      position(1), position(2), position(3)));
                
                % Actualizar ángulos de Euler
                euler_angles = currentEuler(selectedIndex, :) * 180/pi;
                set(eulerDisplay, 'String', sprintf('φ: %.1f°  θ: %.1f°  ψ: %.1f°', ...
                                                   euler_angles(1), euler_angles(2), euler_angles(3)));
            else
                set(mthDisplay, 'String', {'MTH no calculada aún.'});
                set(positionDisplay, 'String', 'X: ---  Y: ---  Z: ---');
                set(eulerDisplay, 'String', 'φ: ---  θ: ---  ψ: ---');
            end
            
        catch ME
            fprintf('Error actualizando display MTH: %s\n', ME.message);
        end
    end
    
    % Función para actualizar la tabla resumen
    function updateSummaryTable()
        try
            tableData = cell(6, 7);
            
            for i = 1:6
                if ~isempty(currentMTH{i})
                    T = currentMTH{i};
                    position = T(1:3, 4);
                    euler_angles = currentEuler(i, :) * 180/pi;
                    
                    if i <= 5
                        tableData{i, 1} = sprintf('SC{0}→SC{%d}', i);
                    else
                        tableData{i, 1} = 'SC{0}→EF';
                    end
                    
                    tableData{i, 2} = sprintf('%.4f', position(1));
                    tableData{i, 3} = sprintf('%.4f', position(2));
                    tableData{i, 4} = sprintf('%.4f', position(3));
                    tableData{i, 5} = sprintf('%.2f', euler_angles(1));
                    tableData{i, 6} = sprintf('%.2f', euler_angles(2));
                    tableData{i, 7} = sprintf('%.2f', euler_angles(3));
                else
                    if i <= 5
                        tableData{i, 1} = sprintf('SC{0}→SC{%d}', i);
                    else
                        tableData{i, 1} = 'SC{0}→EF';
                    end
                    for j = 2:7
                        tableData{i, j} = '---';
                    end
                end
            end
            
            set(summaryTable, 'Data', tableData);
            
        catch ME
            fprintf('Error actualizando tabla resumen: %s\n', ME.message);
        end
    end
    
    % ===============================================
    % FUNCIONES AUXILIARES MODIFICADAS
    % ===============================================
    
    % Función para aplicar offset a un ángulo en grados
    function angleWithOffset = applyOffset(jointIndex, angleRadians)
        angleDegrees = rad2deg(angleRadians);
        angleWithOffset = angleDegrees + JOINT_OFFSETS(jointIndex);
    end
    
    % Función para obtener el ángulo con offset para envío serial
    function angleForSerial = getAngleForSerial(jointIndex, angleRadians)
        angleWithOffset = applyOffset(jointIndex, angleRadians);
        % Asegurar que el valor esté en el rango 0-180 después del offset
        angleForSerial = max(0, min(180, round(angleWithOffset)));
    end
    
    % Función para procesar datos recibidos de potenciómetros
    function processPotentiometerData(dataString)
        try
            % Verificar que la trama tenga exactamente 20 caracteres
            if length(dataString) ~= 20
                fprintf('⚠ Trama incorrecta. Longitud: %d (esperada: 20)\n', length(dataString));
                return;
            end
            
            % Verificar formato: debe terminar en # y tener # cada 4 caracteres
            if dataString(20) ~= '#' || dataString(4) ~= '#' || dataString(8) ~= '#' || ...
               dataString(12) ~= '#' || dataString(16) ~= '#'
                fprintf('⚠ Formato de trama incorrecto: %s\n', dataString);
                return;
            end
            
            % Extraer los 5 ángulos
            angles = zeros(5,1);
            angles(1) = str2double(dataString(1:3));
            angles(2) = str2double(dataString(5:7));
            angles(3) = str2double(dataString(9:11));
            angles(4) = str2double(dataString(13:15));
            angles(5) = str2double(dataString(17:19));
            
            % Verificar que todos los valores sean válidos
            if any(isnan(angles))
                fprintf('⚠ Valores no numéricos en la trama: %s\n', dataString);
                return;
            end
            
            % Actualizar los valores globales
            potValues = angles;
            lastPotUpdate = datetime('now');
            
            % Actualizar la interfaz
            updatePotentiometerDisplay();
            
            if SERIAL_CONFIG.DEBUG_MODE
                fprintf('✓ Potenciómetros actualizados: [%.0f°, %.0f°, %.0f°, %.0f°, %.0f°]\n', ...
                        angles(1), angles(2), angles(3), angles(4), angles(5));
            end
            
        catch ME
            fprintf('❌ Error procesando datos de potenciómetros: %s\n', ME.message);
        end
    end
    
    % Función para actualizar la visualización de potenciómetros
    function updatePotentiometerDisplay()
        for i = 1:5
            % Mostrar el valor en grados con formato colorido
            if potValues(i) >= 0 && potValues(i) <= 180
                potValueLabels(i).String = sprintf('%.0f°', potValues(i));
                potValueLabels(i).ForegroundColor = [0, 0.6, 0]; % Verde para valores válidos
                potValueLabels(i).BackgroundColor = [0.9, 1, 0.9]; % Fondo verde claro
            else
                potValueLabels(i).String = sprintf('%.0f°', potValues(i));
                potValueLabels(i).ForegroundColor = [0.8, 0.2, 0.2]; % Rojo para valores inválidos
                potValueLabels(i).BackgroundColor = [1, 0.9, 0.9]; % Fondo rojo claro
            end
        end
        
        % Actualizar estado de recepción
        timeElapsed = seconds(datetime('now') - lastPotUpdate);
        if timeElapsed < 2
            receptionStatus.String = sprintf('Estado: Datos actualizados (hace %.1fs)', timeElapsed);
            receptionStatus.ForegroundColor = [0, 0.6, 0];
        else
            receptionStatus.String = sprintf('Estado: Sin datos recientes (hace %.1fs)', timeElapsed);
            receptionStatus.ForegroundColor = [0.8, 0.4, 0];
        end
    end
    
    % Función para actualizar el robot cuando cambia un slider - MODIFICADA
    function updateRobot(jointIndex, value)
        % Actualizar el valor en el array
        jointValues(jointIndex) = value;
        
        % Actualizar las etiquetas de valores (SIN offset para radianes)
        valueLabels(jointIndex).String = sprintf('%.2f rad', value);
        
        % Actualizar etiqueta de grados (CON offset)
        angleWithOffset = applyOffset(jointIndex, value);
        angleDegreeLabels(jointIndex).String = sprintf('%.1f°', angleWithOffset);
        
        % Guardar la vista actual antes de actualizar
        currentView = get(ax, 'View');
        currentCameraPosition = get(ax, 'CameraPosition');
        currentCameraTarget = get(ax, 'CameraTarget');
        currentCameraUpVector = get(ax, 'CameraUpVector');
        
        % Actualizar la visualización del robot
        show(robot, jointValues, 'Parent', ax, 'Visuals', 'on', 'PreservePlot', false);
        
        % Restaurar la vista guardada
        set(ax, 'View', currentView);
        set(ax, 'CameraPosition', currentCameraPosition);
        set(ax, 'CameraTarget', currentCameraTarget);
        set(ax, 'CameraUpVector', currentCameraUpVector);
        
        % Mantener los límites y la grilla
        setPlotLimits();
        grid(ax, 'on');
        
        % Calcular y actualizar MTH automáticamente
        calculateAllMTH();
        
        % Enviar datos por serial
        sendSerialData();
        
        drawnow; % Forzar actualización inmediata
    end
    
    % Función para resetear todas las articulaciones - MODIFICADA
    function resetJoints(~, ~)
        % Guardar la vista actual antes de actualizar
        currentView = get(ax, 'View');
        currentCameraPosition = get(ax, 'CameraPosition');
        currentCameraTarget = get(ax, 'CameraTarget');
        currentCameraUpVector = get(ax, 'CameraUpVector');
        
        for j = 1:5
            % Resetear al valor medio del rango permitido para cada joint
            midValue = (JOINT_LIMITS.MIN(j) + JOINT_LIMITS.MAX(j)) / 2;
            sliders(j).Value = midValue;
            jointValues(j) = midValue;
            
            % Actualizar etiquetas
            valueLabels(j).String = sprintf('%.2f rad', midValue);
            angleWithOffset = applyOffset(j, midValue);
            angleDegreeLabels(j).String = sprintf('%.1f°', angleWithOffset);
        end
        
        show(robot, jointValues, 'Parent', ax, 'Visuals', 'on', 'PreservePlot', false);
        
        % Restaurar la vista guardada
        set(ax, 'View', currentView);
        set(ax, 'CameraPosition', currentCameraPosition);
        set(ax, 'CameraTarget', currentCameraTarget);
        set(ax, 'CameraUpVector', currentCameraUpVector);
        
        % Mantener los límites y la grilla
        setPlotLimits();
        grid(ax, 'on');
        
        % Calcular y actualizar MTH
        calculateAllMTH();
        
        % Enviar datos por serial
        sendSerialData();
        
        drawnow;
    end
    
    % Función para configuración aleatoria - MODIFICADA
    function randomConfig(~, ~)
        % Guardar la vista actual antes de actualizar
        currentView = get(ax, 'View');
        currentCameraPosition = get(ax, 'CameraPosition');
        currentCameraTarget = get(ax, 'CameraTarget');
        currentCameraUpVector = get(ax, 'CameraUpVector');
        
        for j = 1:5
            % Valor aleatorio dentro del rango permitido para cada joint
            randomValue = JOINT_LIMITS.MIN(j) + ...
                         (JOINT_LIMITS.MAX(j) - JOINT_LIMITS.MIN(j)) * rand;
            sliders(j).Value = randomValue;
            jointValues(j) = randomValue;
            
            % Actualizar etiquetas
            valueLabels(j).String = sprintf('%.2f rad', randomValue);
            angleWithOffset = applyOffset(j, randomValue);
            angleDegreeLabels(j).String = sprintf('%.1f°', angleWithOffset);
        end
        
        show(robot, jointValues, 'Parent', ax, 'Visuals', 'on', 'PreservePlot', false);
        
        % Restaurar la vista guardada
        set(ax, 'View', currentView);
        set(ax, 'CameraPosition', currentCameraPosition);
        set(ax, 'CameraTarget', currentCameraTarget);
        set(ax, 'CameraUpVector', currentCameraUpVector);
        
        % Mantener los límites y la grilla
        setPlotLimits();
        grid(ax, 'on');
        
        % Calcular y actualizar MTH
        calculateAllMTH();
        
        % Enviar datos por serial
        sendSerialData();
        
        drawnow;
    end
    
    % Función para resetear la vista
    function resetView(~, ~)
        view(ax, 3);
        setPlotLimits();
        grid(ax, 'on');
    end
    
    % ===============================================
    % FUNCIONES DE COMUNICACIÓN SERIAL
    % ===============================================
    
    % Función para inicializar la comunicación serial
    function initializeSerial()
        if SERIAL_CONFIG.ENABLED
            try
                % Verificar si ya existe una conexión serial abierta
                if ~isempty(serialObj) && isvalid(serialObj)
                    fclose(serialObj);
                    delete(serialObj);
                end
                
                fprintf('Intentando conectar al puerto serial %s...\n', SERIAL_CONFIG.PORT);
                
                % Crear objeto serial con configuración moderna
                serialObj = serial(SERIAL_CONFIG.PORT);
                
                % Configurar propiedades
                set(serialObj, 'BaudRate', SERIAL_CONFIG.BAUD_RATE);
                set(serialObj, 'Terminator', 'LF');
                set(serialObj, 'Timeout', 2);
                set(serialObj, 'DataBits', 8);
                set(serialObj, 'Parity', 'none');
                set(serialObj, 'StopBits', 1);
                set(serialObj, 'FlowControl', 'none');
                
                % Configuración para recepción de datos
                set(serialObj, 'InputBufferSize', 512);
                set(serialObj, 'OutputBufferSize', 512);
                
                % Configurar callback para datos entrantes
                set(serialObj, 'BytesAvailableFcn', @serialDataReceived);
                set(serialObj, 'BytesAvailableFcnCount', 20);
                set(serialObj, 'BytesAvailableFcnMode', 'byte');
                
                % Abrir la conexión
                fopen(serialObj);
                
                % Limpiar buffers
                flushinput(serialObj);
                flushoutput(serialObj);
                
                % Pequeña pausa para estabilizar la conexión
                pause(0.5);
                
                % Actualizar interfaz
                serialStatus.String = 'Serial: Conectado';
                serialStatus.ForegroundColor = [0, 0.6, 0];
                serialButton.String = 'Desconectar';
                
                fprintf('Conexión serial establecida exitosamente.\n');
                
                % Enviar datos iniciales
                sendSerialData();
                
            catch ME
                fprintf('Error al conectar serial: %s\n', ME.message);
                serialStatus.String = 'Serial: Error';
                serialStatus.ForegroundColor = [0.8, 0.2, 0.2];
                serialButton.String = 'Conectar Serial';
                serialObj = [];
            end
        else
            fprintf('Comunicación serial deshabilitada en configuración.\n');
        end
    end
    
    % Callback para datos seriales recibidos
    function serialDataReceived(src, ~)
        try
            if ~isempty(serialObj) && isvalid(serialObj) && strcmp(serialObj.Status, 'open')
                % Verificar cuántos bytes están disponibles
                bytesAvailable = get(serialObj, 'BytesAvailable');
                
                if SERIAL_CONFIG.DEBUG_MODE
                    fprintf('📡 Bytes disponibles: %d\n', bytesAvailable);
                end
                
                if bytesAvailable >= 20
                    % Leer exactamente 20 bytes
                    receivedData = fscanf(serialObj, '%c', 20);
                    
                    if SERIAL_CONFIG.DEBUG_MODE
                        fprintf('📥 Datos recibidos: "%s" (Longitud: %d)\n', receivedData, length(receivedData));
                    end
                    
                    % Procesar datos de potenciómetros
                    processPotentiometerData(receivedData);
                    
                    % Limpiar buffer restante si hay más datos
                    if get(serialObj, 'BytesAvailable') > 0
                        flushinput(serialObj);
                    end
                end
            end
        catch ME
            fprintf('❌ Error leyendo datos seriales: %s\n', ME.message);
            try
                if ~isempty(serialObj) && isvalid(serialObj)
                    flushinput(serialObj);
                end
            catch
                % Ignorar errores de limpieza
            end
        end
    end
    
    % Función para alternar conexión serial
    function toggleSerial(~, ~)
        if isempty(serialObj) || ~isvalid(serialObj) || strcmp(serialObj.Status, 'closed')
            % Conectar
            initializeSerial();
        else
            % Desconectar
            try
                fclose(serialObj);
                delete(serialObj);
                serialObj = [];
                
                serialStatus.String = 'Serial: Desconectado';
                serialStatus.ForegroundColor = [0.8, 0.2, 0.2];
                serialButton.String = 'Conectar Serial';
                
                % Limpiar display de potenciómetros
                for i = 1:5
                    potValueLabels(i).String = 'Sin conexión';
                    potValueLabels(i).ForegroundColor = [0.5, 0.5, 0.5];
                    potValueLabels(i).BackgroundColor = [1, 1, 1];
                end
                receptionStatus.String = 'Estado: Desconectado';
                receptionStatus.ForegroundColor = [0.8, 0.2, 0.2];
                
                fprintf('Conexión serial cerrada.\n');
            catch ME
                fprintf('Error al cerrar serial: %s\n', ME.message);
            end
        end
    end
    
    % Función para enviar datos por serial
    function sendSerialData()
        % Convertir ángulos aplicando offsets y redondear
        anglesForSerial = zeros(5, 1);
        for i = 1:5
            anglesForSerial(i) = getAngleForSerial(i, jointValues(i));
        end
        
        % Crear la trama de datos con formato de 3 dígitos con ceros a la izquierda
        dataString = sprintf('%03d%s%03d%s%03d%s%03d%s%03d', ...
            anglesForSerial(1), SERIAL_CONFIG.SEPARATOR, ...
            anglesForSerial(2), SERIAL_CONFIG.SEPARATOR, ...
            anglesForSerial(3), SERIAL_CONFIG.SEPARATOR, ...
            anglesForSerial(4), SERIAL_CONFIG.SEPARATOR, ...
            anglesForSerial(5));
        
        % Mostrar en consola si el modo debug está habilitado
        if SERIAL_CONFIG.DEBUG_MODE
            fprintf('📤 Datos a enviar: %s\n', dataString);
            fprintf('Longitud del mensaje: %d caracteres\n', length(dataString));
        end
        
        % Intentar enviar por serial si hay conexión
        if ~isempty(serialObj) && isvalid(serialObj) && strcmp(serialObj.Status, 'open')
            try
                % Enviar datos
                fprintf(serialObj, '%s', dataString);
                
                if SERIAL_CONFIG.DEBUG_MODE
                    fprintf('✓ Enviado exitosamente por %s\n', SERIAL_CONFIG.PORT);
                end
                
            catch ME
                fprintf('❌ Error al enviar datos: %s\n', ME.message);
                % Marcar como desconectado
                if ~isempty(serialObj)
                    try
                        fclose(serialObj);
                        delete(serialObj);
                    catch
                    end
                    serialObj = [];
                    serialStatus.String = 'Serial: Desconectado';
                    serialStatus.ForegroundColor = [0.8, 0.2, 0.2];
                    serialButton.String = 'Conectar Serial';
                end
            end
        elseif SERIAL_CONFIG.DEBUG_MODE
            fprintf('⚠ No hay conexión serial activa\n');
        end
    end
    
    % Función de limpieza al cerrar la figura
    function cleanupSerial(~, ~)
        if ~isempty(serialObj) && isvalid(serialObj)
            try
                fclose(serialObj);
                delete(serialObj);
                fprintf('Conexión serial cerrada al cerrar la aplicación.\n');
            catch
                % Ignorar errores al cerrar
            end
        end
    end
    
    % ===============================================
    % FUNCIONES AUXILIARES
    % ===============================================
    
    % Función auxiliar para establecer los límites de la gráfica
    function setPlotLimits()
        xlim(ax, [PLOT_LIMITS.X_MIN, PLOT_LIMITS.X_MAX]);
        ylim(ax, [PLOT_LIMITS.Y_MIN, PLOT_LIMITS.Y_MAX]);
        zlim(ax, [PLOT_LIMITS.Z_MIN, PLOT_LIMITS.Z_MAX]);
    end
    
    % Función auxiliar para crear etiquetas de ángulos
    function labels = createAngleLabels(values)
        labels = cell(size(values));
        for i = 1:length(values)
            val = values(i);
            if abs(val) < 0.1
                labels{i} = '0°';
            elseif abs(val - pi/4) < 0.1
                labels{i} = '45°';
            elseif abs(val - pi/2) < 0.1
                labels{i} = '90°';
            elseif abs(val - 3*pi/4) < 0.1
                labels{i} = '135°';
            elseif abs(val - pi) < 0.1
                labels{i} = '180°';
            elseif abs(val + pi/4) < 0.1
                labels{i} = '-45°';
            elseif abs(val + pi/2) < 0.1
                labels{i} = '-90°';
            elseif abs(val + 3*pi/4) < 0.1
                labels{i} = '-135°';
            elseif abs(val + pi) < 0.1
                labels{i} = '-180°';
            else
                labels{i} = sprintf('%.1f', val);
            end
        end
    end
    
    % Función auxiliar para extraer datos de potenciómetro
    function [potData, found] = extractPotentiometerData(dataString)
        found = false;
        potData = '';
        
        try
            % Buscar el patrón: 3 dígitos + # repetido 5 veces
            pattern = '\d{3}#\d{3}#\d{3}#\d{3}#\d{3}#';
            matches = regexp(dataString, pattern, 'match');
            
            if ~isempty(matches)
                potData = matches{end}; % Tomar el último match válido
                found = true;
                
                if SERIAL_CONFIG.DEBUG_MODE
                    fprintf('✅ Patrón encontrado: "%s"\n', potData);
                end
            else
                % Método alternativo: buscar manualmente
                if length(dataString) >= 20
                    % Buscar desde el final hacia atrás
                    for i = length(dataString):-1:20
                        testData = dataString(i-19:i);
                        if length(testData) == 20 && testData(20) == '#' && ...
                           testData(4) == '#' && testData(8) == '#' && ...
                           testData(12) == '#' && testData(16) == '#'
                            potData = testData;
                            found = true;
                            if SERIAL_CONFIG.DEBUG_MODE
                                fprintf('🔎 Búsqueda manual exitosa: "%s"\n', potData);
                            end
                            break;
                        end
                    end
                end
            end
            
        catch extractError
            if SERIAL_CONFIG.DEBUG_MODE
                fprintf('❌ Error extrayendo datos: %s\n', extractError.message);
            end
        end
    end
    
    % Timer para actualizar display de potenciómetros periódicamente
    displayTimer = timer('Period', 1.0, 'ExecutionMode', 'fixedRate', ...
                     'TimerFcn', @(src,event) updatePotentiometerDisplay());

    start(displayTimer);
    
    % Timer para verificar datos seriales manualmente (método de respaldo)
    serialReadTimer = timer('Period', 0.1, 'ExecutionMode', 'fixedRate', ...
                           'TimerFcn', @checkSerialData);
    start(serialReadTimer);
    
    % Verificación manual de datos seriales (método de respaldo)
    function checkSerialData(~, ~)
        try
            if ~isempty(serialObj) && isvalid(serialObj) && strcmp(serialObj.Status, 'open')
                bytesAvailable = get(serialObj, 'BytesAvailable');
                
                if bytesAvailable > 0
                    if SERIAL_CONFIG.DEBUG_MODE && mod(round(rand*100), 50) == 0 % Debug ocasional
                        fprintf('🔍 Verificación manual: %d bytes disponibles\n', bytesAvailable);
                    end
                    
                    % Si tenemos al menos 20 bytes, intentar leer
                    if bytesAvailable >= 20
                        try
                            % Leer todos los datos disponibles como string
                            allData = fscanf(serialObj, '%c', bytesAvailable);
                            
                            if SERIAL_CONFIG.DEBUG_MODE
                                fprintf('📖 Lectura manual: "%s" (%d bytes)\n', allData, length(allData));
                            end
                            
                            % Buscar patrones de 20 caracteres que terminen en #
                            [potData, found] = extractPotentiometerData(allData);
                            
                            if found
                                processPotentiometerData(potData);
                            end
                            
                        catch readError
                            if SERIAL_CONFIG.DEBUG_MODE
                                fprintf('⚠ Error en lectura manual: %s\n', readError.message);
                            end
                            % Limpiar buffer en caso de error
                            flushinput(serialObj);
                        end
                    end
                end
            end
        catch ME
            if SERIAL_CONFIG.DEBUG_MODE
                fprintf('❌ Error en verificación serial: %s\n', ME.message);
            end
        end
    end
    
    % ===============================================
    % CONFIGURACIÓN INICIAL Y LIMPIEZA
    % ===============================================
    
    % Mostrar mensaje de inicio
    fprintf('============================================\n');
    fprintf('GUI del brazo robótico con MTH iniciado correctamente.\n');
    fprintf('============================================\n');
    fprintf('✓ Panel de MTH y ángulos de Euler agregado\n');
    fprintf('✓ Cálculo automático de transformaciones\n');
    fprintf('✓ Visualización de 6 MTH intermedias (SC{0} a SC{5})\n');
    fprintf('✓ Ángulos de Euler en convención ZYZ\n');
    fprintf('✓ Tabla resumen de todas las transformaciones\n');
    fprintf('✓ Actualización en tiempo real habilitada\n');
    fprintf('✓ Puerto configurado: %s a %d baudios\n', SERIAL_CONFIG.PORT, SERIAL_CONFIG.BAUD_RATE);
    fprintf('============================================\n');
    fprintf('📌 Parámetros DH corregidos:\n');
    for i = 1:5
        fprintf('   Joint %d: a=%.3fm, α=%.2f°, d=%.3fm, θ_offset=%.2f°\n', i, ...
                DH_PARAMS(i,1), rad2deg(DH_PARAMS(i,2)), DH_PARAMS(i,3), rad2deg(DH_PARAMS(i,4)));
    end
    fprintf('============================================\n');
    fprintf('🔧 Distancias actualizadas:\n');
    fprintf('   • S0→S1: %.1f cm (d1)\n', DH_PARAMS(1,3)*100);
    fprintf('   • S1→S2: %.1f cm (a2)\n', DH_PARAMS(2,1)*100);
    fprintf('   • S2→S3: %.1f cm (a3)\n', DH_PARAMS(3,1)*100);
    fprintf('   • S3→S4: %.1f cm (d4)\n', DH_PARAMS(4,3)*100);
    fprintf('============================================\n');
    fprintf('🔧 Instrucciones del panel MTH:\n');
    fprintf('   • Usa el selector para ver diferentes MTH\n');
    fprintf('   • Las MTH se calculan automáticamente al mover sliders\n');
    fprintf('   • La tabla muestra resumen de todas las transformaciones\n');
    fprintf('   • Los ángulos de Euler usan convención ZYZ (φ, θ, ψ)\n');
    fprintf('============================================\n');
    
    % Configurar callback de limpieza al cerrar la figura
    set(fig, 'CloseRequestFcn', @(src, evt) cleanupAndClose(src, evt));
    
    % Inicializar comunicación serial después de crear la interfaz
    if SERIAL_CONFIG.AUTO_CONNECT
        initializeSerial();
    else
        fprintf('Conexión automática deshabilitada. Usa el botón "Conectar Serial" cuando tengas el STM listo.\n');
    end
    
    % Calcular MTH inicial
    calculateAllMTH();
    
    % Función para limpiar y cerrar
    function cleanupAndClose(src, evt)
        % Detener y eliminar timers
        if exist('displayTimer', 'var') && isvalid(displayTimer)
            stop(displayTimer);
            delete(displayTimer);
        end
        if exist('serialReadTimer', 'var') && isvalid(serialReadTimer)
            stop(serialReadTimer);
            delete(serialReadTimer);
        end
        cleanupSerial();
        delete(src);
        clc;
        clear;
    end
end