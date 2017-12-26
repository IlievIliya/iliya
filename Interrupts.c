#include "processor.h"
#include "global_data.h"
#include "Interrupts.h"

void enableInterrupts(void)
{
	/* Set CPU IPL to 0, enable level 1-7 interrupts */
	/* No restoring of previous CPU IPL state performed here */
	SRbits.IPL = 0;
}
void disableInterrupts(void)
{
	/* Set CPU IPL to 7, disable level 1-7 interrupts */
	/* No saving of current CPU IPL setting performed here */	
	SRbits.IPL = 7;
}

void initInterrupts(void)
{
	/* Interrupt nesting enabled here */
	INTCON1bits.NSTDIS = 0;
    
    /* UART TX */
    IPC3bits.U1TXIP     = 2;    //Set interrupt priority level = 5  
    IFS0bits.U1TXIF     = 0;	// Clear the Transmit Interrupt Flag
	IEC0bits.U1TXIE     = 1;	// Enable Transmit Interrupts
    
    /* UART RX */
    IPC2bits.U1RXIP     = 2;	// Set interrupt priority level = 5        
	IFS0bits.U1RXIF     = 0;	// Clear the Recieve Interrupt Flag
	IEC0bits.U1RXIE     = 1;	// Enable Recieve Interrupts only
    
    /* Timer 1*/    
    IPC0bits.T1IP       = 5;    // Timer1 Interrupt priority level=6
    IFS0bits.T1IF       = 0;    // Reset Timer1 interrupt flag
 	IEC0bits.T1IE       = 1;    // Enable Timer1 interrupt
    
    /* Timer 2*/ 
    IPC1bits.T2IP       = 4;    // Timer2 Interrupt priority level=5
    IFS0bits.T2IF       = 0;    // Reset Timer2 interrupt flag
    IEC0bits.T2IE       = 1;    // Enable Timer2 interrupt
    
    /* Timer 3 */
    IPC2bits.T3IP       = 6;  // Set Timer1 Interrupt Priority Level
    IFS0bits.T3IF       = 0;     // Clear Timer1 Interrupt Flag
    IEC0bits.T3IE       = 1;     // Enable Timer1 interrupt
    
    /* PWM 2 */
    IPC18bits.PWM2IP    = 4;    //PWM2 interrupt priority level 4
    IFS4bits.PWM2IF     = 0;    //Reset PWM2 interrupt flag
    IEC4bits.PWM2IE     = 0;    //PWM2 enable interrupt

} // End initInterrupts






