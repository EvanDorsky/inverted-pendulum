clear all, clc;

%% Part 2

bata = load('data/step.txt');
data = bata(101:end,:);
Vm = mod(data(:,1), 255)/255*12; % V
w = smooth(data(:,2)*2*pi); % rps
lenw = numel(w);
target = mean(w(lenw/2:end));
w = target - w;

Im = data(:,3); % A
time = linspace(0, 4, numel(data(:,2)))'; % sec

%% Part 1
figure(1)
clf
plot(time, w, 'linewidth', 3)
% plot(time, log(w))
hold on
% plot(time, 1000*exp(-time/.065))

%% Polyfit
timefit = time(1:floor(lenw/19));
wfit = w(1:floor(lenw/19));
coefs = polyfit(timefit, log(wfit), 1);
tau = -1/coefs(1)
A = exp(coefs(2));

plot(timefit, wfit, 'linewidth', 3)
plot(time, A*exp(-time/tau), 'linewidth', 3)
xlabel('Time (seconds)')
ylabel('Motor Velocity (Offset rps)')
title('Determining the Motor Electromechanical Time Constant')
legend('Step data', 'Fit data', 'Exponential fit')
xlim([0 0.5])
% tau = .0596 sec