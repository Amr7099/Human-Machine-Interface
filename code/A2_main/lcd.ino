#include <avr/io.h>
#include <util/delay.h>

#include "lcd.h"

// Send 4 bits to LCD
void LCD_SendNibble(unsigned char Nibble)
{
    unsigned char Data;

    // Map nibble to PD4, PD5, PD6 and PD7
    Data = (Nibble & 0x0F) << 4;

    // Preserve lower PORTD bits
    PORTD = (PORTD & 0x0F) | Data;

    // Enable pulse
    PORTD |= (1 << PD3);
    _delay_us(1);

    PORTD &= ~(1 << PD3);
    _delay_us(50);
}

// Send 8-bit data or command
void LCD_SendByte(unsigned char Data, unsigned char RS_State)
{
    if (RS_State == 1)
    {
        PORTD |= (1 << PD2);
    }
    else
    {
        PORTD &= ~(1 << PD2);
    }

    LCD_SendNibble(Data >> 4);

    LCD_SendNibble(Data & 0x0F);
}

// Send command
void LCD_Command(unsigned char Command)
{
    LCD_SendByte(Command, 0);

    if (Command == 0x01 || Command == 0x02)
    {
        _delay_ms(2);
    }
}

// Write one character
void LCD_WriteChar(unsigned char Character)
{
    LCD_SendByte(Character, 1);
}

// Write a complete string
void LCD_WriteString(const char DataString[])
{
    unsigned int i;

    for (i = 0; DataString[i] != '\0'; i++)
    {
        LCD_WriteChar(DataString[i]);
    }
}

// Clear LCD
void LCD_Clear(void)
{
    LCD_Command(0x01);
}

// Set cursor position
void LCD_SetCursor(unsigned char Row, unsigned char Column)
{
    unsigned char Address;

    if (Row > 1 || Column > 15)
    {
        return;
    }

    if (Row == 0)
    {
        Address = 0x00 + Column;
    }
    else
    {
        Address = 0x40 + Column;
    }

    LCD_Command(0x80 | Address);
}

// Initialize LCD
void LCD_Init(void)
{
    // Configure PD2-PD7 as outputs
    DDRD |= 0xFC;

    // Set RS and Enable LOW
    PORTD &= ~((1 << PD2) | (1 << PD3));

    _delay_ms(40);

    // LCD initialization sequence
    LCD_SendNibble(0x03);
    _delay_ms(5);

    LCD_SendNibble(0x03);
    _delay_us(150);

    LCD_SendNibble(0x03);
    _delay_us(150);

    LCD_SendNibble(0x02);

    // Configure 4-bit mode, 2 lines, 5x8 font
    LCD_Command(0x28);

    // Display ON, cursor OFF
    LCD_Command(0x0C);

    // Cursor increments automatically
    LCD_Command(0x06);

    // Clear display
    LCD_Command(0x01);
}
