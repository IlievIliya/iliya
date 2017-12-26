#include "PID.h"
#include "global_data.h"
#include "processor.h"

#define SAMP_TIME   0.13
#define SETPOINT    500
#define PWM_MAX     65000
#define PWM_MIN     0

void InitPID(pid *p){
    
    
    p->y0 = 0;    
    p->y1 = 0;    
    p->x0 = 0;   
    p->x1 = 0;   
    p->x2 = 0;   
    
    
    p->Kp = 0;   
    p->Ki = 0;   
    p->Kd = 0;   
    
    p->Tsamp = SAMP_TIME;
    
    p->setpoint = SETPOINT;
    
    p->a0 = p->Kp + p->Ki*p->Tsamp + p->Kd/p->Tsamp;
    p->a1 = -(p->Kp) -2*(p->Kd/p->Tsamp);
    p->a2 = p->Kd/p->Tsamp;
    
}

void PIDControl(pid *p){
    
    unsigned char PWM_DC;
    
    //This is ISR on timer interrupt = 160ms (sampling time)
    
    //update variables
    p->y1 = p->y0;
    p->x2 = p->x1;
    p->x1 = p->x0;
    
    //p->measured = read_adc();
    p->x0 = p->setpoint - p->measured;
    
    //PID equation 
    p->y0 = p->y1 + p->a0*p->x0 + p->a1*p->x1 + p->a2*p->x2;
    
    //Limit the PWM range
    if (p->y0 > PWM_MAX)   p->y0 = PWM_MAX;
    if (p->y0 < PWM_MIN)   p->y0 = PWM_MIN;
    
    //cast 
    PWM_DC = (unsigned char)p->y0;
    
    //send to PWM
    P2DC1 = PWM_DC;
    
}

