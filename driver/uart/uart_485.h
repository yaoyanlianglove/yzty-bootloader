#ifndef __UART_485_H
#define __UART_485_H
#include "stm32f10x.h"
#define SerialPutString(x) Serial_PutString((u8*)(x))

#define  _485_DIR_T GPIO_SetBits(GPIOD, GPIO_Pin_1)
#define  _485_DIR_R GPIO_ResetBits(GPIOD, GPIO_Pin_1)

void SerialPutChar(u8 c);
u32  SerialKeyPressed(u8 *key);
void Delay_us(u16 us);
void Serial_PutString(u8 *s);
u8   GetKey(u32 timeout);
void UART_485_GPIO_Init(void);
void UART_485_Setup(void);
#endif
