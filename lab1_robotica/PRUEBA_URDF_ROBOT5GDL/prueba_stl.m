function [Vt] = transformSTL(V, T)
%ESTA FUNCIÓN ES LA QUE HACE TODA LA MAGIA, NOS PERMITE TRANSFORMAR
%GEOMETRICAMENTE CADA STL USANDO SU MARCO DE REFERENCIA, LO HACE
%DESTRUYENDO Y VOLVIENDO A ARMRAR SUS VÉRTICES

    Vh = [V ones(size(V,1),1)];   % convertir a homogéneo
    Vt = (T * Vh')';              % aplicar T
    Vt = Vt(:,1:3);               % volver a 3D
end


function drawSTL(F, V, T, color)
    Vt = transformSTL(V, T); % Aquí se vuelven a dibujar los vértices según las tranformación que le pasemos xd
    % Dibujar STL
    patch('Faces',F,'Vertices',Vt, ...
          'FaceColor',color, ...
          'EdgeColor','none','FaceAlpha',0.9);

    % DDIBUJA LOS EJES DE CADA ESLABÓN
    O = T(1:3,4);    % origen
    L = 0.05;        % QUÉ TAN LARGO QUERÉS LOS EJES PIBEEEE
    quiver3(O(1),O(2),O(3), T(1,1)*L, T(2,1)*L, T(3,1)*L,'r','LineWidth',2); % X EN ROJO
    quiver3(O(1),O(2),O(3), T(1,2)*L, T(2,2)*L, T(3,2)*L,'g','LineWidth',2); % Y EN VERDE
    quiver3(O(1),O(2),O(3), T(1,3)*L, T(2,3)*L, T(3,3)*L,'b','LineWidth',2); % Z EN AZUL 
end


figure; hold on; axis equal; view(3);
xlabel('X'); ylabel('Y'); zlabel('Z');



% Cargar STL 1
TR1 = stlread("URDF_ROBOT5GDL/meshes/base_real.stl");
F1 = TR1.ConnectivityList;
V1 = TR1.Points* 0.001;
T1 = eye(4);   % en el origen
drawSTL(F1, V1, T1, [0.8 0.8 1]);   % azul

% Cargar STL 2
TR2 = stlread("URDF_ROBOT5GDL/meshes/J0_v2.stl");
F2 = TR2.ConnectivityList;
V2 = TR2.Points* 0.001;
T2 = transl(0,0,0.1); % desplazado 
drawSTL(F2, V2, T2, [1 0.6 0.6]);   % rojo

% Cargar STL 3
TR3 = stlread("URDF_ROBOT5GDL/meshes/J1_v2.stl");
F3 = TR3.ConnectivityList;
V3 = TR3.Points* 0.001;
T3 = transl(0,0,0.18)*trotx(pi/2); % desplazado 
drawSTL(F3, V3, T3, [0.6 1 0.6]);   % verde
% Cargar STL 4
TR4 = stlread("URDF_ROBOT5GDL/meshes/J2_v2.stl");
F4 = TR4.ConnectivityList;
V4 = TR4.Points* 0.001;
T4 = transl(0,0,0.22)*trotx(-pi/2); % desplazado 
drawSTL(F4, V4, T4, [0.6 0.6 1]);  
% Cargar STL 5
TR5 = stlread("URDF_ROBOT5GDL/meshes/J3_v2.stl");
F5 = TR5.ConnectivityList;
V5 = TR5.Points*0.001;
T5 = transl(0,0,0.27); % desplazado 
drawSTL(F5, V5, T5, [1 0.6 0.6]); 
% Cargar STL 6
TR6 = stlread("URDF_ROBOT5GDL/meshes/J4_v3STL.stl");
F6 = TR6.ConnectivityList;
V6 = TR6.Points*0.001;
offset = transl(-0.01, -0.01, -0.033);   % OFFSET PARA QUE EL EJE CUADRE
V6_new = transformSTL(V6, offset);    % reescribir vértices
%R_offset = trotx(-pi/2);    % rotar 90° en Z
%V6_rotated = (R_offset(1:3,1:3) * V6_new')';  % aplicar solo la rotación a vértices

T6 = transl(0,0,0.35); % desplazado 
drawSTL(F6, V6_new, T6, [0.3 1 0.3]);

camlight; lighting gouraud;



%% FUNCIONES PRIMIGENIAS:
%Traslación
function T = transl(x,y,z)
    T = eye(4);
    T(1:3,4) = [x;y;z];
end
%ROTACIÓN EN Z
function T = trotz(theta)
    T = [cos(theta) -sin(theta) 0 0;
         sin(theta)  cos(theta) 0 0;
         0           0          1 0;
         0           0          0 1];
end
%ROTACIÓN EN X
function T = trotx(theta)
    T = [1 0 0 0;
         0 cos(theta) -sin(theta) 0;
         0 sin(theta) cos(theta) 0;
         0 0 0 1];
end
%ROTACIÓN EN Y
function T = troty(theta)
    T = [cos(theta) 0 sin(theta) 0;
         0 1 0 0;
         -sin(theta) 0 cos(theta) 0;
         0 0 0 1];
end