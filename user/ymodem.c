/******************************************************************************
* @file    $PROJ_DIR$\..\user\ymodem.c 
* @author  yyl
* @version V1.0
* @date    09/12/2019
* @brief   ymodem协议
*******************************************************************************/ 

#include "common.h"
#include "stm32f10x_flash.h"
#include "uart_485.h"
#include "ymodem.h"

u32 FlashDestination = ApplicationAddress_B; /* Flash user program offset */
u8  file_name[FILE_NAME_LENGTH];
u32 RamSource;
u32 EraseCounter = 0x0;
u32 NbrOfPage = 0;
u16 PageSize = PAGE_SIZE;
FLASH_Status FLASHStatus = FLASH_COMPLETE;
u8  Number_EOT = 0;
/*******************************************************************************
** 函数名称: Send_Byte
** 功能描述: 发送一个字节
** 参    数: 发送的字节
** 返 回 值: None       
** 作　  者: yyl                   
** 日  　期: 09/12/2019
*******************************************************************************/
static u32 Send_Byte (u8 c)
{
    _485_DIR_T;
    Delay_us(200);
    SerialPutChar(c);
    Delay_us(2000);
    _485_DIR_R;
    return 0;
}
/*******************************************************************************
** 函数名称: Receive_Byte
** 功能描述: 接收一个字节
** 参    数: c 接收字节存放， timeout 接收字节超时时间
** 返 回 值: 0 收到数据，-1 未收到数据       
** 作　  者: yyl                   
** 日  　期: 09/12/2019
*******************************************************************************/
static  s32 Receive_Byte (u8 *c, u32 timeout)
{
    while (timeout-- > 0)
    {
        if (SerialKeyPressed(c) == 1)
        {
            return 0;
        }
    }
    return -1;
}
/*******************************************************************************
** 函数名称: UpdateCRC16
** 功能描述: 校验和
** 参    数: data 校验数据的地址，size 数据长度
** 返 回 值: None       
** 作　  者: yyl                   
** 日  　期: 09/17/2019
*******************************************************************************/
u16 UpdateCRC16(u16 crcIn, u8 byte)
{
    u32 crc = crcIn;
    u32 in = byte | 0x100;
    do
    {
        crc <<= 1;
        in <<= 1;
        if(in & 0x100)
            ++crc;
        if(crc & 0x10000)
            crc ^= 0x1021;
    }
    while(!(in & 0x10000));
    return crc & 0xffffu;
}
/*******************************************************************************
** 函数名称: Cal_CRC16
** 功能描述: 校验和
** 参    数: data 校验数据的地址，size 数据长度
** 返 回 值: None       
** 作　  者: yyl                   
** 日  　期: 09/17/2019
*******************************************************************************/
u16 Cal_CRC16(const u8* data, u32 size)
{
    u32 crc = 0;
    const u8* dataEnd = data+size;
    while(data < dataEnd)
        crc = UpdateCRC16(crc, *data++);
    crc = UpdateCRC16(crc, 0);
    crc = UpdateCRC16(crc, 0);
    return crc & 0xffffu;
}
/*******************************************************************************
** 函数名称: Receive_Packet
** 功能描述: 接收一个包
** 参    数: data 接收到的数据，length 数据长度， timeout 接收字节超时时间
** 返 回 值: None       
** 作　  者: yyl                   
** 日  　期: 09/12/2019
*******************************************************************************/
static s32 Receive_Packet (u8 *data, s32 *length, u32 timeout)
{
    u16 i, packet_size, crc;
    u8  c;
    *length = 0;
    if (Receive_Byte(&c, timeout) != 0)
    {
        return -1;
    }
    switch (c)
    {
        case SOH:
        packet_size = PACKET_SIZE;
        break;
        case STX:
        packet_size = PACKET_1K_SIZE;
        break;
        case EOT:
        return 0;
        case CA:
        if ((Receive_Byte(&c, timeout) == 0) && (c == CA))
        {
            *length = -1;
            return 0;
        }
        else
        {
            return -1;
        }
        case ABORT1:
        case ABORT2:
        return 1;
        default:
        return -1;
    }
    *data = c;
    for (i = 1; i < (packet_size + PACKET_OVERHEAD); i ++)
    {
        if (Receive_Byte(data + i, timeout) != 0)
        {
            return -1;
        }
    }
    crc = ((u16)(data[packet_size + PACKET_OVERHEAD - 2]) << 8) |
                     ((u16)(data[packet_size + PACKET_OVERHEAD - 1]) << 0);
    if (crc != Cal_CRC16(&(data[PACKET_HEADER]), packet_size))
        return -1;
    if (data[PACKET_SEQNO_INDEX] != ((data[PACKET_SEQNO_COMP_INDEX] ^ 0xff) & 0xff))
        return -1;
    
    *length = packet_size;
    return 0;
}

/*******************************************************************************
** 函数名称: Ymodem_Receive
** 功能描述: ymodem协议出路
** 参    数: buf 接收到的数据
** 返 回 值: None       
** 作　  者: yyl                   
** 日  　期: 09/12/2019
*******************************************************************************/
int32_t Ymodem_Receive (u8 *buf)
{
    u8 packet_data[PACKET_1K_SIZE + PACKET_OVERHEAD], file_size[FILE_SIZE_LENGTH], *file_ptr, *buf_ptr, No_Response_Timeout;
    int32_t i, j, packet_length, session_done, file_done, packets_received, errors, session_begin, size = 0;

    /* Initialize FlashDestination variable */
    FlashDestination = ApplicationAddress_B;

    for (session_done = 0, errors = 0, session_begin = 0, No_Response_Timeout = 0; ;)
    {
        for (packets_received = 0, file_done = 0, buf_ptr = buf; ;)
        {
            switch (Receive_Packet(packet_data, &packet_length, NAK_TIMEOUT))
            {
            case 0:
                errors = 0;
                switch (packet_length)
                {
                /* Abort by sender */
                case - 1:
                    Send_Byte(ACK);
                return 0;
                /* End of transmission */
                case 0:
                  //  Send_Byte(ACK);
                  //  file_done = 1;
            
                    if(Number_EOT == 0)
                    {
                        Send_Byte(NAK);
                        Number_EOT++;
                    }
                    else if(Number_EOT == 1)
                    {
                        Send_Byte(ACK);
                        Send_Byte(CR);
                        file_done = 1;
                        Number_EOT = 0;
                    }
                    
                break;
                /* Normal packet */
                default:
                    if ((packet_data[PACKET_SEQNO_INDEX] & 0xff) != (packets_received & 0xff))
                    {
                        Send_Byte(NAK);
                    }
                    else
                    {
                        if (packets_received == 0)
                        {
                            /* Filename packet */
                            if (packet_data[PACKET_HEADER] != 0)
                            {
                            /* Filename packet has valid data */
                                for (i = 0, file_ptr = packet_data + PACKET_HEADER; (*file_ptr != 0) && (i < FILE_NAME_LENGTH);)
                                {
                                    file_name[i++] = *file_ptr++;
                                }
                                file_name[i++] = '\0';
                                for (i = 0, file_ptr ++; (*file_ptr != ' ') && (i < FILE_SIZE_LENGTH);)
                                {
                                    file_size[i++] = *file_ptr++;
                                }
                                file_size[i++] = '\0';
                                Str2Int(file_size, &size);
    
                                /* Test the size of the image to be sent */
                                /* Image size is greater than Flash size */
                                if (size > (FLASH_SIZE - 1))
                                {
                                      /* End session */
                                    Send_Byte(CA);
                                    Send_Byte(CA);
                                    return -1;
                                }
    
                                /* Erase the needed pages where the user application will be loaded */
                                /* Define the number of page to be erased */
                                NbrOfPage = FLASH_PagesMask(size);
    
                                /* Erase the FLASH pages *//*擦除启动选择区和程序更新区*/
                                for (EraseCounter = 0; (EraseCounter < NbrOfPage) && (FLASHStatus == FLASH_COMPLETE); EraseCounter++)
                                {
                                    FLASHStatus = FLASH_ErasePage(FlashDestination + (PageSize * EraseCounter));
                                }
                                
                                Send_Byte(ACK);
                                Send_Byte(CR);
                            }
                            /* Filename packet is empty, end session */
                            else
                            {
                                Send_Byte(ACK);
                                file_done = 1;
                                session_done = 1;
                                break;
                            }
                        }
                        /* Data packet */
                        else
                        {
                            memcpy(buf_ptr, packet_data + PACKET_HEADER, packet_length);
                            RamSource = (uint32_t)buf;
                            for (j = 0;(j < packet_length) && (FlashDestination <  ApplicationAddress_B + size);j += 4)
                            {
                                /* Program the data received into STM32F10x Flash */
                                FLASH_ProgramWord(FlashDestination, *(uint32_t*)RamSource);
    
                                if (*(uint32_t*)FlashDestination != *(uint32_t*)RamSource)
                                {
                                   /* End session */
                                    Send_Byte(CA);
                                    Send_Byte(CA);
                                    return -2;
                                }
                                FlashDestination += 4;
                                RamSource += 4;
                            }
                            Send_Byte(ACK);
                        }
                        packets_received ++;
                        session_begin = 1;
                    }
                }
            break;
            case 1:
                Send_Byte(CA);
                Send_Byte(CA);
            return -3;
            default:
                if (session_begin > 0)
                {
                    errors ++;
                }
                if (errors > MAX_ERRORS)
                {
                    Send_Byte(CA);
                    Send_Byte(CA);
                    return 0;
                }
                Send_Byte(CR);
                No_Response_Timeout ++;
                if(No_Response_Timeout > 10)
                {
                    Send_Byte(CA);
                    Send_Byte(CA);
                    return -4;
                }
            break;
            }
            if (file_done != 0)
            {
                break;
            }
        }
        if (session_done != 0)
        {
            break;
        }
    }
    return (int32_t)size;
}



