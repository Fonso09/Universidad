function robot_5dof_gui()
    % Robot 5DOF GUI - Visualizador interactivo con URDF, cálculos DH y comunicación serial
    % Combina visualización URDF con cálculos manuales de MTH y envío por puerto serie
    
    % Variables globales
    global robot fig_handle ax_handle sliders angle_values mth_text dh_params
    global serial_port serial_connected status_text
    
    % Inicializar variables
    angle_values = [0, 0, 0, 0, 0]; % Ángulos iniciales en radianes
    serial_connected = false;
    serial_port = [];
    
    % Parámetros DH del robot [theta, d, a, alpha]
    % Estos deben coincidir con tu URDF
    dh_params = [
        0,    0.162,  0,      pi/2;   % Base a Joint1
        0,    0,      0.425,  0;      % Joint1 a Joint2  
        0,    0,      0.392,  0;      % Joint2 a Joint3
        0,    0.133,  0,      pi/2;   % Joint3 a Joint4
        0,    0.099,  0,      0;      % Joint4 a Joint5 (end-effector)
    ];
    
    % Crear la interfaz gráfica
    create_gui();
    
    % Cargar robot URDF (cambia la ruta por tu archivo)
    try
        % Intenta cargar tu URDF - cambia esta ruta
        robot = importrobot('URDF_ROBOT5GDL/urdf/URDF_ROBOT5GDL.urdf'); 
    catch
        % Si no encuentra el URDF, crea uno básico para demo
        fprintf('No se pudo cargar el URDF. Usando modelo de demostración.\n');
    end
    
    % Configuración inicial
    update_robot_display();
    
    % Fijar los límites de los ejes después de la primera visualización
    set_fixed_axis_limits();
    
    % Configurar cierre de ventana para limpiar puerto serial
    set(fig_handle, 'CloseRequestFcn', @close_gui_callback);
end

function create_gui()
    % Crear figura principal
    global fig_handle ax_handle sliders mth_text status_text
    
    fig_handle = figure('Name', 'Robot 5 GDL - Control Interactivo con Serial', ...
                       'NumberTitle', 'off', ...
                       'Position', [100, 100, 1400, 800], ...
                       'Resize', 'on');
    
    % Panel izquierdo para controles
    control_panel = uipanel('Parent', fig_handle, ...
                           'Title', 'Control del Robot', ...
                           'FontSize', 12, 'FontWeight', 'bold', ...
                           'Position', [0.02, 0.02, 0.25, 0.96]);
    
    % Panel derecho para visualización 3D
    viz_panel = uipanel('Parent', fig_handle, ...
                       'Title', 'Visualización 3D', ...
                       'FontSize', 12, 'FontWeight', 'bold', ...
                       'Position', [0.29, 0.5, 0.69, 0.48]);
    
    % Panel inferior derecho para MTH
    mth_panel = uipanel('Parent', fig_handle, ...
                       'Title', 'Matriz de Transformación Homogénea Final', ...
                       'FontSize', 12, 'FontWeight', 'bold', ...
                       'Position', [0.29, 0.02, 0.69, 0.46]);
    
    % Crear controles de comunicación serial
    create_serial_controls(control_panel);
    
    % Crear sliders
    create_sliders(control_panel);
    
    % Crear área de visualización 3D
    ax_handle = axes('Parent', viz_panel, 'Position', [0.1, 0.1, 0.8, 0.8]);
    
    % Crear área de texto para MTH
    mth_text = uicontrol('Parent', mth_panel, ...
                        'Style', 'text', ...
                        'String', 'MTH Final aparecerá aquí...', ...
                        'FontName', 'Courier New', ...
                        'FontSize', 10, ...
                        'HorizontalAlignment', 'left', ...
                        'Position', [20, 20, 850, 300], ...
                        'BackgroundColor', 'white');
    
    % Botón para cargar URDF
    uicontrol('Parent', control_panel, ...
              'Style', 'pushbutton', ...
              'String', 'Cargar URDF', ...
              'FontSize', 10, ...
              'Position', [20, 50, 120, 30], ...
              'Callback', @load_urdf_callback);
    
    % Botón para resetear ángulos
    uicontrol('Parent', control_panel, ...
              'Style', 'pushbutton', ...
              'String', 'Reset Ángulos', ...
              'FontSize', 10, ...
              'Position', [160, 50, 120, 30], ...
              'Callback', @reset_angles_callback);
    
    % Botón para centrar vista
    uicontrol('Parent', control_panel, ...
              'Style', 'pushbutton', ...
              'String', 'Centrar Vista', ...
              'FontSize', 10, ...
              'Position', [20, 10, 120, 30], ...
              'Callback', @center_view_callback);
    
    % Botón para ajustar límites
    uicontrol('Parent', control_panel, ...
              'Style', 'pushbutton', ...
              'String', 'Ajustar Límites', ...
              'FontSize', 10, ...
              'Position', [160, 10, 120, 30], ...
              'Callback', @adjust_limits_callback);
end

function create_serial_controls(parent)
    % Crear controles para comunicación serial
    global status_text
    
    % Título de sección
    uicontrol('Parent', parent, ...
              'Style', 'text', ...
              'String', 'COMUNICACIÓN SERIAL', ...
              'FontSize', 11, 'FontWeight', 'bold', ...
              'Position', [20, 720, 260, 20], ...
              'HorizontalAlignment', 'center', ...
              'BackgroundColor', [0.9, 0.9, 0.9]);
    
    % Campo de texto para puerto COM
    uicontrol('Parent', parent, ...
              'Style', 'text', ...
              'String', 'Puerto COM:', ...
              'FontSize', 10, ...
              'Position', [20, 690, 80, 20], ...
              'HorizontalAlignment', 'left');
    
    uicontrol('Parent', parent, ...
              'Style', 'edit', ...
              'String', 'COM3', ...
              'FontSize', 10, ...
              'Position', [105, 690, 60, 25], ...
              'Tag', 'com_port_edit', ...
              'HorizontalAlignment', 'center');
    
    % Campo de texto para baud rate
    uicontrol('Parent', parent, ...
              'Style', 'text', ...
              'String', 'Baud Rate:', ...
              'FontSize', 10, ...
              'Position', [175, 690, 70, 20], ...
              'HorizontalAlignment', 'left');
    
    uicontrol('Parent', parent, ...
              'Style', 'edit', ...
              'String', '9600', ...
              'FontSize', 10, ...
              'Position', [250, 690, 50, 25], ...
              'Tag', 'baud_rate_edit', ...
              'HorizontalAlignment', 'center');
    
    % Botones de conexión/desconexión
    uicontrol('Parent', parent, ...
              'Style', 'pushbutton', ...
              'String', 'Conectar', ...
              'FontSize', 10, ...
              'Position', [20, 655, 80, 30], ...
              'Callback', @connect_serial_callback, ...
              'BackgroundColor', [0.7, 1, 0.7]);
    
    uicontrol('Parent', parent, ...
              'Style', 'pushbutton', ...
              'String', 'Desconectar', ...
              'FontSize', 10, ...
              'Position', [110, 655, 80, 30], ...
              'Callback', @disconnect_serial_callback, ...
              'BackgroundColor', [1, 0.7, 0.7]);
    
    % Botón para enviar datos manualmente
    uicontrol('Parent', parent, ...
              'Style', 'pushbutton', ...
              'String', 'Enviar Datos', ...
              'FontSize', 10, ...
              'Position', [200, 655, 80, 30], ...
              'Callback', @send_data_callback, ...
              'BackgroundColor', [0.7, 0.7, 1]);
    
    % Checkbox para envío automático
    uicontrol('Parent', parent, ...
              'Style', 'checkbox', ...
              'String', 'Envío Automático', ...
              'FontSize', 10, ...
              'Position', [20, 620, 150, 25], ...
              'Value', 1, ...
              'Tag', 'auto_send_checkbox');
    
    % Estado de conexión
    status_text = uicontrol('Parent', parent, ...
                           'Style', 'text', ...
                           'String', 'Estado: Desconectado', ...
                           'FontSize', 10, ...
                           'Position', [20, 590, 260, 20], ...
                           'HorizontalAlignment', 'left', ...
                           'BackgroundColor', [1, 0.9, 0.9], ...
                           'ForegroundColor', [0.8, 0, 0]);
    
    % Información de datos enviados
    uicontrol('Parent', parent, ...
              'Style', 'text', ...
              'String', 'Formato: "J1,J2,J3,J4,J5\n"', ...
              'FontSize', 9, ...
              'Position', [20, 565, 260, 15], ...
              'HorizontalAlignment', 'left', ...
              'ForegroundColor', [0, 0, 0.8]);
    
    uicontrol('Parent', parent, ...
              'Style', 'text', ...
              'String', 'Valores en grados [-180 a +180]', ...
              'FontSize', 9, ...
              'Position', [20, 550, 260, 15], ...
              'HorizontalAlignment', 'left', ...
              'ForegroundColor', [0, 0, 0.8]);
end

function create_sliders(parent)
    global sliders angle_values
    
    joint_names = {'Base (J1)', 'Hombro (J2)', 'Codo (J3)', 'Muñeca 1 (J4)', 'Muñeca 2 (J5)'};
    sliders = cell(5, 1);
    
    for i = 1:5
        y_pos = 480 - (i-1) * 80;
        
        % Etiqueta del joint
        uicontrol('Parent', parent, ...
                  'Style', 'text', ...
                  'String', joint_names{i}, ...
                  'FontSize', 10, 'FontWeight', 'bold', ...
                  'Position', [20, y_pos + 30, 200, 20], ...
                  'HorizontalAlignment', 'left');
        
        % Slider
        sliders{i} = uicontrol('Parent', parent, ...
                              'Style', 'slider', ...
                              'Min', -180, 'Max', 180, ...
                              'Value', 0, ...
                              'Position', [20, y_pos, 200, 20], ...
                              'Callback', {@slider_callback, i});
        
        % Etiqueta del valor
        uicontrol('Parent', parent, ...
                  'Style', 'text', ...
                  'String', '0.0°', ...
                  'FontSize', 9, ...
                  'Position', [230, y_pos, 50, 20], ...
                  'HorizontalAlignment', 'center', ...
                  'Tag', sprintf('value_label_%d', i));
    end
end

function connect_serial_callback(~, ~)
    global serial_port serial_connected status_text
    
    try
        % Obtener configuración del puerto
        com_port_edit = findobj('Tag', 'com_port_edit');
        baud_rate_edit = findobj('Tag', 'baud_rate_edit');
        
        port_name = get(com_port_edit, 'String');
        baud_rate = str2double(get(baud_rate_edit, 'String'));
        
        % Validar baud rate
        if isnan(baud_rate) || baud_rate <= 0
            baud_rate = 9600;
            set(baud_rate_edit, 'String', '9600');
        end
        
        % Cerrar puerto anterior si existe
        if ~isempty(serial_port) && isvalid(serial_port)
            fclose(serial_port);
            delete(serial_port);
        end
        
        % Crear y configurar puerto serial
        serial_port = serial(port_name);
        set(serial_port, 'BaudRate', baud_rate);
        set(serial_port, 'DataBits', 8);
        set(serial_port, 'Parity', 'none');
        set(serial_port, 'StopBits', 1);
        set(serial_port, 'FlowControl', 'none');
        set(serial_port, 'Timeout', 1);
        
        % Abrir conexión
        fopen(serial_port);
        
        serial_connected = true;
        
        % Actualizar estado
        set(status_text, 'String', ['Estado: Conectado a ', port_name], ...
                        'BackgroundColor', [0.9, 1, 0.9], ...
                        'ForegroundColor', [0, 0.6, 0]);
        
        fprintf('Conexión serial establecida: %s @ %d baud\n', port_name, baud_rate);
        
        % Enviar datos iniciales
        send_serial_data();
        
    catch ME
        serial_connected = false;
        set(status_text, 'String', ['Error: ', ME.message], ...
                        'BackgroundColor', [1, 0.9, 0.9], ...
                        'ForegroundColor', [0.8, 0, 0]);
        fprintf('Error de conexión serial: %s\n', ME.message);
    end
end

function disconnect_serial_callback(~, ~)
    global serial_port serial_connected status_text
    
    try
        if ~isempty(serial_port) && isvalid(serial_port)
            fclose(serial_port);
            delete(serial_port);
        end
        
        serial_port = [];
        serial_connected = false;
        
        set(status_text, 'String', 'Estado: Desconectado', ...
                        'BackgroundColor', [1, 0.9, 0.9], ...
                        'ForegroundColor', [0.8, 0, 0]);
        
        fprintf('Conexión serial cerrada\n');
        
    catch ME
        fprintf('Error al cerrar conexión serial: %s\n', ME.message);
    end
end

function send_data_callback(~, ~)
    send_serial_data();
end

function send_serial_data()
    global serial_port serial_connected angle_values
    
    if ~serial_connected || isempty(serial_port) || ~isvalid(serial_port)
        return;
    end
    
    try
        % Convertir ángulos a grados
        angles_deg = rad2deg(angle_values);
        
        % Crear string con formato: "J1,J2,J3,J4,J5\n"
        data_string = sprintf('%.2f,%.2f,%.2f,%.2f,%.2f\n', ...
                             angles_deg(1), angles_deg(2), angles_deg(3), ...
                             angles_deg(4), angles_deg(5));
        
        % Enviar datos
        fprintf(serial_port, data_string);
        
        % Mostrar en consola (opcional)
        fprintf('Enviado: %s', data_string);
        
    catch ME
        fprintf('Error al enviar datos: %s\n', ME.message);
        % Marcar como desconectado si hay error de comunicación
        global serial_connected status_text
        serial_connected = false;
        set(status_text, 'String', 'Error: Comunicación perdida', ...
                        'BackgroundColor', [1, 0.9, 0.9], ...
                        'ForegroundColor', [0.8, 0, 0]);
    end
end

function slider_callback(hObject, ~, joint_idx)
    global angle_values
    
    % Obtener valor del slider en grados
    angle_deg = get(hObject, 'Value');
    
    % Convertir a radianes
    angle_values(joint_idx) = deg2rad(angle_deg);
    
    % Actualizar etiqueta del valor
    value_label = findobj('Tag', sprintf('value_label_%d', joint_idx));
    set(value_label, 'String', sprintf('%.1f°', angle_deg));
    
    % Actualizar visualización
    update_robot_display();
    
    % Enviar datos por serial si está habilitado el envío automático
    auto_send_checkbox = findobj('Tag', 'auto_send_checkbox');
    if ~isempty(auto_send_checkbox) && get(auto_send_checkbox, 'Value')
        send_serial_data();
    end
end

function reset_angles_callback(~, ~)
    global sliders angle_values
    
    angle_values = [0, 0, 0, 0, 0];
    
    for i = 1:5
        set(sliders{i}, 'Value', 0);
        value_label = findobj('Tag', sprintf('value_label_%d', i));
        set(value_label, 'String', '0.0°');
    end
    
    update_robot_display();
    
    % Enviar datos por serial si está habilitado el envío automático
    auto_send_checkbox = findobj('Tag', 'auto_send_checkbox');
    if ~isempty(auto_send_checkbox) && get(auto_send_checkbox, 'Value')
        send_serial_data();
    end
end

function center_view_callback(~, ~)
    global ax_handle
    % Vista más cercana y mejor ángulo
    view(ax_handle, 45, 15); % Ángulo más bajo para ver mejor
    set_fixed_axis_limits();
    % Zoom automático más agresivo
    camzoom(ax_handle, 2.5); % Acercar 150% más
end

function adjust_limits_callback(~, ~)
    set_fixed_axis_limits();
end

function auto_fit_callback(~, ~)
    auto_fit_view();
    view(gca, 45, 15); % Vista optimizada
    camzoom(gca, 2.0); % Zoom adicional
end

function zoom_in_callback(~, ~)
    global ax_handle
    camzoom(ax_handle, 1.5);
end

function zoom_out_callback(~, ~)
    global ax_handle
    camzoom(ax_handle, 0.7);
end

function load_urdf_callback(~, ~)
    global robot
    
    [filename, pathname] = uigetfile('*.urdf', 'Seleccionar archivo URDF');
    
    if isequal(filename, 0)
        return; % Usuario canceló
    end
    
    try
        full_path = fullfile(pathname, filename);
        robot = importrobot(full_path);
        msgbox('URDF cargado exitosamente!', 'Éxito');
        update_robot_display();
        set_fixed_axis_limits();
    catch ME
        msgbox(['Error al cargar URDF: ' ME.message], 'Error');
    end
end

function close_gui_callback(~, ~)
    % Función llamada al cerrar la ventana
    global serial_port serial_connected fig_handle
    
    % Cerrar conexión serial si existe
    if serial_connected && ~isempty(serial_port) && isvalid(serial_port)
        try
            fclose(serial_port);
            delete(serial_port);
            fprintf('Conexión serial cerrada al salir\n');
        catch
            % Ignorar errores al cerrar
        end
    end
    
    % Cerrar figura
    delete(fig_handle);
end

function set_fixed_axis_limits()
    % Establece límites MUY ajustados para robot grande
    global ax_handle dh_params
    
    % Calcular el alcance máximo del robot
    max_reach = sum(dh_params(:,2)) + sum(dh_params(:,3)); % suma de d y a
    
    % Límites MUCHO más ajustados - factor 0.4 en lugar de 0.8
    axis_limit = max(max_reach * 0.4, 0.3); % límite mínimo muy pequeño
    
    % Límites muy cercanos y asimétricos
    set(ax_handle, 'XLim', [-axis_limit*0.3, axis_limit*0.8]); % Muy poco espacio atrás
    set(ax_handle, 'YLim', [-axis_limit*0.6, axis_limit*0.6]); % Centrado en Y
    set(ax_handle, 'ZLim', [0, axis_limit * 0.9]); % Altura limitada
    
    % Mantener proporciones iguales
    axis(ax_handle, 'equal');
    
    % Fijar los límites
    set(ax_handle, 'XLimMode', 'manual');
    set(ax_handle, 'YLimMode', 'manual');
    set(ax_handle, 'ZLimMode', 'manual');
    
    grid(ax_handle, 'on');
end

function auto_fit_view()
    % Función para ajustar automáticamente la vista al robot
    global ax_handle robot angle_values
    
    try
        % Calcular la envolvente del robot en la configuración actual
        [T_final, ~] = forward_kinematics();
        
        % Obtener posición del efector final
        end_pos = T_final(1:3, 4);
        
        % Límites ULTRA ajustados basados en configuración actual
        range_x = max(0.2, abs(end_pos(1)) * 0.7); % Muy ajustado
        range_y = max(0.2, abs(end_pos(2)) * 0.7);
        range_z = max(0.3, abs(end_pos(3)) * 0.8);
        
        % Centrar en la posición del robot
        center_x = end_pos(1) * 0.3; % Ligeramente adelante
        center_y = end_pos(2) * 0.3;
        
        % Aplicar límites súper ajustados
        set(ax_handle, 'XLim', [center_x - range_x*0.4, center_x + range_x*0.8]);
        set(ax_handle, 'YLim', [center_y - range_y*0.6, center_y + range_y*0.6]);
        set(ax_handle, 'ZLim', [0, range_z]);
        
    catch
        % Si falla, usar límites súper ajustados por defecto
        set(ax_handle, 'XLim', [-0.2, 0.6]);
        set(ax_handle, 'YLim', [-0.4, 0.4]);
        set(ax_handle, 'ZLim', [0, 0.8]);
    end
    
    % Mantener configuraciones
    axis(ax_handle, 'equal');
    set(ax_handle, 'XLimMode', 'manual');
    set(ax_handle, 'YLimMode', 'manual');
    set(ax_handle, 'ZLimMode', 'manual');
    grid(ax_handle, 'on');
end

function T = dh_transform(theta, d, a, alpha)
    % Calcula matriz de transformación homogénea usando parámetros DH
    
    ct = cos(theta);
    st = sin(theta);
    ca = cos(alpha);
    sa = sin(alpha);
    
    T = [ct, -st*ca,  st*sa, a*ct;
         st,  ct*ca, -ct*sa, a*st;
         0,   sa,     ca,    d;
         0,   0,      0,     1];
end

function [T_final, T_matrices] = forward_kinematics()
    % Calcula cinemática directa usando parámetros DH
    global dh_params angle_values
    
    T_matrices = cell(5, 1);
    T_cumulative = eye(4);
    
    for i = 1:5
        % Obtener parámetros DH
        theta = dh_params(i, 1) + angle_values(i); % theta_offset + joint_angle
        d = dh_params(i, 2);
        a = dh_params(i, 3);
        alpha = dh_params(i, 4);
        
        % Calcular matriz de transformación
        T_i = dh_transform(theta, d, a, alpha);
        T_matrices{i} = T_i;
        
        % Acumular transformación
        T_cumulative = T_cumulative * T_i;
    end
    
    T_final = T_cumulative;
end

function update_robot_display()
    global robot ax_handle mth_text angle_values
    
    try
        % Calcular MTH usando DH
        [T_final, ~] = forward_kinematics();
        
        % Mostrar robot usando MATLAB Robotics Toolbox
        axes(ax_handle);
        cla(ax_handle);
        
        % Crear configuración para el robot
        config = homeConfiguration(robot);
        
        % Asignar ángulos (asumiendo que el orden coincide)
        for i = 1:min(length(config), length(angle_values))
            config(i).JointPosition = angle_values(i);
        end
        
        % Mostrar robot SIN permitir que ajuste automáticamente los límites
        show(robot, config, 'Parent', ax_handle, 'Frames', 'off', 'PreservePlot', false);
        
        % Restaurar los límites fijos después de mostrar el robot
        restore_axis_settings();
        
        % Mostrar sistema de coordenadas del efector final (más grande)
        hold(ax_handle, 'on');
        plot_coordinate_frame(ax_handle, T_final, 0.08); % Marcos más grandes
        hold(ax_handle, 'off');
        
    catch
        % Si falla el URDF, dibujar usando solo DH
        plot_robot_dh();
    end
    
    % Actualizar información de MTH
    update_mth_display(T_final);
end

function restore_axis_settings()
    % Restaura las configuraciones súper ajustadas de los ejes
    global ax_handle dh_params
    
    % Usar los mismos límites súper ajustados
    max_reach = sum(dh_params(:,2)) + sum(dh_params(:,3));
    axis_limit = max(max_reach * 0.4, 0.3);
    
    % Restaurar límites súper ajustados
    set(ax_handle, 'XLim', [-axis_limit*0.3, axis_limit*0.8]);
    set(ax_handle, 'YLim', [-axis_limit*0.6, axis_limit*0.6]);
    set(ax_handle, 'ZLim', [0, axis_limit * 0.9]);
    
    % Restaurar configuraciones
    axis(ax_handle, 'equal');
    set(ax_handle, 'XLimMode', 'manual');
    set(ax_handle, 'YLimMode', 'manual');
    set(ax_handle, 'ZLimMode', 'manual');
    
    grid(ax_handle, 'on');
    title(ax_handle, 'Robot 5 GDL - Vista 3D', 'FontSize', 12);
    xlabel(ax_handle, 'X (m)');
    ylabel(ax_handle, 'Y (m)');
    zlabel(ax_handle, 'Z (m)');
end

function plot_robot_dh()
    % Dibuja el robot usando solo los cálculos DH (fallback)
    global ax_handle
    
    [T_final, T_matrices] = forward_kinematics();
    
    axes(ax_handle);
    cla(ax_handle);
    
    % Calcular posiciones de cada joint
    positions = zeros(6, 3); % 6 puntos (base + 5 joints)
    T_cumulative = eye(4);
    
    positions(1, :) = T_cumulative(1:3, 4)'; % Base
    
    for i = 1:5
        T_cumulative = T_cumulative * T_matrices{i};
        positions(i+1, :) = T_cumulative(1:3, 4)';
    end
    
    % Dibujar enlaces más gruesos
    plot3(ax_handle, positions(:, 1), positions(:, 2), positions(:, 3), ...
          'b-', 'LineWidth', 5);
    hold(ax_handle, 'on');
    
    % Dibujar joints más grandes
    scatter3(ax_handle, positions(:, 1), positions(:, 2), positions(:, 3), ...
             150, 'r', 'filled');
    
    % Dibujar sistema de coordenadas final
    plot_coordinate_frame(ax_handle, T_final, 0.08);
    
    % Restaurar configuraciones de ejes
    restore_axis_settings();
    view(ax_handle, 45, 15);
    
    hold(ax_handle, 'off');
end

function plot_coordinate_frame(ax, T, scale)
    % Dibuja un sistema de coordenadas en la posición dada por T
    
    origin = T(1:3, 4);
    x_axis = T(1:3, 1) * scale;
    y_axis = T(1:3, 2) * scale;
    z_axis = T(1:3, 3) * scale;
    
    % Eje X (rojo) - más grueso
    quiver3(ax, origin(1), origin(2), origin(3), ...
            x_axis(1), x_axis(2), x_axis(3), ...
            'r', 'LineWidth', 3, 'MaxHeadSize', 0.4);
    
    % Eje Y (verde) - más grueso
    quiver3(ax, origin(1), origin(2), origin(3), ...
            y_axis(1), y_axis(2), y_axis(3), ...
            'g', 'LineWidth', 3, 'MaxHeadSize', 0.4);
    
    % Eje Z (azul) - más grueso
    quiver3(ax, origin(1), origin(2), origin(3), ...
            z_axis(1), z_axis(2), z_axis(3), ...
            'b', 'LineWidth', 3, 'MaxHeadSize', 0.4);
end

function update_mth_display(T_final)
    global mth_text
    
    % Extraer información de la MTH final
    position = T_final(1:3, 4);
    rotation_matrix = T_final(1:3, 1:3);
    
    % Convertir matriz de rotación a ángulos de Euler (ZYX)
    euler_angles = rotm2eul(rotation_matrix, 'ZYX') * 180/pi;
    
    % Formatear la matriz para mostrar
    matrix_str = sprintf('MTH FINAL (Base → Efector Final):\n\n');
    matrix_str = [matrix_str, sprintf('[%8.4f  %8.4f  %8.4f  %8.4f]\n', T_final(1,:))];
    matrix_str = [matrix_str, sprintf('[%8.4f  %8.4f  %8.4f  %8.4f]\n', T_final(2,:))];
    matrix_str = [matrix_str, sprintf('[%8.4f  %8.4f  %8.4f  %8.4f]\n', T_final(3,:))];
    matrix_str = [matrix_str, sprintf('[%8.4f  %8.4f  %8.4f  %8.4f]\n', T_final(4,:))];
    
    matrix_str = [matrix_str, sprintf('\n\nINFORMACIÓN DEL EFECTOR FINAL:\n')];
    matrix_str = [matrix_str, sprintf('═══════════════════════════════════\n')];
    matrix_str = [matrix_str, sprintf('Posición:\n')];
    matrix_str = [matrix_str, sprintf('  X = %8.4f m\n', position(1))];
    matrix_str = [matrix_str, sprintf('  Y = %8.4f m\n', position(2))];
    matrix_str = [matrix_str, sprintf('  Z = %8.4f m\n', position(3))];
    matrix_str = [matrix_str, sprintf('\nOrientación (Euler ZYX):\n')];
    matrix_str = [matrix_str, sprintf('  Roll  (X) = %7.2f°\n', euler_angles(3))];
    matrix_str = [matrix_str, sprintf('  Pitch (Y) = %7.2f°\n', euler_angles(2))];
    matrix_str = [matrix_str, sprintf('  Yaw   (Z) = %7.2f°\n', euler_angles(1))];
    
    % Distancia desde el origen
    distance = norm(position);
    matrix_str = [matrix_str, sprintf('\nDistancia desde origen: %.4f m\n', distance)];
    
    set(mth_text, 'String', matrix_str);
end

