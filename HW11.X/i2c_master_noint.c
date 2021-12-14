// I2C Master utilities, using polling rather than interrupts
// The functions must be called in the correct order as per the I2C protocol
// I2C pins need pull-up resistors, 2k-10k
#include "i2c_master_noint.h"

void i2c_master_setup(void) {
    // using a large BRG to see it on the nScope, make it smaller after verifying that code works
    // look up TPGD in the datasheet
    I2C1BRG = 55; //1000;  //55; // I2CBRG = [1/(2*Fsck) - TPGD]*Pblck - 2 (TPGD is the Pulse Gobbler Delay). Generating 400 kHz chip frequency
    I2C1CONbits.ON = 1; // turn on the I2C1 module
}

void i2c_master_start(void) {
    I2C1CONbits.SEN = 1; // send the start bit
    while (I2C1CONbits.SEN) {
        ;
    } // wait for the start bit to be sent
}

void i2c_master_restart(void) {
    I2C1CONbits.RSEN = 1; // send a restart 
    while (I2C1CONbits.RSEN) {
        ;
    } // wait for the restart to clear
}

void i2c_master_send(unsigned char byte) { // send a byte to slave
    I2C1TRN = byte; // if an address, bit 0 = 0 for write, 1 for read
    while (I2C1STATbits.TRSTAT) {
        ;
    } // wait for the transmission to finish
    if (I2C1STATbits.ACKSTAT) { // if this is high, slave has not acknowledged
        // ("I2C1 Master: failed to receive ACK\r\n");
        while(1){} // get stuck here if the chip does not ACK back
    }
}

unsigned char i2c_master_recv(void) { // receive a byte from the slave
    I2C1CONbits.RCEN = 1; // start receiving data
    while (!I2C1STATbits.RBF) {
        ;
     
    } // wait to receive the data
    return I2C1RCV; // read and return the data
}

void i2c_master_ack(int val) { // sends ACK = 0 (slave should send another byte)
    // or NACK = 1 (no more bytes requested from slave)
    I2C1CONbits.ACKDT = val; // store ACK/NACK in ACKDT
    I2C1CONbits.ACKEN = 1; // send ACKDT
    while (I2C1CONbits.ACKEN) {
        ;
      
    } // wait for ACK/NACK to be sent
}

void i2c_master_stop(void) { // send a STOP:
    I2C1CONbits.PEN = 1; // comm is complete and master relinquishes bus
    while (I2C1CONbits.PEN) {
        ;
        
        
    } // wait for STOP to complete
}

void i2c_pintoggle(unsigned char add, unsigned char reg, unsigned char val){
        i2c_master_start(); //start bit
        i2c_master_send(add);   //device address, write configuration
        i2c_master_send(reg); //register address 
        i2c_master_send(val); //set all to output
        i2c_master_stop(); //stop bit
     
    
}

unsigned char i2c_pinread(unsigned char add, unsigned char reg){
    
    unsigned char byte; //Read data from pin

    i2c_master_start();    
    i2c_master_send(add << 1); //Write address
    i2c_master_send(reg);
    i2c_master_restart();
    i2c_master_send((add << 1 ) | 1); //Read address
    byte = i2c_master_recv(); 
    i2c_master_ack(1); //read once only
    i2c_master_stop();


    return byte;
    
}


void I2C_pin_read_multiple(unsigned char add, unsigned char reg, unsigned char * data, int length){
    int i =0;
        
    i2c_master_start();
        
    i2c_master_send(add << 1); //Write address
    i2c_master_send(reg);
    i2c_master_restart();
    i2c_master_send((add << 1 ) | 1); //Read address
        
        
        
    for (i = 0; i < length - 1; i++) {
        data[i] = i2c_master_recv(); 
        i2c_master_ack(0);
    }

        data[13]=i2c_master_recv(); // OUTZ_H_XL   
        i2c_master_ack(1); //read 
  
        i2c_master_stop();
           
}

void I2C_IMU_setup(){
    
    unsigned char echo;
    
    echo = i2c_pinread(IMU_ADDR, IMU_WHOAMI);

    if (echo!= 0b01101001) {
     while(1){
     _CP0_SET_COUNT(0);
     while(_CP0_GET_COUNT()<24000000){}
     LATAINV = 0x10; //infinite loop  
          }
     }
    
    i2c_pintoggle(IMU_ADDR<<1,IMU_CTRL1_XL,0b10000010); //1.66 kHz, 2g sensitivity, 100 Hz filter for accel
    i2c_pintoggle(IMU_ADDR<<1,IMU_CTRL2_G,0b10001000); //1.66 kHz, 1000 dps gyro
    i2c_pintoggle(IMU_ADDR<<1,IMU_CTRL3_C,0b00000100); //Set IF_INC, sequential reading enabled
    
 
    
}