#ifndef UART_H
#define UART_H

void Uart_Init(void);

void Uart_SetBaudRate(unsigned long BaudRate);

void Uart_SendChar(unsigned char DataByte);

void Uart_SendString(const char DataString[]);

unsigned char Uart_DataAvailable(void);

unsigned char Uart_ReadData(void);

#endif