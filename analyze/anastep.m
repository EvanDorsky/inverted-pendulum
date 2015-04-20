clear all, clc;

%% Part 2

data = load('data/step.txt');
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
plot(time, w)
% plot(time, log(w))
hold on
% plot(time, 1000*exp(-time/.065))

%% Polyfit
timefit = time(1:floor(lenw/19));
wfit = w(1:floor(lenw/19));
coefs = polyfit(timefit, log(wfit), 1);
tau = -1/coefs(1)
A = exp(coefs(2));

plot(timefit, wfit)
plot(time, A*exp(-time/tau))
% tau = .0596 sec