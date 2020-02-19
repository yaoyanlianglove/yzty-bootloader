#include "common.h"
#include "uart_485.h"
#include "ymodem.h"

extern u8 file_name[FILE_NAME_LENGTH];

extern u16 PageSize;
extern FLASH_Status FLASHStatus;
u8 tab_1024[1024] ={0};
/*******************************************************************************
** 函数名称: Erase_Start_Slect_Area
** 功能描述: 擦除启动选择区
** 参    数: None
** 返 回 值: None       
** 作　  者: yyl                   
** 日  　期: 09/12/2019
*******************************************************************************/
void Erase_Start_Slect_Area(void)
{
    if(FLASHStatus == FLASH_COMPLETE)
        FLASHStatus = FLASH_ErasePage(SART_APP_ADDR);
}
/*******************************************************************************
** 函数名称: Write_B_To_Start_Slect_Area
** 功能描述: 写启动选择区
** 参    数: None
** 返 回 值: None       
** 作　  者: yyl                   
** 日  　期: 09/12/2019
*******************************************************************************/
void Write_B_To_Start_Slect_Area(void)
{
    u8 data = 'B';
    if(FLASHStatus == FLASH_COMPLETE)
        FLASHStatus = FLASH_ProgramWord(SART_APP_ADDR, (u32)data);
}
/*******************************************************************************
** 函数名称: SerialDownload
** 功能描述: 程序下载函数
** 参    数: None
** 返 回 值: None       
** 作　  者: yyl                   
** 日  　期: 09/12/2019
*******************************************************************************/
void SerialDownload(void)
{
    u8 Number[10] = "          ";
    int32_t Size = 0;
    if(FLASHStatus == FLASH_COMPLETE)
        Erase_Start_Slect_Area();
    SerialPutString("waiting for the file to be sent ... (press 'a' to abort)\n\r");
    Size = Ymodem_Receive(&tab_1024[0]);
    if (Size > 0)
    {
        Write_B_To_Start_Slect_Area();
        Delay_us(10000);
        SerialPutString("\n\n\r programming completed successfully!\n\r--------------------------------\r\n name: ");
        SerialPutString(file_name);
        Int2Str(Number, Size);
        SerialPutString("\n\r size: ");
        SerialPutString(Number);
        SerialPutString(" bytes\r\n");
        SerialPutString("--------------------------------\r\n");
    }
    else if (Size == -1)
    {
        SerialPutString("\n\n\rthe image size is higher than the allowed space memory!\n\r");
    }
    else if (Size == -2)
    {
        SerialPutString("\n\n\rverification failed!\n\r");
    }
    else if (Size == -3)
    {
        SerialPutString("\r\n\naborted by user.\n\r");
    }
    else if (Size == -4)
    {
        SerialPutString("\r\n\nno file to receive. timeout\n\r");
    }
    else
    {
        SerialPutString("\n\rfailed to receive the file!\n\r");
    }
}




