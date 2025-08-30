%% ----- Robot 5GDL: DH estándar + STL en postura inicial -----
clear; clc; close all

% ====== Longitudes (EDITA ESTOS VALORES) ======
L1 = 0.0435;   % m
L3 = 0.105;   % m
L4 = 0.02693;   % m

% ====== Construcción del árbol rígido (DH estándar) ======
robot = rigidBodyTree('DataFormat','row','MaxNumBodies',5);

% --- Base fija (añadir visual directamente al nodo Base) ---
addVisual(robot.Base,'Mesh','base_fija.STL',eye(4));

% ====== J1 ======
b1 = rigidBody('link1');
j1 = rigidBodyJoint('j1','revolute');
% DH: [a alpha d theta]  ->  [0, +pi/2, L1, q1]
setFixedTransform(j1,[0,  pi/2,  L1,  0],'dh');
b1.Joint = j1;
Tvis1 = trvec2tform([10 50 30Sí m])*axang2tform([1 0 0 -pi/2]);
addVisual(b1,'Mesh','J0.STL',Tvis1);
addBody(robot,b1,'base');

% ====== J2 ======
b2 = rigidBody('link2');
j2 = rigidBodyJoint('j2','revolute');
setFixedTransform(j2,[0,  0,  0,  0],'dh');
b2.Joint = j2;
Tvis2 = eye(4);
addVisual(b2,'Mesh','J1.STL',Tvis2);
addBody(robot,b2,'link1');

% ====== J3 ======
b3 = rigidBody('link3');
j3 = rigidBodyJoint('j3','revolute');
setFixedTransform(j3,[L3,  pi/2,  0,  0],'dh');
j3.HomePosition = pi/2;   % compensación q3+pi/2
b3.Joint = j3;
Tvis3 = eye(4);
addVisual(b3,'Mesh','J2.STL',Tvis3);
addBody(robot,b3,'link2');

% ====== J4 ======
b4 = rigidBody('link4');
j4 = rigidBodyJoint('j4','revolute');
setFixedTransform(j4,[0, -pi/2,  L4,  0],'dh');
b4.Joint = j4;
Tvis4 = eye(4);
addVisual(b4,'Mesh','J3.STL',Tvis4);
addBody(robot,b4,'link3');

% ====== J5 ======
b5 = rigidBody('link5');
j5 = rigidBodyJoint('j5','revolute');
setFixedTransform(j5,[0, 0, 0, 0],'dh');
b5.Joint = j5;
Tvis5 = eye(4);
addVisual(b5,'Mesh','J4.STL',Tvis5);
addBody(robot,b5,'link4');

% ====== Postura inicial ======
q0 = [0 0 0 0 0]; % q1..q5 = 0
figure('Color','w'); 
show(robot,q0,'Visuals','on'); 
axis equal
view(135,20); grid on
title('Robot 5GDL – STL en postura inicial (q = 0)');


%% ====== Notas de calibración ======
% - Si alguna malla no cae bien sobre su eje/articulación:
%     * ajusta Tvis# (rototraslación del STL respecto al marco del eslabón).
%         p.ej.:  Tvis1 = trvec2tform([dx dy dz]) * axang2tform([1 0 0 rx]) * ...
%     * o corrige el desfase angular con: j#.HomePosition = j#.HomePosition + delta;
% - Asegúrate de que las unidades del STL estén en metros. Si tus STL están en mm,
%   conviértelos previamente (addVisual no admite escala en la transformada).
