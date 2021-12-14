#include "servopos.h"
#include <xc.h>


void actuate(unsigned int ang){ 

    
    unsigned int pulse = (unsigned int) (94.0 + (float) 2.083*ang); //convert angle to PWM duty cycle
    
    if (pulse<94) {
        OC5RS = 94; //0, minimum angular position
    }
    
    if (pulse>469) {
        OC5RS = 469; //180, maximum angular position
    }


    
    
    OC5RS = pulse; //Set duty cycle
    
    
    
}
