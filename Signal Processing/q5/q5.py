#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sun Apr  4 18:11:21 2021

@author: iankennedy
"""

#Ian Kennedy

import matplotlib.pyplot as plt #import libraries
import numpy as np
import csv
from statistics import mean

t_a = [] #signal time vectors
t_b = []
t_c = []
t_d = []

data_a = [] #signal data vectors
data_b = []
data_c = []
data_d = []

data_a_maf =[] #create moving average filter vectors
data_b_maf = []
data_c_maf = []
data_d_maf = []

with open('sigA.csv') as a: #importing data from files
    readera = csv.reader(a)
    for row in readera:
        t_a.append(float(row[0]))
        data_a.append(float(row[1]))
    
with open('sigB.csv') as b: 
    readerb = csv.reader(b)
    for row in readerb:
        t_b.append(float(row[0]))
        data_b.append(float(row[1]))    
        
with open('sigC.csv') as c: 
    readerc = csv.reader(c)
    for row in readerc:
        t_c.append(float(row[0]))
        data_c.append(float(row[1]))
    
with open('sigD.csv') as d: 
    readerd = csv.reader(d)
    for row in readerd:
        t_d.append(float(row[0]))
        data_d.append(float(row[1]))   


Fs_a = (len(t_a)-1)/t_a[-1] #calculating sampling rates
Fs_b = (len(t_b)-1)/t_b[-1]
Fs_c = (len(t_c)-1)/t_c[-1]
Fs_d = (len(t_d)-1)/t_d[-1]

Ts_a = 1/Fs_a #calculating sampling intervals
Ts_b = 1/Fs_b
Ts_c = 1/Fs_c
Ts_d = 1/Fs_d

n_a = len(data_a) #calculating data time length
n_b = len(data_b)
n_c = len(data_c)
n_d = len(data_d)

k_a = np.arange(n_a)  
k_b = np.arange(n_b)
k_c = np.arange(n_c)
k_d = np.arange(n_d)

T_a = n_a/Fs_a
T_b = n_b/Fs_b
T_c = n_c/Fs_c
T_d = n_d/Fs_d

frq_a = k_a/T_a
frq_b = k_b/T_b
frq_c = k_c/T_c
frq_d= k_d/T_d

frq_a = frq_a[range(int(n_a/2))] # one sided frequency
frq_b = frq_b[range(int(n_b/2))]
frq_c = frq_c[range(int(n_c/2))]
frq_d = frq_d[range(int(n_d/2))]

Y_a = np.fft.fft(data_a)/n_a   # compute FFT  
Y_b = np.fft.fft(data_b)/n_b    
Y_c = np.fft.fft(data_c)/n_c    
Y_d = np.fft.fft(data_d)/n_d

Y_a = Y_a[range(int(n_a/2))] #subselect frequency range
Y_b = Y_b[range(int(n_b/2))]
Y_c = Y_c[range(int(n_c/2))]
Y_d = Y_d[range(int(n_d/2))]

window_a = 100 #Processing signal A
       
for i in range(0,len(t_a)): #compute moving average
    if i < window_a-1:
        data_a_maf.append(float(0))
    else:
        temp_mean = mean(data_a[i-window_a+1:i+1]) #apply in all cases
        data_a_maf.append(temp_mean)

Y_a_maf = np.fft.fft(data_a_maf)/n_a #compute Fourier transform 
Y_a_maf = Y_a_maf[range(int(n_a/2))]

fig, (ax1) = plt.subplots(1,1)

ax1.loglog(frq_a,abs(Y_a),'k',frq_a,abs(Y_a_maf),'r') #Plot FFT
plt.legend(["FFT", "MAF FFT"])
plt.title('Signal A FFT Comparison, X=100')
plt.xlabel('Freq (Hz)')
plt.ylabel('Magnitude')

fig, (ax1) = plt.subplots(1,1)

ax1.plot(t_a,data_a,'k',t_a,data_a_maf,'r') #plot data
plt.title('Signal A Data Comparison, X= 100')
plt.legend(["Data","MAF Data"])
plt.xlabel('Time (s)')
plt.ylabel('Magnitude')


window_b = 50 #process signal B

for i in range(0,len(t_b)): #compute moving average
    if i < window_b-1:
        data_b_maf.append(float(0))
    else:
        temp_mean = mean(data_b[i-window_b+1:i+1])
        data_b_maf.append(temp_mean)

Y_b_maf = np.fft.fft(data_b_maf)/n_b #compute Fourier transform 
Y_b_maf = Y_b_maf[range(int(n_b/2))]

fig, (ax1) = plt.subplots(1,1)

ax1.loglog(frq_b,abs(Y_b),'k',frq_b,abs(Y_b_maf),'r') #Plot FFT
plt.legend(["FFT", "MAF FFT"])
plt.title('Signal B FFT Comparison, X=50')
plt.xlabel('Freq (Hz)')
plt.ylabel('Magnitude')

fig, (ax1) = plt.subplots(1,1)

ax1.plot(t_b,data_b,'k',t_b,data_b_maf,'r') #plot data
plt.title('Signal B Data Comparison, X=50')
plt.legend(["Data","MAF Data"])
plt.xlabel('Time (s)')
plt.ylabel('Magnitude')

window_c = 1 #process signal C

for i in range(0,len(t_c)): #compute moving average
    if i < window_c-1:
        data_c_maf.append(float(0))
    else:
        temp_mean = mean(data_c[i-window_c+1:i+1])
        data_c_maf.append(temp_mean)

Y_c_maf = np.fft.fft(data_c_maf)/n_c #compute Fourier transform 
Y_c_maf = Y_c_maf[range(int(n_c/2))]

fig, (ax1) = plt.subplots(1,1)

ax1.loglog(frq_c,abs(Y_c),'k',frq_c,abs(Y_c_maf),'r') #Plot FFT
plt.legend(["FFT", "MAF FFT"])
plt.title('Signal C FFT Comparison, X=1')
plt.xlabel('Freq (Hz)')
plt.ylabel('Magnitude')
fig, (ax1) = plt.subplots(1,1)

ax1.plot(t_c,data_c,'k',t_c,data_c_maf,'r') #plot data
plt.title('Signal C Data Comparison, X=1')
plt.legend(["Data","MAF Data"])
plt.xlabel('Time (s)')
plt.ylabel('Magnitude')

window_d = 10 #process signal D

for i in range(0,len(t_d)): #compute moving average
    if i < window_d-1:
        data_d_maf.append(float(0))
    else:
        temp_mean = mean(data_d[i-window_d+1:i+1])
        data_d_maf.append(temp_mean)

Y_d_maf = np.fft.fft(data_d_maf)/n_d #compute Fourier transform 
Y_d_maf = Y_d_maf[range(int(n_d/2))]

fig, (ax1) = plt.subplots(1,1)

ax1.loglog(frq_d,abs(Y_d),'k',frq_d,abs(Y_d_maf),'r') #Plot FFT
plt.legend(["FFT", "MAF FFT"])
plt.title('Signal D FFT Comparison, X=10')
plt.xlabel('Freq (Hz)')
plt.ylabel('Magnitude')

fig, (ax1) = plt.subplots(1,1)

ax1.plot(t_d,data_d,'k',t_d,data_d_maf,'r') #plot data
plt.title('Signal D Data Comparison, X=10')
plt.legend(["Data","MAF Data"])
plt.xlabel('Time (s)')
plt.ylabel('Magnitude')