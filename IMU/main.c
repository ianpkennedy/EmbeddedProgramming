#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include <stdio.h>
#include <math.h>
#include "i2c_master_noint.h"
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
   unsigned char arr[14]; //Data vector for temp, rate, and acceleration
   signed short  temp;
   signed short x_gyro,y_gyro,z_gyro;
   signed short x_accel, y_accel, z_accel;
   
   int i,j,k=0;
   int max = 120;
   
   for (i = 0; i < 14; i++) {
        arr[i] = 0;
    }
 
        
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
    i2c_master_setup();
    I2C_IMU_setup();
    LCD_clearScreen(BLACK);

    __builtin_enable_interrupts();

    _CP0_SET_COUNT(0);

    sprintf(message,"temper:" );
   drawString(0,20,WHITE,message); 
    
   sprintf(message,"gyro:" );
   drawString(0,30,WHITE,message); 
   
   sprintf(message,"accl:" );
   drawString(0,40,WHITE,message); 
   
   while (k<150) {
  
    LATAINV = 0x10;  //heartbeat  
    
        
   I2C_pin_read_multiple(IMU_ADDR, IMU_OUT_TEMP_L, arr, 14); //query data
       
   temp = arr[1] << 8; //construct data
   temp = temp | arr[0];
   x_gyro = arr[3] << 8;
   x_gyro = x_gyro | arr[2];
   y_gyro = arr[5] << 8;
   y_gyro = y_gyro | arr[4];  
   z_gyro = arr[7] << 8;
   z_gyro = z_gyro | arr[6]; 
   x_accel = arr[9] << 8;
   x_accel = x_accel | arr[8]; 
   y_accel = arr[11] << 8;
   y_accel = y_accel | arr[10];
   z_accel = arr[13] << 8;
   z_accel = -(z_accel | arr[12]);
   
   sprintf(message,"%d",temp );
   drawString(40,20,WHITE,message); 
   sprintf(message,"%d",x_gyro );
   drawString(40,30,WHITE,message);
   sprintf(message,"%d",y_gyro );
   drawString(60,30,WHITE,message);
   sprintf(message,"%d",z_gyro );
   drawString(85,30,WHITE,message);
   sprintf(message,"%d",x_accel );
   drawString(40,40,WHITE,message);
   sprintf(message,"%d",y_accel );
   drawString(65,40,WHITE,message);
   sprintf(message,"%d",z_accel );
   drawString(90,40,WHITE,message);
   
   
   
   
   
    while(_CP0_GET_COUNT()<(24000000/20)){} //Update at 20 Hz
         _CP0_SET_COUNT(0);
         k++;
      for (i=8*5;i<130;i++){
             for (j=20; j<50;j++){
                 LCD_drawPixel(i, j, BLACK);
                }  
             }

    } 

      _CP0_SET_COUNT(0);
      LCD_clearScreen(BLACK); //Clear screen

     
      while (1) {
      LATAINV = 0x10;  //heartbeat 

         
    I2C_pin_read_multiple(IMU_ADDR, IMU_OUT_TEMP_L, arr, 14); //Query data
       
   temp = arr[1] << 8; //construct data
   temp = temp | arr[0];
   x_gyro = arr[3] << 8;
   x_gyro = x_gyro | arr[2];
   y_gyro = arr[5] << 8;
   y_gyro = y_gyro | arr[4];
   z_gyro = arr[7] << 8;
   z_gyro = z_gyro | arr[6];         
   x_accel = arr[9] << 8;
   x_accel = x_accel | arr[8];
   y_accel = arr[11] << 8;
   y_accel = y_accel | arr[10];
   z_accel = arr[13] << 8;
   z_accel = -(z_accel | arr[12]);

   drawX(120,120,-y_accel);//Draw inclinometer bars
   drawY(120,120,-x_accel); 
   
    while(_CP0_GET_COUNT()<(24000000/20)){} //Sample at 20 Hz
    _CP0_SET_COUNT(0); 
        } 
 }
