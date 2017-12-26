#ifndef CMDMENU
#define	CMDMENU

extern unsigned char frequency [4];
extern unsigned char f_index;
  
extern unsigned char state_p;
extern unsigned char state;


void InitMenu (void);
void _MENU (volatile unsigned char data);
 
#endif	// CMDMENU

