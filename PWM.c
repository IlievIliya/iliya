#include "processor.h"
#include "global_data.h"
#include "PWM.h"
#include "Trapezoidal_generator.h"
#include "Sine_Generator.h"

#include <Libpic30.h> 
#include <stdint.h>

void InitPWM2(unsigned char PRESCALER, unsigned char PT_MOD, unsigned char MODE) {

   P2TCONbits.PTEN      = 0;    //PWM time base is off
   
   P2TCONbits.PTMOD     = PT_MOD;    //PWM time base operates in Free Running mode
   P2TCONbits.PTCKPS    = PRESCALER; //PWM prescaler set to 1:1 - resulting operational frequency of 5 Mhz
   P2TCONbits.PTOPS     = 0;         //PWM Time Base Output Postscale (1:1 postscale)   

   PWM2CON1bits.PMOD1   = MODE;    //PWM2 operates in complementary mode
   PWM2CON1bits.PEN1H   = 1;    //PWM2H1 pin is enabled for PWM output       
   PWM2CON1bits.PEN1L   = 1;    //PWM2L1 pin is enabled for PWM output
   
   PWM2CON2bits.IUE     = 1;    //Immediate update of PWM enabled
   
   //Dead time control registers
   
   P2DTCON1bits.DTAPS   = 2;    //Clock period for Dead Time Unit A is TcY
   P2DTCON1bits.DTBPS   = 2;    //Clock period for Dead Time Unit B is TcY
   
   P2DTCON1bits.DTA     = 0;   //Dead time value for Dead Time Unit A
   P2DTCON1bits.DTB     = 0;   //Dead time value for Dead Time Unit B 

   P2TPER   = 0x0600;           //Period Register - 2.5us 400Khz
   P2DC1    = 0x0000;           //Duty Cycle Register
   
   P2TCONbits.PTEN      = 1;    //PWM time base is on
 
}



void __attribute__((interrupt, no_auto_psv)) _MPWM2Interrupt (void)
{
	IFS4bits.PWM2IF = 0;	// Clear interrupt flag
}

