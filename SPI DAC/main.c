#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include <stdio.h>
#include <math.h>

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


void initSPI();
unsigned char spi_io(unsigned char word);
void maketriangle();
void makesin();


unsigned short Wave_sin[200];
unsigned short Wave_tri[200];

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

    // do your TRIS and LAT commands here
    TRISAbits.TRISA4 = 0; // Output
    //TRISBbits.TRISB4 = 1; // Input
    LATAbits.LATA4 = 0; // Set the LED to off
     
    initSPI(); //initialize SPI
    
    makesin(); //make sinusoid
    maketriangle(); //make triangle waveform
    
    __builtin_enable_interrupts();

    unsigned short datb = 0; //total SPI word to send, 16 bit
    unsigned short chanb = 1; //is 1 for B
    
    unsigned short data = 0;
    unsigned short chana = 0; //Channel A
    
    int i = 0;
    

    
    while (1) {
        
        
        for (i=0;i<200;i++){
            _CP0_SET_COUNT(0);
            
             LATAbits.LATA0 = 0; //Chip select is on   
      
                datb = (chanb<<15);
                datb = datb | (0b111<<12);
                datb = datb | (Wave_sin[i]<<2);
                spi_io(datb>>8); //send the left side of word
                spi_io(datb); //send the right side
                
            LATAbits.LATA0 = 1; //Chip select is off      
            LATAbits.LATA0 = 0; //Chip select is on       
                
                data = (chana<<15);
                data = data | (0b111<<12);
                data = data | (Wave_tri[i] << 2);
                spi_io(data>>8); //send the left side of word
                spi_io(data); //send the right side
                
            LATAbits.LATA0 = 1; //Chip select is off   
            //LATAINV = 0x10;

            while (_CP0_GET_COUNT()<120000){ //every .005 seconds
                ;
            }
            
            
 
        }  
       
    }
    
}

void makesin(){ //Generate sinusoid
    
    int t1;
    float temp;
    for (t1 = 0; t1 < 200; t1++) {
        temp = 511.0*sin(2.0*3.1416*((float) t1/100.0))+512.0;
        Wave_sin[t1] = (unsigned short) temp;
    }
}


void maketriangle(){ //Generate triangle wave
       
    int t2;
    float temp = 0;
  
    for (t2 = 0; t2 < 200; t2++) {
        
       Wave_tri[t2] = (unsigned short) temp;
       
       if (t2<100) {
           temp = temp + 1012.0/100.0;
        }
       
       if (t2>=100){
           temp = temp - 1012.0/100.0;
       } 
    }
}

// initialize SPI1
void initSPI() {
    // Pin B14 has to be SCK1
    // Turn of analog pins
    ANSELA = 0;
    // Make an output pin for CS
    TRISAbits.TRISA0 = 0; //output
    LATAbits.LATA0 = 1; //Chip select is off
    // Set SDO1
    RPA1Rbits.RPA1R = 0b0011;
    // Set SDI1
    SDI1Rbits.SDI1R = 0b0001;

    // setup SPI1
    SPI1CON = 0; // turn off the spi module and reset it
    SPI1BUF; // clear the rx buffer by reading from it
    SPI1BRG = 1; // 1000 for 24kHz, 1 for 12MHz; // baud rate to 10 MHz [SPI1BRG = (48000000/(2*desired))-1]
    SPI1STATbits.SPIROV = 0; // clear the overflow bit
    SPI1CONbits.CKE = 1; // data changes when clock goes from hi to lo (since CKP is 0)
    SPI1CONbits.MSTEN = 1; // master operation
    SPI1CONbits.ON = 1; // turn on spi 
}


// send a byte via spi and return the response
unsigned char spi_io(unsigned char word) {
  SPI1BUF = word;
  while(!SPI1STATbits.SPIRBF) { // wait to receive the byte
    ;
  }
  return SPI1BUF;
}


