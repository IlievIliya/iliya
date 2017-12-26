#include "processor.h"
#include "InputCapture.h"


void test_input (void){
        
    if ( PORTBbits.RB10 == 0b1){
    LATCbits.LATC9 = 0b0;
    }else 
    LATCbits.LATC9 = 0b1;
    
}
