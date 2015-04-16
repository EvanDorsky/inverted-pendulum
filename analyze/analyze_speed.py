import matplotlib.pyplot as plt
from numpy import *
from scipy.signal import argrelextrema
from math import factorial

def savitzky_golay(y, window_size, order, deriv=0, rate=1):
  try:
    window_size = abs(int(window_size))
    order = abs(int(order))
  except ValueError, msg:
    raise ValueError("window_size and order have to be of type int")
  if window_size % 2 != 1 or window_size < 1:
    raise TypeError("window_size size must be a positive odd number")
  if window_size < order + 2:
    raise TypeError("window_size is too small for the polynomials order")
  order_range = range(order+1)
  half_window = (window_size -1) // 2
  # precompute coefficients
  b = mat([[k**i for i in order_range] for k in range(-half_window, half_window+1)])
  m = linalg.pinv(b).A[deriv] * rate**deriv * factorial(deriv)
  # pad the signal at the extremes with
  # values taken from the signal itself
  firstvals = y[0] - abs( y[1:half_window+1][::-1] - y[0] )
  lastvals = y[-1] + abs(y[-half_window-1:-1][::-1] - y[-1])
  y = concatenate((firstvals, y, lastvals))
  return convolve( m[::-1], y, mode='valid')

def find_nearest(array, value):
  idx = abs(array - value).argmin()
  return array[idx]

def find_nearest_ind(array, value):
  return abs(array - value).argmin()

with open('data/speed50.txt', 'r') as f:
  data = f.readlines()

with open('data/speedcurrent.txt', 'r') as f:
  data2 = f.readlines()

time = array(data[0::2]).astype('float')*1e-3 # time was in millis

dataB = array(data[1::2]).astype('float')

encB = 1/(64*1e-6*dataB)

accel = diff(encB)

acceltime = delete(time, -1)

time2 = array(data2[0::2]).astype('float')+12.34
current = array(data2[1::2]).astype('float')

scaled = current*10
smaccel = savitzky_golay(accel, 7, 2)
KtoJ = interp(time2, acceltime, smaccel)/current

plt.figure()
plt.hold(True)
# plt.plot(time, encB)
plt.plot(acceltime, savitzky_golay(accel, 7, 2))
plt.plot(time2, KtoJ)
plt.plot(time2, scaled)
plt.legend(['Acceleration (rads/sec^2)', 'Accel/Current or K_t/J', 'Current (Amps*10)'])
plt.plot
plt.ylabel('Y axis')
plt.xlabel('Time (sec)')
plt.grid(True)
plt.xlim([12.13, 12.6])
plt.ylim([-15, 15])
plt.title('Finding K_t and J: Motor Acceleration and Current')

plt.show()