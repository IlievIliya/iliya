#include "Quadrature_Encoder.h"
#include "processor.h"
#include "PrintOutput.h"

#include <stdio.h>

int  Pos[2] = {0,0};
int  POSCNTmapped;
char print = 0b0;

unsigned char cntrr = 0;

int  Speed;
int  SpeedRPM;
 
//QEI - will be used for FB for speed;

void QEIinit (void){
    
///*** Register that controls operation and provides flags ***///
    
   QEI1CONbits.QEIM      = 0b000; // Disable the QEI module

    //READ only //
    QEI1CONbits.INDX;
    QEI1CONbits.UPDN; //1 - (+) 0 - (-); // tested (working)
 
    //Write//
    QEI1CONbits.SWPAB   = 0b0;    // A and B are not swapped;
    QEI1CONbits.PCDOUT  = 0b0;   // Normal I/O operation.
    QEI1CONbits.CNTERR  = 0b0;   // Clear counter errors. 
    QEI1CONbits.QEISIDL = 0b0;  // Continue operation in Sleep mode.
    QEI1CONbits.POSRES  = 0b1;   //Index pulse resets the position counter.
    QEI1CONbits.TQCKPS  = 0b00;  // 1:1 prescaler value
    QEI1CONbits.TQCS    = 0b0;     // Internal clovk (Tcy)
    
    MAX1CNT = 0xFFFF;
    POS1CNT = 0x0000;
    
    
    //CNTERR: Count Error Status Flag bit
    //1 = Position count error has occurred
    //0 = Position count error has not occurred
    
    DFLTCONbits.CEID    = 0b1; // Count error interrupts disabled.
   
    QEI1CONbits.QEIM    = 6; // x4 with position counter reset by index;
    
    
///*** REgister that controls the digital input filter ***///
//    DFLT1CON;
      
} //End QEI init


void PositionCheckHudson (void){
    
    POSCNTmapped = (int)POS1CNT;
    
    if (POSCNTmapped < 0){
        POSCNTmapped = -POSCNTmapped;
    }
    
    Pos[1] = Pos [0];
    Pos[0] = (unsigned int)(((unsigned long)POSCNTmapped * 4.096)/ 91); // 4.096 // 8000 counts.
    
   
    if (print == 0b1){
    printf ("%d", Pos[0]);
    NewLine();
    printf ("%d", (unsigned int)POS1CNT);
    NewLine();
   
    }// end if (flag check)
    
  
} // end PositionCheck
 
 



void VelocityCheckHudson (void){
    
  //  PositionCheckHudson();

    Speed = Pos[0] - Pos[1];   // Speed meassured in degrees per 0.005s
   

if (Speed >= 0){

if(Speed >= (HalfMaxspeed){       
   Speed = (Speed - (MaxSpeed);
} // end if

} else {
   
    if (Speed < -(HalfMaxspeed){
    Speed = (Speed + (MaxSpeed);
    } // End if

}
    
//Speed *= 2;
    
    if ((Speed <= 361) && (Speed >= -361)){ // Software limitation
    SpeedRPM = ((Speed/0.005)* 0.083); // (Speed (deg)/ 0.005s)*(60s / 360 (deg)) / 2
    }
    
    cntrr = (cntrr + 1)%100; // cycles through 100 and loops back
    
if ((print == 0b1)&& (cntrr == 99)) // print one of each 100 values 
    {
   // printf ("%d", (unsigned int)SpeedRPM);
    NewLine();
    }



} // End VelocityCheckHudson
   
