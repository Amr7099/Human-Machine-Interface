#include "Input_Output_define.h"  

unsigned char selected_port;
unsigned char selected_pin;


void IO_SelectPin(unsigned char port, unsigned char pin) // this function is used to declare which port and which pin i used
{
    selected_port = port;
    selected_pin = pin;
}

void IO_SetPinMode(unsigned char mode) // this function is used to set the pin mode to either ouput or input
{
    if (selected_port == IO_PORT_B) // check if the selected port is port B.
    {
        if (mode == IO_OUTPUT) // if mode is set to output which we defined as 1, it will make the pin an output pin
        {
            DDRB |= (1 << selected_pin); // set the selected pin to output without affecting the rest of the pins
        }
        else
        {
            DDRB &= ~(1 << selected_pin); // if else then set it to 0 with is input
        }
    }

    else if (selected_port == IO_PORT_C) // do the same but if it is port C
    {
        if (mode == IO_OUTPUT)
        {
            DDRC |= (1 << selected_pin);
        }
        else
        {
            DDRC &= ~(1 << selected_pin);
        }
    }

    else if (selected_port == IO_PORT_D) // do the same but if it is port D
    {
        if (mode == IO_OUTPUT)
        {
            DDRD |= (1 << selected_pin);
        }
        else
        {
            DDRD &= ~(1 << selected_pin);
        }
    }
}