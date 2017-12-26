#include "processor.h"
#include "PrintOutput.h"
#include "CMDMENU.h"
#include "BLDC_commutation.h"

#include "test.h"

#define WAIT_C          0
#define CONFIRM_C       1
#define SELECT_C        2
#define EXE             3

#define TMR1I           4
#define TMR2I           5
#define TMR12           6
#define SET_D1          7
#define SET_D2          8
#define SET_D3          9
#define SET_D4          10
#define SET_D5          11
#define SHOW_MENU       12

unsigned char state;

void InitMenu (void){
    
state = WAIT_C;
    
}

 void _MENU (volatile unsigned char data){
     
switch (state){
    
    case WAIT_C:
                    if (data == 'c'){
                        state = CONFIRM_C;
                    } else{
                        state = WAIT_C;
                    }
                    break;
                    
    case CONFIRM_C:                
                    if (data == ':'){
                        state = EXE;
                    }else {
                        state = WAIT_C;
                    }
                    break;
        
    case EXE:   
                    if (data == '1'){
                        state = TMR1I;

                      IEC0bits.T1IE       = 1;    // Enable Timer1 interrupt
                      IEC0bits.T2IE       = 0;    // Enable Timer2 interrupt

                          
                     UART_Put_String ("First timer ISR enabled.");
                                              
                        
                    }else if (data == '2'){
                        state = TMR2I;

                     IEC0bits.T1IE       = 0;    // Enable Timer1 interrupt
                     IEC0bits.T2IE       = 1;    // Enable Timer2 interrupt

                     UART_Put_String("Second timer ISR enabled.");

                        
                    }else if (data == '3'){
                        state = TMR12;
                             
                     IEC0bits.T1IE       = 1;    // Enable Timer1 interrupt
                     IEC0bits.T2IE       = 1;    // Enable Timer2 interrupt
                     
                       UART_Put_String ("Both of the timer ISR are enabled.");
                        
                    }else if (data == '4'){
                        state = SET_D1;
                        
                      P2DC1 = 0x1F40;
                      UART_Put_String ("P2DC1 = 0x1F40");
     

                    }else if (data == '5'){
                        state = SET_D2;
                        
                      P2DC1 = 0x3E80;
                       UART_Put_String ("P2DC1 = 0x3E80");
                       
                    }else if (data == '6'){
                        state = SET_D3;
                        
                      P2DC1 =  0x5DC0;
                      UART_Put_String ("P2DC1 = 0x5DC0");
                   
                    }else if (data == '7'){
                           state = SET_D4;
                           
                      P2DC1 =  0x7D00;
                     UART_Put_String ("P2DC1 = 0x7D00");
                   
                    }else if (data == '8'){
                         state = SET_D5;
                       
                      P2DC1 = 0x9C40;
                     UART_Put_String ("P2DC1 = 0x9C40");

                    }else if (data == 'm'){
                        state = SHOW_MENU;
                        
                        flag_o = 0b1;
                        
                    }         
                    break;
                    
                    
           
    
    default:            state = WAIT_C;          
         
}//end switch

//return state;

}// end CMDINTERP

 
 
