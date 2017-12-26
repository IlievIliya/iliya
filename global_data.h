#ifndef global_data
#define global_data

#include <stdint.h>

// *** UART labels ***///

#define bits9_no_p      0b11
#define bits8_odd_p     0b10
#define bits8_even_p    0b01
#define bits8_no_p      0b00

#define stop_b2         0b1
#define stop_b1         0b0

#define BRG9600         259  // Fcy 40Mhz osc, 9600 Baud U1BRG = 259
#define BRG14400        172  // Fcy 40Mhz osc, 14400 Baud U1BRG = 172       
#define BRG19200        129  // Fcy 40Mhz osc, 19200 Baud U1BRG = 129
#define BRG28800        86   // Fcy 40Mhz osc, 28800 Baud U1BRG = 86
#define BRG38400        64   // Fcy 40Mhz osc, 38400 Baud U1BRG = 64
#define BRG57600        42   // Fcy 40Mhz osc, 38400 Baud U1BRG = 42
#define BRG115200       20   // Fcy 40Mhz osc, 38400 Baud U1BRG = 20

// ******///


///*** TMR Labels ***///

#define PRESCALER_1to1     0b00
#define PRESCALER_1to8     0b01
#define PRESCALER_1to64    0b10
#define PRESCALER_1to256   0b11


///*** PWM Labels ***///

#define FreeRunning        0b00
#define SingleEvent        0b01
#define UP_DOWN            0b10
#define DOUBLE_Update_DC   0b11

#define Complementary      0b00
#define Independent        0b01



extern unsigned int cntr;
extern unsigned char index_clear_screen;
extern unsigned char clearscreen;

#endif



