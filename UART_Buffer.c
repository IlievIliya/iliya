#include "UART_Buffer.h"
#include "processor.h"
#include <stdint.h>


// Flags used to indicate the status of the ring buffer.
unsigned char BUFFER_FULL = 0;
unsigned char BUFFER_READY = 0;
unsigned char BUFFER_EMPTY = 0;


/// --- The function receives pointer to an array and the value which will be
/// --- written in the buffer.
/// --- Updates FULL and READY statuses.
/// --- The data is arranged into a circular buffer with auto roll over (line 16)
 void bufferWrite(buff *buffer, unsigned char byte){
     
    unsigned char  next_index = (((buffer->newest_index)+1) % BUFFER_SIZE);
    // Updates the index of the array to the next available position.
 
    if (next_index == buffer->oldest_index){
        BUFFER_FULL = 1; // No available space in the buffer 
    }
 
    buffer->data[buffer->newest_index] = byte; // Input the new data
    buffer->newest_index = next_index;
    BUFFER_READY = 1; // Indicates that there is available data in the buffer
       
} // End write.
 
/// --- Receives pointer to the array and a pointer to 8 bit variable.
/// --- Extracts the oldest elements from the ring buffer  
void bufferRead(buff *buffer, unsigned char *byte){
 
    if (buffer->newest_index == buffer->oldest_index){
         BUFFER_EMPTY = 1; // No data in the ring buffer.
    }
    
    *byte = buffer->data[buffer->oldest_index]; // 
    buffer->oldest_index = ((buffer->oldest_index+1) % BUFFER_SIZE); // Updates 
    // the index of the array and rolls it over.
    
    BUFFER_READY = 1; // Indicates that the read operation was successful and the 
                      // byte value has been updated.
     
} // End Read.
       