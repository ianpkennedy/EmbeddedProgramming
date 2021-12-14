#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include <stdio.h>
#include <math.h>
#include "i2c_master_noint.h"
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

    __builtin_disable_interrupts(); // disable interrupts while initializing things
    
    // set the CP0 CONFIG register to indicate that kseg0 is cacheable (0x3)
    __builtin_mtc0(_CP0_CONFIG, _CP0_CONFIG_SELECT, 0xa4210583);

    // 0 data RAM access wait states
    BMXCONbits.BMXWSDRM = 0x0;

    // enable multi vector interrupts
    INTCONbits.MVEC = 0x1;

    // disable JTAG to get pins back
    DDPCONbits.JTAGEN = 0;

    TRISAbits.TRISA4 = 0; // Output
    LATAbits.LATA4 = 0; // Set the LED to off
             
    i2c_master_setup();
    
    __builtin_enable_interrupts();

  
    i2c_pintoggle(0b01000000,0x01,0b11111111); //pin 1 (GPB0) as input
    i2c_pintoggle(0b01000000,0x00,0b00000000); //Toggle output for all A pin
    i2c_pintoggle(0b01000000,0x14,0b00000000); //Turn off LED
  
    
    while (1) {
        
        while (i2c_pinread(0b0100000,0x13)){ //Read pin
      
          LATAINV = 0x10;
        _CP0_SET_COUNT(0);
        while(_CP0_GET_COUNT()<1200000){;}
            
        LATAINV = 0x10;
        _CP0_SET_COUNT(0);
        while(_CP0_GET_COUNT()<1200000){;}
    
        }
        
        i2c_pintoggle(0b01000000,0x14,0b10000000); //Turn on LED
        
        while (!(i2c_pinread(0b0100000,0x13))){
         LATAINV = 0x10;
        _CP0_SET_COUNT(0);
        while(_CP0_GET_COUNT()<1200000){;}
            
        LATAINV = 0x10;
        _CP0_SET_COUNT(0);
        while(_CP0_GET_COUNT()<1200000){;}
        }
        
        i2c_pintoggle(0b01000000,0x14,0b00000000); //Turn off LED
        
        
    }  
       
 }
