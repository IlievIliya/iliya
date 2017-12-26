#ifndef UART_BUFFER
#define	UART_BUFFER

extern unsigned char BUFFER_FULL;
extern unsigned char BUFFER_READY;
extern unsigned char BUFFER_EMPTY;

#define BUFFER_SIZE  128

 typedef struct Buffer {
     
    unsigned char data[BUFFER_SIZE];
    unsigned char newest_index;
    unsigned char oldest_index;
    
} buff;


void bufferWrite(buff *buffer, unsigned char byte);
void bufferRead(buff *buffer, unsigned char *byte);



#endif	// UART_BUFFER

