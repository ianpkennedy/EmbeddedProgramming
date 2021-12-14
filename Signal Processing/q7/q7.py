#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Apr  5 11:02:38 2021

@author: iankennedy
"""

#Ian Kennedy

import matplotlib.pyplot as plt #Importing libraries
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

data_a_fir =[] #create FIR vectors
data_b_fir = []
data_c_fir = []
data_d_fir = []

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


fir_a = [ #Processing signal A
   0.001867519746044820,
    0.002275293677911463,
    0.002692845467883248,
    0.003119400211540369,
    0.003554151391578438,
    0.003996262853518041,
    0.004444870878876082,
    0.004899086349965267,
    0.005357997000181999,
    0.005820669743355830,
    0.006286153075466025,
    0.006753479541784039,
    0.007221668262275161,
    0.007689727507889525,
    0.008156657320192495,
    0.008621452166627776,
    0.009083103623574271,
    0.009540603079249766,
    0.009992944448431896,
    0.010439126890909214,
    0.010878157525543523,
    0.011309054131818317,
    0.011730847830767882,
    0.012142585737226980,
    0.012543333575412086,
    0.012932178249941664,
    0.013308230364524597,
    0.013670626680692492,
    0.014018532509122373,
    0.014351144026291085,
    0.014667690509420725,
    0.014967436482914990,
    0.015249683769748768,
    0.015513773441556675,
    0.015759087661469738,
    0.015985051414072162,
    0.016191134117190663,
    0.016376851110586923,
    0.016541765016997088,
    0.016685486971350718,
    0.016807677714403370,
    0.016908048547430839,
    0.016986362145057821,
    0.017042433223728004,
    0.017076129063764694,
    0.017087369883420026,
    0.017076129063764694,
    0.017042433223728004,
    0.016986362145057821,
    0.016908048547430839,
    0.016807677714403370,
    0.016685486971350718,
    0.016541765016997088,
    0.016376851110586923,
    0.016191134117190663,
    0.015985051414072162,
    0.015759087661469738,
    0.015513773441556675,
    0.015249683769748768,
    0.014967436482914990,
    0.014667690509420725,
    0.014351144026291085,
    0.014018532509122373,
    0.013670626680692492,
    0.013308230364524597,
    0.012932178249941664,
    0.012543333575412086,
    0.012142585737226980,
    0.011730847830767882,
    0.011309054131818317,
    0.010878157525543523,
    0.010439126890909214,
    0.009992944448431896,
    0.009540603079249766,
    0.009083103623574271,
    0.008621452166627776,
    0.008156657320192495,
    0.007689727507889525,
    0.007221668262275161,
    0.006753479541784039,
    0.006286153075466025,
    0.005820669743355830,
    0.005357997000181999,
    0.004899086349965267,
    0.004444870878876082,
    0.003996262853518041,
    0.003554151391578438,
    0.003119400211540369,
    0.002692845467883248,
    0.002275293677911463,
    0.001867519746044820
]
window_a = len(fir_a) 
temp_sum_a =0    
for i in range(0,len(t_a)): #compute moving average
    if i < window_a-1: 
        data_a_fir.append(float(0))
    else:
        for j in range(0,window_a):
            temp_val = data_a[i-window_a+j+1]*fir_a[j] 
            temp_sum_a = temp_sum_a+temp_val  
        data_a_fir.append(temp_sum_a)
        temp_sum_a=0
             
Y_a_fir = np.fft.fft(data_a_fir)/n_a #compute Fourier transform 
Y_a_fir = Y_a_fir[range(int(n_a/2))]

fig, (ax1) = plt.subplots(1,1)

ax1.loglog(frq_a,abs(Y_a),'k',frq_a,abs(Y_a_fir),'r') #Plot FFT
plt.legend(["FFT", "FIR FFT"])
plt.title('Signal A FFT Comparison, 91 coefficients low pass FIR filter \n Transition bandwidth = 100 Hz, Cutoff frequency = 100Hz')
plt.xlabel('Freq (Hz)')
plt.ylabel('Magnitude')
plt.show()

fig, (ax1) = plt.subplots(1,1)

ax1.plot(t_a,data_a,'k',t_a,data_a_fir,'r') #plot data
plt.title('Signal A Data Comparison, 91 coefficients low pass FIR filter \n Transition bandwidth = 100 Hz, Cutoff frequency = 100Hz')
plt.legend(["Data","FIR Data"])
plt.xlabel('Time (s)')
plt.ylabel('Magnitude')
plt.show()

#Process signal b

fir_b = [ #FIR oefficients
    0.001867519746044820,
    0.002275293677911463,
    0.002692845467883248,
    0.003119400211540369,
    0.003554151391578438,
    0.003996262853518041,
    0.004444870878876082,
    0.004899086349965267,
    0.005357997000181999,
    0.005820669743355830,
    0.006286153075466025,
    0.006753479541784039,
    0.007221668262275161,
    0.007689727507889525,
    0.008156657320192495,
    0.008621452166627776,
    0.009083103623574271,
    0.009540603079249766,
    0.009992944448431896,
    0.010439126890909214,
    0.010878157525543523,
    0.011309054131818317,
    0.011730847830767882,
    0.012142585737226980,
    0.012543333575412086,
    0.012932178249941664,
    0.013308230364524597,
    0.013670626680692492,
    0.014018532509122373,
    0.014351144026291085,
    0.014667690509420725,
    0.014967436482914990,
    0.015249683769748768,
    0.015513773441556675,
    0.015759087661469738,
    0.015985051414072162,
    0.016191134117190663,
    0.016376851110586923,
    0.016541765016997088,
    0.016685486971350718,
    0.016807677714403370,
    0.016908048547430839,
    0.016986362145057821,
    0.017042433223728004,
    0.017076129063764694,
    0.017087369883420026,
    0.017076129063764694,
    0.017042433223728004,
    0.016986362145057821,
    0.016908048547430839,
    0.016807677714403370,
    0.016685486971350718,
    0.016541765016997088,
    0.016376851110586923,
    0.016191134117190663,
    0.015985051414072162,
    0.015759087661469738,
    0.015513773441556675,
    0.015249683769748768,
    0.014967436482914990,
    0.014667690509420725,
    0.014351144026291085,
    0.014018532509122373,
    0.013670626680692492,
    0.013308230364524597,
    0.012932178249941664,
    0.012543333575412086,
    0.012142585737226980,
    0.011730847830767882,
    0.011309054131818317,
    0.010878157525543523,
    0.010439126890909214,
    0.009992944448431896,
    0.009540603079249766,
    0.009083103623574271,
    0.008621452166627776,
    0.008156657320192495,
    0.007689727507889525,
    0.007221668262275161,
    0.006753479541784039,
    0.006286153075466025,
    0.005820669743355830,
    0.005357997000181999,
    0.004899086349965267,
    0.004444870878876082,
    0.003996262853518041,
    0.003554151391578438,
    0.003119400211540369,
    0.002692845467883248,
    0.002275293677911463,
    0.001867519746044820,
]

window_b = len(fir_b) 
temp_sum_b = 0
    
for i in range(0,len(t_b)): #compute moving average
    if i < window_b-1:
        data_b_fir.append(float(0))
    else:
        for j in range(0,window_b):
            temp_val = data_b[i-window_b+j+1]*fir_b[j]
            temp_sum_b = temp_sum_b+temp_val  
        data_b_fir.append(temp_sum_b)
        temp_sum_b=0


Y_b_fir = np.fft.fft(data_b_fir)/n_b #compute Fourier transform 
Y_b_fir = Y_b_fir[range(int(n_b/2))]

fig, (ax1) = plt.subplots(1,1)

ax1.loglog(frq_b,abs(Y_b),'k',frq_b,abs(Y_b_fir),'r') #Plot FFT
plt.legend(["FFT", "FIR FFT"])
plt.title('Signal B FFT Comparison, 91 coefficients low pass FIR filter \n Cutoff frequency : 33 Hz , Transition bandwidth: 33 Hz')
plt.xlabel('Freq (Hz)')
plt.ylabel('Magnitude')

fig, (ax1) = plt.subplots(1,1)

ax1.plot(t_b,data_b,'k',t_b,data_b_fir,'r') #plot data
plt.title('Signal B Data Comparison, 91 coefficients low pass FIR filter \n Cutoff frequency : 33 Hz , Transition bandwidth: 33 Hz')
plt.legend(["Data","FIR Data"])
plt.xlabel('Time (s)')
plt.ylabel('Magnitude')

#process signal C

fir_c = [ #FIR coefficients
      0.001867519746044820,
    0.002275293677911463,
    0.002692845467883248,
    0.003119400211540369,
    0.003554151391578438,
    0.003996262853518041,
    0.004444870878876082,
    0.004899086349965267,
    0.005357997000181999,
    0.005820669743355830,
    0.006286153075466025,
    0.006753479541784039,
    0.007221668262275161,
    0.007689727507889525,
    0.008156657320192495,
    0.008621452166627776,
    0.009083103623574271,
    0.009540603079249766,
    0.009992944448431896,
    0.010439126890909214,
    0.010878157525543523,
    0.011309054131818317,
    0.011730847830767882,
    0.012142585737226980,
    0.012543333575412086,
    0.012932178249941664,
    0.013308230364524597,
    0.013670626680692492,
    0.014018532509122373,
    0.014351144026291085,
    0.014667690509420725,
    0.014967436482914990,
    0.015249683769748768,
    0.015513773441556675,
    0.015759087661469738,
    0.015985051414072162,
    0.016191134117190663,
    0.016376851110586923,
    0.016541765016997088,
    0.016685486971350718,
    0.016807677714403370,
    0.016908048547430839,
    0.016986362145057821,
    0.017042433223728004,
    0.017076129063764694,
    0.017087369883420026,
    0.017076129063764694,
    0.017042433223728004,
    0.016986362145057821,
    0.016908048547430839,
    0.016807677714403370,
    0.016685486971350718,
    0.016541765016997088,
    0.016376851110586923,
    0.016191134117190663,
    0.015985051414072162,
    0.015759087661469738,
    0.015513773441556675,
    0.015249683769748768,
    0.014967436482914990,
    0.014667690509420725,
    0.014351144026291085,
    0.014018532509122373,
    0.013670626680692492,
    0.013308230364524597,
    0.012932178249941664,
    0.012543333575412086,
    0.012142585737226980,
    0.011730847830767882,
    0.011309054131818317,
    0.010878157525543523,
    0.010439126890909214,
    0.009992944448431896,
    0.009540603079249766,
    0.009083103623574271,
    0.008621452166627776,
    0.008156657320192495,
    0.007689727507889525,
    0.007221668262275161,
    0.006753479541784039,
    0.006286153075466025,
    0.005820669743355830,
    0.005357997000181999,
    0.004899086349965267,
    0.004444870878876082,
    0.003996262853518041,
    0.003554151391578438,
    0.003119400211540369,
    0.002692845467883248,
    0.002275293677911463,
    0.001867519746044820,
]


window_c = len(fir_c)

temp_sum_c = 0
    
for i in range(0,len(t_c)): #compute moving average
    if i < window_c-1:
        data_c_fir.append(float(0))
    else:
        for j in range(0,window_c):
            temp_val = data_c[i-window_c+j+1]*fir_c[j]
            temp_sum_c = temp_sum_c+temp_val  
        data_c_fir.append(temp_sum_c)
        temp_sum_c=0



Y_c_fir = np.fft.fft(data_c_fir)/n_c #compute Fourier transform 
Y_c_fir = Y_c_fir[range(int(n_c/2))]


fig, (ax1) = plt.subplots(1,1)

ax1.loglog(frq_c,abs(Y_c),'k',frq_c,abs(Y_c_fir),'r') #Plot FFT
plt.legend(["FFT", "FIR FFT"])
plt.title('Signal C FFT Comparison, 91 coefficients low pass FIR filter \n Cutoff frequency : 25 Hz , Transition bandwidth: 25 Hz')
plt.xlabel('Freq (Hz)')
plt.ylabel('Magnitude')

fig, (ax1) = plt.subplots(1,1)

ax1.plot(t_c,data_c,'k',t_c,data_c_fir,'r') #plot data
plt.title('Signal C Data Comparison, 91 coefficients low pass FIR filter \n Cutoff frequency : 25 Hz , Transition bandwidth: 25 Hz')
plt.legend(["Data","FIR Data"])
plt.xlabel('Time (s)')
plt.ylabel('Magnitude')

  #process signal D

fir_d = [ #FIR coefficients
    -0.012107409421291218,
    -0.011494470729150804,
    -0.010396569217120706,
    -0.008802158723855749,
    -0.006711683061779858,
    -0.004137958456587272,
    -0.001106230583553266,
    0.002346101341655805,
    0.006170091593255768,
    0.010306174840714933,
    0.014685392484306534,
    0.019230889272608544,
    0.023859643106165330,
    0.028484383979140007,
    0.033015652264841801,
    0.037363942188128198,
    0.041441873484316771,
    0.045166333012392292,
    0.048460528514530685,
    0.051255898789905489,
    0.053493828225020024,
    0.055127118794511938,
    0.056121179169959769,
    0.056454898263769808,
    0.056121179169959769,
    0.055127118794511938,
    0.053493828225020024,
    0.051255898789905489,
    0.048460528514530685,
    0.045166333012392292,
    0.041441873484316771,
    0.037363942188128198,
    0.033015652264841801,
    0.028484383979140007,
    0.023859643106165330,
    0.019230889272608544,
    0.014685392484306534,
    0.010306174840714933,
    0.006170091593255768,
    0.002346101341655805,
    -0.001106230583553266,
    -0.004137958456587272,
    -0.006711683061779858,
    -0.008802158723855749,
    -0.010396569217120706,
    -0.011494470729150804,
    -0.012107409421291218,
]

window_d = len(fir_d)


temp_sum_d = 0
    
for i in range(0,len(t_d)): #compute moving average
    if i < window_d-1:
        data_d_fir.append(float(0))
    else:
        for j in range(0,window_d):
            temp_val = data_d[i-window_d+j+1]*fir_d[j]
            temp_sum_d = temp_sum_d+temp_val  
        data_d_fir.append(temp_sum_d)
        temp_sum_d=0

Y_d_fir = np.fft.fft(data_d_fir)/n_d #compute Fourier transform 
Y_d_fir = Y_d_fir[range(int(n_d/2))]

fig, (ax1) = plt.subplots(1,1)

ax1.loglog(frq_d,abs(Y_d),'k',frq_d,abs(Y_d_fir),'r') #Plot FFT
plt.legend(["FFT", "FIR FFT"])
plt.title('Signal D FFT Comparison, 47 coefficient low pass FIR filter \n Cutoff frequency: 12 Hz, Transition bandwidth: 8 Hz')
plt.xlabel('Freq (Hz)')
plt.ylabel('Magnitude')

fig, (ax1) = plt.subplots(1,1)

ax1.plot(t_d,data_d,'k',t_d,data_d_fir,'r') #plot data
plt.title('Signal D Data Comparison, 47 coefficients low pass FIR filter \n Cutoff frequency: 12 Hz, Transition bandwidth: 8 Hz')
plt.legend(["Data","FIR Data"])
plt.xlabel('Time (s)')
plt.ylabel('Magnitude')
