#include "processor.h"
#include "global_data.h"
#include "ADC.h"
#include "UART.h"


void InitADC (void){
    
    //Turn off the ADC module
    AD1CON1bits.ADON    = 0;        //ADC - OFF

    AD1CON1bits.AD12B   = 0;        //10bit ADC
    AD1CON2bits.CHPS    = 0;        //Single channel (CH0)
    AD1CON2bits.VCFG    = 0b111;    //Using AVdd AVss for VREFH and VREFL
    
    //System clock setting
    //Tad must be at least 117,6ns 
    //Tad = Tcy*(ADCS+1) = 25*(4+1) = 125ns
    AD1CON3bits.ADRC    = 0;        //using system clock
    AD1CON3bits.ADCS    = 4;            
    
    AD1CON1bits.FORM    = 0;        //Unsigned integer data format    
    AD1CON2bits.SMPI    = 0;        //Generate interrupt after every sample/conversion
    AD1CON1bits.SSRC    = 0b111;    //Automatic conversion trigger
    AD1CON1bits.ASAM    = 1;        //Auto-sampling after last conversion
    
    AD1CON2bits.ALTS    = 0;        //Always uses channel input selects for Sample A
    AD1CON2bits.BUFM    = 0;        //Always starts filling the buffer from the start address
    AD1CON2bits.CSCNA   = 1;        //Scan inputs
    
    //Sampling time setting
    //Tsamp = 3*TAD = 3*125 = 375ns
    AD1CON3bits.SAMC    = 3;
    
    AD1CHS0bits.CH0SA   = 0;        //Channel 0 positive input is AN0
    AD1CHS0bits.CH0NA   = 0;        //Channel 0 negative input is VREFL
    
    AD1CSSLbits.CSS0    = 1;        //Select AN0 for scan
    
    //Turn on the ADC module   
    AD1CON1bits.ADON    = 1;        //ADC - ON
    
}


void __attribute__((interrupt, no_auto_psv)) _ADC1Interrupt(void)
{
    IFS0bits.AD1IF      = 0;
}


