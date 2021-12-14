//
//  c24q3.c
//  
//
//  Created by Ian Kennedy on 2/23/21.
//

#include <stdio.h>
#include "NU32.h"
#define NUMSAMPS 1000

static volatile int Waveform[NUMSAMPS];

void makeWaveform();

void __ISR(_TIMER_2_VECTOR, IPL5SOFT) Controller(void){
    static int counter=0;
    
    OC1RS = Waveform[counter];    //Set OC1RS
    
    counter++;
    if (counter ==NUMSAMPS){
        counter =0;
    }
    
    IFS0bits.T2IF= 0; //Clear interrupt flag
    
}

int main(){
    
    NU32_Startup();
    makeWaveform(); //Generate waveform
    
    __builtin_disable_interrupts();
    T3CONbits.TCKPS = 0; //Set prescaler to 1
    PR3 = 3999; //Set period register for 20 kHz PWM
    TMR3 = 0; //Initiate timer to 0
    
    OC1CONbits.OCM = 6;
    OC1RS = 3000;
    OC1R = 3000; //75% duty cycle
    OC1CONbits.OCTSEL = 1; //Select timer 3
    
    T3CONbits.ON = 1; //Start timer
    OC1CONbits.ON = 1; //Initate OC1 PWM
    
    PR2 = 1249; //Set period
    TMR2 = 0; //Set timer
    T2CONbits.TCKPS = 6; //set 1:64 prescaler
    T2CONbits.ON = 1; //Turn on Timer2
    
    IFS0bits.T2IF= 0; //Clear interrupt flag
    IPC2bits.T2IP = 5; //Set interrupt priority
    IPC2bits.T2IS = 0; //Set subpriority
    IEC0bits.T2IE = 1; //Enable interrupt
    
    __builtin_enable_interrupts();
    
    
    while(1){ //Infinite loop
        ;
    }
    
    return 0;
}

void makeWaveform(){
    
    int i=0;
    int A=1000; //initialize variables
    int center=2000;
    
    for(i=0;i<NUMSAMPS;++i){
        if(i<NUMSAMPS/2){
            Waveform[i] = center+A; //calculate waveform
        } else{
            Waveform[i] = center-A;
        }
    }
}
