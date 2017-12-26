#ifndef TRAPEZOIDAL_GENERATOR
#define	TRAPEZOIDAL_GENERATOR

void Trapezoidal_table_ISR_TMR2 (void);
void TrapezoidalFuncCalculator (void);
void TrapezoidalTable_P_Slope (void);

void Trapezoidal_table_ISR_TMR2_N_slope (void);
void Trapezoidal_table_ISR_TMR2_slope (void);
void StateInitARRAY (void);

void AutoFrfequencyChange(void);

void STATE_1i (void);
void STATE_2i (void);
void STATE_3i (void);
void STATE_4i (void);
void STATE_5i (void);
void STATE_6i (void);

void STATE_1 (void);
void STATE_2 (void);
void STATE_3 (void);
void STATE_4 (void);
void STATE_5 (void);
void STATE_6 (void);



#endif	//TRAPEZOIDAL_GENERATOR

