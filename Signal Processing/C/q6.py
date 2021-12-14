#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Apr  5 08:27:11 2021

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

data_a_iir =[] #create IIR vectors
data_b_iir = []
data_c_iir = []
data_d_iir = []

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


#Process signal A
coef_a_a = 0.9 # IIR  coefficients
coef_b_a = 1-coef_a_a

for i in range(0,len(t_a)): #compute moving average
    if i < 1:
        data_a_iir.append(float(0))
    else:
        temp_mean = coef_a_a*data_a_iir[i-1]+coef_b_a*data_a[i]
        data_a_iir.append(temp_mean)

Y_a_iir = np.fft.fft(data_a_iir)/n_a #compute Fourier transform 
Y_a_iir = Y_a_iir[range(int(n_a/2))]

fig, (ax1) = plt.subplots(1,1)

ax1.loglog(frq_a,abs(Y_a),'k',frq_a,abs(Y_a_iir),'r') #Plot FFT
plt.legend(["FFT", "IIR FFT"])
plt.title('Signal A FFT Comparison, A=0.9, B=0.1')
plt.xlabel('Freq (Hz)')
plt.ylabel('Magnitude')


fig, (ax1) = plt.subplots(1,1)

ax1.plot(t_a,data_a,'k',t_a,data_a_iir,'r') #plot data
plt.title('Signal A Data Comparison, A=0.9, B=0.1')
plt.legend(["Data","IIR Data"])
plt.xlabel('Time (s)')
plt.ylabel('Magnitude')


#process signal B
coef_b_a = 0.95 #IIR coefficients
coef_b_b = 1-coef_b_a


for i in range(0,len(t_b)): #compute moving average
    if i < 1:
        data_b_iir.append(float(0))
    else:
        temp_mean = coef_b_a*data_b_iir[i-1]+coef_b_b*data_b[i]
        data_b_iir.append(temp_mean)

Y_b_iir = np.fft.fft(data_b_iir)/n_b #compute Fourier transform 
Y_b_iir = Y_b_iir[range(int(n_b/2))]

fig, (ax1) = plt.subplots(1,1)

ax1.loglog(frq_b,abs(Y_b),'k',frq_b,abs(Y_b_iir),'r') #Plot FFT
plt.legend(["FFT", "IIR FFT"])
plt.title('Signal B FFT Comparison, A=0.95, B=0.05')
plt.xlabel('Freq (Hz)')
plt.ylabel('Magnitude')


fig, (ax1) = plt.subplots(1,1)

ax1.plot(t_b,data_b,'k',t_b,data_b_iir,'r') #plot data
plt.title('Signal B Data Comparison, A=0.95, B=0.05 ')
plt.legend(["Data","IIR Data"])
plt.xlabel('Time (s)')
plt.ylabel('Magnitude')

#process signal C

coef_c_a = 0.5 #IIR coefficients
coef_c_b = 1-coef_c_a


for i in range(0,len(t_c)): #compute moving average
    if i < 1:
        data_c_iir.append(float(0))
    else:
        temp_mean = coef_c_a*data_c_iir[i-1]+coef_c_b*data_c[i]
        data_c_iir.append(temp_mean)

Y_c_iir = np.fft.fft(data_c_iir)/n_c #compute Fourier transform 
Y_c_iir = Y_c_iir[range(int(n_c/2))]

fig, (ax1) = plt.subplots(1,1)

ax1.loglog(frq_c,abs(Y_c),'k',frq_c,abs(Y_c_iir),'r') #Plot FFT
plt.legend(["FFT", "IIR FFT"])
plt.title('Signal C FFT Comparison, A= 0.5,B=0.5 ')
plt.xlabel('Freq (Hz)')
plt.ylabel('Magnitude')


fig, (ax1) = plt.subplots(1,1)

ax1.plot(t_c,data_c,'k',t_c,data_c_iir,'r') #plot data
plt.title('Signal C Data Comparison, A=0.5, B=0.5 ')
plt.legend(["Data","IIR Data"])
plt.xlabel('Time (s)')
plt.ylabel('Magnitude')


#process signal D

coef_d_a = 0.85   #IIR coefficients
coef_d_b = 1-coef_d_a


for i in range(0,len(t_d)): #compute moving average
    if i < 1:
        data_d_iir.append(float(0))
    else:
        temp_mean = coef_d_a*data_d_iir[i-1]+coef_d_b*data_d[i]
        data_d_iir.append(temp_mean)

Y_d_iir = np.fft.fft(data_d_iir)/n_d #compute Fourier transform 
Y_d_iir = Y_d_iir[range(int(n_d/2))]

fig, (ax1) = plt.subplots(1,1)

ax1.loglog(frq_d,abs(Y_d),'k',frq_d,abs(Y_d_iir),'r') #Plot FFT
plt.legend(["FFT", "IIR FFT"])
plt.title('Signal D FFT Comparison, A=0.85, B=0.15')
plt.xlabel('Freq (Hz)')
plt.ylabel('Magnitude')

fig, (ax1) = plt.subplots(1,1)

ax1.plot(t_d,data_d,'k',t_d,data_d_iir,'r') #plot data
plt.title('Signal D Data Comparison, A=0.85, B=0.15')
plt.legend(["Data","IIR Data"])
plt.xlabel('Time (s)')
plt.ylabel('Magnitude')
