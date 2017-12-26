#ifndef RINGBUFFER
#define	RINGBUFFER

#include <stdint.h>


void CircularBuffPush(void); 
//Function executed each time a recieve interrupt is generated.
// Transfers a byte from U1RXREG to the sf buffer to the position of the 'first' 
//element of the buffer.

void CircularBuffPop(void);
//Function executed each time a transmit interrupt is generated
// Transfers a byte from the sf buffer to the T1TXREG hd buffer.


void UartSend (unsigned char data); 
//UartSendByte functionalities:
//Check if the buffer is not full.
//If there is room inthe buffer transfer 8bit data from hd FIFO to the sf buffer.
//Set the overflow flag in case the sf buffer is already full.


unsigned char UartRecieve (void);
//UartRecieveByte functionalities:
//Check if there is available data in the sf buffer.
//If there is available data return the oldest entry.


/////flags used to monitor the status of the software buffer /////
//
//extern  unsigned int      rx_buff_not_empty; // this flag is automatically set and cleared by the software buffer
//extern  unsigned int     rx_buff_full;      // this flag is automatically set and cleared by the software buffer
//extern  unsigned int     rx_buff_ovf;       // this flag is not automatically cleared by the software buffer
//
//extern  unsigned int     tx_buff_not_empty; // this flag is automatically set and cleared by the software buffer
//extern  unsigned int     tx_buff_ovf;       // this flag is not automatically cleared by the software buffer
//extern  unsigned int     tx_buff_full;      // this flag is automatically set and cleared by the software buffer

#endif	//RINGBUFFER

