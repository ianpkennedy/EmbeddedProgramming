#include "NU32.h"          // config bits, constants, funcs for startup and UART
// include other header files here
#include "encoder.h"
#include "LCD.h"
#include "utilities.h"

#define BUF_SIZE 200
#define IPTS 100
#define DATPTS 1000


//Declaring global variables

static volatile Mode State;
static volatile int Pulsewidth;
static volatile unsigned int Duty;
static volatile int Duty_negative;
static volatile int Iwaveform[IPTS];
static volatile int Ivalue[IPTS];
static volatile int Send=0;
static volatile int Send_pos=0;
static volatile float Eint=0;
static volatile float Kp=0.0;
static volatile float Ki=0.0;
static volatile float Kp_pos=0.0;
static volatile float Ki_pos=0.0;
static volatile float Kd_pos=0.0;
static volatile int Target_pos=0;
static volatile int Itarget=0;
static volatile float Eint_pos=0;
static volatile float Pos_ref[DATPTS];
static volatile int Pos_out[DATPTS];

void iwave(void); //Itest wavveform

void __ISR(_TIMER_4_VECTOR, IPL5SOFT) Icontrol(void){ //Current controlling ISR
    
    static int iref = 0;
    static int count =0;
    static float u=0;
    static float error=0;
    static int current_whole_temp=0; //should this be updated to float?
    
    
    switch (State) {
        case 0: //IDLE
        {
            OC1RS = 0;
            break;
        }
        case 1: //PWM
        {
            if (Pulsewidth>0) {
                Duty = (unsigned int) ((Pulsewidth) * 10);
                LATDbits.LATD1 = 0; //Positive direction
                OC1RS=Duty;
            } if (Pulsewidth<0)  {
                Duty_negative =  -((Pulsewidth) * 10);
                LATDbits.LATD1 = 1; //Negative direction
                OC1RS = Duty_negative;
            }
            break;
        }
        case 2:
        {//ITEST
            
            current_whole_temp = sample_ADC_ma();
            
           
                iref=Iwaveform[count]; // Update the reference current for control signal
            
            
            error = iref-current_whole_temp; //Proportional error
            Eint = Eint+error; //Integral error
           
            u = Kp*error+Ki*Eint; //The control effort is in a PWM duty cycle %
            
            if (u<-100.0) { //Bound control effort
                u = -100.0;
            } else if(u>100){
                u=100.0;
            }
            
            if (u>0) { //Set the motor direction from control signal
                Duty = (unsigned int) ((u) * 10);
                LATDbits.LATD1 = 0; //positive direction
                OC1RS=Duty;
                
            } else if(u<0){
                LATDbits.LATD1=1; //Negative direction
                Duty_negative = (int) -((u) * 10);
                OC1RS = Duty_negative;
                
            }
            
            
            if (Send) {
                Ivalue[count] = current_whole_temp; //Update current output array
            }
            
            count++; //Increment the counter
            
            if (count == IPTS) {
                count=0; //reset the counter
                Send=0;
   
            }
            
            break;
        }
        case 3:
        { //HOLD
            
            current_whole_temp=sample_ADC_ma(); //Current readig
            error= Itarget-current_whole_temp;
            Eint = Eint+error;
            
            u = Kp*error+Ki*Eint; //Current controller output
           
            if (u<-100.0) {
                u = -100.0;
            } else if(u>100.0){
            
                u=100.0;
            }
            
            if (u>0) { //Set the motor direction from control signal
                Duty = (unsigned int) ((u) * 10);
                LATDbits.LATD1 = 0; //positive direction
                OC1RS=Duty;
                
            } else if(u<0){
                LATDbits.LATD1=1; //Negative direction
                Duty_negative = (int) -((u) * 10);
                OC1RS = Duty_negative;
                
            }
            
            break;
            
            
        }
        case 4:
        { //TRACK mode
            current_whole_temp=sample_ADC_ma();
            error= Itarget-current_whole_temp;
            Eint = Eint+error;
            
            u = Kp*error+Ki*Eint;
           
            if (u<-100.0) {
                u = -100.0;
            } else if(u>100.0){
            
                u=100.0;
            }
            
            if (u>0) { //Set the motor direction from control signal
                Duty = (unsigned int) ((u) * 10);
                LATDbits.LATD1 = 0; //positive direction
                OC1RS=Duty;
                
            } else if(u<0){
                LATDbits.LATD1=1; //Negative direction
                Duty_negative = (int) -((u) * 10);
                OC1RS = Duty_negative;
                
            }
        
            break;
        }
            
        default:
        {
            break;
        }
    }

    
   IFS0bits.T4IF=0; //clear interrupt flag

}

__ISR(_TIMER_3_VECTOR, IPL4SOFT) Poscontrol(void){ //Position controller
    
    static int pos=0;
    static int error_pos=0;
    static int error_prev=0;
    static int u_pos=0;
    static int error_dot=0;
    static int pcount=0;
    
    

    switch (State) {
        case 3: //HOLD
        {
            pos = encoder_angle(); //Calculate position
            error_pos = Target_pos-pos; //Calculating theta angle error
            Eint_pos = Eint_pos+error_pos; //Calculate integral error
            error_dot=error_pos-error_prev; //Calculate differential error
            
            u_pos = Kp_pos*error_pos+Ki_pos*Eint_pos+Kd_pos*error_dot;
            Itarget = (int) u_pos; //We are sending a int value of mA
        
    
            
            break;
        }
        
        
        case 4: //TRACK
        {
            pos = encoder_angle(); //Read angle
            error_pos = Pos_ref[pcount]-pos; //Calculate error
            Eint_pos = Eint_pos+error_pos; //Integral
            error_dot=error_pos-error_prev; //Derivative
            u_pos = Kp_pos*error_pos+Ki_pos*Eint_pos+Kd_pos*error_dot; //Control effort
            Itarget = (int) u_pos; //Send to current controller
            Pos_out[pcount]=pos; //Save the current position
            Target_pos = (int) Pos_ref[pcount]; //Assign target position
            pcount++; //increment counter

            
            if (pcount==DATPTS) {
                pcount=0;
                Send_pos=0;
                State=HOLD; //Exit the position traacking
            }
            
            break;
            
            
        }
            
    }
    
    error_prev=error_pos;
    
    IFS0bits.T3IF=0; //clear the interrupt flag
}

int main() 
{
  Pulsewidth = 0; //Set initial duty cycle
  Duty = 0;
    
    
  float kptemp = 0.0, kitemp=0.0, kp_postemp=0.0,ki_postemp=0.0,kd_postemp=0.0; //temporary gain variables
  int i=0, angle=0;
    
  iwave(); //Initialize Itest reference signal
  char buffer[BUF_SIZE]; //Initialize communication buffer
  State = IDLE;
  float current= -1.0;
  int whole_current = -1;
  int len = IPTS;
    
  NU32_Startup(); // cache on, min flash wait, interrupts on, LED/button init, UART init
    
  NU32_LED1 = 1;  // turn off the LEDs
  NU32_LED2 = 1;        
  __builtin_disable_interrupts();
  // in future, initialize modules or peripherals here
    LCD_Setup();
    LCD_Clear();
    LCD_Move(0,0);
    LCD_WriteString("Initializing"); //Check the program is working
    
    encoder_init(); //Initialize encoder
    
    ADC_init(); //Initialize ADC

    periph_init(); //Initalize OC1, TIMER 2 and TIMER 4
    
    pos_control_init(); //Initalize position control with timer 3
    
    
    IPC4bits.T4IP = 5; //Set priority level 5
    IPC4bits.T4IS = 0; //Set subpriority to 0
    IFS0bits.T4IF=0; //clear interrupt flag
    IEC0bits.T4IE = 1; //Enable timer4 interrupt
    
    IPC3bits.T3IP = 4; //Priority level 4
    IPC3bits.T3IS = 0; //Subpriority 0
    IFS0bits.T3IF=0; //Clear interrupt flag
    IEC0bits.T3IE = 1; //Enable timer3 interrupt
    
    TRISDbits.TRISD3 = 0; //Set digital output to RD6
    LATDbits.LATD3 = 1; //Initially set to lo
    
  __builtin_enable_interrupts();

    

    
  while(1)
  {
    NU32_ReadUART3(buffer,BUF_SIZE); // we expect the next character to be a menu command
    NU32_LED2 = 1;                   // clear the error LED
    switch (buffer[0]) {
      case 'a': //Read ADC
        {
            sprintf(buffer,"%d \r\n",sample_ADC());
            NU32_WriteUART3(buffer);
            break;
        }
        case 'b': //Read ADC in mA
        {
            current =  1.294*sample_ADC() -  654.22;
            whole_current = (int) current;
            sprintf(buffer,"%d \r\n", whole_current);
            NU32_WriteUART3(buffer);
            break; //Used least squares curve fit implemented in Matlab
            //Sources: https://www.mathworks.com/help/optim/ug/lsqcurvefit.html and Matlab help function for lsqcurvefit and plot functions
            
        }
        
      case 'd':                      // Encoder angle
      {
        sprintf(buffer,"%d \r\n",encoder_angle());
        sprintf(buffer,"%d \r\n",encoder_angle());
        NU32_WriteUART3(buffer);
        break;
      }
        case 'c': //Read encoder count
        {
            sprintf(buffer,"%d \r\n",encoder_counts());
            sprintf(buffer,"%d \r\n",encoder_counts());
            NU32_WriteUART3(buffer);
            break;
        }
            
        case 'e':
        {
            encoder_reset(); //Reset encounter
            break;
        }
        case 'f': //PWM mode - enter PWM in %
        {
            State = PWM;
            NU32_ReadUART3(buffer,BUF_SIZE);
            sscanf(buffer, "%d", &Pulsewidth);
            
            break;
            
        case 'g':
            {
                __builtin_disable_interrupts();
                
                NU32_ReadUART3(buffer,BUF_SIZE); //Read in values for current control gains
                sscanf(buffer,"%f %f",&kptemp,&kitemp);
                
                Kp=kptemp; //50 is a good number
                Ki=kitemp; //2 is a good number
                
                __builtin_enable_interrupts();
               
    
                break;
            }
        }
        case 'h':
        {
            sprintf(buffer,"%f %f \r\n",Kp,Ki); //Read current gains
            NU32_WriteUART3(buffer);
            break;
        }
         
        case 'i':
        {
            __builtin_disable_interrupts();
            NU32_ReadUART3(buffer,BUF_SIZE); //Inputting the position gains
            sscanf(buffer,"%f %f %f",&kp_postemp,&ki_postemp,&kd_postemp);
            Kp_pos = kp_postemp;
            Ki_pos = ki_postemp;
            Kd_pos = kd_postemp;
            
            __builtin_enable_interrupts();
            
            break;
        }
        case 'j':
        {
            sprintf(buffer,"%f %f %f \r\n",Kp_pos,Ki_pos,Kd_pos); //Read position gains
            NU32_WriteUART3(buffer);
            break;
        }
        
        case 'k':
        {
            Eint=0; //Reset integral error before Itest
            
            sprintf(buffer,"%d \r\n",len);
            NU32_WriteUART3(buffer);
            
            State = ITEST; //Initiate the Itest
            Send = 1;
            
            while (Send) { //collecting data
                ;
            }

            State = IDLE; //Return to IDLE

            
            for (i=0; i<IPTS; i++) {
                sprintf(buffer,"%d %d \r\n",Iwaveform[i],Ivalue[i]);
                NU32_WriteUART3(buffer);
            }
            
            break;
        }
        case 'l':
        {
            Eint_pos=0; //Reset integral error
            Eint=0;
            
            __builtin_disable_interrupts();
            State = HOLD; //Go to hold state
            NU32_ReadUART3(buffer,BUF_SIZE); //Inputting the position gains
            sscanf(buffer,"%d",&angle);
            Target_pos=angle; //Setting target position
            __builtin_enable_interrupts();
            break;
        }
        case 'm': //Set step trajectory
        {
            for (i=0; i<DATPTS; i++) {
                NU32_ReadUART3(buffer,BUF_SIZE);
                sscanf(buffer, "%f",&Pos_ref[i]);
            }
            break;
        }
        case 'n': //Set cubic trajectory
        {
            for (i=0; i<DATPTS; i++) {
                NU32_ReadUART3(buffer,BUF_SIZE);
                sscanf(buffer, "%f",&Pos_ref[i]);
            }
            break;
        }
        case 'o': //Execute trajectory
        {
            Eint_pos=0;
            Eint=0;
            
            __builtin_disable_interrupts();
            
            Send_pos = 1;
            State=TRACK;
            
            __builtin_enable_interrupts();

            while (Send_pos) {
                ;
            }
            
            for (i=0; i<DATPTS; i++) { //Send position for plotting
                sprintf(buffer,"%d \r\n",Pos_out[i]);
                NU32_WriteUART3(buffer);
            }
            break;
            
        }
        case 'u':
        {
            State = IDLE; //Switching to IDLE
            
            break;
        }
        
      case 'q':
      {
          State = IDLE; //Switch to IDLE when quitting client
          
        break;
      }
      case 'r':
        {
         
            get_mode(State); //Get the current mode
            
        }
      default:
      {
        NU32_LED2 = 0;  // turn on LED2 to indicate an error
        break;
      }
    }
  }
  return 0;
}


void iwave(void){ //Generate the ITEST waveform
    
    int i=0;
    int amp=200;
    
    for (i=0;i<IPTS;i++) {
        if (i<IPTS/4) {
            Iwaveform[i] = amp;
        } else if (i>=IPTS/4 & i<IPTS/2){
            Iwaveform[i]=-amp;
            
        } else if (i>=IPTS/2 & i<3*IPTS/4){
            Iwaveform[i] = amp;
        } else {
            
            Iwaveform[i]=-amp;
        }
        
    }
    
    
    
}
