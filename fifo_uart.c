#include "processor.h"
#include "global_data.h"
#include "fifo_uart.h"

void UartFifoInit (fifo *f, int size){
    
    f->count = 0;
    f->start = 0;
    f->size = size; 
    f->element = malloc(sizeof(f->element)*size);
}

_Bool CheckEmpty (fifo * f){   
    
    if (f->count == 0) 
        return true;
    else 
        return false;
   
}

_Bool CheckFull (fifo *f){
   
     if (f->count == f->size) 
        return true;
     else 
        return false;
 
}

void UartFifoPush (fifo * f, unsigned char entry){
    
    int index;
    
    if (CheckFull(f) == false)
        index = f->start + f->count++;
        if (index >= f->size) 
            index = 0;
        
    f->element[index] = entry;
    
    
}

unsigned char UartFifoPop (fifo * f){
    
    unsigned char element;
     
    if (CheckEmpty(f) == true) 
       return 0;
    else{
       element = f->element[f->start];
       f->start++;
       f->count--;
       if (f->start == f->size) 
           f->start = 0;
       return element;
    }
}


