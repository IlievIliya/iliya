#ifndef GENERICBUFFER
#define	GENERICBUFFER

#include <stdint.h>

//Function declarations//

void SoftwareBuffInit (void); // Initialize the vectors of the software buffer.
void TxInterrupt (void); // Function to serve the transmit subroutine.
void RxInterrupt (void); // Function to serve the receive subroutine.
uint8_t UartGet (void); // Receive a single chharacter.
void UartSend (const uint8_t data); // Transmit a single character.


#endif	// GENERICBUFFER
