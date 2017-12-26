#ifndef PID
#define	PID

typedef struct {
    
    float y0;   //output   
    float y1;   //previous output 
    float x0;   //input
    float x1;   //previous input
    float x2;   //previous^2 input 
    
    //PID parameters
    float Kp;   //proportional gain
    float Ki;   //integral gain
    float Kd;   //derivative gain
    
    //Sampling time
    float Tsamp;
    
    float setpoint;
    float measured;
    
    float a0;
    float a1;
    float a2;
    
}pid;

void InitPID(pid *p);
void PIDControl(pid *p);

#endif	

