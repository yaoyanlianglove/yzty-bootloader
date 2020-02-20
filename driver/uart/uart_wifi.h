#ifndef __UART_WIFI_H
#define __UART_WIFI_H
#include "stm32f10x.h"
#define SerialPutString(x) Serial_PutString((u8*)(x))

void SerialPutChar(u8 c);
u32  SerialKeyPressed(u8 *key);
void Delay_us(u16 us);
void Serial_PutString(u8 *s);
u8   GetKey(u32 timeout);
void UART_WIFI_GPIO_Init(void);
void UART_WIFI_Setup(void);
#endif
