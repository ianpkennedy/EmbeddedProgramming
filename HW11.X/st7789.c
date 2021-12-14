// functions to operate the ST7789 on the PIC32
// adapted from https://github.com/sumotoy/TFT_ST7789
// and https://github.com/adafruit/Adafruit-ST7789-Library

// pin connections:
// GND - GND
// VCC - 3.3V
// SCL - B14
// SDA - B13
// RES - B15
// DC - B12
// BLK - NC

#include <xc.h>
#include "ST7789.h"
#include "spi.h"
#include "font.h"
#include <string.h>

void LCD_command(unsigned char com) {
    LATBbits.LATB12 = 0; // DC
    spi_io(com);
}

void LCD_data(unsigned char dat) {
    LATBbits.LATB12 = 1; // DC
    spi_io(dat);
}

void LCD_data16(unsigned short dat) {
    LATBbits.LATB12 = 1; // DC
    spi_io(dat>>8);
    spi_io(dat);
}

void LCD_init() {
  unsigned int time = 0;
  LCD_command(ST7789_SWRESET); //software reset
  time = _CP0_GET_COUNT();
  while (_CP0_GET_COUNT() < time + 48000000/2*0.15) {}
  
  LCD_command(ST7789_SLPOUT); //exit sleep
  time = _CP0_GET_COUNT();
  while (_CP0_GET_COUNT() < time + 48000000/2*0.5) {}
  
  LCD_command(ST7789_COLMOD);
  LCD_data(0x55);
  time = _CP0_GET_COUNT();
  while (_CP0_GET_COUNT() < time + 48000000/2*0.01) {}
  
  LCD_command(ST7789_MADCTL);
  LCD_data(0x00);
  time = _CP0_GET_COUNT();
  while (_CP0_GET_COUNT() < time + 48000000/2*0.001) {}
  
  LCD_command(ST7789_CASET);
  LCD_data(0x00);
  LCD_data(ST7789_XSTART);
  LCD_data((240+ST7789_XSTART)>>8);
  LCD_data((240+ST7789_XSTART)&0xFF);
  time = _CP0_GET_COUNT();
  while (_CP0_GET_COUNT() < time + 48000000/2*0.001) {}

  LCD_command(ST7789_RASET);
  LCD_data(0x00);
  LCD_data(ST7789_YSTART);
  LCD_data((240+ST7789_YSTART)>>8);
  LCD_data((240+ST7789_YSTART)&0xFF);
  time = _CP0_GET_COUNT();
  while (_CP0_GET_COUNT() < time + 48000000/2*0.001) {}
  
  LCD_command(ST7789_INVON);
  time = _CP0_GET_COUNT();
  while (_CP0_GET_COUNT() < time + 48000000/2*0.01) {}

  LCD_command(ST7789_NORON);
  time = _CP0_GET_COUNT();
  while (_CP0_GET_COUNT() < time + 48000000/2*0.01) {}
  
  LCD_command(ST7789_DISPON);
  time = _CP0_GET_COUNT();
  while (_CP0_GET_COUNT() < time + 48000000/2*0.5) {}
}

void LCD_drawPixel(unsigned short x, unsigned short y, unsigned short color) {
  // should check boundary first
  LCD_setAddr(x,y,x+1,y+1);
  LCD_data16(color);
}

void LCD_setAddr(unsigned short x0, unsigned short y0, unsigned short x1, unsigned short y1) {
  LCD_command(ST7789_CASET); // Column
  LCD_data16(x0+ST7789_XSTART);
  LCD_data16(x1+ST7789_XSTART);
  
  LCD_command(ST7789_RASET); // Page
  LCD_data16(y0+ST7789_YSTART);
  LCD_data16(y1+ST7789_YSTART);

  LCD_command(ST7789_RAMWR); // Into RAM
}

void LCD_clearScreen(unsigned short color) {
  int i;
  LCD_setAddr(0,0,_GRAMWIDTH,_GRAMHEIGH);
	for (i = 0;i < _GRAMSIZE; i++){
		LCD_data16(color);
	}
}


void drawChar(unsigned short x_1, unsigned short y_1, unsigned short color, unsigned char letter){
    int i;
    int j;
    unsigned char pix;
    
     for (i = x_1; i < x_1+5; i++) {
         pix = ASCII[letter-0x20][i-x_1];
         for (j = y_1; j < y_1+8; j++) {
             if( (pix  & (0b1 << (j-y_1))) != 0 ){
             LCD_drawPixel(i,j,color);
             
             }
         }
     }
}


void drawString(unsigned short x, unsigned short y, unsigned short color, unsigned char * s){

    int k=0;
    unsigned short xloc = x;
    unsigned short yloc = y;
        
    while (s[k] != '\0'){
   
    drawChar(xloc,yloc,color,s[k]);
   
    xloc += 5;
    k++;
    }
}

void drawX(unsigned short x, unsigned short y, signed short mag){
    
    int max = 120; //Half the screen

    signed short len = (mag/137); //scale sensor reading
    
    int i,j;
    
    if(len>0){
    for (i = 0; i < len; i++) {    
        for (j = 0; j < 8; j++) {
           LCD_drawPixel(x+i, y+j-4, GREEN); 
        }
    }
    
      for (i = len; i < max; i++) {    
        for (j = 0; j < 8; j++) {
           LCD_drawPixel(x+i, y+j-4, BLACK); 
        }
      }
      
       for (i = 0; i < 120; i++) {    
        for (j = 0; j < 8; j++) {
           LCD_drawPixel(x-i, y+j-4, BLACK); 
             }  
         }
    
    }
    
    if(len<0){
    for (i = 0; i < -len; i++) {    
        for (j = 0; j < 8; j++) {
           LCD_drawPixel(x-i, y+j-4, GREEN); 
        }
    }
    
      for (i = -len; i < max; i++) {    
        for (j = 0; j < 8; j++) {
           LCD_drawPixel(x-i, y+j-4, BLACK); 
        }
      }
      
       for (i = 0; i < 120; i++) {    
        for (j = 0; j < 8; j++) {
           LCD_drawPixel(x+i, y+j-4, BLACK); 
             }  
         }
    
    }
    
}

void drawY(unsigned short x, unsigned short y, signed short mag){
    
    int max = 120; //Half the screen
    signed short len = (mag/137); //scale sensor reading
    int i,j;
    
    if(len>0){
    for (i=0; i<8;i++) {    
        for (j=0;j<len;j++) {
           LCD_drawPixel(x+i-4, y+j, GREEN); 
        }
    }
      for (i=0;i<8;i++) {    
        for (j=len;j<max;j++) {
           LCD_drawPixel(x+i-4, y+j, BLACK); 
        }
      }    
       for (i=0;i<8;i++) {    
        for (j=0;j<max;j++) {
           LCD_drawPixel(x+i-4,y-j, BLACK); 
             }  
         }
    }  
    if(len<0){
    for (i=0;i<8;i++) {    
        for (j=0;j<-len;j++) {
           LCD_drawPixel(x+i-4,y-j,GREEN); 
        }
    }  
      for (i=0;i<8;i++) {    
        for (j=-len;j<max;j++) {
           LCD_drawPixel(x+i-4, y-j, BLACK); 
        }
      } 
       for (i=0;i<8;i++) {    
        for (j=0;j<max;j++) {
           LCD_drawPixel(x+i-4, y+j, BLACK); 
             }  
         }
    
    }
    
}
