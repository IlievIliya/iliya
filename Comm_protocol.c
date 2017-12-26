#define INIT            0
#define READ_INPUT      1
#define PARSE_INPUT     2
#define EXECUTE_CMD     3
#define TX_RESPONSE     4

#define MaxSize         25

#include "Comm_protocol.h"
#include "processor.h"
#include "Ringbuffer.h"
#include "global_data.h"

#include <stdint.h>
#include <stdbool.h>

uint8_t CommState;
uint8_t CommBuff[MaxSize];

void InitCommProtocol (void){
    
CommState = INIT; // Set the state machine to IDLE state. 

}

uint8_t CommProtHandler (uint8_t data){
            
        switch(CommState){
                    
            case INIT: 
            if (data == '*')
                CommState = READ_INPUT;
            else
                CommState = INIT;
            break;
            
            case READ_INPUT:
            if (data != '\r'){  // Carriage return associated with Enter key ~ CR, ASCII value of 13 or 0x0D.
            CircularBuffPush();
            
                CommState = READ_INPUT;
            }
            else 
                CommState = PARSE_INPUT;
//            
//            case PARSE_INPUT:
//                
//            case:EXECUTE_CMD:
//                
//            case:TX_RESPONSE
            
        } // End Switch case
        
      
    
} // End CommProtHandler 

//
//uint8_t CommandParser(uint8_t buff[]){ //Receive the rx_buffer 
//
//    
//                
//}// End command parser

