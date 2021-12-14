#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include <stdio.h>
#include "st7789.h"
#include "font.h"
#include "spi.h"


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
    
    char message[100];
    int i;
    int j,k;
    float fps;
    
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
         
    initSPI(); //Initialize SPI for LCD
    LCD_init(); //Initialize LCD screen
    
    __builtin_enable_interrupts();

    LCD_clearScreen(BLACK); 
    _CP0_SET_COUNT(0);
    
    for(i=0;i<48;i++){ //Draw letter to screen
        for(j=0;j<30;j++){
               drawChar(5*i, 8*j, RED, 'A');
 
        }
        
    }
    fps = (float) 24000000/_CP0_GET_COUNT(); //Measure FPS
    
    while(_CP0_GET_COUNT()<120000000/1.5){}
    LCD_clearScreen(BLACK); 

    sprintf(message,"FPS:%f", fps);

    drawString(20,20,RED,message);

    while(_CP0_GET_COUNT()<240000000/1.5){}
    
    LCD_clearScreen(BLACK); //Reset
    sprintf(message,"hello world");
    drawString(28,32,GREEN,message);
     
    while (1) {
        
        for(i=0;i<100;i++){
          
         LATAINV = 0x10;
          
         _CP0_SET_COUNT(0);
         sprintf(message, "%d", i); 
         drawString(28+12*5,32,GREEN,message);
         drawBar(28, 52, i); //Update progress bar
         fps = (float) 24000000/_CP0_GET_COUNT();
         
        sprintf(message,"FPS:%f", fps); 
        
        drawString(28,150,GREEN,message);
         
         while(_CP0_GET_COUNT()<2400000){} //Wait till 0.1 s, 10 Hz
         
         for (j=88;j<98;j++){ //Clear 
             for (k=32; k<40;k++){
                 LCD_drawPixel(j, k, BLACK); //Clear integer i printing
                }  
             }
        
         for (j=28+20;j<(28+20+9*5);j++){
             for (k=150; k<158;k++){
                 LCD_drawPixel(j, k, BLACK); //Clear FPS estimate
                }  
             }
        }
    }
}