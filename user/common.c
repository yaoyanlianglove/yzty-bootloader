/******************************************************************************
* @file    $PROJ_DIR$\..\user\common.c 
* @author  yyl
* @version V1.0
* @date    09/12/2019
* @brief   功能程序
*******************************************************************************/ 

/* Includes ------------------------------------------------------------------*/
#include "common.h"
#include "ymodem.h"
#include "uart_485.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
pFunction Jump_To_Application;
u32 JumpAddress;
u32 BlockNbr = 0, UserMemoryMask = 0;


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
** 函数名称: Int2Str
** 功能描述: 整型转字符串
** 参    数: None
** 返 回 值: None       
** 作　  者: yyl                   
** 日  　期: 09/12/2019
*******************************************************************************/
void Int2Str(u8* str, int32_t intnum)
{
    u32 i, Div = 1000000000, j = 0, Status = 0;
    for (i = 0; i < 10; i++)
    {
        str[j++] = (intnum / Div) + 48;
        intnum = intnum % Div;
        Div /= 10;
        if ((str[j-1] == '0') & (Status == 0))
        {
            j = 0;
        }
        else
        {
            Status++;
        }
    }
}
/*******************************************************************************
** 函数名称: GotoAPP
** 功能描述: 跳转到应用程序函数
** 参    数: None
** 返 回 值: None       
** 作　  者: yyl                   
** 日  　期: 09/12/2019
*******************************************************************************/
void GotoAPP(u8 area)
{ 
    if(area == 'A')
    {
        /* Test if user code is programmed starting from address "ApplicationAddress" */
        if (((*(__IO u32*)ApplicationAddress_A) & 0x2FFE0000 ) == 0x20000000)
        { 
            SerialPutString("goto app_a\r\n");
            /* Jump to user application */
            JumpAddress = *(__IO u32*) (ApplicationAddress_A + 4);
            Jump_To_Application = (pFunction) JumpAddress;
            /* Initialize user application's Stack Pointer */
            __set_MSP(*(__IO u32*) ApplicationAddress_A);
            Jump_To_Application();
        }
    }
    else //'B'
    {
        /* Test if user code is programmed starting from address "ApplicationAddress" */
        if (((*(__IO u32*)ApplicationAddress_B) & 0x2FFE0000 ) == 0x20000000)
        { 
            SerialPutString("goto app_b\r\n");
            /* Jump to user application */
            JumpAddress = *(__IO u32*) (ApplicationAddress_B + 4);
            Jump_To_Application = (pFunction) JumpAddress;
            /* Initialize user application's Stack Pointer */
            __set_MSP(*(__IO u32*) ApplicationAddress_B);
            Jump_To_Application();
        }
    }
    
}
/*******************************************************************************
** 函数名称: FLASH_ReadOneByte
** 功能描述: 从flash读一个字节的数据
** 参    数: None
** 返 回 值: 读到的内部flash数据       
** 作　  者: yyl                   
** 日  　期: 09/12/2019
*******************************************************************************/
u8 FLASH_ReadOneByte(u32 address)
{
    return *(__IO u8*)address; 
}
/*******************************************************************************
** 函数名称: FLASH_DisableWriteProtectionPages
** 功能描述: 判断程序块是否写保护.如果有写保护，去除写保护
** 参    数: None
** 返 回 值: 不为0解除写保护失败       
** 作　  者: yyl                   
** 日  　期: 09/12/2019
*******************************************************************************/
u8 FLASH_DisableWriteProtectionPages(void)
{
    u32 useroptionbyte = 0, WRPR = 0;
    u16 var1 = OB_IWDG_SW, var2 = OB_STOP_NoRST, var3 = OB_STDBY_NoRST;
    FLASH_Status status = FLASH_BUSY;

    /* Get the number of block (4 or 2 pages) from where the user program will be loaded */
    BlockNbr = (SART_APP_ADDR - 0x08000000) >> 12;

  /* Compute the mask to test if the Flash memory, where the user program will be
     loaded, is write protected */
#if defined (STM32F10X_MD) || defined (STM32F10X_MD_VL)
    UserMemoryMask = ((u32)~((1 << BlockNbr) - 1));
#else /* USE_STM3210E_EVAL */
    if (BlockNbr < 62)
    {
        UserMemoryMask = ((u32)~((1 << BlockNbr) - 1));
    }
    else
    {
        UserMemoryMask = ((u32)0x80000000);
    }
#endif /* (STM32F10X_MD) || (STM32F10X_MD_VL) */

    WRPR = FLASH_GetWriteProtectionOptionByte();

  /* Test if user memory is write protected */
    if ((WRPR & UserMemoryMask) != UserMemoryMask)
    {
        useroptionbyte = FLASH_GetUserOptionByte();
        UserMemoryMask |= WRPR;
        status = FLASH_EraseOptionBytes();
        if (UserMemoryMask != 0xFFFFFFFF)
        {
            status = FLASH_EnableWriteProtection((u32)~UserMemoryMask);
        }

    /* Test if user Option Bytes are programmed */
        if ((useroptionbyte & 0x07) != 0x07)
        { 
      /* Restore user Option Bytes */
            if ((useroptionbyte & 0x01) == 0x0)
            {
                var1 = OB_IWDG_HW;
            }
            if ((useroptionbyte & 0x02) == 0x0)
            {
                var2 = OB_STOP_RST;
            }
            if ((useroptionbyte & 0x04) == 0x0)
            {
                var3 = OB_STDBY_RST;
            }
            FLASH_UserOptionByteConfig(var1, var2, var3);
        }
        if (status == FLASH_COMPLETE)
        {
            SerialPutString("write protection disabled...\r\n");
            SerialPutString("...and a system reset will be generated to re-load the new option bytes\r\n");
            /* Generate System Reset to load the new option byte values */
            NVIC_SystemReset();
        }
        else
        {
            SerialPutString("error: flash write unprotection failed...\r\n");
            return 1;
        }
    }
    else
    {
        SerialPutString("flash memory not write protected\r\n");
    }
    return 0;
}
/*******************************************************************************
** 函数名称: Str2Int
** 功能描述: 字符转整型
** 参    数: inputstr 字符地址，intnum，整型地址
** 返 回 值: 0 错误，1正确       
** 作　  者: yyl                   
** 日  　期: 09/12/2019
*******************************************************************************/
u32 Str2Int(u8 *inputstr, s32 *intnum)
{
    u32 i = 0, res = 0;
    u32 val = 0;

    if (inputstr[0] == '0' && (inputstr[1] == 'x' || inputstr[1] == 'X'))
    {
        if (inputstr[2] == '\0')
        {
            return 0;
        }
        for (i = 2; i < 11; i++)
        {
            if (inputstr[i] == '\0')
            {
                *intnum = val;
                /* return 1; */
                res = 1;
                break;
            }
            if (ISVALIDHEX(inputstr[i]))
            {
                val = (val << 4) + CONVERTHEX(inputstr[i]);
            }
            else
            {
                /* return 0, Invalid input */
                res = 0;
                break;
            }
        }
        /* over 8 digit hex --invalid */
        if (i >= 11)
        {
           res = 0;
        }
    }
    else /* max 10-digit decimal input */
    {
        for (i = 0;i < 11;i++)
        {
            if (inputstr[i] == '\0')
            {
                *intnum = val;
                /* return 1 */
                res = 1;
                break;
            }
            else if ((inputstr[i] == 'k' || inputstr[i] == 'K') && (i > 0))
            {
                val = val << 10;
                *intnum = val;
                res = 1;
                break;
            }
            else if ((inputstr[i] == 'm' || inputstr[i] == 'M') && (i > 0))
            {
                val = val << 20;
                *intnum = val;
                res = 1;
                break;
            }
            else if (ISVALIDDEC(inputstr[i]))
            {
                val = val * 10 + CONVERTDEC(inputstr[i]);
            }
            else
            {
                /* return 0, Invalid input */
                res = 0;
                break;
            }
        }
            /* Over 10 digit decimal --invalid */
        if (i >= 11)
        {
            res = 0;
        }
    }

    return res;
}
/*******************************************************************************
** 函数名称: FLASH_PagesMask
** 功能描述: 计算文件需要的页数
** 参    数: 镜像大小
** 返 回 值: 页数       
** 作　  者: yyl                   
** 日  　期: 09/12/2019
*******************************************************************************/
u32 FLASH_PagesMask(__IO u32 Size)
{
    u32 pagenumber = 0x0;
    u32 size = Size;
    if ((size % PAGE_SIZE) != 0)
    {
        pagenumber = (size / PAGE_SIZE) + 1;
    }
    else
    {
        pagenumber = size / PAGE_SIZE;
    }
    return pagenumber;
}
/**
  * @}
  */

/*******************(C)COPYRIGHT 2010 STMicroelectronics *****END OF FILE******/
