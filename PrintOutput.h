#ifndef RINGBUFFER_DEBUG_H
#define	RINGBUFFER_DEBUG_H

#define LENGHT 128 // Asociated with printout buffe lenght

void UART_Put_Character ( char c);
extern void UART_Put_String ( char *s);
extern void __Recieve_ISR (void);
extern void NewLine (void);

void SHOW_MENU (void);
void InitMessage (void);
 
extern  unsigned char flag_o;
extern  unsigned char flag_i;
extern  unsigned char flag_p;
extern  unsigned char i_T;

#endif	//

