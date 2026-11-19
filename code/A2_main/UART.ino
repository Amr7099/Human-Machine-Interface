#include <avr/io.h>
#include "uart.h"  

void Uart_Init(void)
{
    Uart_SetBaudRate(9600); // set baud rate to 9600

    UCSR0B = (1 << TXEN0) | (1 << RXEN0);  // enable transimt and receive bits

    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // enable 8 data bits and 1 stop bit
}

void Uart_SetBaudRate(unsigned long BaudRate)
{
    unsigned long UBRR = ((F_CPU / 16UL) / BaudRate) - 1UL;

    UBRR0L = (unsigned char)(UBRR & 0xFF); // store the lower 8 bits of UBRR in the low register and shift UBRR right by 8 bits, then store its upper 8 bits so we can have 16 bits
    UBRR0H = (unsigned char)((UBRR >> 8) & 0xFF);
}

void Uart_SendChar(unsigned char DataByte)
{
    while (!(UCSR0A & (1 << UDRE0))); // wait until the UART data register is ready for new data

    UDR0 = DataByte; // write the character to the data register
}

void Uart_SendString(const char DataString[])
{
    unsigned int i;
    for (i = 0; DataString[i] != '\0'; i++)
    {
        Uart_SendChar(DataString[i]);
    }
}

unsigned char Uart_DataAvailable(void) // check if a character has been received through uart
{
    return (UCSR0A & (1 << RXC0)) != 0; // return 1 if data is available in the uart and 0 if not
}

unsigned char Uart_ReadData(void)
{
    while (!(UCSR0A & (1 << RXC0))); 

    return UDR0; // read the received character from the UART data register.
}