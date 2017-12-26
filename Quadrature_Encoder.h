#ifndef QUADRATURE_ENCODER
#define	QUADRATURE_ENCODER

///*** Scaling Factors ***///
#define Max_count__revolution (8000 - 1)
#define MaxSpeed (unsigned int)(((unsigned long)Max_count__revolution*4.096)/91)) // 6000 RPM
#define HalfMaxspeed (MaxSpeed>>1) // 3000 RPM

extern int Pos[2];
extern int POSCNTmapped;
extern char print;


extern unsigned char cntrr ;

extern int Speed;
extern int SpeedRPM;

extern void QEIinit(void);

void VelocityCheckHudson (void);
void PositionCheckHudson (void);


#endif	// QUADRATURE_ENCODER

