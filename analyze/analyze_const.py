import matplotlib.pyplot as plt
from numpy import *
from scipy.signal import argrelextrema

with open('data/const25v.txt', 'r') as f:
  data = f.readlines()

time = array(data[0::2]).astype('float')*1e-3 # time was in millis

dataB = array(data[1::2]).astype('float')

encB = 1/(64*1e-6*dataB)

plt.figure()
plt.hold(True)
plt.plot(time, encB)
plt.ylabel('Speed (rps)')
plt.xlabel('Time (sec)')
plt.grid(True)
plt.title('Motor speed over time')

print 'Average speed:', average(encB), 'rps'
print 'Average speed:', average(encB)/(2*pi), 'Hz'

plt.show()