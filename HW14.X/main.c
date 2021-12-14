#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include <stdio.h>
#include <math.h>
#include <proc/p32mx170f256b.h>
#include "servopos.h"


// DEVCFG0
#pragma config DEBUG = OFF // disable debugging
#pragma config JTAGEN = OFF // disable jtag
#pragma config ICESEL = ICS_PGx1 // use PGED1 and PGEC1
#pragma config PWP = OFF // disable flash write protect
#pragma config BWP = OFF // disable boot write protect
#pragma config CP = OFF // disable code protect

// DEVCFG1
#pragma config FNOSC = FRCPLL // use internal oscillator
#pragma config FSOSCEN = OFF // disable secondary oscillator
#pragma config IESO = OFF // disable switching clocks
#pragma config POSCMOD = OFF // internal RC 
#pragma config OSCIOFNC = OFF // disable clock output
#pragma config FPBDIV = DIV_1 // divide sysclk freq by 1 for peripheral bus clock
#pragma config FCKSM = CSDCMD // disable clock switch and FSCM
#pragma config WDTPS = PS1048576 // use largest wdt
#pragma config WINDIS = OFF // use non-window mode wdt
#pragma config FWDTEN = OFF // wdt disabled
#pragma config FWDTWINSZ = WINSZ_25 // wdt window at 25%

// DEVCFG2 - get the sysclk clock to 48MHz from the 8MHz crystal
#pragma config FPLLIDIV = DIV_2 // divide input clock to be in range 4-5MHz
#pragma config FPLLMUL = MUL_24 // multiply clock after FPLLIDIV
#pragma config FPLLODIV = DIV_2 // divide clock after FPLLMUL to get 48MHz

// DEVCFG3
#pragma config USERID = 0 // some 16bit userid, doesn't matter what
#pragma config PMDL1WAY = OFF // allow multiple reconfigurations
#pragma config IOL1WAY = OFF // allow multiple reconfigurations


int main() {
    int i;
    unsigned int position;
    unsigned int pwm;

    __builtin_disable_interrupts(); // disable interrupts while initializing things
    
    // set the CP0 CONFIG register to indicate that kseg0 is cacheable (0x3)
    __builtin_mtc0(_CP0_CONFIG, _CP0_CONFIG_SELECT, 0xa4210583);

    // 0 data RAM access wait states
    BMXCONbits.BMXWSDRM = 0x0;

    // enable multi vector interrupts
    INTCONbits.MVEC = 0x1;

    // disable JTAG to get pins back
    DDPCONbits.JTAGEN = 0;

    TRISAbits.TRISA4 = 0; // Output on A4
    LATAbits.LATA4 = 0; // Set the LED to off

    RPA2Rbits.RPA2R = 0b0110; //Set A2 to OC5
    
    T2CONbits.TCKPS = 0b111; //Set timer 2 prescaler to 256
    PR2 = 3750; //Configure period register
    TMR2 = 0; //Set timer
    
    OC5RS = 94; 
    OC5R = 94; //Set initial position to 0

    OC5CONbits.OCTSEL = 0; //Select timer 2
    OC5CONbits.OCM = 0b110; //Disable fault pin functionality
    
    T2CONbits.ON = 1; //Start timer
    OC5CONbits.ON = 1; //Start PWM
    actuate(180); //180 deg position

    __builtin_enable_interrupts();
     
     while (1) {
      
         for (i = 0; i < 6300; i+=63) { // source: http://www.ee.ic.ac.uk/pcheung/teaching/DE1_EE/stores/sg90_datasheet.pdf
        pwm = 282 + (unsigned int) 167*sin(i/1000.0);  //calculate duty cycle, source: //source: http://hades.mech.northwestern.edu/images/e/e3/EmbeddedComputingMechatronicsSampleChapters.pdf  
        position = (unsigned int) ((pwm-94)/2.083); //convert to position in degrees
        actuate(position); //move servo
        _CP0_SET_COUNT(0);
         while(_CP0_GET_COUNT()<(480000)){} //Wait .02 seconds

        LATAINV = 0x10; //Heartbeat
     
        }
     } 
 }




