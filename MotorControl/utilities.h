//
//  utilities.h
//  
//
//  Created by Ian Kennedy on 3/5/21.
//

#ifndef utilities_h
#define utilities_h

#include <stdio.h>

typedef enum {IDLE, PWM, ITEST, HOLD, TRACK} Mode; //Source: https://www.geeksforgeeks.org/enumeration-enum-c/

int sample_ADC();

int sample_ADC_ma();

void ADC_init();

void periph_init();

void get_mode(int var);

void pos_control_init();

#endif /* utilities_h */
