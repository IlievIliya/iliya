#include "ChangeNotification.h"
#include "processor.h"
#include "PrintOutput.h"
#include "BLDC_commutation.h"
#include "Hudson_23.h"

#include <stdio.h>

// Variable used in Period_CN function function to record the time //
unsigned int a = 0;
unsigned int b = 0;
unsigned int PERIOD = 0;


unsigned int HALL_VALUE; // HALL value (concatenated from the three inputs).
unsigned int mask = 0x00E0; // Mask used to perserve the informational bits.


void InitCN (void){
    
    //**CN my board**//
//      rb5 , cn27 (41)
//      rb6, cn24 (42)
//      rb7 cn23 (43)

//     RB5
    CNEN2bits.CN27IE = 1;
    CNPU2bits.CN27PUE = 0;
    
 //   RB6
    CNEN2bits.CN24IE = 1;
    CNPU2bits.CN24PUE = 0;
    
 //   RB7
    CNEN2bits.CN23IE = 1;
    CNPU2bits.CN23PUE = 0;
    
    IEC1bits.CNIE = 1;
    IFS1bits.CNIF = 0; // Reset CN interrupt

 
}

void __attribute__ ((interrupt, no_auto_psv)) _CNInterrupt(void)
{
    
      
 UART_Put_String ("   CN_ISR_   ");
  
 Hudson_Commutation_StartUp (); 
   
IFS1bits.CNIF = 0; // Clear CN interrupt

}

