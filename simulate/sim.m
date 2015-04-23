clc, clear all

s = tf('s');

%% motor tf
Ke = .0111; % V*sec/rad
tauM = .0596; % sec

M = 1/Ke/(s*(1 + tauM*s)); % X/Vin

% velocity minor loop
kV = .04;
enc = kV*s;

Mc = 1/(1/M + enc); % X/Vin 

% position positive feedback
kP = 0;

%% series compensator tf (lag)

K = (s + 15)/(s + 1);

KM = K*Mc;

KMp = KM/(1 - kP*KM); % large minor loop

% pendulum tf
g = 9.8; % m/sec^2
l = 0.0936; % m
tauL = sqrt(l/g); % sec

G = -s^2/g/((tauL*s + 1)*(tauL*s - 1));

Sys = KMp/(1 - G*KMp); % X/Theta (- because G is -)

figure(1)
clf
subplot(211)
margin(M)
hold on
margin(Mc)
subplot(223)
pzmap(G,'b',Mc,'r', K, 'g')
subplot(224)
step(1/(1+1/M))
hold on
step(1/(1+1/Mc))

% figure(2)
% clf
% rlocus(G*Mc*K)