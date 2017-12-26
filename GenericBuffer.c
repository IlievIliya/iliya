//Defining the lenght of the software rx & tx buffer.
#define UART_RX_BUFF_SIZE 8
#define UART_TX_BUFF_SIZE 8

//Defining the lenght of the hardware rx & tx buffer. (FIFOs)
#define UART_HD_RX_BUFF 1
#define UART_HD_TX_BUFF 1

#include "GenericBuffer.h"
#include "processor.h"
#include "UART.h"
#include "GenericBufferDebugger.h"


#include <stdint.h>
#include <stdbool.h>


//Implemented as union since non of the elements wiil be in simultaneous usage.
//One byte allocated in the memory - corresponding to the largest element of the  union.

 typedef union{
    
    struct{
        
    volatile    uint8_t full:1; //Single bit usage.
    volatile   uint8_t empty:1; //Single bit usage.
    volatile    uint8_t reserved:6; //Six bits usage.             
        
    }s;
   
    volatile    uint8_t status;
    
}UART_STATUS;

 typedef struct{
        
       // Assigning pointers for the receive buffer.
    
 volatile      uint8_t       *ReceiveTail; // Correspondent to the newest element.
 volatile      uint8_t       *ReceiveHead; // Correspondent to the oldest element.
       
       // Assigning pointers for the transmit buffer.
       
 volatile      uint8_t       *TransmitTail; //Correspondent to the newest element.
 volatile      uint8_t       *TransmitHead; //Correspondent to the oldest element.
       
 volatile      UART_STATUS   ReceiveStatus; //Monitors the status for the receive action.
 volatile     UART_STATUS   TransmitStatus; //Monitors the status for the transmit action.
       
}UART_OBJECT;


//Variables declaration//

/////uart contains (status + pointers) /////

UART_OBJECT uart;

/////Defining receive and transmit software buffers./////

 volatile uint8_t UartReceive [UART_RX_BUFF_SIZE];
 volatile uint8_t UartTransmit [UART_TX_BUFF_SIZE];


void SoftwareBuffInit (void){
    
    
    uart.ReceiveHead = UartReceive;
    uart.ReceiveTail = UartReceive;
    
    uart.TransmitHead = UartTransmit;
    uart.TransmitTail = UartTransmit;
    
    uart.ReceiveStatus.s.empty = true;
    uart.ReceiveStatus.s.full = false;
    uart.TransmitStatus.s.empty = true;
    uart.TransmitStatus.s.full = false;   
    
}


void TxInterrupt(void){
    
    if(uart.TransmitStatus.s.empty)
    {
        IEC0bits.U1TXIE = false; // If the buffer is empty disable the transmit 
        //interrupt.
    }
    
        IFS0bits.U1TXIF = false; //Clear transmit interrupt request flag. 

    volatile    uint8_t i_t = 0; // counter;
    
        // Hardware error checking if the buffer is not full (requires 
        //space for at least one more word).
        while ((i_t < UART_TX_BUFF_SIZE) && !(U1STAbits.UTXBF == 1)){
             
            i_t ++;
        
        U1TXREG = *uart.TransmitHead; // Transmit the oldest data in the fifo buffer.
        
        uart.TransmitHead ++;
        
        if(uart.TransmitHead == (UartTransmit + UART_TX_BUFF_SIZE)){
            
            uart.TransmitHead = UartTransmit;
        }
        //Increment the head index by one and roll over when it reaches the max size.
       // uart.TransmitHead = (uart.TransmitHead + 1)%UART_TX_BUFF_SIZE; // COnflicting types.
        
        uart.TransmitStatus.s.full = false;
        
        //Condition corresponding to an empty buffer.
        if(uart.TransmitHead == uart.TransmitTail) {
            
            uart.TransmitStatus.s.empty = true;
            
            } //end if;
       
        }//end while;
        
}// end TxInterrupt;


void RxInterrupt (void){
    
 volatile   uint8_t i_r = 0;
    
    while ((i_r < UART_RX_BUFF_SIZE) && (U1STAbits.URXDA == 1)){
     
        i_r ++;
        
        *uart.ReceiveTail = U1RXREG;
        
        uart.ReceiveTail ++;
        
        if(uart.ReceiveTail ==(UartReceive + UART_RX_BUFF_SIZE)){
            
            uart.ReceiveTail = UartReceive;
            
        }
        
       // uart.ReceiveTail = (uart.ReceiveTail +1)%UART_RX_BUFF_SIZE; // Conflicting types
        
        uart.ReceiveStatus.s.empty = false;
        
        if(uart.ReceiveHead == uart.ReceiveTail){
            
            uart.ReceiveStatus.s.full = true;

        }//end if;
        
    }//end while;
    
    IFS0bits.U1RXIF = false;
    
}//end rx srt;


uint8_t UartGet (void){
    
  volatile  uint8_t data = 0;
    
    data = *uart.ReceiveHead;
    
    uart.ReceiveHead ++;
    
    if(uart.ReceiveHead ==(UartReceive + UART_RX_BUFF_SIZE)){
        
        uart.ReceiveHead = UartReceive;
    }
    
  //  uart.ReceiveHead = (uart.ReceiveHead + 1)%UART_RX_BUFF_SIZE; //Conflicting types.
    
    if(uart.ReceiveHead == uart.ReceiveTail){
        
        uart.ReceiveStatus.s.empty = true;
    }//end if
    
    uart.ReceiveStatus.s.full = false;
    
    return data;
  
}//end Get;


void UartSend (volatile uint8_t byte){
    
        
        IEC0bits.U1TXIE = false;

        *uart.TransmitTail = byte;
        
        uart.TransmitTail ++;
       
        if(uart.TransmitTail == (UartTransmit + UART_TX_BUFF_SIZE)){
            
            uart.TransmitTail = UartTransmit;
            
        }
        
     //   uart.TransmitHead = (uart.TransmitHead + 1)%UART_RX_BUFF_SIZE; // Conflicting type.
        
        uart.TransmitStatus.s.empty = false;
    
        if (uart.TransmitHead == uart.TransmitTail){
            
            uart.TransmitStatus.s.full = true;
            
        }
        
        IEC0bits.U1TXIE = true;
        
        IFS0bits.U1TXIF = true;
}




