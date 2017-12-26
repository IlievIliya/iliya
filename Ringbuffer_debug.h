#ifndef RINGBUFFER_DEBUG_H
#define	RINGBUFFER_DEBUG_H

void Echo (void);
void rx_overflow (void);
void tx_overflow (void); 
 extern int character;
 void UartSendtest (unsigned char byte);
 void InputSimulation (void);
 void FeedLoop (void );
 void UART_Put_Character (unsigned char c);
 void UART_Put_String (volatile unsigned char *s);
 void __Recieve_ISR (void);
 void WELCOME_MSG (void);
 
 extern volatile unsigned char rx_buffer_t[];
 extern volatile unsigned char S5 [];
  
 void menu (void);
 void CLEAR_BUFF (void);
  
#endif	//

