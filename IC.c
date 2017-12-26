#include "processor.h"
#include "global_data.h"
#include "IC.h"

void InitIC1(void){
    
    IC1CONbits.ICSIDL   =   0;      //Input capture continues to operate in CPU Idle mode
    IC1CONbits.ICTMR    =   1;      //TMR2 contents are captured on capture event
    IC1CONbits.ICI      =   0b01;   //Interrupt on every second capture event
    IC1CONbits.ICM      =   0b011;  //Capture mode, every rising edge
    
}

void Measure_time_period(void){
    unsigned int t1, t2;
    t1=IC1BUF;
    t2=IC1BUF;
    IFS0bits.IC1IF=0;
    if(t2>t1)
        time_period = t2-t1;
    else
        time_period = (PR2 - t1) + t2;
}

void __attribute__((interrupt, no_auto_psv)) _IC1Interrupt (void)
{	
    Measure_time_period();
}


void __attribute__((interrupt, no_auto_psv)) _IC2Interrupt (void)
{	
	_IC2IF = 0;	// Cleat interrupt flag
}




