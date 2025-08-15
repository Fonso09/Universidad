% se hace un polinomio deseado nuevo debido a que el tiempo de
% establecimiento debe ser más corto:

pd_observable = [86.24 2182.656 14505.8816 48189.0304];
% mediante place que es asignación de polos
k_obs_asig_polos = place(Afco', Cfco', pd_observable);
disp('K observable mediante asignación de polos:');
disp(k_obs_asig_polos);
%mediante Ackermann
k_obs_acker = acker(Afco', Cfco', pd_observable);
disp('K observable mediante ackerman:');
disp(k_obs_acker);
% mediante Matriz de Transformación



%% Ahora para FCC
% mediante place que es asignación de polos
m_obs_FCC= obsv(Afcc, Cfcc)
det(m_obs_FCC)
rank(m_obs_FCC)
k_obs_asig_polos = place(A', C', pd_observable);
disp('FCC K observable mediante asignación de polos:');
disp(k_obs_asig_polos);

%mediante Ackermann
k_obs_acker = acker(Afcc', Cfcc', pd_observable);
disp('FCC K observable mediante ackerman:');
disp(k_obs_acker);
% mediante Matriz de Transformación