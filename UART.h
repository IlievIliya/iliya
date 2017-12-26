#ifndef UART
#define UART

#include "UART_Buffer.h"


void InitUART(unsigned char frame, unsigned char trailer,  int BAUD_rate );

extern unsigned char received;

#endif



