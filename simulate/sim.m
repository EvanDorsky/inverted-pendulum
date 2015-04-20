clc, clear all

s = tf('s');

Ke = .0111;
taum = .0596;

M = tf(1/Ke/(1 + taum*s));

figure(1)
clf
subplot(211);
margin(M);
subplot(212);
step(M);