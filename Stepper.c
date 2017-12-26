#include "Stepper.h"
#include "processor.h"
#include "global_data.h"

                    
#define phasea  0x0080   
#define phaseb  0x0100
#define phasec  0x0040
#define phased  0x0200

#define state0  0   
#define state1  1
#define state2  2
#define state3  3

#define stop    1
#define right   1



void InitStepFSM(void){
    en = PORTCbits.RC0;
    dir = PORTCbits.RC1;
    state_reg = state0;
}

void StepFSM(unsigned char en, unsigned char dir){
    
    switch(state_reg){
            case state0:    if(en==stop)   
                                state_reg=state0;
                            else if(dir==right)
                                state_reg=state1;
                            else    
                                state_reg=state3;
                            break;
                            
            case state1:    if(en==stop)
                                state_reg=state1;
                            else if(dir==right)
                                state_reg=state2;
                            else
                                state_reg=state0;
                            break;
            
            case state2:    if(en==stop)
                                state_reg=state2;
                            else if(dir==right)
                                state_reg=state3;
                            else   
                                state_reg=state1;
                            break;
            
            case state3:    if(en==stop)
                                state_reg=state3;
                            else if(dir==right)
                                state_reg=state0;
                            else
                                state_reg=state2;
                            break;
            
            default:        state_reg=state0;
    }
            
}

void StepAction(void){

    switch(state_reg){
            case state0:    
                        LATA = phasea;
                        break;
                            
            case state1:    
                        LATA = phaseb;
                        break;
                            
            case state2:    
                        LATA = phasec;
                        break;
                            
            case state3:    
                        LATA = phased;
                        break;
                            
            default:    
                        LATA = phasea;
    }
}




