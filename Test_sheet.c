#include "Test_sheet.h"
#include "processor.h"

typedef struct BUFFER {
    
    unsigned char *rx_buffer;
    unsigned int head;
    unsigned int tail;
    unsigned int size;
    
}ringbuffer;

 ringbuffer rx_buff;

void BUFFER_INIT (unsigned char *buff, unsigned int size ){
    
    rx_buff.head = 0;
    rx_buff.tail = 0;
    rx_buff.rx_buffer = *buff;
    rx_buff.size = (int)size;
    
}

unsigned char PUSH (unsigned char *buff, unsigned count){
    
unsigned int k;
unsigned char *p;

p = buff; // point to the array;

for (k=0; k<count; k++){
    if (rx_buff.head != rx_buff.tail){ // Condition for available data;
    }
    
    
    
}


}