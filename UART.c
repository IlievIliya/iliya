#include "processor.h"
#include "UART.h"
#include "PrintOutput.h"
#include "CMDMENU.h"
#include "UART_Buffer.h"
#include "global_data.h"
#include "Communication_.h"


#include <string.h>

#include "test.h"

 buff rx_buffer = {{0}, 0, 0}; // UART buffer initialization.

unsigned char received; // Variable used to store the received data and pass it
                        // to the ring buffer.


void InitUART(unsigned char frame, unsigned char trailer, int BAUD_rate ){
    
	//U1MODE
	U1MODEbits.UARTEN   = 0;	//UART is disabled
    
	U1MODEbits.USIDL    = 0;	//Continues operation in Idle mode
	U1MODEbits.IREN     = 0;	//IrDA encoder and decoder are disabled
	U1MODEbits.RTSMD    = 1;	//UxRTS is in Simplex mode
	U1MODEbits.UEN      = 0;    //UxTX and UxRX pins are enabled and used
	U1MODEbits.WAKE     = 0;	//Wake-up is disabled
	U1MODEbits.LPBACK   = 0;	//Loopback mode is disabled
	U1MODEbits.ABAUD    = 0;	//Baud rate measurement is disabled or complete (would require sending '55')
	U1MODEbits.URXINV   = 0;	//UxRX Idle state is ?1?
	U1MODEbits.BRGH     = 0;	//BRG generates 16 clocks per bit period
	U1MODEbits.PDSEL    = frame;	//8-bit data, no parity
	U1MODEbits.STSEL    = trailer;	//1 Stop bit
	
	//Load a value into Baud Rate Generator.Example is for 9600.
	//U2BRG = (Fcy/(16*BaudRate))-1
    
	U1BRG = BAUD_rate;         

	//U1STA

	U1STAbits.UTXISEL0  = 0;
    U1STAbits.UTXISEL1  = 0;
    
	U1STAbits.UTXINV    = 0;    //UxTX Idle state is ?1?
	U1STAbits.UTXBRK    = 0;    //Sync Break transmission is disabled or complete
    
    //UART RX will generate interrupt after every received character   
	U1STAbits.URXISEL0  = 0;    
    U1STAbits.URXISEL1  = 0;   
    
	U1STAbits.ADDEN     = 0;    //Address Detect mode is disabled
	
    //Turn on UART
    U1MODEbits.UARTEN   = 1;    //UART is enabled   
    U1STAbits.UTXEN     = 1;    //Transmit is enabled
        
}

void __attribute__ ((interrupt, no_auto_psv)) _U1RXInterrupt(void) {
   
   
  // __Recieve_ISR(); // Echo test
  bufferWrite (&rx_buffer, U1RXREG);  // Function arguments - pointer to array and 
                                      // received data via You Art.
  
  bufferRead (&rx_buffer, &received); //Read from the ring buffer and store in 
                                      // received variable
 
  if(BUFFER_READY == 1){ 

      Comm(received); // Communication.c
  
    SPECIFIC_CM (received);

  }   
  
  if (U1STAbits.OERR) _OERR = 0; // IF OVERFLOW OCCURS CLEAR THE BUFFER.
                                 // And the OERR flag.
  
    IFS0bits.U1RXIF = 0; // Clear ISR flag.
  
} // End RX iISR

void __attribute__((interrupt, no_auto_psv)) _U1TXInterrupt(void){
    
    IFS0bits.U1TXIF = 0;

} // End TX ISR

