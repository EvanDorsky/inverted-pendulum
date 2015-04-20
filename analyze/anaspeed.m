clear all, clc;

%% Part 2

data = load('SPEED2.txt');
Vm = mod(data(:,1), 255)/255*12; % V
w = abs(data(:,2))*2*pi; % rps
Im = data(:,3); % A

%% Part 1
% plot(Vm, Im);

%% Rm calculation
% Rm*Im = dv
Rm = 2.4;

%% Ke calculation
% Ke*w = (Vm - Im*Rm)
Ve = Vm - Im.*Rm;
Ke = (Ve'*w)\(Ve'*Ve) % V*sec/rad
% Ke = .0062