#include "processor.h"
#include "test.h"

#define s1      0
#define s2      1
#define s3      2
#define s4      3
#define s5      4
#define s6      5

#define A       LATBbits.LATB5
#define B       LATBbits.LATB6
#define C       LATBbits.LATB7


unsigned char state_test = s1;

void CL (unsigned char value){
    
    switch (state_test){
        
        case s1:
            if (value == 1){
                state_test = s2;
                scl2();
            }else if (value == 0){
                state_test = s1;
                scl6();
            }
            break;
            
        case s2:
            if (value == 2){
                state_test = s3;
                scl3();
            }else if (value == 1){
                state_test = s2;
                scl2();
            }
            break;
           
        case s3:
            if(value == 3){
                state_test = s4;
                scl4();
            }else if (value == 2){
                state_test = s3;
                scl3();
            }
            break;
            
        case s4:
            if(value == 4){
                state_test = s5;
                scl5();
            }else if (value == 3){
                state_test = s4;
                scl4();
            }
            break;
            
        case s5:
            if(value == 5){
                state_test = s6;
                scl6();
            }else if (value == 4){
                state_test = s5;
                scl5();
            }
            break;
         
        case s6:
            if(value == 0){
                state_test = s1;
                scl1();
            }else if (value == 5){
                state_test = s6;
                scl6();
            }
    }
    
}

void scl1 (void){
    A = 1;
    B = 0;
    C = 1; 
}

void scl2 (void){
    A = 1;
    B = 0;
    C = 0; 
}

void scl3 (void){
    A = 1;
    B = 1;
    C = 0; 
    
}

void scl4 (void){
    A = 0;
    B = 1;
    C = 0; 
}

void scl5 (void){
    A = 0;
    B = 1;
    C = 1; 
}

void scl6 (void){
    A = 0;
    B = 0;
    C = 1; 
}