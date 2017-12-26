#include "Communication_.h"
#include "processor.h"
#include "PrintOutput.h"
#include "global_data.h"
#include "Commutation_Logic.h"
#include "Quadrature_Encoder.h"

#include <stdio.h>

///*** Initialization(variables) ***//
int temp = 0;   
int parameter[4] = {0,0,0,0}; // Array used to parse the input parameters
unsigned char data_d = 0;   

unsigned char Comm_State;
unsigned char COM; // Secondary comands state reg.

void InitComm (void){
    
     Comm_State = WAIT_HEADER;
     COM = WAITSTART;
    
}

void Comm (unsigned char data){
    
    switch (Comm_State){
        
        /// *** HEADER BLOCK ***//
        case WAIT_HEADER:
            if (data == 'c'){
                Comm_State = CONFIRM_HEADER;
            }else {
                Comm_State = WAIT_HEADER;
            }   
            break;
        
        case CONFIRM_HEADER:
            if (data == ':'){
                Comm_State = Comm_Distribution;
            }else{
                Comm_State = INVALID;  
            }
            break;
        /// ***END HEADER BLOCK ***//
            
        /// *** Command Distribution ***//   
        case Comm_Distribution:   
                  if (data == 't'){
                Comm_State = TMRconf;
            }else if (data == 'p'){
                Comm_State = PWMHconf0;
            }else if (data == 'm'){
                Comm_State = MCconf;
            }else if (data == 'd'){
                Comm_State = DISPLAY;
            }else if (data == '?'){
                Comm_State = MENU;                
            }else {
                Comm_State = INVALID;  
            }
            break;
         /// ***End Command Distribution ***//  
            

            
        ///*** PWM_Config ***///  
        case PWMHconf0:
            if (data == ' '){
                Comm_State = PWMHconf1;
            }else if (data == '?'){
                Comm_State = PWMOPTIONS;                
            }else{    
                Comm_State = INVALID;
            }
            break;
            
        case PWMHconf1:
            if (data == '='){
                Comm_State = RECEIVE1;
            }else {
                Comm_State = INVALID;
            }
            break;
            
            ///*** Parameters parsing ***//
        case RECEIVE1:
                if (((data >= 0x0030) && (data <= 0x0039)) || ((data >= 0x0040) && (data <= 0x0046))){
                
                Comm_State = RECEIVE2;
            
                data_d = EvaluateHEX (data);
               
                parameter[3] = data_d;
                parameter[3] =  parameter[3] << 12;
                
            }else {
                Comm_State = INVALID;
            }        
            break;
            
            case RECEIVE2:
            if (((data >= 0x0030) && (data <= 0x0039)) || ((data >= 0x0040) && (data <= 0x0046))){
                
                 Comm_State = RECEIVE3;
            
                data_d = EvaluateHEX (data);
                
                parameter[2] = data_d;
                parameter[2] =  parameter[2] << 8;
                
            }else {
                Comm_State = INVALID;
            }        
            break;
            
        case RECEIVE3:
            if (((data >= 0x0030) && (data <= 0x0039)) || ((data >= 0x0040) && (data <= 0x0046))){
                
                Comm_State = RECEIVE4;
                
                data_d = EvaluateHEX (data);

                parameter[1] = data_d;
                parameter[1] =  parameter[1] << 4;
                
            }else {
                Comm_State = INVALID;
            }        
            break;
                
            
        case RECEIVE4:
            if (((data >= 0x0030) && (data <= 0x0039)) || ((data >= 0x0040) && (data <= 0x0046))){

                Comm_State = UPDATE;
            
                data_d = EvaluateHEX (data);
                
                parameter[0] = data_d;
                            
            }else {
            
               Comm_State = INVALID;
            }        
            break;
            ///***End Parameters parsing***//
            
            ///*** UPDATE parameters ***//
        case UPDATE:
            if (data == 'D'){
                
                Comm_State = WAIT_HEADER;
                
                temp = (parameter [3] | parameter [2] | parameter[1] | parameter[0]);
                P2DC1 = temp;
                                
                UART_Put_String ("DC_Updated.");   
                NewLine();
              //  printf("%d", P2DC1);
                NewLine();
                
            }else if (data == 'P'){
                Comm_State = WAIT_HEADER;
                
                temp = (parameter [3] | parameter [2] | parameter[1] | parameter[0]);
                P2TPER = temp;
                
                UART_Put_String ("Period_Updated.");         
                NewLine();
                
            }else if (data == 'T'){
                
                Comm_State = TMR1_2;
                
            }else{
                Comm_State = INVALID;
            }
            break;  
            ///***End UPDATE parameters ***//
                   
            ///***End PWM_Config ***///  
         
        case TMR1_2:    
                if (data == '1'){
                    
                Comm_State = WAIT_HEADER;
                temp = (parameter [3] | parameter [2] | parameter[1] | parameter[0]);
                PR1 = temp;
                
                UART_Put_String ("TMR1_Period_Updated.");         
                NewLine();
                
            }else if (data == '2'){
                
                Comm_State = WAIT_HEADER;
                temp = (parameter [3] | parameter [2] | parameter[1] | parameter[0]);
                PR2 = temp;
                
                UART_Put_String ("TMR2_Period_Updated.");         
                NewLine();
                
            }else{
                 Comm_State = INVALID;
            }
            break;
            
        case MCconf:
                  if (data == 'R'){
                
                Comm_State = CCW_ROTATION;
                
            }else if (data == 'L'){
                
                Comm_State = CW_ROTATION;
                
            }else if (data == 'B'){
                
                Comm_State = BREAK;
                
            }else if (data == 'A'){
                
                Comm_State = AUTO_DC;
                
             }else if (data == 'S'){
              
                Comm_State = EMERGENCYSTOP;
                
                
             }else if (data == 'P'){
             
                 Comm_State = PROGRAM;
                 
            }else  {
                 
                 Comm_State = INVALID;
            }
            break;
            
 
            
        case PROGRAM:
                  if (data == '1'){
                      
                 Comm_State = PROGRAM1;
                
            }else {
                 Comm_State = INVALID;  
            }
             break;
        
        case DISPLAY:       
                  if(data == '+'){
                      
                Comm_State = DISPLAYON;
                
            }else if(data == '-'){
                
                Comm_State = DISPLAYOFF;
                
            }else{
                Comm_State = INVALID;
            }
            break;
            
        case PROGRAM1:     if (data == ';'){Comm_State = WAIT_HEADER; 
                            PROGRAMEXE();
        }else Comm_State = INVALID;break;
        
        case EMERGENCYSTOP:if (data == ';'){Comm_State = WAIT_HEADER;
                            EMERGENCYSTOPEXE();
        }else Comm_State = INVALID;break;
        
        case AUTO_DC:      if (data == ';'){Comm_State = WAIT_HEADER;
                            AUTOEXE ();
        }else Comm_State = INVALID;break;
        
        case BREAK:        if (data == ';'){ Comm_State = WAIT_HEADER;
                             BREAKEXE();
        }else Comm_State = INVALID;break;
        
        case CW_ROTATION:  if (data == ';'){ Comm_State = WAIT_HEADER;
                             CWEXE ();
        }else Comm_State = INVALID;break;
        
        case CCW_ROTATION: if (data == ';'){ Comm_State = WAIT_HEADER;
                             CCWEXE ();
        }else Comm_State = INVALID;break;
        
        case MENU:         if (data == ';'){ Comm_State = WAIT_HEADER;
                             flag_o = 0b1; // Display Menu
        }else Comm_State = INVALID;break;
        
        case DISPLAYON:    if (data == ';'){ Comm_State = WAIT_HEADER;
                             print = 0b1;
        }else Comm_State = INVALID;break;
        
        case DISPLAYOFF:   if (data == ';'){ Comm_State = WAIT_HEADER;
                             print = 0b0;
        }else Comm_State = INVALID;break;
        
        case PWMOPTIONS:   if (data == ';'){ Comm_State = WAIT_HEADER;
                             flag_p = 0b1; // Display PWM options;
        }else Comm_State = INVALID;break;
            
        case INVALID:{
              Comm_State  = WAIT_HEADER;
                             UART_Put_String("INVALID COMMAND!");
                             NewLine();
        }break;
        
        default:Comm_State = WAIT_HEADER;
        
    } // End Switch
    
} //  End Comm


void SPECIFIC_CM (unsigned char data){
    
    switch (COM){
        
        case WAITSTART:
            if (data == '*'){
            COM = CLEARAWINDOW;                
            }else if (data == 'M'){
            COM = MOD;
            }else {
            COM = WAITSTART;
            }
            break;
            
        case MOD:
            if (data == '+'){
                
                COM = WAITSTART;
                Modulation = 0b1;
                UART_Put_String ("Mod enabled.");
                NewLine();
                
            }else if (data == '-'){
                
                COM = WAITSTART;
                Modulation = 0b0;
                UART_Put_String ("Mod disabled.");
                NewLine();
                
            }else{
                COM = INVALIDCOM;
            }
            break;
            
        case CLEARAWINDOW:
            if (data == 'C'){
                COM = WAITSTART;
                clearscreen = 0b1;
                
            }else {
                COM = INVALIDCOM;
            }
            break;
            
        case INVALIDCOM:{
               COM = WAITSTART;
               UART_Put_String("INVALID COMMAND!");
               NewLine();
        }break;

            
       default:COM = WAITSTART;

            
    }
            
    
}

void CLEARSCREEN (void){
    
    if (clearscreen == 0b1){
    for (index_clear_screen = 0; index_clear_screen <20 ;index_clear_screen++){
        
         NewLine();    
         
        if (index_clear_screen == 19){
            clearscreen = 0b0;
        }
    }
    
    }// End if (clear screen flag check)
    
}// End ClearScreen

void PROGRAMEXE (void){ 
    
                 InitReverseDirection();
              //   P2DC1          = 0x00F0;
                 
                 Counter        = 0;
                 Reverse        = 0b1;
                 AutoDC         = 0b0;
                 Break          = 0b0;      
                 
                 // Enable CL
                 LATBbits.LATB5 = 0b0;
                 
                UART_Put_String ("Executing Program1");   
                NewLine();
}


void EMERGENCYSTOPEXE(void){
    
                Reverse     = 0b0;
                AutoDC      = 0b0;
                P2DC1       = 0;
                SoftStart   = 0b0;
                Modulation  = 0b0;
                Break       = 0b0;
                
                // Disable CL
                LATBbits.LATB5 = 0b1;
                
                UART_Put_String ("The motor is stopped.");   
                NewLine();
}

void AUTOEXE (void){
    
                // Clear break flag
                Break      = 0b0;
                // Disable Reverse function
                Reverse    = 0b0;
                //Set auto flag
                AutoDC     = 0b1;
                
                UART_Put_String ("Auto enabled.");
                NewLine();
}

void BREAKEXE (void){
    
                // Auto flag cleared
                AutoDC     = 0b0;
                // Disable Reverse function
                Reverse    = 0b0;
                // Disable Modulations
                Modulation = 0b0;
                // Set Break flag
                Break      = 0b1;
                       
                UART_Put_String ("Breaking.");
                NewLine();
    
}

void CWEXE (void){
    
                Reverse = 0b0;
                CW();
                
                UART_Put_String ("Currently CCW spinning.");
                NewLine();
}

void CCWEXE (void){
    
                Reverse = 0b0;                
                CCW();   
                
                UART_Put_String ("Currently CW spinning.");
                NewLine();
    
}

unsigned char EvaluateHEX (unsigned char symbol ){
    
    unsigned char hex;
    
    if (symbol == '0'){
        hex = 0b0000;
    }else if (symbol == '1'){
        hex = 0b0001;
    }else if (symbol == '2'){
        hex = 0b0010;
    }else if (symbol == '3'){
        hex = 0b0011;
    }else if (symbol == '4'){
        hex = 0b0100;
    }else if (symbol == '5'){
        hex = 0b0101;
    }else if (symbol == '6'){
        hex = 0b0110;
    }else if (symbol == '7'){
        hex = 0b0111;
    }else if (symbol == '8'){
        hex = 0b1000;
    }else if (symbol == '9'){
        hex = 0b1001;
    }else if (symbol == 'A'){
        hex = 0b1010;
    }else if (symbol == 'B'){
        hex = 0b1011;
    }else if (symbol == 'C'){
        hex = 0b1100;
    }else if (symbol == 'D'){
        hex = 0b1101;
    }else if (symbol == 'E'){
        hex = 0b1110;
    }else if (symbol == 'F'){
        hex = 0b1111;
    }
    
    return hex;
}