//
//  utilities.c
//  
//
//  Created by Ian Kennedy on 3/8/21.
//

#include <stdio.h>
#include "utilities.h"
#include <xc.h>
#include "NU32.h"

#define IPTS 100


int sample_ADC(){ //Query the ADC
    
    AD1CHSbits.CH0SA = 0; //AN0
    AD1CON1bits.SAMP = 1; //Start sample
    
    while (!AD1CON1bits.DONE) {;}
    
    return ADC1BUF0; //return value
    
}


int sample_ADC_ma(){
    
    int iwhole = 0;
    float ifloat =0;
    
    ifloat =  1.294*sample_ADC() -  654.22;
    iwhole = (int) ifloat;
  
    return iwhole;
}

void ADC_init(){
    
    AD1PCFGbits.PCFG0=0; //AN0 is an analog input
    AD1CON1bits.ASAM=0; //Manually sample
    AD1CON1bits.SSRC = 7; //Autoconvert
    AD1CON3bits.ADCS = 2; //Set Tad
    AD1CON3bits.SAMC = 2; //Set sample time
    AD1CON1bits.ADON = 1;
    
    
}

void get_mode(int var){ //Return mode type
    
    char msg[10];
    
    sprintf(msg,"%d \r\n",var);
    NU32_WriteUART3(msg);
}


void periph_init(){
    
    PR4 = 249; //Set period register
    TMR4 = 0;
    T4CONbits.TCKPS = 6; //64X prescaler
    T4CONbits.ON = 1; //Initiate Timer 4 for 5 kHz ISR
    
    
    T2CONbits.TCKPS = 2;
    PR2 = 999; //Period register for PWM
    TMR2 = 0;
    
    OC1CONbits.OCM = 6;
    OC1CONbits.OCTSEL = 0; //Select timer 2
    
    OC1RS = 50;
    OC1R = 50;
    
    T2CONbits.ON=1;//Initiate PWM Clock
    OC1CONbits.ON=1;//Initiate PWM
    
    TRISDbits.TRISD1 = 0; //Set digital output to RD1
    LATDbits.LATD1 = 0; //Initially set to low, which means "positive"
    

}

void pos_control_init(){
    
    PR3 = 6249; //Period register value
    TMR3 =0;
    T3CONbits.TCKPS=6; //64X prescaler
    T3CONbits.ON=1;
    
}


