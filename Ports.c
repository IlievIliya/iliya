#include "processor.h"
#include "Ports.h"
#include "global_data.h"

void LockIO(void)
{

asm volatile ("mov #OSCCON,w1 \n"
				"mov #0x46, w2 \n"
				"mov #0x57, w3 \n"
				"mov.b w2,[w1] \n"
				"mov.b w3,[w1] \n"
				"bset OSCCON, #6");
}

void UnlockIO(void)
{

asm volatile ("mov #OSCCON,w1 \n"
				"mov #0x46, w2 \n"
				"mov #0x57, w3 \n"
				"mov.b w2,[w1] \n"
				"mov.b w3,[w1] \n"
				"bclr OSCCON, #6");
}

void InitPorts(void) 
{
	UnlockIO();	
    
    ///*** UART ***///
    //UART RX & TX (RP) pins 
     RPINR18bits.U1RXR   = 0x13;         // RxD connected to the RP19 (pin36)
     RPOR10bits.RP20R    = 3;            // TxD connected to the RP20 (pin37)
    
     ///*** QEI ***//
     //QEI QEA, QEB, Index (RP) pins A - R7 (43), B - RP21 (38), INDEX - RP24 (4)
 //    AD1PCFGL = 0xFFFF; // Set as digital pins that share AN function;
 
     RPINR14bits.QEA1R = 0x07;
     RPINR14bits.QEB1R = 0x15;
     
     RPINR15bits.INDX1R = 0x18;
         
     
            
        
     //*** CN board I ***//
     // rb5 , cn27 (41)
     // rb6, cn24 (42)
     // rb7 cn23 (43)
  
     /*
   /// *** Inputs for CN (Hall values) *** ///
     TRISBbits.TRISB5 = 0b1;     
     TRISBbits.TRISB6 = 0b1;
     TRISBbits.TRISB7 = 0b1;
     */
     
  /// *** Control Outputs for the combinational logic *** ///
     TRISBbits.TRISB5 = 0b0;     // Enable
     TRISBbits.TRISB6 = 0b0;     // Direction
     
  // Initialization of the Enable and Direction;
     LATBbits.LATB5 = 0b1;
     LATBbits.LATB6 = 0b1;
     
     
    //*** CPU BANDWITDH MEASSUREMENT FLAG***//
//    TRISCbits.TRISC3 = 0b0;
//    LATCbits.LATC3 = 0b0;
//    TRISCbits.TRISC4 = 0b0;
//    LATCbits.LATC4 =0b0;
    
    //** SINE GENERATOR DEBUG **//
    //Indication pin//
   // TRISBbits.TRISB12 = 0b0; // Set as digital output;
   // LATBbits.LATB12 = 0b1; //Initialize to zero;
    
    
    //*** PINS FOR TESTS ***// 1 March
    //(3) CN17/RC7 input - 1
    //(4) CN20 /RC8 input - 1
   // (5) CN19/RC9 input - 1
       
/// *** Test CL sequence *** ///
//     
//  TRISBbits.TRISB5 = 0b0;     
//  TRISBbits.TRISB6 = 0b0;
//  TRISBbits.TRISB7 = 0b0;
//  
    
	LockIO();
}



