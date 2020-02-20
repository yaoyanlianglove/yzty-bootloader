
/******************************************************************************
* @file    $PROJ_DIR$\..\driver\uart\uart_485.c 
* @author  yyl
* @version V1.0
* @date    02/20/2020
* @brief   WIFI串口相关的初始化
*******************************************************************************/ 
#include "uart_wifi.h"
/*******************************************************************************
** 函数名称: UART_WIFI_GPIO_Init
** 功能描述: WIFIuart口IO的初始化
** 参    数: None
** 返 回 值: None       
** 作　  者: yyl                   
** 日  　期: 02/20/2020
*******************************************************************************/
void UART_WIFI_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);//使能APB1外设时钟
    RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOC, ENABLE);//使能APB2外设时钟

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

}
/*******************************************************************************
** 函数名称: UART_WIFI_Setup
** 功能描述: WIFIuart口参数配置
** 参    数: None
** 返 回 值: None       
** 作　  者: yyl                   
** 日  　期: 02/20/2020
*******************************************************************************/
void UART_WIFI_Setup(void)
{ 
    USART_InitTypeDef USART_InitStructure;				//串口参数
    USART_InitStructure.USART_BaudRate            = 9600; 
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
    USART_InitStructure.USART_Parity              = USART_Parity_No; 
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;	
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;		
    USART_Init(UART4, &USART_InitStructure);
        
    USART_ClearFlag(UART4, USART_FLAG_TC|USART_FLAG_TXE|USART_FLAG_RXNE);
    USART_Cmd(UART4, ENABLE);
}

/*******************************************************************************
** 函数名称: Delay_us
** 功能描述: 延时函数
** 参    数: None
** 返 回 值: None       
** 作　  者: yyl                   
** 日  　期: 09/12/2019
*******************************************************************************/
void Delay_us(u16 us) 
{ 
    u16 i=0;
    while(us--)
    {
        i=9;
        while(i--);
    }
}
/*******************************************************************************
** 函数名称: SerialPutChar
** 功能描述: 输出字符到上位机
** 参    数: None
** 返 回 值: None       
** 作　  者: yyl                   
** 日  　期: 09/12/2019
*******************************************************************************/
void SerialPutChar(u8 c)
{
    USART_SendData(UART4, c);
    while (USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET)
    {

    }
}

/*******************************************************************************
** 函数名称: Serial_PutString
** 功能描述: 输出字符串到上位机
** 参    数: None
** 返 回 值: None       
** 作　  者: yyl                   
** 日  　期: 09/12/2019
*******************************************************************************/
void Serial_PutString(u8 *s)
{
    while (*s != '\0')
    {
        SerialPutChar(*s);
        s++;
    }
}

/*******************************************************************************
** 函数名称: SerialKeyPressed
** 功能描述: 读上位机命令函数
** 参    数: None
** 返 回 值: None       
** 作　  者: yyl                   
** 日  　期: 09/12/2019
*******************************************************************************/
u32 SerialKeyPressed(u8 *key)
{
    if ( USART_GetFlagStatus(UART4, USART_FLAG_RXNE) != RESET)
    {
        *key = (u8)UART4->DR;
        return 1;
    }
    else
    {
        return 0;
    }
}

/*******************************************************************************
** 函数名称: GetKey
** 功能描述: 读上位机命令函数
** 参    数: timeout 超时时间
** 返 回 值: None       
** 作　  者: yyl                   
** 日  　期: 09/12/2019
*******************************************************************************/
u8 GetKey(u32 timeout)
{
    uint8_t key = 0;

  /* Waiting for user input */
    while (timeout-- > 0)
    {
        if (SerialKeyPressed((uint8_t*)&key)) break;
    }
    return key;
}


