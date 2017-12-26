#ifndef Timer
#define Timer

void InitTMR1(unsigned char PRESCALER);
void InitTMR2(unsigned char PRESCALER);
void InitTMR3(unsigned char PRESCALER);

extern unsigned char test_flag;
extern unsigned char test_index;
extern unsigned char cycles;



#endif

