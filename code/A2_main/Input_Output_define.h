#ifndef INPUT_OUTPUT_DEFINE_H
#define INPUT_OUTPUT_DEFINE_H

#include <avr/io.h>

#define IO_INPUT   0 // defined IO_INPUT to 0 so it can be used as input
#define IO_OUTPUT  1 // defined IO_OUTPUT to 1 so it can be used as output

#define IO_PORT_B  1 // defined port B as 1
#define IO_PORT_C  2 // defined port C as 2
#define IO_PORT_D  3 // defined port D as 3

void IO_SelectPin(unsigned char port, unsigned char pin);
void IO_SetPinMode(unsigned char mode);

#endif
