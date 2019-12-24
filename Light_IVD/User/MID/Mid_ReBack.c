#ifndef __MID_REBACK_C__
#define __MID_REBACK_C__

#include "My_includes.h"

UINT8 MidReBack_QueBuf[MIDREBACK_QUEBUF_LEN] = {0};
UINT8 MidReBack_Buf[MIDREBACK_QUEBUF_LEN] = {0};
st_QueWaitPara sBackWaitPara;

/***************************************************************************************
** 函数名称：void Mid_ReBack_Fill(st_DataMsg *s_BackMsg)
** 函数功能：自身或者时序协议处理
** 参    数：s_BackMsg，返回信息表
            back_ctrl：返回操作码
** 返 回 值：无
***************************************************************************************/
void Mid_ReBack_Fill(st_DataMsg *s_BackMsg,UINT8 ctrl_num)
{
    UINT8  ctrl_data = 0;        // 控制位
    UINT16 data_num = 0;
    UINT8  data_crc = 0;
    OS_ERR os_err;
    
    // 数据帧头
    queue_in_byte(MidReBack_QueBuf,FRAM_HEAD_DATA);
    
    // 控制位
    ctrl_data = ctrl_num | s_BackMsg->st_dataSendId;//| s_BackMsg->st_dataRecFlag;
    
    //-----------------------------------------
    // 故障处理
//    if(s_BackMsg->st_dataErrNum != ERR_NONE)
//    {
//        ctrl_data = FRAM_CTRL_DATA_ERR; // 错误控制位
//        
//        
//        s_BackMsg->st_dataBuf[0] = s_BackMsg->st_dataErrPos;
//        s_BackMsg->st_dataBuf[1] = s_BackMsg->st_dataErrNum;
//        s_BackMsg->st_dataLen = 2;
//        if(s_BackMsg->st_dataBuf[1] > 200)
//        {
//            ctrl_data = FRAM_CTRL_DATA_WAR; // 错误控制位
//            s_BackMsg->st_dataBuf[0] = MARA_Flag;
//            MARA_Flag = 0;
//            if(s_BackMsg->st_dataBuf[1] == 240)
//            {
//                s_BackMsg->st_dataBuf[2] = MtxCtrl.mtxCount[SP_UD_MT].mStepsCompareAdd >> 8;
//                s_BackMsg->st_dataBuf[3] = MtxCtrl.mtxCount[SP_UD_MT].mStepsCompareAdd & 0xff;
//            }
//            else
//            {
//                s_BackMsg->st_dataBuf[2] = 0 >> 8;
//                s_BackMsg->st_dataBuf[3] = 0 & 0xff;
//            }
//            s_BackMsg->st_dataLen = 4;
//        }
//            
//        s_BackMsg->st_dataErrNum = ERR_NONE;
//        
//        
//    }
    
#if 1
    // 加载错误信息
    if(s_BackMsg->st_dataErrNum != ERR_NONE)
    {
        ctrl_data                   = FRAM_CTRL_DATA_ERR;
//        s_BackMsg->st_dataBuf[0]    = s_BackMsg->st_dataErrModule;
        s_BackMsg->st_dataBuf[1]    = s_BackMsg->st_dataErrNum;
        s_BackMsg->st_dataBuf[2]    = s_BackMsg->st_dataErrPos;
        s_BackMsg->st_dataLen       = 2;
        if(s_BackMsg->st_dataErrNum == ERR_RUN_AUTO_ENTER_CUP)
            ctrl_data               = FRAM_CTRL_DATA_WAR;
        
        s_BackMsg->st_dataErrNum = 0;
        s_BackMsg->st_dataErrPos = 0;
    }
#endif
    s_BackMsg->st_dataLen+=1;
    
    // 数据控制位
    queue_in_byte(MidReBack_QueBuf,ctrl_data);
    
    queue_in_byte(MidReBack_QueBuf,(UINT8)(s_BackMsg->st_dataLen>>8));
    queue_in_byte(MidReBack_QueBuf,(UINT8)(s_BackMsg->st_dataLen));
    
    // 命令返回
    queue_in_byte(MidReBack_QueBuf,s_BackMsg->st_dataCmd);
    
    // 数据返回
    for(data_num = 0; data_num < (s_BackMsg->st_dataLen-1); data_num++)
    {
        queue_in_byte(MidReBack_QueBuf,s_BackMsg->st_dataBuf[data_num]);
    }    
    
    // 数据校验位
    queue_in_byte(MidReBack_QueBuf,data_crc);
    
    // 数据帧尾
    queue_in_byte(MidReBack_QueBuf,FRAM_TAIL_DATA);
    
    OSTaskSemPost(&MIDReBackTaskTCB,OS_OPT_POST_NONE,&os_err);
}

UINT8 Mid_SendString(UINT8 *senf_buf, UINT16 send_len, UINT8 send_flag)
{
    switch(send_flag)
    {
        case COM_RECIEVE_FLAG:
        {
            Usart_ComSendString(MAIN_COM,senf_buf,send_len);
            break;
        }
#ifdef TCP_IP_ENABLE   
        case ETH_RECIEVE_FLAG:
        {
            break;
        }
#endif
    }
    return TRUE;
}


void Mid_ReBack_Task(void *p_arg)
{
    OS_ERR err;
    OS_TICK ts;
    UINT16 len = 0;
    UINT8  rec_flag = 0;
    UINT8 *RebackQue_buf;
    (void)p_arg;
    
    
    sBackWaitPara.data_len = 0;
    sBackWaitPara.get_len = 0;
    sBackWaitPara.len = 0;
    sBackWaitPara.RecStaue = Idel;
    queue_inital(MidReBack_QueBuf,MIDREBACK_QUEBUF_LEN);
    
    while(1)
    {
        // 信号量被处理Post几次，就会Pend几次
        OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,&ts,&err);
        
        printf("reback_task\r\n");
        //----------------------------------------------------------------------
        // CAN口接收数据
        if(queue_len_query(sCanCtrl.sCanConfig.canRecieveBuf))
        {
            RebackQue_buf = sCanCtrl.sCanConfig.canRecieveBuf;
        }
        else if(queue_len_query(MidReBack_QueBuf))
        {
            RebackQue_buf = MidReBack_QueBuf;
        }

        while(queue_len_query(RebackQue_buf))
        {
            if((len = Que_WaitFram(RebackQue_buf,MidReBack_Buf,&sBackWaitPara,1)) != 0)
            {
                sCanCtrl.sCanConfig.canID = MidReBack_Buf[1] & 0x0F;
                if(sCanCtrl.sCanConfig.canID)
                    MidReBack_Buf[1] = (MidReBack_Buf[1] & 0xF0) | CAN_ID;
                sCanCtrl.sCanFunList.canSendBuf(MidReBack_Buf,len);
//                Handle_BackMsg(MidReBack_Buf,len);
            }
        }
    }
}

#endif
