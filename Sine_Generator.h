#ifndef SINE_LOOK_UP_TABLE
#define	SINE_LOOK_UP_TABLE


//const unsigned char Sine_table[128];
//unsigned char DC_count;

void TMR1_ISR_Sine_update (void);
void SineGenerator_Status(void);
void TMR2_ISR_SINE_UPDATE (void);

void InitialvalueIndication (void);

#endif	// SINE_LOOK_UP_TABLE

