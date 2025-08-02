syms F x theta xp thetap ppp
k1 = 100;
k2 = 150;
b1 = 20;
b2 = 30;
m1 = 2;
m2 = 1;
xpp = (F -xp*(b1+b2)-x(k1+k2)+m1*g*sin(theta))/m2;
thetapp= (-gsin(theta)-ppp-)