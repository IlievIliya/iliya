#include "Trapezoidal_generator.h"
#include "processor.h"


#include <stdbool.h>

#define Samples_T 60 // Number of points
#define Samples0 10 // Number of points
#define intervals 6 
#define FCY 40000000 // Hz

bool I_D_flag_f = false;

unsigned char SamplesPerInterval = 0;
unsigned int  TMR2_PresC = 0;
float Sample_time;
float Slope;
unsigned int DUTY_CYCLE;
unsigned int increment = 0;

unsigned char DC_T1 = 11;
unsigned char DC_T2 = 0;
unsigned char DC_T3 = 31;

unsigned char DC_index_commutation = 0;
unsigned char DC_index_commutation0 = 0;

unsigned char DC_BLCD = 0;

/// Trapezoidal table for one full period /// 
// Adapted for max value of the amplitude of 1600 (d)
// Fpwm = 50Khz


const unsigned int TrapezoidalTable [Samples_T] = {

0, 161, 322, 483, 644, 805, 966, 1127, 1288, 1449, //60 degrees y = 4.6x , m = 4,6; 
        
1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, //120 dgrees m = 0;

1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, //180 dgrees m = 0;

1499, 1289, 1227, 966, 805, 644, 483, 322, 161, 0, // 240 degrees m = -4,6

0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 300 degress m= 0;

0, 0, 0, 0, 0, 0, 0, 0, 0, 0 // 360 degrees m = 0;


};

////***Tables used to modulate the PWM signal ***////

const unsigned int TrapezoidalTablePositiveSlope [Samples0] = {
    
322, 483, 644, 805, 966, 1127, 1288, 1449, 1500, 1500//y = mx
            
};

const unsigned int SymetricalModulation [Samples0] = {
    
800, 800, 800, 800, 800, 800, 800, 800, 800, 800 // (2*P1TPER / 2) - 50% DC
    
};

const unsigned int TrapezoidalTableNegativeSlope [Samples0] = {
  
 1499, 1289, 1227, 966, 805, 644, 483, 322, 161, 0
    
};

const unsigned TrapezoidalTableZeroSlope_ZeroValue[Samples0] = {
  
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    
};


const unsigned int Modulation [Samples0] = {
    
300, 400, 500, 600, 700, 800, 900, 1000, 1200, 1600// 50% transition to 100%
            
};

void Trapezoidal_table_ISR_TMR2 (void ){
    
    P1DC1 = TrapezoidalTable [DC_T1]; //Update of the value for the firsst PWM 
                                       // channel.
    DC_T1 = (DC_T1 + 1)%Samples_T;     // Update of the index poinitng to the table.
    
    P1DC2 = TrapezoidalTable [DC_T2];
    DC_T2 = (DC_T2 + 1)%Samples_T;
    
    P1DC3 = TrapezoidalTable [DC_T3];
    DC_T3 = (DC_T3 + 1)%Samples_T;
   
    
}// End of Trapezoidal_table_ISR_TMR2

//
//void TrapezoidalFuncCalculator (void){
//   
//    SamplesPerInterval = (Samples_T / intervals); // I
//    
//    if (T2CONbits.TCKPS == 0){   // II
//        TMR2_PresC = 1;
//    }else if (T2CONbits.TCKPS == 1){
//        TMR2_PresC = 8;
//    }else if (T2CONbits.TCKPS == 2){
//        TMR2_PresC = 64;
//    }else if (T2CONbits.TCKPS == 3){
//        TMR2_PresC = 256;
//    }
//    
//    Sample_time = ((PR2 *(1/FCY))/TMR2_PresC ); //III
//    
//    Slope = ((2*P1TPER) /(Sample_time * SamplesPerInterval));
//    
//    DUTY_CYCLE = ((int)Slope *((int)Sample_time)*increment);
//    increment = (increment + 1)%SamplesPerInterval;
//    
//    DUTY_CYCLE = P1DC1;
//   
//}


void Trapezoidal_table_ISR_TMR2_slope (void ){
    
    P1DC1 = TrapezoidalTablePositiveSlope [DC_T1]; //Update of the value for the firsst PWM 
                                       // channel.
    DC_T1 = (DC_T1 + 1)%Samples0;     // Update of the index poinitng to the table.
    
    P1DC2 = TrapezoidalTablePositiveSlope [DC_T2];
    DC_T2 = (DC_T2 + 1)%Samples0;
    
    P1DC3 = TrapezoidalTablePositiveSlope [DC_T3];
    DC_T3 = (DC_T3 + 1)%Samples0;
   
    
}


void Trapezoidal_table_ISR_TMR2_N_slope (void){
    
   P1DC1 = TrapezoidalTableNegativeSlope [DC_T1]; //Update of the value for the firsst PWM 
                                       // channel.
    DC_T1 = (DC_T1 + 1)%Samples0;     // Update of the index poinitng to the table.
    
    P1DC2 = TrapezoidalTableNegativeSlope [DC_T2];
    DC_T2 = (DC_T2 + 1)%Samples0;
    
    P1DC3 = TrapezoidalTableNegativeSlope [DC_T3];
    DC_T3 = (DC_T3 + 1)%Samples0;
}


//*** FUNCTIONS USED FOR THE COMMUTATION SEQUENCE ***//

//*** Override control is used ****//

void STATE_1 (void){
    
    //*** OVDCON register configuration ***//
    
    //***POVD bits ***//
    
    //** Determines if the output is driven by PWM (1) or set by POUT bit (0).
    
    
    P1OVDCONbits.POVD1H = 1;
    P1OVDCONbits.POVD1L = 0;

    P1OVDCONbits.POVD2H = 0;
    P1OVDCONbits.POVD2L = 0;

    P1OVDCONbits.POVD3H = 0;
    P1OVDCONbits.POVD3L = 0;
    
    
    //*** Determines if the output is HIGH or LOW.***//
    
    P1OVDCONbits.POUT1H = 0;
    P1OVDCONbits.POUT1L = 0;

    P1OVDCONbits.POUT2H = 0;
    P1OVDCONbits.POUT2L = 0;

    
    P1OVDCONbits.POUT3H = 0;
    P1OVDCONbits.POUT3L = 1;
    
      
    
 P1DC1 = SymetricalModulation [DC_index_commutation];
 DC_index_commutation = (DC_index_commutation + 1)%Samples0;
    P1DC3 = SymetricalModulation [DC_index_commutation0];
    DC_index_commutation0 = (DC_index_commutation0 + 1)%Samples0;
    


}

void STATE_2 (void){
    
    //** Determines if the output is driven by PWM (1) or set by POUT bit (0).

    
    P1OVDCONbits.POVD1H = 1;
    P1OVDCONbits.POVD1L = 0;

    P1OVDCONbits.POVD2H = 0;
    P1OVDCONbits.POVD2L = 0;

    P1OVDCONbits.POVD3H = 0;
    P1OVDCONbits.POVD3L = 0;
    
    
    //*** Determines if the output is HIGH or LOW.***//
    
    P1OVDCONbits.POUT1H = 0;
    P1OVDCONbits.POUT1L = 0;

    P1OVDCONbits.POUT2H = 0;
    P1OVDCONbits.POUT2L = 1;

    
    P1OVDCONbits.POUT3H = 0;
    P1OVDCONbits.POUT3L = 0;

    P1DC1 = SymetricalModulation [DC_index_commutation];
    DC_index_commutation = (DC_index_commutation + 1)%Samples0;
    P1DC2 = SymetricalModulation [DC_index_commutation0];
    DC_index_commutation0 = (DC_index_commutation0 + 1)%Samples0;

    
}

void STATE_3 (void) {
    
    //** Determines if the output is driven by PWM (1) or set by POUT bit (0).
    
    P1OVDCONbits.POVD1H = 0;
    P1OVDCONbits.POVD1L = 0;

    P1OVDCONbits.POVD2H = 0;
    P1OVDCONbits.POVD2L = 0;

    P1OVDCONbits.POVD3H = 1;
    P1OVDCONbits.POVD3L = 0;
    
    
    //*** Determines if the output is HIGH or LOW.***//
    
    P1OVDCONbits.POUT1H = 0;
    P1OVDCONbits.POUT1L = 0;

    P1OVDCONbits.POUT2H = 0;
    P1OVDCONbits.POUT2L = 1;

    
    P1OVDCONbits.POUT3H = 0;
    P1OVDCONbits.POUT3L = 0;
    
   P1DC3 = SymetricalModulation [DC_index_commutation];
   DC_index_commutation = (DC_index_commutation + 1)%Samples0;
   P1DC2 = SymetricalModulation [DC_index_commutation0];
   DC_index_commutation0 = (DC_index_commutation0 + 1)%Samples0;

}

void STATE_4 (void) {
    
   //** Determines if the output is driven by PWM (1) or set by POUT bit (0).
    
    P1OVDCONbits.POVD1H = 0;
    P1OVDCONbits.POVD1L = 0;

    P1OVDCONbits.POVD2H = 0;
    P1OVDCONbits.POVD2L = 0;

    P1OVDCONbits.POVD3H = 1;
    P1OVDCONbits.POVD3L = 0;
    
    
    //*** Determines if the output is HIGH or LOW.***//
    
    P1OVDCONbits.POUT1H = 0;
    P1OVDCONbits.POUT1L = 1;

    P1OVDCONbits.POUT2H = 0;
    P1OVDCONbits.POUT2L = 0;

    
    P1OVDCONbits.POUT3H = 0;
    P1OVDCONbits.POUT3L = 0;

   P1DC3 = SymetricalModulation [DC_index_commutation];
   DC_index_commutation = (DC_index_commutation + 1)%Samples0;
   P1DC1 = SymetricalModulation [DC_index_commutation0];
   DC_index_commutation0 = (DC_index_commutation0 + 1)%Samples0;
   
  
    
}

void STATE_5 (void){
    
 //** Determines if the output is driven by PWM (1) or set by POUT bit (0).
    
    P1OVDCONbits.POVD1H = 0;
    P1OVDCONbits.POVD1L = 0;

    P1OVDCONbits.POVD2H = 1;
    P1OVDCONbits.POVD2L = 0;

    P1OVDCONbits.POVD3H = 0;
    P1OVDCONbits.POVD3L = 0;
    
    
    //*** Determines if the output is HIGH or LOW.***//
    
    P1OVDCONbits.POUT1H = 0;
    P1OVDCONbits.POUT1L = 0;

    P1OVDCONbits.POUT2H = 0;
    P1OVDCONbits.POUT2L = 1;

    
    P1OVDCONbits.POUT3H = 0;
    P1OVDCONbits.POUT3L = 0;
  
   
   P1DC2 = SymetricalModulation [DC_index_commutation];
   DC_index_commutation = (DC_index_commutation + 1)%Samples0;   

}

void STATE_6 (void){
    
 //** Determines if the output is driven by PWM (1) or set by POUT bit (0).
    
    P1OVDCONbits.POVD1H = 0;
    P1OVDCONbits.POVD1L = 0;

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
    P1OVDCONbits.POUT3L = 1;
    
   P1DC2 = SymetricalModulation [DC_index_commutation];
   DC_index_commutation = (DC_index_commutation + 1)%Samples0;
   P1DC3 = SymetricalModulation [DC_index_commutation0];
   DC_index_commutation0 = (DC_index_commutation + 1)%Samples0;
   

    
}

//***Increment and decrement the frequency  using TMR!1 period register (between 0001 - FFFF) ***//



void AutoFrfequencyChange (void){
    if (I_D_flag_f == true){
        
        PR1 ++;
        
        if(PR1 == 0xFFFF){
            I_D_flag_f = false;
        }
    } // end if
    
    if (I_D_flag_f == false){
        
        PR1 --;
        
        if(PR1 == 0x0001){
            I_D_flag_f = true;
        }
        
    }
    
}// end Autofrequency


//*** FUNCTIONS USED FOR THE COMMUTATION SEQUENCE DURING THE FIRST 3 seconds ***//

//*** THE MOTOR RECEIVES 70% OF THE VDD ***//

//27 Feb.


void STATE_1i (void){
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
    
    //*** Inactive PWM outputs ***//

    P1DC1 = Modulation [DC_T1];         //Update of the value for the firsst PWM 
                                      // channel.
    DC_T1 = (DC_T1 + 1)%Samples0;     // Update of the index poinitng to the table.    
    
    P1DC3 = Modulation [DC_T3];
    DC_T3 = (DC_T3 + 1)%Samples0;

    
}

void STATE_2i (void){
   
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
    
    
    P1DC1 = Modulation [DC_T1];        //Update of the value for the firsst PWM 
                                       // channel.
    DC_T1 = (DC_T1 + 1)%Samples0;     // Update of the index poinitng to the table.    
    
    P1DC2 = Modulation [DC_T2];
    DC_T2 = (DC_T2 +1)%Samples0;

    
}

void STATE_3i (void) {
    
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
    
    
    P1DC3 = Modulation [DC_T3];
    DC_T3 = (DC_T3 + 1)%Samples0;
    
    P1DC2 = Modulation [DC_T2];
    DC_T2 = (DC_T2 +1)%Samples0;
    
    

}

void STATE_4i (void) {
    
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

    
    P1DC3 = Modulation [DC_T3];
    DC_T3 = (DC_T3 + 1)%Samples0;
    
    P1DC1 = Modulation [DC_T1];       
    DC_T1 = (DC_T1 + 1)%Samples0; 
    
    
    
}

void STATE_5i (void){
    
   //** Determines if the output is driven by PWM (1) or set by POUT bit (0).
    
    P1OVDCONbits.POVD1H = 0;
    P1OVDCONbits.POVD1L = 0;

    P1OVDCONbits.POVD2H = 1;
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
   
    P1DC2 = Modulation [DC_T2];
    DC_T2 = (DC_T2 + 1)%Samples0;
    

}

void STATE_6i (void){
    
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
    
     P1DC2 = Modulation [DC_T2];
     DC_T2 = (DC_T2 + 1)%Samples0;
    
    P1DC3 = Modulation [DC_T3];
    DC_T3 = (DC_T3 + 1)%Samples0;
     
     
}

