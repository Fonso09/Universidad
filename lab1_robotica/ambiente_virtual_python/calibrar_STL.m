function calibrar_STL()
    % === Configuración del robot ===
    L1 = 0.200; L3 = 0.300; L4 = 0.150;
    robot = rigidBodyTree('DataFormat','row','MaxNumBodies',5);

    % Base fija
    addVisual(robot.Base,'Mesh','base_fija.STL',eye(4));

    % J1
    b1 = rigidBody('link1');
    j1 = rigidBodyJoint('j1','revolute');
    setFixedTransform(j1,[0,  pi/2,  L1,  0],'dh');
    b1.Joint = j1;
    addVisual(b1,'Mesh','J0.STL',eye(4));
    addBody(robot,b1,'base');

    % J2
    b2 = rigidBody('link2');
    j2 = rigidBodyJoint('j2','revolute');
    setFixedTransform(j2,[0,0,0,0],'dh');
    b2.Joint = j2;
    addVisual(b2,'Mesh','J1.STL',eye(4));
    addBody(robot,b2,'link1');

    % J3
    b3 = rigidBody('link3');
    j3 = rigidBodyJoint('j3','revolute');
    setFixedTransform(j3,[L3, pi/2, 0, 0],'dh');
    j3.HomePosition = pi/2;
    b3.Joint = j3;
    addVisual(b3,'Mesh','J2.STL',eye(4));
    addBody(robot,b3,'link2');

    % J4
    b4 = rigidBody('link4');
    j4 = rigidBodyJoint('j4','revolute');
    setFixedTransform(j4,[0,-pi/2,L4,0],'dh');
    b4.Joint = j4;
    addVisual(b4,'Mesh','J3.STL',eye(4));
    addBody(robot,b4,'link3');

    % J5
    b5 = rigidBody('link5');
    j5 = rigidBodyJoint('j5','revolute');
    setFixedTransform(j5,[0,0,0,0],'dh');
    b5.Joint = j5;
    addVisual(b5,'Mesh','J4.STL',eye(4));
    addBody(robot,b5,'link4');

    % === Interfaz gráfica ===
    f = uifigure('Name','Calibrar STL','Position',[100 100 650 550]);

    % Menú desplegable para elegir eslabón
    dd = uidropdown(f,'Position',[20 500 120 22],...
        'Items',{'J0','J1','J2','J3','J4'},'Value','J0');

    % Sliders para traslación y rotación
    labels = {'tx','ty','tz','rx','ry','rz'};
    sliders = gobjects(1,6);
    for i=1:6
        uilabel(f,'Position',[20 470-50*(i-1) 30 22],'Text',labels{i});
        sliders(i) = uislider(f,'Position',[60 480-50*(i-1) 400 3],...
            'Limits',[-180 180],'Value',0);
    end
    % Ajustar rangos: traslación ±0.2m, rotación ±180°
    for i=1:3, sliders(i).Limits = [-0.2 0.2]; end
    for i=4:6, sliders(i).Limits = [-180 180]; end

    % Botón actualizar
    btn = uibutton(f,'Text','Actualizar','Position',[500 500 80 25],...
        'ButtonPushedFcn',@(src,event) actualizar());

    % === Panel + axes clásico para show() ===
    pnl = uipanel(f,'Position',[20 20 600 250]);
    ax = axes('Parent',pnl);
    show(robot,[0 0 0 0 0],'Parent',ax,'Visuals','on');
    view(ax,135,20); axis(ax,'equal'); grid(ax,'on');

    % === Función actualizar ===
    % === Función actualizar ===
    function actualizar()
        % Obtener valores sliders
        tx = sliders(1).Value; ty = sliders(2).Value; tz = sliders(3).Value;
        rx = sliders(4).Value*pi/180; ry = sliders(5).Value*pi/180; rz = sliders(6).Value*pi/180;

        % Construir Tvis
        T = trvec2tform([tx ty tz]) * ...
            axang2tform([1 0 0 rx]) * ...
            axang2tform([0 1 0 ry]) * ...
            axang2tform([0 0 1 rz]);

        % Mostrar en consola
        fprintf('\n%s: Tvis = \n',dd.Value);
        disp(T);
        fprintf('trvec = [%.3f %.3f %.3f], rot (deg) = [%.1f %.1f %.1f]\n',...
            tx,ty,tz,sliders(4).Value,sliders(5).Value,sliders(6).Value);

        % Redibujar todo
        cla(ax);  % limpia los ejes
        show(robot,[0 0 0 0 0],'Parent',ax,'Visuals','on');
        hold(ax,'on');

        % Cargar STL correspondiente y dibujar con la nueva Tvis
        switch dd.Value
            case 'J0', stlFile = 'J0.STL';
            case 'J1', stlFile = 'J1.STL';
            case 'J2', stlFile = 'J2.STL';
            case 'J3', stlFile = 'J3.STL';
            case 'J4', stlFile = 'J4.STL';
        end

        [fvc,vcs] = stlread(stlFile);  % necesitas tener función stlread
        % aplicar T a los vértices
        verts = (T(1:3,1:3)*vcs' + T(1:3,4))';
        patch(ax,'Faces',fvc,'Vertices',verts,...
            'FaceColor',[0.8 0.8 1],'EdgeColor','none','FaceAlpha',0.7);

        hold(ax,'off');
        view(ax,135,20); axis(ax,'equal'); grid(ax,'on');
    end

end
