#ifndef COMMUNICATION
#define	COMMUNICATION

void Comm (unsigned char data);
void SPECIFIC_CM (unsigned char data);
unsigned char EvaluateHEX (unsigned char symbol );
void InitComm (void);

extern unsigned char Comm_State; // state REG declaration global
extern unsigned char COM;


extern int temp;
extern int parameter[4];
extern unsigned char data_d;

/// *** State labels ***//
#define WAIT_HEADER         1    // c
#define CONFIRM_HEADER      2    // :
#define Comm_Distribution   3

#define PWMHconf1           4
#define PWMHconf0           5    // p

#define TMRconf             6    // t

#define MCconf              7    // m

#define MENU                8   // ?
#define PWMOPTIONS          9   // ?



///*** PWM parameter parsing ***///
#define RECEIVE1            10
#define RECEIVE2            11
#define RECEIVE3            12
#define RECEIVE4            13
#define UPDATE              14

///*** MC ***///
#define CCW_ROTATION        15
#define CW_ROTATION         16
#define AUTO_DC             17
#define BREAK               18
#define EMERGENCYSTOP       19
#define PROGRAM             20

///*** Specific CMD ***///
#define CAN                 21 // cancel //24 C
#define EM                  22 // End of medium //25 e
#define ESC                 23 // ESCAPE // 27 E
#define DEL                 24 // DELETE // 127 D
#define WAITSTART           25 // *
#define START               26
#define USEPARATOR          27 // unit separator // 31

///*** Display Velocity/Position ***///
#define DISPLAY             28
#define DISPLAYON           29
#define DISPLAYOFF          30

#define PROGRAM1            31
#define MOD                 32
#define MODSTART            33
#define MODSTOP             34

#define TMR1_2              35

#define INVALID             36
#define INVALIDCOM          37

#define CLEARAWINDOW        38



void PROGRAMEXE (void);
void EMERGENCYSTOPEXE(void);
void AUTOEXE (void);
void BREAKEXE (void);
void CWEXE (void);
void CCWEXE (void);

void CLEARSCREEN (void);


#endif
