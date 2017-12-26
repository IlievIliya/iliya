#include "processor.h"
#include "global_data.h"
#include "PrintOutput.h"

#include <libpic30.h>

/// *** Flags used to manage the initial menu ***///
unsigned char flag_o = 0b0;
unsigned char flag_i = 0b1;
unsigned char flag_p = 0b0;
unsigned char i_T =0;



/// *** Text strings - templates *** ///
volatile unsigned char S0 [] = "c:mR; -- CW rotation.";
volatile unsigned char S1 [] = "c:mL; -- CCW rotation.";
volatile unsigned char S2 [] = "c:mB; -- Break.";
volatile unsigned char S3 [] = "c:mA; -- Auto DC regulation.";
volatile unsigned char S4 [] = "c:mS; -- Emergency Stop.";
volatile unsigned char S5 [] = "c:p?; -- Show PWM config. options.";
volatile unsigned char S6 [] = "c:?;  -- Show Menu";


volatile unsigned char S7 [] = "c:p =XXXXD -- SET XXXX as DC";
volatile unsigned char S8 [] = "c:p =XXXXP -- SET XXXX as Period";

volatile unsigned char S9 [] = "c:D(+/-)  -- ON/OFF position/velocity visualization";


    
/// *** Printout buffer variables *** ///  
 volatile unsigned char t;                   // Single byte buffer
 volatile unsigned char rx_buffer_index = 0; // index for the elements in the receive
                                             // buffer.
 volatile unsigned char rx_buffer_t[LENGHT]; // Buffer size
 

 void UART_Put_Character (char c){
     
     rx_buffer_index = 0;
     
     while (!U1STAbits.TRMT);
     U1TXREG = c;

 } // End UART_Put_Character
 
 // Receives pointer to a string / calls a function to printout characters 1by1; 
 void UART_Put_String ( char *s){
     
     while (*s){
         UART_Put_Character (*s); // pointer to the first element of the string;
         s++;
     }
     
 } //  End UART_Put_String
 
 void NewLine (void){
     UART_Put_Character(13);
     UART_Put_Character(10);    
 } // End NewLine
 
 // UART receive function (using simple buffer no ovf, empty checks)//
 void __Recieve_ISR (void){
     
     t = U1RXREG;
     
     if ((t != '/') && (rx_buffer_index < LENGHT)){
          
         rx_buffer_t[rx_buffer_index] = t;
         rx_buffer_index ++;
         
     }
     else{
         rx_buffer_index = 0;
         UART_Put_String((char *)rx_buffer_t); // Echo back the received character.
     }
     
 } // __Recieve_ISR
 
 
 // *** User menu displaying the available commands/ called via c:m ***//
 // Not visualized by default (flag_o is 0 after reset)
 void SHOW_MENU (void){ 
     
     if(flag_o == 0b1){
                           
          UART_Put_String ((char *)S0);
          NewLine ();
          UART_Put_String ((char *)S1);
          NewLine ();
          UART_Put_String ((char *)S2);
          NewLine ();            
          UART_Put_String ((char *)S3);
          NewLine ();
          UART_Put_String ((char *)S4);
          NewLine ();
          UART_Put_String ((char *)S5);
          NewLine ();
          UART_Put_String ((char *)S9);
          NewLine ();
          UART_Put_String ((char *)S6);
          NewLine ();
          
          flag_o = 0b0; //  Clear the flag (One time executable)

      }//end if  flag_o check
      
      if (flag_p == 0b1){
          
          UART_Put_String ((char *)S7);
          NewLine ();
          UART_Put_String ((char *)S8);
          NewLine ();
          UART_Put_String ((char *)S6);
          NewLine ();
          NewLine ();
          
          flag_p = 0b0;   
      }// End if flag_p check
      
         
      
 }// end SHOW_MENU
 

 void InitMessage (void){
     
    if (flag_i == 0b1) {
        
    __delay32(100);
     NewLine();
    UART_Put_String("___________________________");
    NewLine();
    UART_Put_String("    Loaded successfully");
    NewLine();
    UART_Put_String("___________________________");
    NewLine();
    NewLine();
    NewLine();  
    flag_i = 0b0;
    }
        
 }