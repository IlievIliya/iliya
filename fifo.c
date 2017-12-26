#include "processor.h"
#include "global_data.h"
#include "fifo.h"

void UartFifoInit (fifo *f, int size){
    
    f->rd = 0;    
    f->wr = 0;    //clear the tail (nothing in the fifo is pushed)
    f->size = size; //set the depth of the fifo
    //f->element = malloc(sizeof(f->element)*size);
}

_Bool CheckEmpty (fifo *f){   
	if (f->rd == f->wr)
		return true;
	else
		return false;
   
}

_Bool CheckFull (fifo *f){
	return false;
}

void UartFifoPush (fifo * f, unsigned char entry){
   
	if (f->wr == f->size)
		f->wr = 0;
	
	if (CheckFull(f) == false){
		f->element[f->wr] = entry;
		f->wr++;
	}
}

unsigned char UartFifoPop (fifo * f){
    
    unsigned char element;
     
	 if (f->rd == f->size)
		 f->rd = 0;
	 
    if (CheckEmpty(f) == true) 
       return 0;
    else{
       element = f->element[f->rd];
		f->rd++;
    }
	return element;
}