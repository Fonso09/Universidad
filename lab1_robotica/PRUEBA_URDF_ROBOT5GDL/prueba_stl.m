%% Ángulos y distancias:
q1= 0;
q2= 0;
q3= 0;
q4= 0;
q5= 0;
L1=  0.093;
L2= 0.14;
L3= 0.103;
L4 = 0.0284;

%% CREAMOS LA FIGURA
figure; hold on; axis equal; view(3);
xlabel('X'); ylabel('Y'); zlabel('Z');

%%CARGAMOS STLs: 

% Cargar STL 1, que es la base fija
%Para manipularlo usar Fb, Vb y Tb
TRb = stlread("URDF_ROBOT5GDL/meshes/base_real.stl");
Fb = TRb.ConnectivityList;
Vb = TRb.Points* 0.001;
Tb = eye(4);   % como es la base, queremos que esté en el origen entonces le pasamos una transformación vacía
drawSTL(Fb, Vb, Tb, [0.8 0.8 1]);   % azul

% Cargar STL 2, que es la base rotatoria
%Para manipularlo usar F0, V0 y T0
TR0 = stlread("URDF_ROBOT5GDL/meshes/J0_v2.stl");
F0 = TR0.ConnectivityList;
V0 = TR0.Points* 0.001;
T0 = transl(0,0,0.05)*trotz(q1); %  
drawSTL(F0, V0, T0, [1 0.6 0.6]);   % rojo

% Cargar STL 3, que es el brazo 1, o sea J1
TR1 = stlread("URDF_ROBOT5GDL/meshes/J1_v2.stl");
F1 = TR1.ConnectivityList;
V1 = TR1.Points* 0.001;
Cq1= cos(q1);
Sq1= sin(q1);
A1= [Cq1 0 -Sq1 0;
    Sq1 0 -Cq1 0;
    0 1 0 L1;
    0 0 0 1];
T1 = A1*trotz(q2);
drawSTL(F1, V1, T1, [0.6 1 0.6]);   % verde

% Cargar STL 4, que es el brazo 2, o sea J2
%Para manipularlo usar F2, V2 y T2 
TR2 = stlread("URDF_ROBOT5GDL/meshes/J2_v2.stl");
F2 = TR2.ConnectivityList;
V2 = TR2.Points* 0.001;
Cq2= cos(q2);
Sq2= sin(q2);
A2= [Cq2 -Sq2 0 L2*Cq2;
    Sq2 Cq2 0 L2*Sq2;
    0 0 1 0;
    0 0 0 1];
T2= A1* A2*trotz(q3);
%T4 = transl(0,0,0.22)*trotx(-pi/2); % desplazado 
drawSTL(F2, V2, T2, [0.6 0.6 1]);  

% Cargar STL 5, que es el brazo 3, o sea J3
%Para manipular usar F3, V3 y T3
TR3 = stlread("URDF_ROBOT5GDL/meshes/J3_v2.stl");
F3 = TR3.ConnectivityList;
V3 = TR3.Points*0.001;
Cq3= cos(q3+pi/2);
Sq3= sin(q3+pi/2);
A3 = [Cq3 0 Sq3 0;
    Sq3 0 -Cq3 0;
    0 1 0 0;
    0 0 0 1];
T3 = A1*A2*A3*transl(0,0,L3)*trotz(q4);
%T5 = transl(0,0,0.27); % desplazado 
drawSTL(F3, V3, T3, [1 0.6 0.6]); 

% Cargar STL 6, que es la base del gripper, o sea J4
% Para manipular usar F4, V4_new y T4
TR4 = stlread("URDF_ROBOT5GDL/meshes/J4_v3STL.stl");
F4 = TR4.ConnectivityList;
V4 = TR4.Points*0.001;
offset = transl(-0.01, -0.01, -0.033);   % OFFSET PARA QUE EL EJE CUADRE
V4_new = transformSTL(V4, offset);    % reescribir vértices
Cq4= cos(q4+pi);
Sq4= sin(q4+pi);
A4 = [Cq4 0 Sq4 0;
    Sq4 0 -Cq4 0;
    0 1 0 L3+L4;
    0 0 0 1];
T4 = A1*A2*A3*A4*trotz(q5);
%R_offset = trotx(-pi/2);    % rotar 90° en Z
%V6_rotated = (R_offset(1:3,1:3) * V6_new')';  % aplicar solo la rotación a vértices

%T6 = transl(0,0,0.35); % desplazado 
drawSTL(F4, V4_new, T4, [0.3 1 0.3]);




camlight; lighting gouraud;
%% FUNCIONES FUNCIONALES XD
function [Vt] = transformSTL(V, T)
%ESTA FUNCIÓN ES LA QUE HACE TODA LA MAGIA, NOS PERMITE TRANSFORMAR
%GEOMETRICAMENTE CADA STL USANDO SU MARCO DE REFERENCIA, LO HACE
%DESTRUYENDO Y VOLVIENDO A ARMRAR SUS VÉRTICES

    Vh = [V ones(size(V,1),1)];   % convertir a homogéneo
    Vt = (T * Vh')';              % aplicar T
    Vt = Vt(:,1:3);               % volver a 3D
end


function drawSTL(F, V, T, color)
    Vt = transformSTL(V, T); 

    % Dibujar con trisurf en lugar de patch
    TR = triangulation(F, Vt);
    trisurf(TR, 'FaceColor',color, ...
                 'EdgeColor','none', ...
                 'FaceAlpha',0.9);

    % Dibujar ejes
    O = T(1:3,4);
    L = 0.05;
    quiver3(O(1),O(2),O(3), T(1,1)*L, T(2,1)*L, T(3,1)*L,'r','LineWidth',2);
    quiver3(O(1),O(2),O(3), T(1,2)*L, T(2,2)*L, T(3,2)*L,'g','LineWidth',2);
    quiver3(O(1),O(2),O(3), T(1,3)*L, T(2,3)*L, T(3,3)*L,'b','LineWidth',2);
end

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

