/******************************************************************************
* @file    $PROJ_DIR$\..\user\main.c 
* @author  yyl
* @version V1.0
* @date    07/26/2019
* @brief   bootloader主程序
*******************************************************************************/ 
#include "uart_wifi.h"
#include "common.h"

int main(void)
{    
    u8 StartCode = 0;
    u8 wait_time = 5;
    SystemInit();
   /*UART config*/
    UART_WIFI_GPIO_Init();
    UART_WIFI_Setup();
    __disable_irq();    //关闭所有中断
    SerialPutString("\r\n--------------bootloader start.please enter 'U' to download-------------\r\n");
    while(wait_time -- >0)
    {
        if(GetKey(100000) == 'U')
        {
            wait_time = 0;
            SerialPutString("download start\r\n");
            FLASH_Unlock();
            if(FLASH_DisableWriteProtectionPages() == 0) //没有写保护
            {
                SerialDownload();
            }
            FLASH_Lock();
        }
        else
            SerialPutString("S\r\n");
    }
    
    StartCode = FLASH_ReadOneByte(START_APP_ADDR);

    if(StartCode == 'B')
        GotoAPP('B');
    else 
        GotoAPP('A');          
    while(1)
    {
        SerialPutString("no app to boot. system reset\r\n");
        NVIC_SystemReset();
    }
}


