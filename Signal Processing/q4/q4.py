#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sun Apr  4 09:05:47 2021

@author: iankennedy
"""

#Ian Kennedy

import matplotlib.pyplot as plt #import libraries
import numpy as np
import csv

t_a = [] #signal time vectors
t_b = []
t_c = []
t_d = []

data_a = [] #signal data vectors
data_b = []
data_c = []
data_d = []

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

fig, (ax1, ax2) = plt.subplots(1,2) #Question 4 plot signal A
ax1.plot(t_a,data_a,'b')
ax1.set_xlabel('Time (s)')
ax1.set_ylabel('Amplitude')
plt.title('Signal A ')
ax2.loglog(frq_a,abs(Y_a),'b') 
ax2.set_xlabel('Freq (Hz)')
#ax2.set_ylabel('abs(Y (freq) )')
plt.title('Signal A Left: Signal, Right: FFT')
plt.show()

fig, (ax1, ax2) = plt.subplots(1,2) #Question 4 plot signal B
ax1.plot(t_b,data_b,'b')
ax1.set_xlabel('Time (s)')
ax1.set_ylabel('Amplitude')
ax2.loglog(frq_b,abs(Y_b),'b') 
ax2.set_xlabel('Freq (Hz)')
#ax2.set_ylabel('abs(Y (freq) )')
plt.title('Signal B Left: Signal, Right: FFT')
plt.show()

fig, (ax1, ax2) = plt.subplots(1,2) #Question 4 plot signal C
ax1.plot(t_c,data_c,'b')
ax1.set_xlabel('Time (s)')
ax1.set_ylabel('Amplitude')
ax2.loglog(frq_c,abs(Y_c),'b') 
ax2.set_xlabel('Freq (Hz)')
#ax2.set_ylabel('abs(Y (freq) )')
plt.title('Signal C Left: Signal, Right: FFT')
plt.show()

fig, (ax1, ax2) = plt.subplots(1,2) #Question 4 plot signal D
ax1.plot(t_d,data_d,'b')
ax1.set_xlabel('Time (s)')
ax1.set_ylabel('Amplitude')
ax2.loglog(frq_d,abs(Y_d),'b') 
ax2.set_xlabel('Freq (Hz)')
#ax2.set_ylabel('abs(Y (freq) )')
plt.title('Signal D Left: Signal, Right: FFT')
plt.show()









         