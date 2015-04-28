clc, clear all

s = tf('s');

%% motor tf
Ke = .0111; % V*sec/rad
n = 1/30; % gearbox reduction
m = 1/2; % pulley reduction
Larm = .127; % m
Ke = Ke*Larm/n/m; % corrected (and linear) motor constant
tauM = .0596; % sec

M = 1/Ke/(s*(1 + tauM*s)); % X/Vin

% velocity minor loop
kV = .04;
enc = kV*s;

Mc = 1/(1/M + enc); % X/Vin 

% position positive feedback
kP = 0.2;

Mpc = 1/(1/Mc - kP);

%% series compensator tf (lag)

K = (s + 13)/(s + .5);

KMp = K*Mpc;

% pendulum tf
g = 9.8; % m/sec^2
l = 0.0936; % m
tauL = sqrt(l/g); % sec

G = -s^2/g/((tauL*s + 1)*(tauL*s - 1)); % Theta/X

Sys = KMp/(1 - G*KMp); % X/Theta (- because G is -)

figure(1)
clf
subplot(211)
bode(Sys)
% hold on
% bode(Sys)
subplot(223)
pzmap(G,'b',Mpc,'r', K, 'g')
legend('Pendulum', 'Motor', 'Compensator')
subplot(224)
step(1/(1+1/M))
hold on
step(1/(1+1/Mpc))

figure(2)
clf
rlocus(-G*Mpc*K)