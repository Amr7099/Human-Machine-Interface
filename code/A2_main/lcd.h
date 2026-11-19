#ifndef LCD_H
#define LCD_H

void LCD_Init(void);

void LCD_Command(unsigned char Command);

void LCD_WriteChar(unsigned char Character);

void LCD_WriteString(const char DataString[]);

void LCD_Clear(void);

void LCD_SetCursor(unsigned char Row, unsigned char Column);

#endif
