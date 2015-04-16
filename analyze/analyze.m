clear all, clc;

%% Part 2

dv = [4.78 7 10 15]'; % volts
di = [1.25 1.88 2.62 3.64]'; % amps

itr = [1136 560 1151 1662]*1e-6; % seconds

taums = [227 201 144 190 162 164 164 154 133 163 197 233]'*1e-3; %seconds

%% Part 1

w = [1.02 .668 .448 .304]'.^-1*2*pi; % rps
Vm = [2.35 3.42 4.95 7.37]'; % volts
Im = [.118 .132 .158 .195]'; % amps

%% Rm calculation
% Rm*Im = dv
Rm = (dv'*di)\(dv'*dv)

%% Lm calculation
tau = mean(itr)/2.2;
Lm = tau*Rm

%% Ke calculation
% Ke*w = (Vm - Im*Rm)
Ve = Vm - Im.*Rm;
Ke = (Ve'*w)\(Ve'*Ve)

%% n calculation
n = 29

%% Kp calculation
Kp = 4.96/(2*pi)

%% Jf calculation
%% nope

%% Jm and Kt calculation


%% taum calculation
taum = mean(taums)