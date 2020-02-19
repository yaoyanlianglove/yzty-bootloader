
/******************************************************************************
* @file    $PROJ_DIR$\..\driver\uart\uart_485.c 
* @author  yyl
* @version V1.0
* @date    05/11/2019
* @brief   485串口相关的初始化
*******************************************************************************/ 
#include "uart_485.h"
/*******************************************************************************
** 函数名称: UART_485_GPIO_Init
** 功能描述: 485uart口IO的初始化
** 参    数: None
** 返 回 值: None       
** 作　  者: yyl                   
** 日  　期: 05/11/2019
*******************************************************************************/
void UART_485_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);//使能APB1外设时钟
    RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);//使能APB2外设时钟
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1;           //串口5半双工切换口 
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    _485_DIR_T;  //485初始化为发送状态
}
/*******************************************************************************
** 函数名称: UART_485_Setup
** 功能描述: 485uart口参数配置
** 参    数: None
** 返 回 值: None       
** 作　  者: yyl                   
** 日  　期: 05/11/2019
*******************************************************************************/
void UART_485_Setup(void)
{ 
    USART_InitTypeDef USART_InitStructure;				//串口参数
    USART_InitStructure.USART_BaudRate            = 9600; 
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
    USART_InitStructure.USART_Parity              = USART_Parity_No; 
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;	
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;		
    USART_Init(UART5, &USART_InitStructure);
        
    USART_ClearFlag(UART5, USART_FLAG_TC|USART_FLAG_TXE|USART_FLAG_RXNE);
    USART_Cmd(UART5, ENABLE);
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
    USART_SendData(UART5, c);
    while (USART_GetFlagStatus(UART5, USART_FLAG_TXE) == RESET)
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
    _485_DIR_T;
    Delay_us(200);
    while (*s != '\0')
    {
        SerialPutChar(*s);
        s++;
    }
    Delay_us(1000);
    _485_DIR_R;
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
    if ( USART_GetFlagStatus(UART5, USART_FLAG_RXNE) != RESET)
    {
        *key = (u8)UART5->DR;
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


