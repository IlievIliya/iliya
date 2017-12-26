#include "Sine_Generator.h"
#include "processor.h"
#include "PrintOutput.h"

#define Samples 60
#define Samples1 60

/***Current Configuration ***/

/***Sample time - 200us
 
    Samples per period for the sine wave - 60
    PWM2 period 20 us 
      -> RC time constant of the filter >= 20 us
      -> 200/ 20 ~ 10 - On each 10 PWM cycles a sample is made;
     
    Frequency of the generated sine signal = 60 samples * 200us = 12 000us ~ 83 Hz
       ***/



//Variable used to update the selsected duty cycle from the table each TMR1 ISR.
unsigned char DC1_count = 0;
unsigned char DC2_count = 20;
unsigned char DC3_count = 40;


unsigned char DC1_count0 = 20;
unsigned char DC2_count0 = 40;
unsigned char DC3_count0 =0;


//Constant values written in the memory used to quantize the sine wave.
// 60 - sample points, Max amplitude ().

/** THE AMPLITUDE IS DIRECTLY PROPORTIONAL TO THE COEFICIENT MULTIPLYING 
ALL TERMS **/

//Amplitude - 100% 1600, 60 samples;
const unsigned int Sine_table [Samples] = { 
   
   
800,884,966,1047,1125,1200,1270,1335,
1395,1447,1493,1531,1561,1583,1596,1600,
1596,1583,1561,1531,1493,1447,1395,1335,
1270,1200,1125,1047,966,884,800,716,
634,553,475,400,330,265,205,153,
107,69,39,17,4,0,4,17,
39,69,107,153,205,265,330,400,
475,553,634,716,

};

const unsigned int Sinetable [Samples] = {

400,442,483,524,563,600,635,668,
697,724,746,765,780,791,798,800,
798,791,780,765,746,724,697,668,
635,600,563,524,483,442,400,358,
317,276,237,200,165,132,103,76,
54,35,20,9,2,0,2,9,
20,35,54,76,103,132,165,200,
237,276,317,358
    
};




void TMR1_ISR_Sine_update (void){
    

        P1DC1 = Sine_table[DC1_count];           // Update the new value of the Duty cycle (DC)
        DC1_count = (DC1_count +1)%Samples; //Alternative for increment and row over to 0.
                             
        P1DC2 =  Sine_table[DC2_count];           // Update the new value of the Duty cycle (DC)
        DC2_count = (DC2_count +1)%Samples; //Alternative for increment and row over to 0.

        P1DC3 = Sine_table[DC3_count];           // Update the new value of the Duty cycle (DC)
        DC3_count = (DC3_count +1)%Samples; //Alternative for increment and row over to 0.

        //Clear the ISR flag.
        
}// End  TMR1_ISR_Sine_update

void TMR2_ISR_SINE_UPDATE (void){
    
        P1DC1 = Sinetable[DC1_count0];           // Update the new value of the Duty cycle (DC)
        DC1_count0 = (DC1_count0 +1)%Samples; //Alternative for increment and row over to 0.
                             
        P1DC2 =  Sinetable[DC2_count0];           // Update the new value of the Duty cycle (DC)
        DC2_count0 = (DC2_count0 +1)%Samples; //Alternative for increment and row over to 0.

        P1DC3 = Sinetable[DC3_count0];           // Update the new value of the Duty cycle (DC)
        DC3_count0 = (DC3_count0 +1)%Samples;    
}