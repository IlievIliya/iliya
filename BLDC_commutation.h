#ifndef BLDC_COMMUTATION_H
#define	BLDC_COMMUTATION_H

#include <stdbool.h>

void InitBLCD_Commutation (void);
extern void Commutation_Sequence (unsigned int data_in);
extern unsigned int  GET_TIME (unsigned char state);


 void MotorStartup (unsigned char cycles);
 void CHECK_cycles (unsigned char cycles);
 
 extern unsigned char commutation_state;
 extern int time;
 


 
#endif	// BLDC_COMMUTATION_H

