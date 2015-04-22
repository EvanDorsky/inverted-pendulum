clc, clear all

s = tf('s');

% pendulum tf
g = 9.8; % m/sec^2
l = 0.0936; % m
tauL = sqrt(g/l); % sec

G = -s^2/g/((tauL*s + 1)*(tauL*s - 1));

%% series compensator tf

K = 1;

%% motor tf
Ke = .0111; % V*sec/rad
tauM = .0596; % sec

M = 1/Ke/(1 + tauM*s); % Vout/Vin

% velocity minor loop
kV = 1;

Mc = M/(s*(1 + M*kV)); % X/Vin 

% position positive feedback
kP = 1;

Lminor = K*Mc*kP; % minor position loop TF
KM = K*Mc;

KMp = KM/(1 - kP*KM); % actual minor loop

Lmajor = KMp*G; % total loop TF
XoverTheta = KMp/(1 - G*KMp); % total TF

figure(1)
clf
bode(Lmajor)
hold on
% subplot(211)
% margin(M)
% subplot(212)
% step(M)