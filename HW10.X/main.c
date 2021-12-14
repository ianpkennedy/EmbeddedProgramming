#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include <stdio.h>
#include "ws2812b.h"

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
    
    
    int i=0;
    int j=0;
    __builtin_disable_interrupts(); // disable interrupts while initializing things
    
    // set the CP0 CONFIG register to indicate that kseg0 is cacheable (0x3)
    __builtin_mtc0(_CP0_CONFIG, _CP0_CONFIG_SELECT, 0xa4210583);

    // 0 data RAM access wait states
    BMXCONbits.BMXWSDRM = 0x0;

    // enable multi vector interrupts
    INTCONbits.MVEC = 0x1;

    // disable JTAG to get pins back
    DDPCONbits.JTAGEN = 0;

    TRISAbits.TRISA4 = 0; // Output, heart beat
    LATAbits.LATA4 = 0; // Set the LED to off
     
    
    ws2812b_setup(); //initialize Neopixels
    
    __builtin_enable_interrupts();

    wsColor led1[4]; //create structure for Neopixels
    
    
    for (i = 0; i<4; i++) { //initialize colors
        led1[i].r = 0;
        led1[i].g = 0;
        led1[i].b = 0;
    }
    
        
            
    ws2812b_setColor(led1, 4); //referenced: https://cboard.cprogramming.com/c-programming/114106-pass-struct-array-function.html
    
    while (1) { //referenced: https://www.youtube.com/watch?v=EXr2_zSfnFw&t=1672s
        

       for( i=1;i<60;i++){
            for(j=0;j<4;j++){
                led1[j] = HSBtoRGB((float) (i +j*60.0), 1,0.5 );  //Set "rainbow" 
            }
        
            ws2812b_setColor(led1, 4);
             _CP0_SET_COUNT(0);
            while(_CP0_GET_COUNT()<120000){;}

        }
        
        
        _CP0_SET_COUNT(0); //heartbeat
        LATAINV = 0x10;
         while(_CP0_GET_COUNT()<12000000){;}
         
      
 }
}