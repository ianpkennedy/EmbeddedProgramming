//
//  c24q2.c
//  
//
//  Created by Ian Kennedy on 2/23/21.
//

#include <stdio.h>
#include "NU32.h"

int main(){
    
    NU32_Startup(); //Initialize PIC32
    
    T3CONbits.TCKPS = 0; //Set prescaler to 1
    PR3 = 3999; //Set period register for 20 kHz PWM
    TMR3 = 0; //Initiate timer to 0
    
    OC1CONbits.OCM = 6;
    OC1RS = 3000;
    OC1R = 3000; //75% duty cycle
    OC1CONbits.OCTSEL = 1; //Select timer 3
    
    T3CONbits.ON = 1; //Start timer
    OC1CONbits.ON = 1; //Initate OC1 PWM
    
    while(1){ //Infinite loop
        ;
    }
    
    return 0;
}
