clc, clear all

format long e

s = tf('s');

%% motor tf
Ke = .0111; % V*sec/rad
n = 1/30; % gearbox reduction
m = 1/2; % pulley reduction
Larm = .1332; % m (5.245in)
Ke = Ke*Larm/n/m; % corrected (and linear) motor constant
tauM = .0596; % sec

M = 1/Ke/(s*(1 + tauM*s)); % X/Vin

% velocity minor loop
kV = 0;
% kV = .04;
enc = kV*s;

Mc = 1/(1/M + enc); % X/Vin 

% position positive feedback
% 960 counts per arm rot -> 0-2pi
kPosConv = (2*pi)/960; % .0065
kP = 0.2;

Mpc = 1/(1/Mc - kP);

%% series compensator tf (lag)

T = 1e-3;
tauA = 1/8;
tauB = 1/.3;
% Kc = 10/(sqrt(10)*3.41);
Kc = sqrt(10)*db2mag(2.4+4+4.5);
Ktest = Kc*(tauA*s + 1)/(tauB*s + 1)
K = Ktest;
eaT = exp(-1/tauA*T)
ebT = exp(-1/tauB*T)
Kd = Kc*tauB/tauA*((1 - eaT)/(1 - ebT))

KMp = K*Mpc;

% pendulum tf
g = 9.8; % m/sec^2
% l = 0.0936; % m (5", now destroyed)
% l = 0.1195; %m (6.5")
% l = 0.2659; % m (15")
% l = 0.1944; % m (10")
% l = 0.1651; % new delrin 10"
l = 0.18923; % new mdf 10"
tauL = sqrt(l/g); % sec

G = -s^2/g/((tauL*s + 1)*(tauL*s - 1)); % Theta/X

Sys = KMp*-G; % X/Theta (- because G is -)

figure(1)
opts = bodeoptions('cstprefs');
opts.XLabel.Interpreter = 'latex';
opts.YLabel.Interpreter = 'latex';
opts.Title.Interpreter = 'latex';
clf
% margin(Ktest)
% hold on
bodeplot(Sys, opts)
h = findobj(gcf, 'type', 'line')
set(h, 'linewidth', 4);
title('System with Positive Position Feedback and Lag Compensator')
grid on

figure(2)
clf
rlocus(-G*Mpc*K)

nopts = nyquistoptions('cstprefs');
nopts.XLabel.Interpreter = 'latex';
nopts.YLabel.Interpreter = 'latex';
nopts.Title.Interpreter = 'latex';
nopts.Title.Interpreter = 'latex';

figure(3)
clf
nyquistplot(Sys, nopts)
h = findobj(gcf, 'type', 'line')
set(h, 'linewidth', 4);
title('System with Positive Position Feedback and Lag Compensator')

figure(4)
clf
pzmap(1/(1+1/Sys))