#include "processor.h"
#include "Ringbuffer.h"
#include "UART.h"
#include "Interrupts.h"

#include <stdint.h>
#include <stdbool.h>

#define  BuffSize 25 // Defines the size of the circular buffer


unsigned char     rx_buff_not_empty = 0; 
unsigned char     rx_buff_full = 0;    
unsigned char     rx_buff_ovf = 0;  

unsigned char      tx_buff_not_empty = 0; 
unsigned char      tx_buff_ovf = 0;      
unsigned char      tx_buff_full = 0;  


//Definition of a structure (fifo)
typedef struct {
    unsigned char buff[BuffSize]; // Current size of the buffer is set to 25 bytes.
    unsigned char  input; // First entry of the buffer, the newest data
    unsigned char  output; // Last entry of the buffer, the oldest data
    unsigned char  count; // Defines the current amount of elements in the buffer.
}fifo;

/////Declaration of the sf buffers, initialize all parameters with zero values//////

fifo rx_buffer = {{0}, 0, 0, 0}; 
fifo tx_buffer = {{0}, 0, 0, 0};


/////Transfer a byte from U1RXREG to sf buffer.//////

void CircularBuffPush(void){
    if (rx_buffer.count == BuffSize){ //Condition for full sf buffer
        rx_buff_ovf = true; // Set overflow flag
        
 }else if ( U1RXREG != '/' && (rx_buffer.count < BuffSize)){ // If there is available space in the buffer
   
   //  while((U1STAbits.OERR == false)&&(U1STAbits.FERR == false)) //while there are no framing errors and hd buff overflow
            
           rx_buffer.buff[rx_buffer.input] = U1RXREG; // Transfer the data from the Hd buffer to the sf buffer
  
           rx_buffer.input = (rx_buffer.input + 1)%BuffSize; // Increment the input pointer by 1 and auto roll over to 0 element.
           rx_buffer.count++; //Increment the number of bytes in sf buffer by 1.
 }
   
  /////Serving the condition flags of the rx_buff//////
    
   if (rx_buffer.count == BuffSize){ // Just filled up after latest data entry
       rx_buff_full = true;
   }
   
    rx_buff_not_empty = true; // Indicates that there is available data. 
    
}

/////Transfer a byte from the sf buffer to the U1TXREG.//////

void CircularBuffPop(void){
    
    if (tx_buffer.count == BuffSize){
        tx_buff_full = false; //clear the full flag
    }
    
    if((tx_buffer.count > 0) && (!U1STAbits.TRMT)){
        
         
        U1TXREG = tx_buffer.buff[tx_buffer.output];
    
        tx_buffer.output = (tx_buffer.output + 1)%BuffSize; // Increment the index of the oldest element, auto roll over to 1st element.
        tx_buffer.count--; //Decrement the amount of elements by 1.
    }
    
      /////Serving the condition flags of the tx_buff//////

    if(tx_buffer.count == 0){ // If no more data exists clear the not empty flag
    tx_buff_not_empty = false;
    //disable UART "TX hw buffer empty" interrupt here 
    U1STAbits.UTXBF = 0;
    
    }

}
 


///// Function used to send data from the sf buff via UART/////
void UartSend (unsigned char  data){
    
   disableInterrupts(); // During the transmission the interrupts are disabled 
   
   if(tx_buffer.count == BuffSize){
       tx_buff_ovf = true;
   }else if (tx_buffer.count < BuffSize){
       tx_buffer.buff[tx_buffer.input] = data; //Inserts the data in the tx_buffer from where it will be send via UART.   
       tx_buffer.input = (tx_buffer.input + 1)%BuffSize;// increment and roll over t 0
       tx_buffer.count ++;
   }
   
   if(tx_buffer.input == BuffSize){ 
       tx_buff_full = true;
   }
   
   enableInterrupts();
   
   if(tx_buffer.count > 0){
       tx_buff_not_empty = true;
       IFS0bits.U1TXIF = 1;   
   }
     
}



unsigned char  UartRecieve (void){
    
    disableInterrupts();
    
    unsigned char  data = 0;
    
    if(rx_buffer.count == BuffSize){ 
        rx_buff_full = false; //clear the flag, the buffer will be emptied.
    }
    
    if(rx_buffer.count > 0){ //If there is available data in the receive buffer
        data = rx_buffer.buff[rx_buffer.output];  // Get the oldest entry and send it to UR1XREG
        
        rx_buffer.output = (rx_buffer.output +1)%BuffSize;
        rx_buffer.count --;
    } else{ //Now the receive buffer has been emptied.
        rx_buff_not_empty = false; // Indicates that the buffer is empty.
          }   
    
    enableInterrupts();
    
    return data;
    
}
