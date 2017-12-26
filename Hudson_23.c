#include "Hudson_23.h"
#include "processor.h"
#include "PrintOutput.h"
#include "Trapezoidal_generator.h"


#include "string.h"

#include <stdint.h>


/*** HUDSON NEMA 23 (connector)***/

/*
 pins 
 - 9 phase R;
 - 10 phase S;
 - 11 phase T;
 
 - 3 COMM S-T C;
 - 4 COMM R-S B;
 - 5 COMM T-R A;
 
 */

HudsonTAG Hudson_23bits;


void init_Hudson23(void){
        
    Hudson_23bits.comstate = BLDC_HEADER;
    
    Hudson_23bits.Break = 0b0;
    Hudson_23bits.CCW_Rotation = 0b0;
    Hudson_23bits.CW_Rotation = 0b0;
    Hudson_23bits.Stop = 0b0;
    
    Hudson_23bits.Comb_logic = 0b0;
    Hudson_23bits.counter = 0b0;
    Hudson_23bits.index = 0b0;
    
    Hudson_23bits.brake_indx = 0b0;
    

    UART_Put_String ("Motor set.");
    
}

void Hudson_Commutation_StartUp (void){
//void Hudson_Commutation_StartUp (unsigned int data_in){
    
    
    //  *** Check direction flag and break flag. *** //
    //  CCW rotation 
    //  m-L command;
    if((Hudson_23bits.CCW_Rotation == 0b1) && (Hudson_23bits.Stop == 0b0)){
    
    switch (Hudson_23bits.state){
        
                 case st1:
                     
                      U1TXREG = '1';
                
                   if ((PORTBbits.RB5 == 0b1) && (PORTBbits.RB6 == 0b0) && (PORTBbits.RB7 == 0b0)){
                                                                    // HALL Combination
                      //  if (data_in == 0b100){                     //0b100 //1
                            Hudson_23bits.state = st2;    
                            step2();
                            
                   }else if ((PORTBbits.RB5 == 0b1) && (PORTBbits.RB6 == 0b0) && (PORTBbits.RB7 == 0b1)){          
                      // }else if (data_in == 0b101){                //0b101 //0
                            Hudson_23bits.state = st1;
                            step1();
                        } // end st1;
                        
                        break;
                        
                case st2:
                    
                     U1TXREG = '2';

                    
                  if ((PORTBbits.RB5 == 0b1) && (PORTBbits.RB6 == 0b1) && (PORTBbits.RB7 == 0b0)){
                        //  if (data_in == 0b110){                      //0b110 //2
                            Hudson_23bits.state = st3;
                            step3();
                 }else if ((PORTBbits.RB5 == 0b1) && (PORTBbits.RB6 == 0b0) && (PORTBbits.RB7 == 0b0)){
                       // }else if (data_in == 0b100){                 //0b100 //1
                            Hudson_23bits.state = st2;
                            step2();
                        } // end st2;
                        
                        break;
                        
                case st3:
                    
                     U1TXREG = '3';
             
                  if ((PORTBbits.RB5 == 0b0) && (PORTBbits.RB6 == 0b1) && (PORTBbits.RB7 == 0b0)){
                    //     if (data_in == 0b010){                     //0b010 //3
                            Hudson_23bits.state = st4;
                            step4();
                 }else  if ((PORTBbits.RB5 == 0b1) && (PORTBbits.RB6 == 0b1) && (PORTBbits.RB7 == 0b0)){
                     //   }else if (data_in == 0b110){                 //0b110 //2
                            Hudson_23bits.state = st3;
                            step3();
                        }// end st3;
                        break;
                        
                case st4:     
                    
                     U1TXREG = '4';
   
                    
                 if ((PORTBbits.RB5 == 0b0) && (PORTBbits.RB6 == 0b1) && (PORTBbits.RB7 == 0b1)){
                     //    if (data_in == 0b011){                    //0b011 //4
                            Hudson_23bits.state = st5;
                            step5();
                 }else  if ((PORTBbits.RB5 == 0b0) && (PORTBbits.RB6 == 0b1) && (PORTBbits.RB7 == 0b0)){                        
                    //    }else if (data_in == 0b010){              //0b010 //5
                            Hudson_23bits.state = st4;
                            step4();
                        } // end st4;
                        
                        break;
                        
                case st5:   
                    
                     U1TXREG = '5';

                   
                  if ((PORTBbits.RB5 == 0b0) && (PORTBbits.RB6 == 0b0) && (PORTBbits.RB7 == 0b1)){
                    
                       //  if (data_in == 0b001){                    //0b001 //5
                            Hudson_23bits.state = st6;
                            step6();
                  }else  if ((PORTBbits.RB5 == 0b0) && (PORTBbits.RB6 == 0b1) && (PORTBbits.RB7 == 0b1)){                                            
                     //   }else if (data_in == 0b011){                //0b011  //6
                            Hudson_23bits.state = st5;
                            step5();
                        } // end st5;
                            
                        break;
                        
                case st6:
                    
                     U1TXREG = '6';
                    
                 if ((PORTBbits.RB5 == 0b1) && (PORTBbits.RB6 == 0b0) && (PORTBbits.RB7 == 0b1)){

                       //  if (data_in == 0b101){                     //0b101  //0
                            Hudson_23bits.state = st1;
                            step1();                            
                 }else  if ((PORTBbits.RB5 == 0b0) && (PORTBbits.RB6 == 0b0) && (PORTBbits.RB7 == 0b1)){                                                                        
                      //  }else if (data_in == 0b001){              //0b001  //5
                            Hudson_23bits.state = st6;
                            step6();
                        } // end st6
                        
                        break;
                        
                default: Hudson_23bits.state = st1;        
             
    }  //end switch case // Cw rotation
    
    } //  End if (CW rotation)
    
    
     //  *** Check direction flag & break flag. *** //
    //CW rotation
    // m-R command
    if ((Hudson_23bits.CW_Rotation == 0b1) && (Hudson_23bits.Stop == 0b0)){
        
         switch (Hudson_23bits.state){
        
                case st1:         
                     U1TXREG = '1'; 

                     if((PORTBbits.RB5 == 0b0) && (PORTBbits.RB6 == 0b0) && (PORTBbits.RB7 == 0b1)){
                                                               // HALL Combination
                     //   if (data_in == 1){                    //0b100
                            Hudson_23bits.state = st6;    
                            step6R();
                     }else if ((PORTBbits.RB5 == 0b1) && (PORTBbits.RB6 == 0b0) && (PORTBbits.RB7 == 0b1)){
                     //}else if (data_in == 0){                //0b101
                            Hudson_23bits.state = st1;
                            step1R();
                        } // end st1;
                        
                        break;
                        
                case st2:
                    U1TXREG = '2';
                    
                     if((PORTBbits.RB5 == 0b1) && (PORTBbits.RB6 == 0b0) && (PORTBbits.RB7 == 0b1)){
                      //  if (data_in == 2){                      //0b110
                            Hudson_23bits.state = st1;
                            step1R();
                      
                     }else if ((PORTBbits.RB5 == 0b1) && (PORTBbits.RB6 == 0b0) && (PORTBbits.RB7 == 0b0)){
                       // }else if (data_in == 1){                 //0b100
                            Hudson_23bits.state = st2;
                            step2R();
                        } // end st2;
                        
                        break;
                        
                case st3:
                    U1TXREG = '3';
                    
                    if((PORTBbits.RB5 == 0b1) && (PORTBbits.RB6 == 0b0) && (PORTBbits.RB7 == 0b0)){
                       // if (data_in == 3){                     //0b010
                            Hudson_23bits.state = st2;
                            step2R();
                            
                    }else if ((PORTBbits.RB5 == 0b1) && (PORTBbits.RB6 == 0b1) && (PORTBbits.RB7 == 0b0)){        
                       // }else if (data_in == 2){                 //0b110
                            Hudson_23bits.state = st3;
                            step3R();
                        }// end st3;
                        break;
                        
                case st4:     
                    U1TXREG = '4';
                       
                     if((PORTBbits.RB5 == 0b1) && (PORTBbits.RB6 == 0b1) && (PORTBbits.RB7 == 0b0)){
                      //  if (data_in == 4){                    //0b011
                    
                            Hudson_23bits.state = st3;
                            step3R();
                            
                     }else if ((PORTBbits.RB5 == 0b0) && (PORTBbits.RB6 == 0b1) && (PORTBbits.RB7 == 0b0)){        
                            //  }else if (data_in == 3){              //0b010
                            Hudson_23bits.state = st4;
                            step4R();
                        } // end st4;
                        
                        break;
                        
                case st5:      
                    U1TXREG = '5';  
                    
                      if((PORTBbits.RB5 == 0b0) && (PORTBbits.RB6 == 0b1) && (PORTBbits.RB7 == 0b0)){
                      //  if (data_in == 5){                    //0b001
                            Hudson_23bits.state = st4;
                            step4R();
                        }else if ((PORTBbits.RB5 == 0b0) && (PORTBbits.RB6 == 0b1) && (PORTBbits.RB7 == 0b1)){        
                            //   }else if (data_in == 4){                //0b011
                            Hudson_23bits.state = st5;
                            step5R();
                        } // end st5;
                            
                        break;
                        
                case st6:
                     U1TXREG = '6';
                    
                       if((PORTBbits.RB5 == 0b0) && (PORTBbits.RB6 == 0b1) && (PORTBbits.RB7 == 0b1)){
                       // if (data_in == 0){                     //0b101
                            Hudson_23bits.state = st5;
                            step5R();
              
                       }else if ((PORTBbits.RB5 == 0b0) && (PORTBbits.RB6 == 0b1) && (PORTBbits.RB7 == 0b1)){                
                       // }else if (data_in == 5){              //0b001
                            Hudson_23bits.state = st6;
                            step6R();
                        } // end st6
                        
                        break;
                        
                default: Hudson_23bits.state = st1;        
        
                
          }  //end switch case // Cw rotation
    
    } //  End if (CW rotation)
       
  
    
}//End Hudson_Commutation_StartUp


///*** Determines the initial state, reading the Hall sensor value ***///
void InitState (void){
    
    if ((PORTBbits.RB5 == 0b1) && (PORTBbits.RB6 == 0b0) && (PORTBbits.RB7 == 0b1)){
        Hudson_23bits.state = st1;
    }else if ((PORTBbits.RB5 == 0b1 ) && (PORTBbits.RB6 == 0b0) && (PORTBbits.RB7 == 0b0)){
        Hudson_23bits.state = st2;
    }else if ((PORTBbits.RB5 == 0b1) && (PORTBbits.RB6 == 0b1) && (PORTBbits.RB7 == 0b0)){
        Hudson_23bits.state = st3;
    }else if ((PORTBbits.RB5 == 0b0) && (PORTBbits.RB6 == 0b1) && (PORTBbits.RB7 == 0b0)){
        Hudson_23bits.state = st4;
    }else if ((PORTBbits.RB5 == 0b0) && (PORTBbits.RB6 == 0b1) && (PORTBbits.RB7 == 0b1)){
        Hudson_23bits.state = st5;
    }else if ((PORTBbits.RB5 == 0b0) && (PORTBbits.RB6 == 0b0) && (PORTBbits.RB7 == 0b0)){
        Hudson_23bits.state = st6;
    }
        
}// End InitState

void BLDC_commands (unsigned char com){     
    
    switch(Hudson_23bits.comstate){
        
        case BLDC_HEADER:
            if(com == 'm'){
                Hudson_23bits.comstate = CONFIRM_HEADER;
            }else{
                Hudson_23bits.comstate = BLDC_HEADER;
            }
                
        case CONFIRM_HEADER:
            if(com == '-'){
                Hudson_23bits.comstate = BLDC_COM;
            }else{
                Hudson_23bits.comstate = BLDC_HEADER;
            }
               
        case BLDC_COM:
            
            if(com == 'R'){
                
                //  set CW rotaton flag;
                Hudson_23bits.CW_Rotation = 0b1;
                //  clear CCWrotation flag;
                Hudson_23bits.CCW_Rotation = 0b0;
                //  clear brake flag;
                Hudson_23bits.Break = 0b0;        
                //Clear stop flag;
                Hudson_23bits.Stop  = 0b0;
                
                P1DC1 = P1DC2 = P1DC3 = 0x0800;
                
                // Set in initial state read from the Hall sensor
                InitState();
                
                UART_Put_String ("R-ok");

                
            }else if (com == 'L'){
                
                // set CCW rotation flag;
                Hudson_23bits.CCW_Rotation = 0b1;
                // clear CCW rotation flag;
                Hudson_23bits.CW_Rotation = 0b0;
                // clear brake flag;
                Hudson_23bits.Break = 0b0;          
                //Clear stop flag
                Hudson_23bits.Stop  = 0b0;

                
                P1DC1 = P1DC2 = P1DC3 = 0x0800;
                
                // Set in initial state read from the Hall sensor
                InitState();
                        
                 UART_Put_String ("L-ok");
             
            }else if (com == 'B'){
                
                // clear CCW flag
                Hudson_23bits.CCW_Rotation = 0b0;
                // clear CW flag
                Hudson_23bits.CW_Rotation = 0b0;
                // Set break flag;
                Hudson_23bits.Break = 0b1;
                // Clear stop flag
                Hudson_23bits.Stop = 0b0;
                
                
               UART_Put_String ("B-ok");

            }else if (com == 'S'){
                
                Hudson_23bits.CCW_Rotation = 0b0;
                
                Hudson_23bits.CW_Rotation = 0b0;
                
                Hudson_23bits.Break = 0b0;
                
                Hudson_23bits.Stop = 0b1;
                
                P1DC1 = P1DC2 = P1DC3 = 0;
                
                UART_Put_String ("The motor is stopped.");
                
            }
            
        default: Hudson_23bits.comstate = BLDC_HEADER;
            
    }// End switch
    
}// End BLDC_commands


//20 March


///*** x = A - my; *** ///
void BreakBLDC (void){
    
  // Check if the break flag is set //
 if(Hudson_23bits.Break == 0b1){
    
P1DC1 = P1DC2 = P1DC3 = P1DC1 + 40;

if (P1DC1 == 0){
    Hudson_23bits.Break = 0b0; //  Clear the break flag;
    UART_Put_String ("The motor is stopped.");
}    

          
 }

}// End Break BLDC


/////*** SIX-STEP commutation sequence ***////


/// *** m - L (Forward rotation) *** ///
// CCW rotation; //
// Asociated with 101 
void step1 (void){
    
    //*** OVDCON register configuration ***//
    
    //***POVD bits ***//
    
    //** Determines if the output is driven by PWM (1) or set by POUT bit (0).
    
    
    P1OVDCONbits.POVD1H = 1;
    P1OVDCONbits.POVD1L = 0;

    P1OVDCONbits.POVD2H = 0;
    P1OVDCONbits.POVD2L = 1;

    P1OVDCONbits.POVD3H = 0;
    P1OVDCONbits.POVD3L = 0;
    
    
    //*** Determines if the output is HIGH or LOW.***//
    
    P1OVDCONbits.POUT1H = 0;
    P1OVDCONbits.POUT1L = 0;

    P1OVDCONbits.POUT2H = 0;
    P1OVDCONbits.POUT2L = 0;

    
    P1OVDCONbits.POUT3H = 0;
    P1OVDCONbits.POUT3L = 0;
    
}


// Asociated with 100
void step2 (void){
    
    
    //*** OVDCON register configuration ***//
    
    //***POVD bits ***//
    
    //** Determines if the output is driven by PWM (1) or set by POUT bit (0).
   
    
    P1OVDCONbits.POVD1H = 1;
    P1OVDCONbits.POVD1L = 0;

    P1OVDCONbits.POVD2H = 0;
    P1OVDCONbits.POVD2L = 0;

    P1OVDCONbits.POVD3H = 0;
    P1OVDCONbits.POVD3L = 1;
    
    
    //*** Determines if the output is HIGH or LOW.***//
    
    P1OVDCONbits.POUT1H = 0;
    P1OVDCONbits.POUT1L = 0;

    P1OVDCONbits.POUT2H = 0;
    P1OVDCONbits.POUT2L = 0;

    
    P1OVDCONbits.POUT3H = 0;
    P1OVDCONbits.POUT3L = 0;
}

//Asociated with 110
void step3 (void){
    
    //*** OVDCON register configuration ***//
    
    //***POVD bits ***//
    
    //** Determines if the output is driven by PWM (1) or set by POUT bit (0).
    
    
    P1OVDCONbits.POVD1H = 0;
    P1OVDCONbits.POVD1L = 0;

    P1OVDCONbits.POVD2H = 1;
    P1OVDCONbits.POVD2L = 0;

    P1OVDCONbits.POVD3H = 0;
    P1OVDCONbits.POVD3L = 1;
    
    
    //*** Determines if the output is HIGH or LOW.***//
    
    P1OVDCONbits.POUT1H = 0;
    P1OVDCONbits.POUT1L = 0;

    P1OVDCONbits.POUT2H = 0;
    P1OVDCONbits.POUT2L = 0;

    
    P1OVDCONbits.POUT3H = 0;
    P1OVDCONbits.POUT3L = 0;
}

//Asociated with 010
void step4 (void){
    
    //*** OVDCON register configuration ***//
    
    //***POVD bits ***//
    
    //** Determines if the output is driven by PWM (1) or set by POUT bit (0).
    
    
    P1OVDCONbits.POVD1H = 0;
    P1OVDCONbits.POVD1L = 1;

    P1OVDCONbits.POVD2H = 1;
    P1OVDCONbits.POVD2L = 0;

    P1OVDCONbits.POVD3H = 0;
    P1OVDCONbits.POVD3L = 0;
    
    
    //*** Determines if the output is HIGH or LOW.***//
    
    P1OVDCONbits.POUT1H = 0;
    P1OVDCONbits.POUT1L = 0;

    P1OVDCONbits.POUT2H = 0;
    P1OVDCONbits.POUT2L = 0;

    
    P1OVDCONbits.POUT3H = 0;
    P1OVDCONbits.POUT3L = 0;
}

//Asociated with 011
void step5 (void){
    
    
    //*** OVDCON register configuration ***//
    
    //***POVD bits ***//
    
    //** Determines if the output is driven by PWM (1) or set by POUT bit (0).
    
    
    P1OVDCONbits.POVD1H = 0;
    P1OVDCONbits.POVD1L = 1;

    P1OVDCONbits.POVD2H = 0;
    P1OVDCONbits.POVD2L = 0;

    P1OVDCONbits.POVD3H = 1;
    P1OVDCONbits.POVD3L = 0;
    
    
    //*** Determines if the output is HIGH or LOW.***//
    
    P1OVDCONbits.POUT1H = 0;
    P1OVDCONbits.POUT1L = 0;

    P1OVDCONbits.POUT2H = 0;
    P1OVDCONbits.POUT2L = 0;

    
    P1OVDCONbits.POUT3H = 0;
    P1OVDCONbits.POUT3L = 0;
}

//Asociated with 001
void step6 (void){
    
    //*** OVDCON register configuration ***//
    
    //***POVD bits ***//
    
    //** Determines if the output is driven by PWM (1) or set by POUT bit (0).
    
    
    P1OVDCONbits.POVD1H = 0;
    P1OVDCONbits.POVD1L = 0;

    P1OVDCONbits.POVD2H = 0;
    P1OVDCONbits.POVD2L = 1;

    P1OVDCONbits.POVD3H = 1;
    P1OVDCONbits.POVD3L = 0;
    
    
    //*** Determines if the output is HIGH or LOW.***//
    
    P1OVDCONbits.POUT1H = 0;
    P1OVDCONbits.POUT1L = 0;

    P1OVDCONbits.POUT2H = 0;
    P1OVDCONbits.POUT2L = 0;

    
    P1OVDCONbits.POUT3H = 0;
    P1OVDCONbits.POUT3L = 0;
}


/// *** m- R (Reverse) *** ///
//CW rotation//
//Asociated with 101
void step1R (void){
    
    //*** OVDCON register configuration ***//
    
    //***POVD bits ***//
    
    //** Determines if the output is driven by PWM (1) or set by POUT bit (0).
    
    
    P1OVDCONbits.POVD1H = 0;
    P1OVDCONbits.POVD1L = 1;

    P1OVDCONbits.POVD2H = 1;
    P1OVDCONbits.POVD2L = 0;

    P1OVDCONbits.POVD3H = 0;
    P1OVDCONbits.POVD3L = 0;
    
    
    //*** Determines if the output is HIGH or LOW.***//
    
    P1OVDCONbits.POUT1H = 0;
    P1OVDCONbits.POUT1L = 0;

    P1OVDCONbits.POUT2H = 0;
    P1OVDCONbits.POUT2L = 0;

    
    P1OVDCONbits.POUT3H = 0;
    P1OVDCONbits.POUT3L = 0;
    
}

//Asociated with 100
void step2R (void){
    
     //*** OVDCON register configuration ***//
    
    //***POVD bits ***//
    
    //** Determines if the output is driven by PWM (1) or set by POUT bit (0).
    
    
    P1OVDCONbits.POVD1H = 0;
    P1OVDCONbits.POVD1L = 1;

    P1OVDCONbits.POVD2H = 0;
    P1OVDCONbits.POVD2L = 0;

    P1OVDCONbits.POVD3H = 1;
    P1OVDCONbits.POVD3L = 0;
    
    
    //*** Determines if the output is HIGH or LOW.***//
    
    P1OVDCONbits.POUT1H = 0;
    P1OVDCONbits.POUT1L = 0;

    P1OVDCONbits.POUT2H = 0;
    P1OVDCONbits.POUT2L = 0;

    
    P1OVDCONbits.POUT3H = 0;
    P1OVDCONbits.POUT3L = 0;   
    
}

//Asociated with 110
void step3R (void){
    //*** OVDCON register configuration ***//
    
    //***POVD bits ***//
    
    //** Determines if the output is driven by PWM (1) or set by POUT bit (0).
    
    
    P1OVDCONbits.POVD1H = 0;
    P1OVDCONbits.POVD1L = 0;

    P1OVDCONbits.POVD2H = 0;
    P1OVDCONbits.POVD2L = 1;

    P1OVDCONbits.POVD3H = 1;
    P1OVDCONbits.POVD3L = 0;
    
    
    //*** Determines if the output is HIGH or LOW.***//
    
    P1OVDCONbits.POUT1H = 0;
    P1OVDCONbits.POUT1L = 0;

    P1OVDCONbits.POUT2H = 0;
    P1OVDCONbits.POUT2L = 0;

    
    P1OVDCONbits.POUT3H = 0;
    P1OVDCONbits.POUT3L = 0;
    
}

//Asociated with 010
void step4R (void){
     //*** OVDCON register configuration ***//
    
    //***POVD bits ***//
    
    //** Determines if the output is driven by PWM (1) or set by POUT bit (0).
    
    
    P1OVDCONbits.POVD1H = 1;
    P1OVDCONbits.POVD1L = 0;

    P1OVDCONbits.POVD2H = 0;
    P1OVDCONbits.POVD2L = 1;

    P1OVDCONbits.POVD3H = 0;
    P1OVDCONbits.POVD3L = 0;
    
    
    //*** Determines if the output is HIGH or LOW.***//
    
    P1OVDCONbits.POUT1H = 0;
    P1OVDCONbits.POUT1L = 0;

    P1OVDCONbits.POUT2H = 0;
    P1OVDCONbits.POUT2L = 0;

    
    P1OVDCONbits.POUT3H = 0;
    P1OVDCONbits.POUT3L = 0;
    
}

//Asociated with 011

void step5R (void){
    //*** OVDCON register configuration ***//
    
    //***POVD bits ***//
    
    //** Determines if the output is driven by PWM (1) or set by POUT bit (0).
    
    
    P1OVDCONbits.POVD1H = 1;
    P1OVDCONbits.POVD1L = 0;

    P1OVDCONbits.POVD2H = 0;
    P1OVDCONbits.POVD2L = 0;

    P1OVDCONbits.POVD3H = 0;
    P1OVDCONbits.POVD3L = 1;
    
    
    //*** Determines if the output is HIGH or LOW.***//
    
    P1OVDCONbits.POUT1H = 0;
    P1OVDCONbits.POUT1L = 0;

    P1OVDCONbits.POUT2H = 0;
    P1OVDCONbits.POUT2L = 0;

    
    P1OVDCONbits.POUT3H = 0;
    P1OVDCONbits.POUT3L = 0;   
    
}

//Asociated with 001

void step6R (void){
     //*** OVDCON register configuration ***//
    
    //***POVD bits ***//
    
    //** Determines if the output is driven by PWM (1) or set by POUT bit (0).
    
    
    P1OVDCONbits.POVD1H = 0;
    P1OVDCONbits.POVD1L = 0;

    P1OVDCONbits.POVD2H = 1;
    P1OVDCONbits.POVD2L = 0;

    P1OVDCONbits.POVD3H = 0;
    P1OVDCONbits.POVD3L = 1;
    
    
    //*** Determines if the output is HIGH or LOW.***//
    
    P1OVDCONbits.POUT1H = 0;
    P1OVDCONbits.POUT1L = 0;

    P1OVDCONbits.POUT2H = 0;
    P1OVDCONbits.POUT2L = 0;

    
    P1OVDCONbits.POUT3H = 0;
    P1OVDCONbits.POUT3L = 0;
    
}


