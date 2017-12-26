#include "GenericBuffer.h"
#include "GenericBufferDebugger.h"
#include "processor.h"
#include "UART.h"

#include <stdint.h>
#include <stdbool.h>
#include <libpic30.h>

void Echo (void){
    
   volatile uint8_t temp = 0;
    
    temp = UartGet ();
    
    while (U1STAbits.UTXBF != 0);
    UartSend (temp);

}

    
