clear all, clc;

%% Part 2

data = load('data/speedpos.txt');
Vm = mod(data(:,1), 255)/255*12; % V
w = abs(data(:,2))*2*pi; % rps
Im = data(:,3); % A
time = linspace(0, 6*4, numel(w));

%% Part 1
figure(1)
clf
plot(time, w)
xlim([0 20])
xlabel('Time (seconds)')
ylabel('Ungeared motor velocity (rps)')
title('Determining the Motor Electrical Constant')

%% Rm calculation
% Rm*Im = dv
Rm = 2.4;

%% Ke calculation
% Ke*w = (Vm - Im*Rm)
Ve = Vm - Im.*Rm;
Ke = (Ve'*w)\(Ve'*Ve) % V*sec/rad
% **** If only positive (good) data is used, Ke = .0111
% Ke = .0062