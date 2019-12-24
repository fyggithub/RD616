/*********************************************************************************
**********************************************************************************
**********************************************************************************
* 文件名称: Load_Parameter.c                                                    *
* 文件简述：参数存储位数，以及设置                                            *
* 创建日期：2014.10.03                                                          *
* 版    本：V1.13                                                               *
* 作    者：LY Chen                                                             *
* 说    明：开机时候进行参数的读取                                             * 
**********************************************************************************
*********************************************************************************/
#ifndef __LOAD_PARAMETER_C__
#define __LOAD_PARAMETER_C__

#include <includes.h>
#include "My_includes.h"

st_lEC_Para s_ecpara;

UINT8 Handle_Para_Cmd(void)
{
    UINT8 ret;
    UINT32 t = 0;
    switch(para_msg.st_dataCmd)
    {
        case CMD_PARA_EC_SET:
        {
            Set_Paramenter();
            break;
        }
        case CMD_PARA_EC_LOAD:
        {
            Load_Paramenter();
            break;
        }
        default:
            break;
    }
    Mid_ReBack_Fill(&para_msg,FRAM_CTRL_DATA_S_M);
    return ret;
}

_STA_ void Set_Default_Para(void)
{
    s_ecpara.ec_ud_pos[0] = 0;
    s_ecpara.ec_ud_pos[1] = 5360;
    s_ecpara.ec_ud_pos[2] = 500;
    s_ecpara.ec_ud_pos[3] = 2000;
    s_ecpara.updata_flag = 1;
}

UINT8 Set_EC_Para(void)
{
    s_ecpara.ec_ud_pos[0]   = (para_msg.st_dataBuf[0]<<8) + para_msg.st_dataBuf[1];//旋转复位偏移量
    s_ecpara.ec_ud_pos[1]   = (para_msg.st_dataBuf[2]<<8) + para_msg.st_dataBuf[3];//试剂位R1旋转偏移量
    s_ecpara.ec_ud_pos[2]   = (para_msg.st_dataBuf[4]<<8) + para_msg.st_dataBuf[5];//试剂位R2旋转偏移量
    s_ecpara.ec_ud_pos[3]   = (para_msg.st_dataBuf[6]<<8) + para_msg.st_dataBuf[7];//试剂位R3旋转偏移量
}

UINT8 Set_Paramenter(void)
{
    UINT16 addr = 0;
    UINT16 len = 0;
    switch(para_msg.st_dataCmd)
    {
        case CMD_PARA_EC_SET:
        {
            if(para_msg.st_dataLen != 8)
            {
                para_msg.st_dataErrNum = ERR_FRAM_LENGTH;
                para_msg.st_dataLen = 2;
                break;
            }
            addr = SP_FRAME_PAGE * FRAME_ADDR_PAGE_LEN;
            len = sizeof(st_lEC_Para);
            Set_EC_Para();
//            memcpy((UINT8*)&sp_para,para_msg.st_dataBuf,para_msg.len);
            s_ecpara.updata_flag = 1;
            
            Write_FM25CL64_nByte(addr,(UINT8*)&s_ecpara,len);
            
            para_msg.st_dataLen = 0;
            
            break;
        }
    }
}

UINT8 Load_SP_Para(void)
{
    para_msg.st_dataBuf[0]  = s_ecpara.ec_ud_pos[0]>>8;
    para_msg.st_dataBuf[1]  = s_ecpara.ec_ud_pos[0]&0xff;
    para_msg.st_dataBuf[2]  = s_ecpara.ec_ud_pos[1]>>8;
    para_msg.st_dataBuf[3]  = s_ecpara.ec_ud_pos[1]&0xff;
    para_msg.st_dataBuf[4]  = s_ecpara.ec_ud_pos[2]>>8;
    para_msg.st_dataBuf[5]  = s_ecpara.ec_ud_pos[2]&0xff;
    para_msg.st_dataBuf[6]  = s_ecpara.ec_ud_pos[3]>>8;
    para_msg.st_dataBuf[7]  = s_ecpara.ec_ud_pos[3]&0xff;
    
}

UINT8 Load_Paramenter(void)
{
    UINT16 addr = 0;
    UINT16 len = 0;
    switch(para_msg.st_dataCmd)
    {
        case CMD_PARA_EC_LOAD:
        {
            if(para_msg.st_dataLen != 0)
            {
                para_msg.st_dataErrNum = ERR_FRAM_LENGTH;
                para_msg.st_dataLen = 2;
                break;
            }
            addr = SP_FRAME_PAGE * FRAME_ADDR_PAGE_LEN;
            len = sizeof(st_lEC_Para);
//            memcpy((UINT8*)&sp_para,para_msg.st_dataBuf,para_msg.len);
            Read_FM25CL64_nByte(addr,(UINT8*)&s_ecpara,len);
            
            if(s_ecpara.updata_flag != 1)
            {
                Set_Default_Para();
                Write_FM25CL64_nByte(addr,(UINT8*)&s_ecpara,len);
            }
            
            Load_SP_Para();
            para_msg.st_dataLen = len;
            
            break;
        }
        
    }
}

UINT8 Load_SysPara(void)
{
    UINT8 i = 0;
    UINT8 buf[64] = {0};
    
    para_msg.st_dataLen = 0;
    para_msg.st_dataCmd = CMD_PARA_EC_LOAD;
    Load_Paramenter();
    
    Usart_ComSendString(MAIN_COM,para_msg.st_dataBuf,para_msg.st_dataLen);
}

#endif

