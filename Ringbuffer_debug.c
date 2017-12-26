#include "processor.h"
#include "Ringbuffer.h"
#include "global_data.h"
#include <stdbool.h>
#include <string.h>
#include "global_data.h"
#include <libpic30.h>
#include "Ringbuffer_debug.h"
#include "CMDMENU.h"

extern volatile unsigned int flag_o = 0b1;
volatile unsigned int flag_i = 0b1;

volatile unsigned char S0 [] = "                      ";
volatile unsigned char S1 [] = "----c:1/ to enable autoPWM function----\n";
volatile unsigned char S2 [] = "----c:2/ to stop ----\n";
volatile unsigned char S3 [] = "----c:3/ to update the frequency of the sine signal----\n ";
volatile unsigned char S4 [] = "----c:4/ to restore the deafult frequency of the sine signal----\n ";

//volatile unsigned char S5 [] = "Auto/\n";


/*
///// Echo loop////
 void Echo (void){
     
 //while (rx_buff_not_empty){
 receive_char = UartRecieve();
 UartSend (receive_char);
// }
 
 }
 /////Serve overflow condition  of the receive buffer//////
 void rx_overflow (void){
 while (rx_buff_ovf){
     
     ////interrupt routine/////

     
 }
 rx_buff_ovf = false;
 
 }
 /////Serve overflow condition of the transmition buffer/////
 void tx_overflow (void){
 while (tx_buff_ovf){
     
       
     ////interrupt routine/////
     
 }
  tx_buff_ovf = false;
}

 
void UartSendtest (char byte){
// while (U1STAbits.UTXBF != 0); // While the Transmit buffer is full.
 byte = U1TXREG;
}
 
 unsigned char  Uartreceivetest (void){
     unsigned char  byte;
     byte = U1RXREG;
     
     return byte;
 }
 /*
 void FeedLoop (void){ //SImulation of incomming data
     
  for (i=0; i<20; i=(i+1)%20){
      
      U1RXREG = (79+i);  
      input = U1RXREG;
      UartSendtest (input);
      IFS0bits.U1TXIF = 1;
      }
  
 }
 
 */
 
 /*
 void InputSimulation (void){
     for (i=0;i<3;i++){
         
              receive_char = (58 + i );
     
     }
     
            
     
 }
 */ 

 
#define LENGHT 128
 
 volatile unsigned char t; // Single byte buffer
 volatile unsigned char rx_buffer_index = 0;
 volatile unsigned char rx_buffer_t[LENGHT];
 
 
 void UART_Put_Character (volatile unsigned char c){
     
     rx_buffer_index = 0;
     
     while (!U1STAbits.TRMT);
     U1TXREG = c;

 }
 
 
 void UART_Put_String (volatile unsigned char *s){
     
     while (*s){
         UART_Put_Character (*s); // pointer to the first element of the string;
         s++;
     }
     
 }
 
 void __Recieve_ISR (void){
     
     t = U1RXREG;
     
     if ((t != '/') && (rx_buffer_index < LENGHT)){
          
         rx_buffer_t[rx_buffer_index] = t;
         rx_buffer_index ++;
         
     }
     else{
         rx_buffer_index = 0;
          UART_Put_String(rx_buffer_t);
         // UART_Put_String(S0);
     }
     
    
 }
 
 void WELCOME_MSG (void){
    
     
     unsigned int imsg = 0;
     unsigned int jmsg =0;

         
  for (imsg = 0; imsg < 127; imsg++){
      
      if ((imsg == 19)&&(flag_o ==0b1)){
                      
          UART_Put_String (S0);
          
  }else if ((imsg == 27) && (flag_o == 0b1)){

          UART_Put_String (S1);
          
      }else if ((imsg == 100) && (flag_o == 0b1)){

          UART_Put_String (S2);
          
      }else if ((imsg == 110) && (flag_o == 0b1)){
                    
          UART_Put_String (S3);
        
          flag_o = 0b0;

      }//end if   
      
  } // end for 
     
 
     //Clear the buffer after displaying the menu.
     if (flag_i == 0b1){
         
     for (jmsg = 0; jmsg < LENGHT; jmsg++){
     rx_buffer_t[jmsg] = 0;
     rx_buffer_index = 0;

     
     flag_i = 0b0;
      } //end for
     }//end if
      
 }// end welcome
 

