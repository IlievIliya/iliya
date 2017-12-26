#include "PLL.h"
#include "processor.h"
#include "global_data.h"

void InitPLL(void)
{
// Configure Oscillator to operate the device at 40Mhz
// Fosc= Fin*M/(N1*N2), Fcy=Fosc/2
// external XT oscilator Fosc= 8M*40/(2*2)=80Mhz for 8M input clock
// internal FRC oscilator Fosc= 7.37*(43)/(2*2)=80Mhz for Fosc, Fcy = 40Mhz */
/* Configure PLL prescaler, PLL postscaler, PLL divisor */
	PLLFBD             = 41;	     /* M = PLLFBD + 2 */
	CLKDIVbits.PLLPOST = 0;		// N1=2
	CLKDIVbits.PLLPRE  = 0;		// N2=2
  
}
