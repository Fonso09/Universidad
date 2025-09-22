function robot_arm_gui()
    %% Parámetros del robot
    L1 = 0.093;
    L2 = 0.14;
    L3 = 0.103;
    L4 = 0.0284;
    
    %% Configuración inicial de ángulos
    q1 = 0; q2 = 0; q3 = 0; q4 = 0; q5 = 0;
    
    %% Configuración serial
    serial_port = [];
    
    %% Crear figura principal
    main_fig = figure('Name', 'Robot Arm Control', 'Position', [100 100 1200 800], ...
                     'CloseRequestFcn', @closeGUI);
    
    %% Panel de control (lado izquierdo)
    control_panel = uipanel(main_fig, 'Title', 'Control Panel', ...
                           'Position', [0.02 0.02 0.25 0.96]);
    
    %% Crear sliders
    slider_height = 0.12;
    slider_spacing = 0.15;
    
    % Slider Q1
    uicontrol(control_panel, 'Style', 'text', 'String', 'Q1 (deg):', ...
             'Units', 'normalized', 'Position', [0.05 0.88 0.4 0.05]);
    q1_slider = uicontrol(control_panel, 'Style', 'slider', ...
                         'Min', -180, 'Max', 180, 'Value', 0, ...
                         'Units', 'normalized', 'Position', [0.05 0.8 0.6 0.08], ...
                         'Callback', @updateRobot);
    q1_text = uicontrol(control_panel, 'Style', 'text', 'String', '0.0°', ...
                       'Units', 'normalized', 'Position', [0.7 0.8 0.25 0.08]);
    
    % Slider Q2
    uicontrol(control_panel, 'Style', 'text', 'String', 'Q2 (deg):', ...
             'Units', 'normalized', 'Position', [0.05 0.73 0.4 0.05]);
    q2_slider = uicontrol(control_panel, 'Style', 'slider', ...
                         'Min', -90, 'Max', 90, 'Value', 0, ...
                         'Units', 'normalized', 'Position', [0.05 0.65 0.6 0.08], ...
                         'Callback', @updateRobot);
    q2_text = uicontrol(control_panel, 'Style', 'text', 'String', '0.0°', ...
                       'Units', 'normalized', 'Position', [0.7 0.65 0.25 0.08]);
    
    % Slider Q3
    uicontrol(control_panel, 'Style', 'text', 'String', 'Q3 (deg):', ...
             'Units', 'normalized', 'Position', [0.05 0.58 0.4 0.05]);
    q3_slider = uicontrol(control_panel, 'Style', 'slider', ...
                         'Min', -90, 'Max', 90, 'Value', 0, ...
                         'Units', 'normalized', 'Position', [0.05 0.5 0.6 0.08], ...
                         'Callback', @updateRobot);
    q3_text = uicontrol(control_panel, 'Style', 'text', 'String', '0.0°', ...
                       'Units', 'normalized', 'Position', [0.7 0.5 0.25 0.08]);
    
    % Slider Q4
    uicontrol(control_panel, 'Style', 'text', 'String', 'Q4 (deg):', ...
             'Units', 'normalized', 'Position', [0.05 0.43 0.4 0.05]);
    q4_slider = uicontrol(control_panel, 'Style', 'slider', ...
                         'Min', -180, 'Max', 180, 'Value', 0, ...
                         'Units', 'normalized', 'Position', [0.05 0.35 0.6 0.08], ...
                         'Callback', @updateRobot);
    q4_text = uicontrol(control_panel, 'Style', 'text', 'String', '0.0°', ...
                       'Units', 'normalized', 'Position', [0.7 0.35 0.25 0.08]);
    
    % Slider Q5
    uicontrol(control_panel, 'Style', 'text', 'String', 'Q5 (deg):', ...
             'Units', 'normalized', 'Position', [0.05 0.28 0.4 0.05]);
    q5_slider = uicontrol(control_panel, 'Style', 'slider', ...
                         'Min', -180, 'Max', 180, 'Value', 0, ...
                         'Units', 'normalized', 'Position', [0.05 0.2 0.6 0.08], ...
                         'Callback', @updateRobot);
    q5_text = uicontrol(control_panel, 'Style', 'text', 'String', '0.0°', ...
                       'Units', 'normalized', 'Position', [0.7 0.2 0.25 0.08]);
    
    %% Botones de control serial
    uicontrol(control_panel, 'Style', 'pushbutton', 'String', 'Connect Serial', ...
             'Units', 'normalized', 'Position', [0.05 0.1 0.4 0.06], ...
             'Callback', @connectSerial);
    
    uicontrol(control_panel, 'Style', 'pushbutton', 'String', 'Send Data', ...
             'Units', 'normalized', 'Position', [0.55 0.1 0.4 0.06], ...
             'Callback', @sendSerialData);
    
    serial_status = uicontrol(control_panel, 'Style', 'text', 'String', 'Serial: Disconnected', ...
                             'Units', 'normalized', 'Position', [0.05 0.02 0.9 0.04], ...
                             'BackgroundColor', [1 0.8 0.8]);
    
    %% Panel de visualización 3D
    robot_panel = uipanel(main_fig, 'Title', '3D Robot Visualization', ...
                         'Position', [0.28 0.52 0.7 0.46]);
    
    robot_axes = axes('Parent', robot_panel, 'Position', [0.05 0.05 0.9 0.9]);
    hold(robot_axes, 'on');
    axis(robot_axes, 'equal');
    view(robot_axes, 3);
    xlabel(robot_axes, 'X (m)'); 
    ylabel(robot_axes, 'Y (m)'); 
    zlabel(robot_axes, 'Z (m)');
    grid(robot_axes, 'on');
    camlight(robot_axes); 
    lighting(robot_axes, 'gouraud');
    
    %% Panel de matrices con scroll
    matrix_panel = uipanel(main_fig, 'Title', 'Transformation Matrices', ...
                          'Position', [0.28 0.02 0.7 0.48]);
    
    % Crear listbox con scroll para las matrices
    matrix_listbox = uicontrol(matrix_panel, 'Style', 'listbox', ...
                              'String', {'Matrices will be displayed here...'}, ...
                              'Units', 'normalized', 'Position', [0.02 0.02 0.96 0.96], ...
                              'FontName', 'Courier', 'FontSize', 9, ...
                              'BackgroundColor', [1 1 1], 'Max', 100);
    
    %% Cargar STL files (solo una vez al inicio)
    loadSTLFiles();
    
    %% Dibujar robot inicial
    updateRobot();
    
    %% Funciones callback
    function updateRobot(~, ~)
        % Obtener valores de los sliders
        q1 = deg2rad(get(q1_slider, 'Value'));
        q2 = deg2rad(get(q2_slider, 'Value'));
        q3 = deg2rad(get(q3_slider, 'Value'));
        q4 = deg2rad(get(q4_slider, 'Value'));
        q5 = deg2rad(get(q5_slider, 'Value'));
        
        % Actualizar textos
        set(q1_text, 'String', sprintf('%.1f°', rad2deg(q1)));
        set(q2_text, 'String', sprintf('%.1f°', rad2deg(q2)));
        set(q3_text, 'String', sprintf('%.1f°', rad2deg(q3)));
        set(q4_text, 'String', sprintf('%.1f°', rad2deg(q4)));
        set(q5_text, 'String', sprintf('%.1f°', rad2deg(q5)));
        
        % Limpiar axes
        cla(robot_axes);
        hold(robot_axes, 'on');
        
        % Calcular matrices de transformación
        [T0, T1, T2, T3, T4] = calculateTransformations(q1, q2, q3, q4, q5);
        
        % Dibujar robot
        drawRobot(T0, T1, T2, T3, T4);
        
        % Actualizar display de matrices
        updateMatrixDisplay(T0, T1, T2, T3, T4);
        
        % Configurar vista
        axis(robot_axes, 'equal');
        view(robot_axes, 3);
        xlabel(robot_axes, 'X (m)'); 
        ylabel(robot_axes, 'Y (m)'); 
        zlabel(robot_axes, 'Z (m)');
        grid(robot_axes, 'on');
        camlight(robot_axes); 
        lighting(robot_axes, 'gouraud');
    end
    
    function [T0, T1, T2, T3, T4] = calculateTransformations(q1, q2, q3, q4, q5)
        % Base fija
        Tb = eye(4);
        
        % Base rotatoria
        T0 = transl(0,0,0.05) * trotz(q1);
        
        % Brazo 1
        Cq1 = cos(q1); Sq1 = sin(q1);
        A1 = [Cq1 0 -Sq1 0;
              Sq1 0 -Cq1 0;
              0 1 0 L1;
              0 0 0 1];
        T1 = A1 * trotz(q2);
        
        % Brazo 2
        Cq2 = cos(q2); Sq2 = sin(q2);
        A2 = [Cq2 -Sq2 0 L2*Cq2;
              Sq2 Cq2 0 L2*Sq2;
              0 0 1 0;
              0 0 0 1];
        T2 = A1 * A2 * trotz(q3);
        
        % Brazo 3
        Cq3 = cos(q3+pi/2); Sq3 = sin(q3+pi/2);
        A3 = [Cq3 0 Sq3 0;
              Sq3 0 -Cq3 0;
              0 1 0 0;
              0 0 0 1];
        T3 = A1 * A2 * A3 * transl(0,0,L3) * trotz(q4);
        
        % Gripper
        Cq4 = cos(q4+pi); Sq4 = sin(q4+pi);
        A4 = [Cq4 0 Sq4 0;
              Sq4 0 -Cq4 0;
              0 1 0 L3+L4;
              0 0 0 1];
        T4 = A1 * A2 * A3 * A4 * trotz(q5);
    end
    
    function drawRobot(T0, T1, T2, T3, T4)
        % Dibujar cada eslabón con su transformación correspondiente
        try
            drawSTL(robot_axes, Fb, Vb, eye(4), [0.8 0.8 1]);      % Base
            drawSTL(robot_axes, F0, V0, T0, [1 0.6 0.6]);          % Base rotatoria
            drawSTL(robot_axes, F1, V1, T1, [0.6 1 0.6]);          % Brazo 1
            drawSTL(robot_axes, F2, V2, T2, [0.6 0.6 1]);          % Brazo 2
            drawSTL(robot_axes, F3, V3, T3, [1 0.6 0.6]);          % Brazo 3
            drawSTL(robot_axes, F4, V4_new, T4, [0.3 1 0.3]);      % Gripper
        catch ME
            fprintf('Error drawing robot: %s\n', ME.message);
        end
    end
    
    function updateMatrixDisplay(T0, T1, T2, T3, T4)
        % Crear array de strings para el listbox
        matrix_strings = {};
        
        % T0
        matrix_strings{end+1} = '=== T0 (Base Rotatoria) ===';
        matrix_strings = [matrix_strings, formatMatrixForListbox(T0)];
        matrix_strings{end+1} = '';
        
        % T1
        matrix_strings{end+1} = '=== T1 (Brazo 1) ===';
        matrix_strings = [matrix_strings, formatMatrixForListbox(T1)];
        matrix_strings{end+1} = '';
        
        % T2
        matrix_strings{end+1} = '=== T2 (Brazo 2) ===';
        matrix_strings = [matrix_strings, formatMatrixForListbox(T2)];
        matrix_strings{end+1} = '';
        
        % T3
        matrix_strings{end+1} = '=== T3 (Brazo 3) ===';
        matrix_strings = [matrix_strings, formatMatrixForListbox(T3)];
        matrix_strings{end+1} = '';
        
        % T4
        matrix_strings{end+1} = '=== T4 (Gripper) ===';
        matrix_strings = [matrix_strings, formatMatrixForListbox(T4)];
        
        % Actualizar listbox
        set(matrix_listbox, 'String', matrix_strings);
        set(matrix_listbox, 'Value', 1); % Resetear selección al inicio
    end
    
    function str_array = formatMatrixForListbox(T)
        str_array = {};
        for i = 1:4
            str_array{end+1} = sprintf('[%8.4f %8.4f %8.4f %8.4f]', T(i,1), T(i,2), T(i,3), T(i,4));
        end
    end
    
    function connectSerial(~, ~)
        try
            % Cerrar puerto previo si existe
            if ~isempty(serial_port) && isvalid(serial_port)
                fclose(serial_port);
                delete(serial_port);
            end
            
            % Lista de puertos disponibles
            available_ports = serialportlist("available");
            if isempty(available_ports)
                error('No hay puertos seriales disponibles');
            end
            
            % Crear conexión serial (usar el primer puerto disponible)
            port_name = available_ports(1);
            serial_port = serialport(port_name, 9600);
            
            set(serial_status, 'String', sprintf('Serial: Connected to %s', port_name), ...
                'BackgroundColor', [0.8 1 0.8]);
            
        catch ME
            set(serial_status, 'String', 'Serial: Connection Failed', ...
                'BackgroundColor', [1 0.8 0.8]);
            fprintf('Error connecting serial: %s\n', ME.message);
        end
    end
    
    function sendSerialData(~, ~)
        if isempty(serial_port) || ~isvalid(serial_port)
            msgbox('Conectar puerto serial primero', 'Error');
            return;
        end
        
        try
            % Obtener valores actuales
            q1_deg = get(q1_slider, 'Value');
            q2_deg = get(q2_slider, 'Value');
            q3_deg = get(q3_slider, 'Value');
            q4_deg = get(q4_slider, 'Value');
            q5_deg = get(q5_slider, 'Value');
            
            % Formatear datos para envío
            data_string = sprintf('%.2f,%.2f,%.2f,%.2f,%.2f\n', ...
                                q1_deg, q2_deg, q3_deg, q4_deg, q5_deg);
            
            % Enviar por serial
            write(serial_port, data_string, "string");
            
            fprintf('Sent: %s', data_string);
            
        catch ME
            msgbox(sprintf('Error enviando datos: %s', ME.message), 'Error');
        end
    end
    
    function closeGUI(~, ~)
        % Cerrar puerto serial al cerrar GUI
        if ~isempty(serial_port) && isvalid(serial_port)
            fclose(serial_port);
            delete(serial_port);
        end
        delete(main_fig);
    end
    
    function loadSTLFiles()
        % Variables globales para STL data
        try
            % Cargar todos los STL files
            TRb = stlread("URDF_ROBOT5GDL/meshes/base_real.stl");
            Fb = TRb.ConnectivityList;
            Vb = TRb.Points * 0.001;
            
            TR0 = stlread("URDF_ROBOT5GDL/meshes/J0_v2.stl");
            F0 = TR0.ConnectivityList;
            V0 = TR0.Points * 0.001;
            
            TR1 = stlread("URDF_ROBOT5GDL/meshes/J1_v2.stl");
            F1 = TR1.ConnectivityList;
            V1 = TR1.Points * 0.001;
            
            TR2 = stlread("URDF_ROBOT5GDL/meshes/J2_v2.stl");
            F2 = TR2.ConnectivityList;
            V2 = TR2.Points * 0.001;
            
            TR3 = stlread("URDF_ROBOT5GDL/meshes/J3_v2.stl");
            F3 = TR3.ConnectivityList;
            V3 = TR3.Points * 0.001;
            
            TR4 = stlread("URDF_ROBOT5GDL/meshes/J4_v3STL.stl");
            F4 = TR4.ConnectivityList;
            V4 = TR4.Points * 0.001;
            
            % Aplicar offset al gripper
            offset = transl(-0.01, -0.01, -0.033);
            V4_new = transformSTL(V4, offset);
            
        catch ME
            fprintf('Error loading STL files: %s\n', ME.message);
            % Crear geometrías simples como fallback
            [Fb, Vb] = createCube([0 0 0], 0.1);
            [F0, V0] = createCube([0 0 0.05], 0.08);
            [F1, V1] = createCube([0 0 0.1], 0.06);
            [F2, V2] = createCube([0 0 0.15], 0.06);
            [F3, V3] = createCube([0 0 0.2], 0.06);
            [F4, V4_new] = createCube([0 0 0.25], 0.04);
        end
        
        % Hacer variables accesibles a otras funciones
        assignin('base', 'Fb', Fb); assignin('base', 'Vb', Vb);
        assignin('base', 'F0', F0); assignin('base', 'V0', V0);
        assignin('base', 'F1', F1); assignin('base', 'V1', V1);
        assignin('base', 'F2', F2); assignin('base', 'V2', V2);
        assignin('base', 'F3', F3); assignin('base', 'V3', V3);
        assignin('base', 'F4', F4); assignin('base', 'V4_new', V4_new);
    end
    
    function [F, V] = createCube(center, size)
        % Crear cubo simple como fallback
        vertices = [
            -size/2 -size/2 -size/2; size/2 -size/2 -size/2; size/2 size/2 -size/2; -size/2 size/2 -size/2;
            -size/2 -size/2  size/2; size/2 -size/2  size/2; size/2 size/2  size/2; -size/2 size/2  size/2
        ];
        V = vertices + center;
        F = [1 2 3; 1 3 4; 5 6 7; 5 7 8; 1 2 6; 1 6 5; 3 4 8; 3 8 7; 1 4 8; 1 8 5; 2 3 7; 2 7 6];
    end

    % Obtener variables del workspace base
    Fb = evalin('base', 'Fb'); Vb = evalin('base', 'Vb');
    F0 = evalin('base', 'F0'); V0 = evalin('base', 'V0');
    F1 = evalin('base', 'F1'); V1 = evalin('base', 'V1');
    F2 = evalin('base', 'F2'); V2 = evalin('base', 'V2');
    F3 = evalin('base', 'F3'); V3 = evalin('base', 'V3');
    F4 = evalin('base', 'F4'); V4_new = evalin('base', 'V4_new');

end

%% Funciones auxiliares
function [Vt] = transformSTL(V, T)
    Vh = [V ones(size(V,1),1)];
    Vt = (T * Vh')';
    Vt = Vt(:,1:3);
end

function drawSTL(ax, F, V, T, color)
    Vt = transformSTL(V, T);
    
    TR = triangulation(F, Vt);
    trisurf(TR, 'Parent', ax, 'FaceColor', color, ...
                'EdgeColor', 'none', 'FaceAlpha', 0.9);
    
    % Dibujar ejes de coordenadas
    O = T(1:3,4);
    L = 0.05;
    quiver3(ax, O(1), O(2), O(3), T(1,1)*L, T(2,1)*L, T(3,1)*L, 'r', 'LineWidth', 2);
    quiver3(ax, O(1), O(2), O(3), T(1,2)*L, T(2,2)*L, T(3,2)*L, 'g', 'LineWidth', 2);
    quiver3(ax, O(1), O(2), O(3), T(1,3)*L, T(2,3)*L, T(3,3)*L, 'b', 'LineWidth', 2);
end

% Funciones de transformación
function T = transl(x,y,z)
    T = eye(4);
    T(1:3,4) = [x;y;z];
end

function T = trotz(theta)
    T = [cos(theta) -sin(theta) 0 0;
         sin(theta)  cos(theta) 0 0;
         0           0          1 0;
         0           0          0 1];
end

function T = trotx(theta)
    T = [1 0 0 0;
         0 cos(theta) -sin(theta) 0;
         0 sin(theta) cos(theta) 0;
         0 0 0 1];
end

function T = troty(theta)
    T = [cos(theta) 0 sin(theta) 0;
         0 1 0 0;
         -sin(theta) 0 cos(theta) 0;
         0 0 0 1];
end