//Corresponds to the length of the software receive and transmit buffers. (Currently 8 bytes)

#define TX_BUFFER_SIZE 8 
#define RX_BUFFER_SIZE 8

#include "processor.h"
#include "generibuffer.h"
#include "UART.h"
#include "global_data.h"


#include <stdint.h>

//Union defining the status of the buffer corresponding to 8 bit word format.

////***Implemented as union type since not all the elements will be in simultaneous usage.***////

typedef union {
        
    struct {
        uint8_t full:1; //Single bit usage, can be interpreted as a flag.
        uint8_t empty:1; //Single bit usage, can be interpreted as a flag.
        uint8_t reserved:6; //Six bits reserved.
    }s;
    
    uint8_t status;
    
}UART_STATUS; // Declaration of an object used to monitor the status of the buffers.

//Structure defining the components of the buffer.

typedef struct{
    // Receive buffer pointers assigning input/output of the software receive buffer
    
    uint8_t    *rxTail; //Correspondent to the oldest element in the buffer.
    uint8_t    *rxHead; // Correspondent to the newest element in the buffer.
    
    // Transmit buffer pointers assigning input/output of the software transmit buffer.
    
    uint8_t    *txTail; //Correspondent to the oldest element in the buffer.
    uint8_t    *txHead; // Correspondent to the newest element in the buffer.
    
    //Variables used to monitor the status of the buffer.
    UART_STATUS ReceiveStatus; 
    UART_STATUS TransmitStatus;
    
}UART_DATA;

UART_DATA uart_data;

uint8_t UartRxBuff[RX_BUFFER_SIZE];
uint8_t UartTxBuff[TX_BUFFER_SIZE];


