#ifndef fifoa
#define fifoa

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


struct rx_fifo{    
    int             size;
    int             wr;
    int             rd;   
    unsigned char   element[];
    //unsigned char   *element;
};

typedef struct rx_fifo fifo;

void UartFifoInit (fifo *f, int size);
_Bool CheckEmpty (fifo * f);
_Bool CheckFull (fifo *f);
void UartFifoPush (fifo * f, unsigned char entry);
unsigned char UartFifoPop (fifo * f);

#endif
        
        