#ifndef HUDSON_23
#define	HUDSON_23

#define st1     0b000
#define st2     0b001
#define st3     0b011
#define st4     0b100
#define st5     0b110
#define st6     0b111

#define stp1    0b000
#define stp2    0b001
#define stp3    0b011
#define stp4    0b100
#define stp5    0b110
#define stp6    0b111



// *** Forward rotation phase sequence *** //
extern void step1 (void);
extern void step2 (void);
extern void step3 (void);
extern void step4 (void);
extern void step5 (void);
extern void step6 (void);

// *** Reverse rotation phase sequence ***//
extern void step1R (void);
extern void step2R (void);
extern void step3R (void);
extern void step4R (void);
extern void step5R (void);
extern void step6R (void);


extern void init_Hudson23 (void);

extern void Hudson_Commutation_StartUp (void);

extern void InitState (void);

extern void StateUpdate (void);
extern void BreakBLDC (void);

extern void BLDC_commands (unsigned char com);

extern unsigned char temp_bldc;
extern unsigned char _ind;

typedef struct Hudson{
    
    unsigned char CW_Rotation:1;
    unsigned char CCW_Rotation:1;
    unsigned char Break:1;
    unsigned char Stop:1;
    unsigned char Comb_logic:1;
    unsigned char unused:3;
    
    unsigned char state;
    unsigned char index;
    unsigned char counter;
    unsigned char temp;
    unsigned char brake_indx;
    
    unsigned char comstate;
    
}HudsonTAG;



#endif	// HUDSON_23

