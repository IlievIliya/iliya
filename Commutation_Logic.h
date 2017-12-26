#ifndef COMMUTATION_LOGIC_H
#define	COMMUTATION_LOGIC_H

///*** Notations used to reverse the increase/decrease direction in AutoPWM ***///
#define decreasing 1
#define	increasing 0

///***Reverse direction program steps***///
#define step1      1
#define step2      2
#define step3      3
#define step4      4

#define Samples    60
 
  /// Outputs/ flags connected to hardware
  extern  unsigned char   Enable;
  extern  unsigned char   Direction;
    
    ///Sftware flags
  extern  unsigned char   Break;
  extern  unsigned char   AutoDC;
  extern  unsigned char   Start;
  extern  unsigned char   I_D_flag;
  extern  unsigned char   Reverse;
  extern  unsigned char   SoftStart;
  extern  unsigned char   Modulation;

    
  extern unsigned int     Counter;
  extern unsigned char    index_mod;

void BreakBLDC (void);
void BLDC_commands (unsigned char com);
void AutoPWM (void);
void SoftStartFUNC(void);
void InitCLFlags (void);

void Modulation__ (void);

void CCW (void);
void CW (void);

extern void InitReverseDirection (void);
extern void ReverseDirection_Auto (unsigned char step);

extern unsigned char stepstate;

#endif	/* COMMUTATION_LOGIC_H */

