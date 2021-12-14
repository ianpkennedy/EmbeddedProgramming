//
//  c24q5.c
//  
//
//  Created by Ian Kennedy on 2/24/21.
//

#include <stdio.h>
#include "NU32.h"
#define NUMSAMPS 1000
#define PLOTPTS 200
#define DECIMATION 10

static volatile int Waveform[NUMSAMPS];
static volatile int ADCarray[PLOTPTS];
static volatile int REFarray[PLOTPTS];
static volatile int StoringData=0; //Define global variables

static volatile float Kp=0,Ki=0; //Define global gains

void makeWaveform();

void __ISR(_TIMER_2_VECTOR, IPL5SOFT) Controller(void){
    static int counter = 0; //initialize counter
    static int plotind = 0; //initialize plot index
    static int decctr = 0;   //initialize plot every DECIMATION
    static int adcval = 0;
    
    OC1RS = Waveform[counter];    //Set OC1RS
    
    if (StoringData) {
        decctr++;
        if (decctr == DECIMATION) {
            decctr = 0;
            ADCarray[plotind] = adcval;
            REFarray[plotind] = Waveform[counter];
            plotind++;
        }
        if (plotind == PLOTPTS) {
            plotind = 0;
            StoringData = 0;
        }
    }
    counter++;
    if (counter ==NUMSAMPS){
        counter =0;
    }
    
    IFS0bits.T2IF= 0; //Clear interrupt flag
}

int main(){
    
    NU32_Startup();
    char message[100];
    float kptemp=0,kitemp=0; //temporary control gain values
    int i=0; //plot data counter
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
        NU32_ReadUART3(message,sizeof(message)); //get message of Matlab
        sscanf(message,"%f %f", &kptemp, &kitemp);
        __builtin_disable_interrupts(); //disable ISR while updating gains
        Kp = kptemp;
        Ki = kitemp;
        __builtin_enable_interrupts();
        StoringData = 1; //Start data storing
        while (StoringData) {
            ;
        }
        for (i=0; i<PLOTPTS; i++) {
            sprintf(message,"%d %d %d\r\n",PLOTPTS-i, ADCarray[i],REFarray[i]);
            NU32_WriteUART3(message);
        }
    }
    return 0;
}

void makeWaveform(){
    
    int j=0;
    int A=1000;
    int center=2000;
    
    for(j=0;j<NUMSAMPS;++j){
        if(j<NUMSAMPS/2){
            Waveform[j] = center+A;
        } else{
            Waveform[j] = center-A;
        }
    }
}
