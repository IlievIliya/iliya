#include "processor.h"
#include "Commutation_Logic.h"
#include "PrintOutput.h"
#include "global_data.h"

#include <libpic30.h>
#include <stdlib.h>

///*** Initialization of the control flags ***///
    
unsigned char    Direction    = 0b0;  // 0 - Forward CW, 1 - Reverse CCW
unsigned char    Enable       = 0b1;  // Active Low
unsigned char    Break        = 0b0;  
unsigned char    Start        = 0b0;
unsigned char    I_D_flag     = 0b0; // Initialization of the I_D_flag in 0;
unsigned char    Reverse      = 0b0; 
unsigned char    SoftStart    = 0b0;
unsigned char    AutoDC       = 0b0;
unsigned char    Modulation   = 0b0;
unsigned int     Counter      = 0b0;
unsigned char    index_mod     = 0;

unsigned char    stepstate;
    
// Reduces the DC gradually 
void BreakBLDC (void){
    
 // Check if the break flag is set //
 if(Break == 0b1){
    
    // Clear the SoftStart bit.
    SoftStart = 0b0;
    AutoDC    = 0b0;
    Reverse   = 0b0;
    
    if ((P2DC1 >= 0x0000) && (P2DC1 <= 0x000F)){ 
        
        P2DC1 = 0x0000;
        
    }else if (P2DC1 >= 0x000F){
    P2DC1 = P2DC1 - 0x000F;

    if (P2DC1 <= 0x000F){
    
    P2DC1 = 0x0000; // Zero duty Cycle;
    
        
    UART_Put_String ("The motor is stopped.");
    NewLine();
    LATBbits.LATB5 = 0b1; //Disable CL
    
                  }  // end if
    }// End else
    
    } // End if (Break flag check)
 
}//  End Break 

///*** Auto increase/decrease the DC to max/ min values ***///
void AutoPWM (void){
 
  if(AutoDC == 0b1){
    
  if(I_D_flag == increasing) // flag used to roll over the direction of progress of the PWM (0 - increasing, 1 -decreasing)
           {
			P2DC1++;  // increasing the P2DC1 register +1
            __delay32(12); // delay function, delay = amount of instruction cycles (12)
            
           if(P2DC1 == 0x0F10) // MAX PWM duty cycle to be reached.
           I_D_flag = decreasing; // set the flag to 1 - start to decrease the duty cycle
          }
     
          if(I_D_flag == decreasing) // flag set to 1 - start decreasing;
          {
          P2DC1--; // decrease the P2DC1 register -1
            __delay32(12); 
          
         if(P2DC1 == 0x000E) // Minimum value of the 0x00EE P2DC1 register 
           I_D_flag = increasing;
           }
              
        __delay32(30);
     
    }// End if
        
	} //  End AutoPWM

// Gradually increase the DC to a value higher than 0x7E20 and drop to 0x5E20;
void SoftStartFUNC(void){

    if((SoftStart == 0b1) && (Break == 0b0)){
    PWM2CON1bits.PEN1H = 1;    // Enable the PWM H1 
    PWM2CON1bits.PEN1L = 1;     // Enable the PWM L1
    
    //Increase gradually the DC
    // - > Gradually loads the bootsstrap
    P2DC1 = P2DC1 + 0x000A;
   
    if (P2DC1 >= 0x10E0){
        P2DC1 = 0x10E0;
        SoftStart = 0b0;
    } // End if 
    
    } // End if
    
} // End SoftStart

///*** Sets CCW direction ***///
void CCW (void){
            
                P2DC1          = 0x000E;
                // Break flag cleared
                Break          = 0b0; 
                // Set SoftStart flag
                SoftStart      = 0b1;
                //  Direction bit - 0 - CCW rotation
                LATBbits.LATB6 = 0b0;  
                // Enable bit cleared (active low)
                LATBbits.LATB5 = 0b0;                   
                // Auto flag cleared
                AutoDC         = 0b0;
                 // Disable Reverse function
                Reverse        = 0b0;
                             
    
} // End CCW


///*** Sets CW direction ***///
void CW (void){
                      
               P2DC1            = 0x000E;
               // Break flag cleared
               Break            = 0b0;
               // Set SoftStart flag
               SoftStart        = 0b1;             
                // Direction bit - 1 - CW rotation
               LATBbits.LATB6   = 0b1;  
                // Enable bit cleared (active low)
               LATBbits.LATB5   = 0b0;                 
               // Auto flag cleared
               AutoDC           = 0b0;
                // Disable Reverse function
               Reverse          = 0b0;
    
               
} // End CW 



/// Initialization of the REVERSE_D FSM ///
 void InitReverseDirection (void){
     
     stepstate = step1;
     
 }


/// Gradually increases and decreases - > reverses the direction (loop)///
void ReverseDirection_Auto (unsigned char step){    
        
    if (Reverse == 0b1){
         
        switch (stepstate){
            
            case step1:
                if (step == 100){
                 stepstate = step2;   
             
                LATBbits.LATB5 = 0b0;
                P2DC1 = P2DC1 + 0x0001;
                if (P2DC1 >= 0x1000){
                P2DC1 = 0x0100;
                }
                }else {
    
                stepstate = step1;
                }break;
                
            case step2:
                if (step == 1){
                stepstate = step1;   
                }else {
                P2DC1 = P2DC1 - 0x000A;
                if (P2DC1 <= 0x000A){
                P2DC1 = 0x0000;
                LATBbits.LATB6 = ~LATBbits.LATB6;
                }
                  stepstate = step2;
                }break;
             
                
             default: stepstate = step1;
                
        }
    
    } // End if (Reverse flag check)
    
} // End ReverDirection
 

unsigned int Sine_table [Samples] = { 
  
800,884,966,1047,1125,1200,1270,1335,
1395,1447,1493,1531,1561,1583,1596,1600,
1596,1583,1561,1531,1493,1447,1395,1335,
1270,1200,1125,1047,966,884,800,716,
634,553,475,400,330,265,205,153,
107,69,39,17,4,0,4,17,
39,69,107,153,205,265,330,400,
475,553,634,716,

};

void Modulation__ (void){
    
    if (Modulation == 0b1){
        
        P2DC1 = Sine_table [index_mod];
        index_mod = (index_mod + 1)%Samples;
        
    }
    
}