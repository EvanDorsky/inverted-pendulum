clc, clear all

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
% 960 counts per motor rot -> 480 per arm -> 0-2pi
kPosConv = (2*pi)/960;
kP = 0.2;

Mpc = 1/(1/Mc - kP)

%% series compensator tf (lag)

T = 1e-3;
zeroW = 6;
poleW = .6;
K = (s + zeroW)/(s + poleW)
c2d(.263*K, T, 'matched')
zzero = exp(-zeroW*T)
zpole = exp(-poleW*T)

KMp = K*Mpc;

% pendulum tf
g = 9.8; % m/sec^2
% l = 0.0936; % m (5", now destroyed)
% l = 0.1195; %m (6.5")
% l = 0.2659; % m (15")
% l = 0.1944; % m (10")
l = 0.1651; % new delrin 10"
tauL = sqrt(l/g); % sec

G = -s^2/g/((tauL*s + 1)*(tauL*s - 1)) % Theta/X

Sys = KMp*-G; % X/Theta (- because G is -)

figure(1)
clf
margin(Sys)

figure(2)
clf
rlocus(-G*Mpc*K)

figure(3)
clf
nyquist(Sys)

figure(4)
clf
pzmap(1/(1+1/Sys))