#ifndef __CTRLPMT_C__
#define __CTRLPMT_C__
/********************************************************************************
writer : ZH Wang
time   : 2017-10-31
*********************************************************************************/
#include <includes.h>
#include "My_include.h"

UINT8 PMT_Handle_Cmd[] = {"Q\r\n"};
UINT8 PMT_End_Cmd[] = {"E\r\n"};
UINT8 PMT_CheckVersion_Cmd[] = {"?\r\n"};
UINT8 PMT_Star_Cmd[] = {"S\r\n"};
UINT8 PMT_Receive_Cmd[] = {"R\r\n"};
UINT8 PMT_RecEnd_Cmd[] = {"J\r\n"};

UINT8 PMT_End(void)
{
    UINT8 ch = 0;
    FUN_USART_SendString(PMT_COM,PMT_End_Cmd,3);
    OSTimeDly(100);
    while(queue_out_byte(ComFunList[PMT_COM].ComMemBuf,&ch,1))
    {
        if(ch == 0x45)
        {
            return ERR_NONE;
        }
    }
    return ERR_PMT_END;
}

UINT8 PMT_Handle(void)
{
    UINT8 ch = 0;
    UINT8 ret = ERR_NONE;
    if((ret = PMT_End()) != ERR_NONE)
        return ret;
   
    FUN_USART_SendString(PMT_COM,PMT_Handle_Cmd,3);
    OSTimeDly(100);
    while(queue_out_byte(ComFunList[PMT_COM].ComMemBuf,&ch,1))
    {
        if(ch == 0x21)
        {
            return ERR_NONE;
        }
    }
    return ERR_PMT_HANDLE;
}

UINT8 PMT_Check_Version(UINT8 *rec_buf)
{
    UINT8 ch = 0;
    UINT8 *buf = rec_buf;
    UINT8 len = 0;
    UINT8 ret = ERR_NONE;
    if((ret = PMT_End()) != ERR_NONE)
        return ret;
   
    FUN_USART_SendString(PMT_COM,PMT_CheckVersion_Cmd,3);
    OSTimeDly(100);
    while(queue_out_byte(ComFunList[PMT_COM].ComMemBuf,&ch,1))
    {
        *buf = ch;
        len++;
        buf++;
    }
    return len;
}


UINT16 PMT_Time(UINT16 set_time)
{
    UINT8 ch = 0;
    UINT8 len = 0;
    UINT8 ret = ERR_NONE;
    UINT16 time = 0;
    UINT8 TimeBuf[8] = {0};
    if((ret = PMT_End()) != ERR_NONE)
        return ret;
    
    TimeBuf[0] = 'T';
    time = (set_time/100) + '0';
    
    TimeBuf[1] = time;
    time = ((set_time%100)/10) + '0';
    TimeBuf[2] = time;
    time = (set_time%10) + '0';
    TimeBuf[3] = time;
    TimeBuf[4] = 0x0A;
    TimeBuf[5] = 0x0D;
    FUN_USART_SendString(PMT_COM,TimeBuf,6);
    OSTimeDly(100);
    time = 0;
    while(queue_out_byte(ComFunList[PMT_COM].ComMemBuf,&ch,1))
    {
        if(ret == 1)
        {
            time = (time<<8) + ch;
        }
        if(ch == 'T')
        {
            ret = 1;
        }
    }
    return time;
}

UINT16 PMT_Resolution(UINT16 set_resol)
{
    UINT8 ch = 0;
    UINT8 len = 0;
    UINT8 ret = ERR_NONE;
    UINT16 time = 0;
    UINT8 ResolBuf[8] = {0};
    if((ret = PMT_End()) != ERR_NONE)
        return ret;
    
    ResolBuf[0] = 'C';
    time = (set_resol/100) + '0';
    
    ResolBuf[1] = time;
    time = ((set_resol%100)/10) + '0';
    ResolBuf[2] = time;
    time = (set_resol%10) + '0';
    ResolBuf[3] = time;
    ResolBuf[4] = 0x0A;
    ResolBuf[5] = 0x0D;
    FUN_USART_SendString(PMT_COM,ResolBuf,6);
    OSTimeDly(100);
    time = 0;
    while(queue_out_byte(ComFunList[PMT_COM].ComMemBuf,&ch,1))
    {
        if(ret == 1)
        {
            time = (time<<8) + ch;
        }
        if(ch == 'C')
        {
            ret = 1;
        }
    }
    return time;
}

UINT32 PMT_Star(void)
{
    UINT8 ch = 0;
    UINT8 len = 0;
    UINT8 ret = ERR_NONE;
    UINT8 crc = 0;
    UINT32 Count = 0;
    FUN_USART_SendString(PMT_COM,PMT_Star_Cmd,3);
    OSTimeDly(10);
    while(queue_out_byte(ComFunList[PMT_COM].ComMemBuf,&ch,1))
    {
        if(ret == 1)
        {
            len++;
            if(len>4)
            {
                if(crc == ch)
                    return Count;
                else
                    return 0;
            }
            crc += ch;
            Count = (Count<<8) + ch;
            
        }
        if(ch == 0xFF)
        {
            ret = 1;
        }
    }
    return 0;
}

UINT32 PMT_Recieve(void)
{
    UINT8 ch = 0;
    UINT8 len = 0;
    UINT8 ret = ERR_NONE;
    UINT8 crc = 0;
    UINT32 Count = 0;
    FUN_USART_SendString(PMT_COM,PMT_Receive_Cmd,3);
    OSTimeDly(10);
    while(queue_out_byte(ComFunList[PMT_COM].ComMemBuf,&ch,1))
    {
        if(ret == 1)
        {
            len++;
            if(len>4)
            {
                if(crc == ch)
                    return Count;
                else
                    return 0;
            }
            crc += ch;
            Count = (Count<<8) + ch;
            
        }
        if(ch == 0xFF)
        {
            ret = 1;
        }
    }
    return 0;
}

UINT32 PMT_RecEnd(void)
{
    UINT8 ch = 0;
    UINT8 len = 0;
    UINT8 ret = ERR_NONE;
    UINT8 crc = 0;
    UINT32 Count = 0;
    FUN_USART_SendString(PMT_COM,PMT_RecEnd_Cmd,3);
    OSTimeDly(10);
    while(queue_out_byte(ComFunList[PMT_COM].ComMemBuf,&ch,1))
    {
        if(ret == 1)
        {
            len++;
            if(len>4)
            {
                if(crc == ch)
                    return Count;
                else
                    return 0;
            }
            crc += ch;
            Count = (Count<<8) + ch;
            
        }
        if(ch == 0xFF)
        {
            ret = 1;
        }
    }
    return 0;
}

UINT8 Handle_PMT_Cmd(void)
{
    UINT8 ret = ERR_NONE;
    UINT8 retbuf[8] = {0};
    UINT8 Pump_Valve = 0;
    UINT8 i;
    UINT32 Count = 0;
    switch(pmt_msg.op)
    {
        case CMD_PMT_HANDLE:
        {
            if(pmt_msg.len != 0)
            {
                pmt_msg.err = ERR_FRAM_LENGTH;
                pmt_msg.len = 2;
                break;
            }
            pmt_msg.err = PMT_Handle();
            break;
        }
        case CMD_PMT_CHECK_VERSION:
        {
            if(pmt_msg.len != 0)
            {
                pmt_msg.err = ERR_FRAM_LENGTH;
                pmt_msg.len = 2;
                break;
            }
            PMT_Check_Version(retbuf);
            pmt_msg.data[0] = retbuf[0];
            pmt_msg.data[1] = retbuf[2];
            pmt_msg.data[2] = retbuf[3];
            pmt_msg.len = 3;
            break;
        }
        case CMD_PMT_STAR:
        {
            if(pmt_msg.len != 0)
            {
                pmt_msg.err = ERR_FRAM_LENGTH;
                pmt_msg.len = 2;
                break;
            }
            Count = PMT_Star();
            pmt_msg.data[0] = (Count>>24)&0xFF;
            pmt_msg.data[1] = (Count>>16)&0xFF;
            pmt_msg.data[2] = (Count>>8)&0xFF;
            pmt_msg.data[3] = Count&0xFF;
            pmt_msg.len = 4;
            break;
        }
        case CMD_PMT_RECEIVE:
        {
            if(pmt_msg.len != 0)
            {
                pmt_msg.err = ERR_FRAM_LENGTH;
                pmt_msg.len = 2;
                break;
            }
            Count = PMT_Recieve();
            pmt_msg.data[0] = (Count>>24)&0xFF;
            pmt_msg.data[1] = (Count>>16)&0xFF;
            pmt_msg.data[2] = (Count>>8)&0xFF;
            pmt_msg.data[3] = Count&0xFF;
            pmt_msg.len = 4;
            break;
        }
        case CMD_PMT_RECEND:
        {
            if(pmt_msg.len != 0)
            {
                pmt_msg.err = ERR_FRAM_LENGTH;
                pmt_msg.len = 2;
                break;
            }
            Count = PMT_RecEnd();
            pmt_msg.data[0] = (Count>>24)&0xFF;
            pmt_msg.data[1] = (Count>>16)&0xFF;
            pmt_msg.data[2] = (Count>>8)&0xFF;
            pmt_msg.data[3] = Count&0xFF;
            pmt_msg.len = 4;
            break;
        }
        case CMD_PMT_END:
        {
            if(pmt_msg.len != 0)
            {
                pmt_msg.err = ERR_FRAM_LENGTH;
                pmt_msg.len = 2;
                break;
            }
            pmt_msg.err = PMT_End();
            break;
        }
        case CMD_PMT_SET_DOOR_TIME:
        {
            if(pmt_msg.len != 2)
            {
                pmt_msg.err = ERR_FRAM_LENGTH;
                pmt_msg.len = 2;
                break;
            }
            Count = (pmt_msg.data[0]<<8) + pmt_msg.data[1];
            PMT_Time(Count);
            break;
        }
        case CMD_PMT_SET_RESOLUTION:
        {
            if(pmt_msg.len != 2)
            {
                pmt_msg.err = ERR_FRAM_LENGTH;
                pmt_msg.len = 2;
                break;
            }
            Count = (pmt_msg.data[0]<<8) + pmt_msg.data[1];
            PMT_Resolution(Count);
            break;
        }
        default:
            break;
            
    }
    Sys_ReBack(&pmt_msg);
    return ret;
}

#endif

