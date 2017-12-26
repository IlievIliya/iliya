// Six stepping sequence //

///***States coding***////

#define state1 0
#define state2 1
#define state3 2
#define state4 3
#define state5 4
#define state6 5


#define start_T 1
#define stop_T  2

#include "BLDC_commutation.h"
#include "processor.h"
#include "PWM.h"
#include "Trapezoidal_generator.h"
#include "PrintOutput.h"
#include "Hudson_23.h"

#include <stdbool.h>
#include <stdio.h>

/// *** Motor startup flags *** ///
unsigned char _states_index = 0; // Index used to enumarate the states
unsigned char start = 0b1; // start flag;
unsigned char _start_index = 0; //

unsigned char commutation_state; // Evaluates the current state


/// *** Init the BLDC FSM to state1 *** ///

void InitBLCD_Commutation (void){
    
commutation_state = state1; // After reset the initial state is state1.
UART_Put_String("BLDC_commutation_initialized."); // Indicates the initialization has beenn completed.

}


///"Data_in" received from sensors (Hall) or other feedback providing information
//about the position of the rotor.

//FSM dictating the sequence of commutation in function of the sensor data.

//SIX stepping sequence.

void Commutation_Sequence (unsigned int data_in) {
    
    
    switch (commutation_state){
        
        case state1:
            
          //  UART_Put_String("  ST1  ");
            
            if (data_in == 1){                       //0b000 //CLockwise rotation /1
                commutation_state = state2;
                STATE_2i();
              //  U1TXREG = '2';
               
            }else if (data_in == 0){                 //0b001 /0
                commutation_state = state1;
                STATE_1i();
           //      U1TXREG = '1';
                 
            }else if (data_in == 5){                 //0b011 //CounterClockwise /5
                                                            // rotation
                commutation_state = state6;
                STATE_5i();
            //     U1TXREG = '6';
            }
                    
          break;

        case state2:
         //   UART_Put_String("  ST2  ");
            
            if (data_in == 2){                      //0b100 /2
                commutation_state = state3;
                STATE_3i();
             //    U1TXREG = '3';
                
            }else if (data_in == 1){                //0b000 /1
                commutation_state = state2;
                STATE_2i();
             //   U1TXREG = '2';
                
            }else if (data_in == 0) {                //0b001 /0
                commutation_state = state1;
                STATE_3i();
            //    U1TXREG = '3';
             
            }
            
            break; 
            
        case state3:
        //    UART_Put_String("  ST3  ");
            
            if (data_in == 3){                      //0b110 /3
                commutation_state = state4;
                STATE_4i();
            //   U1TXREG = '4';
                
            }else if (data_in == 2){                //0b100 /2
                commutation_state = state3;
                STATE_3i();
            //     U1TXREG = '3';
               
            }else if (data_in == 1){                 //0b000 /1
                commutation_state = state2;
                STATE_2i();
            //    U1TXREG = '2';
               
            }                  
            
            break;
        
        case state4:
       //    UART_Put_String("  ST4   ");
            
            if (data_in == 4){                       //0b111 /4
                commutation_state = state5;
                STATE_5i();
            //     U1TXREG = '5';
                 
            }else if (data_in == 3){                 //0b110 /3
                commutation_state = state4;
                STATE_4i();
            //     U1TXREG = '4';
                 
            }else if (data_in == 2){                  //0b100 /2
                commutation_state = state3;
                STATE_3i();
             //    U1TXREG = '3';
               
            }
            
           break;
        
        case state5:
          //  UART_Put_String("  ST5   ");
            
            if (data_in == 5){                          //0b011 /5
                commutation_state = state6;
                STATE_6i();
           //     U1TXREG = '6';
                 
            }else if (data_in == 4){                     //0b111 /4
                commutation_state = state5;
                STATE_5i();
           //      U1TXREG = '5';
               
            }else if (data_in == 3){
                commutation_state = state4;             //0b110 /3
                STATE_4i();
            //    U1TXREG = '4';
               
            }
            break;
            
        case state6:
        //    UART_Put_String("  ST6  ");
            
            if (data_in == 0){                           //0b001 /0
                commutation_state = state1;
                STATE_1i();
            //     U1TXREG = '1';
                
            }else if (data_in == 5){                      //0b011 /5
                commutation_state = state6;
                STATE_6i();
            //     U1TXREG = '6';
                 
            }else if (data_in == 4) {                   //0b111 /4
                commutation_state = state5;
                STATE_5i();
            //    U1TXREG = '5';
                
            }
            break;
            
        default: commutation_state = state1;
        
    }//End Switch case;
    
    
}// End Commutation_Sequence function;


    
  //*** Function which has to be executed once to start the motor with (50-70)%
 // of its maximum speed during the first 10 seconds. ***//
//


 void MotorStartup (unsigned char cycles){
       
if (start == 0b0){               
             if(_states_index == 0){
             STATE_1i();
        
             }else if (_states_index == 1){
             STATE_2i();
       
              }else if (_states_index == 2){
             STATE_3i();
       
               }else if (_states_index == 3){
             STATE_4i();
       
              }else if (_states_index == 4){
             STATE_5i();
                
             }else if (_states_index == 5){
             STATE_6i();
             }
     }  
//         Six step commutation sequence.
             
       _states_index = (_states_index +1)%6;
       _start_index ++;
 
       if (_start_index == cycles){
           start = 0b1;
       }
       
 }// END MOTOR Startup
 
 
// 
// void CHECK_cycles (unsigned char cycles){
//     if (_start_index == cycles){
//         start = 0b1;
//     }
//     
// }
//  
//
// 
// unsigned int  GET_TIME (unsigned char state){
//     
//     if (state == start_T){
//         time = TMR2;
//     } 
//     
//     if (state == stop_T){
//        time = TMR2 - time;
//     }
//    
//    
// }
//
// 
