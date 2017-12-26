#include "Timer.h"
#include "processor.h"
#include "global_data.h"
#include "UART.h"
#include "PWM.h"
#include "PrintOutput.h"
#include "CMDMENU.h"
#include "PrintOutput.h"
#include "Communication_.h"
#include "Quadrature_Encoder.h"
#include "Commutation_Logic.h"
#include "UART_Buffer.h"

#include <libpic30.h>

unsigned char cycles = 0;

void InitTMR1(unsigned char PRESCALER){
    
    T1CONbits.TON   = 0;        // Disable Timer1
    
    T1CONbits.TCS   = 0;        // Select internal instruction cycle clock
    T1CONbits.TGATE = 0;        // Disable Gated Timer mode
    T1CONbits.TCKPS = PRESCALER;// Select 1:8 Prescaler, resulting operating frequency is 5 MHz
    TMR1            = 0x00;     // Clear timer register
    PR1             = 0x00FF;   // Load the period value which determines the sample time for the SineGenerator.
                                // 2000(d) 0x07D0;
                                
    T1CONbits.TON   = 1;          // Enable Timer1
}// End InitTMR1

void InitTMR2(unsigned char PRESCALER){
    
    T2CONbits.TON   = 0;        //Disable Timer2
    
    T2CONbits.TCS   = 0;        // Select internal instruction cycle clock
    T2CONbits.TGATE = 0;        // Disable Gated Timer mode
    T2CONbits.TCKPS = PRESCALER;// Select 1:256 Prescaler
    TMR2            = 0x00;     // Clear timer register
    PR2             = 0x0FFF;   // Load the period value
    

    T2CONbits.TON   = 1;        //Enable Timer2
}// End InitTMR2

void InitTMR3(unsigned char PRESCALER){
    
    T3CONbits.TON = 0;             // Disable Timer3

    T3CONbits.TCS = 0;            // Select internal instruction cycle clock 
    T3CONbits.TGATE = 0;          // Disable Gated Timer mode
    T3CONbits.TCKPS = PRESCALER;  // Select 1:1 Prescaler
    TMR3 = 0x00;                  // Clear timer register
    PR3 = 0x0FFF;                // Load the period value

    T3CONbits.TON = 1;           // Start Timer3
}// End InitTMR3


// Used for basic programs serving the motor drive - variable interrupt period //
void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void){

AutoPWM();   
SoftStartFUNC();
BreakBLDC();
Modulation__();

IFS0bits.T1IF = 0; // Clear the ISR flag.

}

// Used for programs requiring time regulation - variable interrupt period //
void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void){

 cycles = (cycles + 1)%160;
ReverseDirection_Auto (cycles);


        
IFS0bits.T2IF = 0; // Clear the ISR flag.

}

///Used only for sample time and speed feedback - static interrupt period//
void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void){
 
    PositionCheckHudson();
    
IFS0bits.T3IF = 0; 
 
}