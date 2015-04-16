import matplotlib.pyplot as plt
from numpy import *
from scipy.signal import argrelextrema

def find_nearest(array, value):
  idx = abs(array - value).argmin()
  return array[idx]

def find_nearest_ind(array, value):
  return abs(array - value).argmin()

with open('data/square50v.txt', 'r') as f:
  data = f.readlines()

time = array(data[0::2]).astype('float')*1e-3 # time was in millis

dataB = array(data[1::2]).astype('float')

encB = 1/(64*1e-6*dataB)

steady = array([x for x in encB if x > average(encB)])
valleys = array([x if x < 0.6*average(encB) else 1e4 for x in encB])

steadySpeed = average(steady)
topTarget = 0.9*steadySpeed

mins = argrelextrema(valleys, less)[0]

periods = []
for i in xrange(len(mins)):
  if i == len(mins) - 1:
    periods.append((encB[mins[i]:], time[mins[i]:], i))

    break

  periods.append((encB[mins[i]:mins[i+1]-2], time[mins[i]:mins[i+1]-2], i))

def risetime(perioduple):
  period = perioduple[0]
  periodtime = perioduple[1]
  num = perioduple[2]

  start = periodtime[find_nearest_ind(period, 0.1*steadySpeed)]
  end = periodtime[find_nearest_ind(period, 0.9*steadySpeed)]

  risetime = (end - start)*1e3 # ms

  plt.figure()
  plt.hold(True)
  plt.plot(periodtime, period)
  plt.plot([start, start], [min(period), max(period)])
  plt.plot([end, end], [min(period), max(period)])
  plt.ylabel('Speed (rps)')
  plt.xlabel('Time (sec)')
  plt.grid(True)
  plt.title('Motor speed over time: Period ' + str(num))
  plt.annotate('Rise time:'+str(risetime)+'ms', xy=(end, average(period)/2))

  print 'Rise time:', risetime, 'ms'

map(risetime, periods)

plt.show()