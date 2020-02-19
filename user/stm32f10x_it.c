
/*  ******************************************************************************
  * @file    ADC/3ADCs_DMA/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************

  ******************************************************************************
  */ 

#include "stm32f10x_it.h"

/*******************************************************************************
* Function Name  : NMI_Handler
* Description    : This function handles NMI exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NMI_Handler(void)
{
}


/*******************************************************************************
* Function Name  : HardFault_Handler
* Description    : This function handles Hard Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1)
    {
      break;
    }
}
/*******************************************************************************
* Function Name  : MemManage_Handler
* Description    : This function handles Memory Manage exception
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void MemManage_Handler(void)
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while (1)
    {
      break;
    }
}
/*******************************************************************************
* Function Name  : BusFault_Handler
* Description    : This function handles Bus Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void BusFault_Handler(void)
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while (1)
    {
      break;
    }
}
/*******************************************************************************
* Function Name  : UsageFault_Handler
* Description    : This function handles Usage Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UsageFault_Handler(void)
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while (1)
    {
      break;
    }
}
/*******************************************************************************
* Function Name  : SVCHandler
* Description    : This function handles SVCall exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SVC_Handler(void)
{
}
/*******************************************************************************
* Function Name  : DebugMonitor
* Description    : This function handles Debug Monitor exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DebugMon_Handler(void)
{
}
/*******************************************************************************
* Function Name  : PendSVC
* Description    : This function handles PendSVC exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void PendSV_Handler(void)
{
}

/*******************************************************************************
* Function Name  : SysTick_Handler
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void SysTick_Handler(void)
{ 
}
/******************************************************************************/
/*            STM32F10x Peripherals Interrupt Handlers                        */
/******************************************************************************/

/*******************************************************************************
* Function Name  : ADC1_2_IRQHandler
* Description    : This function handles ADC global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ADC1_2_IRQHandler(void)
{      
//  irq_adc_stm32();
}
/*******************************************************************************
* Function Name  : ADC3_IRQHandler
* Description    : This function handles ADC global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ADC3_IRQHandler(void)
{   
}
/*******************************************************************************
* Function Name  : USART1_IRQHandler
* Description    : 
* Input          : None
* Output         : None
* Return         : None
******************************************************************************/

void USART1_IRQHandler(void)
{   
}
/*******************************************************************************
* Function Name  : USART3_IRQHandler
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART3_IRQHandler(void)
{
}
/*******************************************************************************
* Function Name  : UART4_IRQHandler
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
//u16 wifi_send_count = 0;
//u16 wifi_send_length = 0;
//u8 wifi_send_data[128] = {0};
void UART4_IRQHandler(void)
{
    //if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)      
    //{
    //    USART_ClearITPendingBit(UART4,USART_IT_RXNE);           
    //    WIFI_Comm_Process(USART_ReceiveData(UART4)); 
    //}
    //if(USART_GetITStatus(UART4, USART_IT_TC) != RESET)       
    //{       
    //    USART_ClearITPendingBit(UART4,USART_IT_TC);           
    //    if(wifi_send_count < wifi_send_length)       
    //    {
    //        wifi_send_data[wifi_send_count] = wifi_send_count;
    //        USART_SendData(UART4, wifi_send_data[wifi_send_count]); 
    //        wifi_send_count++;
    //    }
    //    else                             
    //    {
    //        USART_ITConfig(UART4, USART_IT_TC, DISABLE);     
    //        USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);     
    //        wifi_send_count = 0;
    //    }
    //} 
}
/*******************************************************************************
* Function Name  : UART5_IRQHandler
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UART5_IRQHandler(void)
{
}
/*******************************************************************************
* Function Name  : TIM2_IRQHandler
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM1_IRQHandler(void)
{
 // TIM_ClearITPendingBit(TIM2,TIM_FLAG_Update);  
 // DataFrame_EndReceive(TIM2,&COM3_ReceiveData);
}
/*******************************************************************************
* Function Name  : TIM2_IRQHandler
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM2_IRQHandler(void)
{   

}
/*******************************************************************************
* Function Name  : TIM4_IRQHandler
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM4_IRQHandler(void)
{
}
/*******************************************************************************
* Function Name  : TIM5_IRQHandler
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM5_IRQHandler(void)
{
}
/*******************************************************************************
* Function Name  : DMA_IRQHandler
* Description    : DMA
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel2_IRQHandler(void)
{  
          
}
/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/*******************************************************************************
* Function Name  : This function handles PPP interrupt request.
* Description    : None.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/ 
void EXTI9_5_IRQHandler(void)
{
  
}



/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

