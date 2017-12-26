#include "fifo_8.h"
#include "processor.h"
#include "UART.h"

#define size 8

typedef struct{
    
    unsigned int H;
    unsigned int T;
    unsigned char A[size];

}s;

s fifo;

unsigned int ic = 0;

void InitFifo_8 (void){
    
 for (ic = 0; ic < size; ic++){
 fifo.A[ic] = 0;
    }
    
 fifo.H = 0;
 fifo.T =0;
    
}


void fifoin (void){
    
    U1RXREG =  fifo.A[fifo.T];
    fifo.T ++;
    
}


void fifoout (void){
    
    fifo.A[fifo.H] = U1TXREG;
    fifo.H ++;
   
}


unsigned char FIFORECEIVE8 (void){
    
    unsigned char a;
    a = fifo.A[fifo.H];
    fifo.H ++;
    return a;
}

void FIFOSEND8 (unsigned  char  b){
    
    b = fifo.A[fifo.T];
    fifo.T++;
    
}