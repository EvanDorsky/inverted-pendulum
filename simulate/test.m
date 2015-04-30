clc, clear all

s = tf('s');

G = s^2/(s^2 - 1);
M = 1/(s - 0.2)/(s + 3.9);
K = 8*(s + 1.5)/(s + 0.1);

L = G*M*K;
figure(1)
clf
rlocus(L)

figure(2)
clf
pzmap(1/(1 + 1/L))

figure(3)
margin(L)