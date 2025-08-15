clc;
% Constantes <3
k1 = 323;     % N/m
k2 = 556;     % N/m
b1 = 432;   % Ns/m
b2 = 23;   % Ns/m
M  = 50;   % kg
m  = 43;   % kg
g  = 9.81;  % m/s^2
L  = 1;   % m
xb= 0.5; %m
% Theta operacion <3
numerador_theta = (k1 + k2) * (1 - 1/m) * xb;
denominador_theta = g * (M * (1 - 1/m) - 1);
thetaop = asin(numerador_theta / denominador_theta); % en radianes

%Fuerza 1 operacion <3
F1 = ((k2 + k1)/m)*xb + ((M/m) - 1)*g*sin(thetaop);

% Matriz A <3
A = [0 1 0 0;
    -((k1 + k2)/m) -((b1 + b2)/m) (M * g * cos(thetaop))/m 0;
     0 0 0 1;
     (k1 + k2)/(m*L) (b1 + b2)/(m*L) ((-M/m - 1)*g*cos(thetaop))/L 0];

% Matriz B <3
B = [0;
     1/m;
     0;
    -1/L];

%  Matriz C <3
C = [1 0 0 0];

% Matriz D <3
D = 0;

% Matriz E <3
E = [0;
     0;
     0;
    -1/L];

% Función de transferencia <3
[num, den] = ss2tf(A, B, C, D);
G = tf(num, den);

% Cpnsola <3
disp('Matriz A:');
A

disp('Matriz B:');
B

disp('Matriz C:');
C

disp('Matriz D:');
D

disp('Ángulo theta en radianes:');
thetaop

disp('Fuerza en N');
F1

disp('Función de transferencia G(s):');
G

% Matriz de controlabilidad y su determinante
S = [B A*B A^2*B A^3*B];
det_Co = det(S);
disp('Determinante de la matriz de controlabilidad:');
disp(det_Co)
if det_Co ~= 0
    disp('El sistema ES controlable (det diferente de 0).');
else
    disp('El sistema NO es controlable (det igual a 0).');
end

%Matriz M <3 (se modifican de acuerdo a el denominador)
M=[50 279.4 490.2 1; 279.4 490.5 1 0; 490.5 1 0 0; 1 0 0 0];
disp('Matriz M:');
M
%Matriz T <3
T=S*M;
%Cálculo de las matrices A B C E en su fcc <3
Afcc = inv(T) * A * T;
Bfcc = inv(T) * B;
Cfcc = C * T;
Efcc=inv(T)*E;


% Matriz de observabilidad
Ob = [C; C*A; C*A^2; C*A^3];

% Verificas que sea invertible
if det(Ob) ~= 0
    disp("Sistema observable");
else
    error("Sistema NO observable");
end
%Cálculo de las matrices A B C E en su fco <3

Q = inv(M*Ob);  % Transformación para forma observable

% Transformaciones
Afco = inv(Q)*A*Q;
Bfco = inv(Q) * B;
Cfco = C * Q;
Efco = inv(Q) * E;
Cfco

%DISEÑO CONTROLADOR
syms k1s k2s k3s k4s kis real
syms s
Ks = [k1s k2s k3s k4s];

% --- A* Original ---
Astar = [A - B*Ks,     B*kis;
         -C       ,    0];

disp('A* (original):');
disp(simplify(Astar));
char_poly = simplify(det(s*eye(5) - Astar));  % Para original
disp('Polinomio característico de A* (original):');
pretty(vpa(char_poly,4))
% --- A* FCC ---
Astar_fcc = [Afcc - Bfcc*Ks,    Bfcc*kis;
             -Cfcc         ,       0];

disp('A* (FCC):');
disp(simplify(Astar_fcc));
char_poly_fcc = simplify(det(s*eye(5) - Astar_fcc));
disp('Polinomio característico de A* (FCC):');
pretty(vpa(char_poly_fcc,4))
% --- A* FCO ---
Astar_fco = [Afco - Bfco*Ks,    Bfco*kis;
             -Cfco         ,        0];

disp('A* (FCO):');
disp(simplify(Astar_fco));
char_poly_fco = simplify(det(s*eye(5) - Astar_fco));
disp('Polinomio característico de A* (FCO):');
pretty(vpa(char_poly_fco,4))
%Asignacion de polos
% Polinomio deseado
pd = [1 25.58 231.4 848.6 1066.8 666.2];

% Polinomio característico de A*
%char_poly = simplify(det(s*eye(5) - Astar));

% Extraer los coeficientes del polinomio de A* (en función de las Ks)
%coeffs_astar = coeffs(char_poly, s, 'All');

% Igualar con el polinomio deseado
%eqs = coeffs_astar == pd;

% Resolver para las Ks
%sol = solve(eqs, [k1s, k2s, k3s, k4s, kis]);

% Mostrar soluciones
%disp('Solución de ganancias K:');
%disp(sol.k1s)
%disp(sol.k2s)
%disp(sol.k3s)
%disp(sol.k4s)
%disp(sol.kis)

% Polinomio característico de A*_FCC
%char_poly_fcc = simplify(det(s*eye(5) - Astar_fcc));

% Extraer los coeficientes del polinomio de A*_FCC (en función de las Ks)
%coeffs_astar_fcc = coeffs(char_poly_fcc, s, 'All');

% Igualar con el polinomio deseado
%eqs_fcc = coeffs_astar_fcc == pd;

% Resolver para las Ks
%sol_fcc = solve(eqs_fcc, [k1s, k2s, k3s, k4s, kis]);

% Mostrar soluciones de forma decimal (simplificadas visualmente)
%disp('--- Solución de ganancias K para FCC (forma decimal) ---');
%disp(['k1 = ', char(vpa(sol_fcc.k1s, 8))]);
%disp(['k2 = ', char(vpa(sol_fcc.k2s, 8))]);
%disp(['k3 = ', char(vpa(sol_fcc.k3s, 8))]);
%disp(['k4 = ', char(vpa(sol_fcc.k4s, 8))]);
%disp(['ki = ', char(vpa(sol_fcc.kis, 8))]);


% Polinomio característico de A*_FCO
%char_poly_fco = simplify(det(s*eye(5) - Astar_fco));

% Extraer los coeficientes del polinomio de A*_FCO (en función de las Ks)
%coeffs_astar_fco = coeffs(char_poly_fco, s, 'All');

% Igualar con el polinomio deseado
%eqs_fco = coeffs_astar_fco == pd;

% Resolver para las Ks
%sol_fco = solve(eqs_fco, [k1s, k2s, k3s, k4s, kis]);

% Mostrar soluciones en forma decimal (simplificadas visualmente)
%disp('--- Solución de ganancias K para FCO (forma decimal) ---');
%disp(['k1 = ', char(vpa(sol_fco.k1s, 8))]);
%disp(['k2 = ', char(vpa(sol_fco.k2s, 8))]);
%disp(['k3 = ', char(vpa(sol_fco.k3s, 8))]);
%disp(['k4 = ', char(vpa(sol_fco.k4s, 8))]);
%disp(['ki = ', char(vpa(sol_fco.kis, 8))]);

%ASIGNACION DE POLOS

KA = [-46190/327, 28467610/962361, -3048419/32700, 2598423281/48118050];
kiA = 66620/981;

KAfcc=[-22691668,516984.21, -10797.714,-24.42];
kiAfcc=[67.910289];

KAfco=[2.7523163,-1216.321,596316.56,-292154900.0 ];
kiAfco=[67.910296]

%AKERMAN





% se hace un polinomio deseado nuevo debido a que el tiempo de
% establecimiento debe ser más corto:
%% Observabilidad para FCO
coeffs_poly = [1 86.24 2182.656 14505.8816 48189.0304];
poles_obs = roots(coeffs_poly);
% mediante place que es asignación de polos
k_obs_asig_polos = place(Afco', Cfco', poles_obs);
disp('K observable mediante asignación de polos:');
disp(k_obs_asig_polos);
%mediante Ackermann
k_obs_acker = acker(Afco', Cfco', poles_obs);
disp('K observable mediante ackerman:');
disp(k_obs_acker);
% mediante Matriz de Transformación
% Observador

%% Ahora para FCC
% mediante place que es asignación de polos
%m_obs_FCC= obsv(Afcc, Cfcc)
%det(m_obs_FCC)
%rank(m_obs_FCC)
k_obs_asig_polos_FCC = place(Afcc', Cfcc', poles_obs);
disp('FCC K observable mediante asignación de polos:');
disp(k_obs_asig_polos_FCC);

%mediante Ackermann
k_obs_acker_FCC = acker(Afcc', Cfcc', poles_obs);
disp('FCC K observable mediante ackerman:');
disp(k_obs_acker_FCC);
% mediante Matriz de Transformación

%% Ahora para F Lineal
% mediante place que es asignación de polos
% m_obs_FCC= obsv(Afcc, Cfcc)
% det(m_obs_FCC)
% rank(m_obs_FCC)
k_obs_asig_polos_F = place(A', C', poles_obs);
disp('F K observable mediante asignación de polos:');
disp(k_obs_asig_polos_F);

%mediante Ackermann
k_obs_acker_F = acker(A', C', poles_obs);
disp('F K observable mediante ackerman:');
disp(k_obs_acker_F);
% mediante Matriz de Transformación
% Observador


%% PRUEBAS OBSERVADOR:
sys_ss = ss(Afcc, Bfcc, Cfcc, D); % Sistema en espacio de estados
G = tf(sys_ss);          % Función de transferencia

% Mostrar coeficientes del numerador y denominador
[numfcc, denfcc] = tfdata(G, 'v'); % 'v' para obtener vectores
disp('Coeficientes del numerador:');
disp(numfcc);
disp('Coeficientes del denominador:');
disp(denfcc);

sys_ss = ss(Afco, Bfco, Cfco, D); % Sistema en espacio de estados
Gfco = tf(sys_ss);          % Función de transferencia

% Mostrar coeficientes del numerador y denominador
[numfco, denfco] = tfdata(G, 'v'); % 'v' para obtener vectores
disp('Coeficientes del numerador fco:');
disp(numfco);
disp('Coeficientes del denominadorfco:');
disp(denfco);

M= generarMatrizDesdeDen(denfco);
Matriz_T=fliplr(M);
%%PARTE DE MATRIZ DE TRANSFORMACIÓN
%Para FCC para comprobar:
m_obs_FCC= obsv(Afcc, Cfcc);
k_cachuchita=coeffs_poly - denfcc;
k_cachuchita = k_cachuchita(2:end);
matriz_P=Matriz_T'*m_obs_FCC;
k_obs_FCC_matriz = inv(matriz_P)*k_cachuchita'
% Para FCO:
m_obs_FCO= obsv(Afco, Cfco);
k_cachuchita_fco=coeffs_poly - denfco;
k_cachuchita_fco = k_cachuchita_fco(2:end);
matriz_P_fco=Matriz_T'*m_obs_FCO;
k_obs_FCO_matriz = inv(matriz_P_fco)*k_cachuchita_fco'
% Para F Lineal:
m_obs_F= obsv(A, C);
k_cachuchita_f=coeffs_poly - denfco;
k_cachuchita_f = k_cachuchita_f(2:end);
matriz_P_f=Matriz_T'*m_obs_F;
k_obs_F_matriz = inv(matriz_P_f)*k_cachuchita_f'
function M = generarMatrizDesdeDen(den)
    alpha = den(2:end-1);           % Ignora el coef. líder (asumimos 1)
    n = length(alpha) + 1;       % Orden del sistema
    M = zeros(n);                 % Inicializa matriz

    for i = 1:n
        fila = [1, alpha(1:end - i + 1)];  % Construye la fila
        fila_reflejada = fliplr(fila);    % Refleja horizontalmente
        M(i, 1:length(fila_reflejada)) = fila_reflejada;
    end
    disp('Matriz M:');
    disp(M);
end