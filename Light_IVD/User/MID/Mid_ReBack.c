#ifndef __MID_REBACK_C__
#define __MID_REBACK_C__

#include "My_includes.h"

UINT8 MidReBack_QueBuf[MIDREBACK_QUEBUF_LEN] = {0};
UINT8 MidReBack_Buf[MIDREBACK_QUEBUF_LEN] = {0};
st_QueWaitPara sBackWaitPara;

/***************************************************************************************
** �������ƣ�void Mid_ReBack_Fill(st_DataMsg *s_BackMsg)
** �������ܣ��������ʱ��Э�鴦��
** ��    ����s_BackMsg��������Ϣ��
            back_ctrl�����ز�����
** �� �� ֵ����
***************************************************************************************/
void Mid_ReBack_Fill(st_DataMsg *s_BackMsg,UINT8 ctrl_num)
{
    UINT8  ctrl_data = 0;        // ����λ
    UINT16 data_num = 0;
    UINT8  data_crc = 0;
    OS_ERR os_err;
    
    // ����֡ͷ
    queue_in_byte(MidReBack_QueBuf,FRAM_HEAD_DATA);
    
    // ����λ
    ctrl_data = ctrl_num | s_BackMsg->st_dataSendId;//| s_BackMsg->st_dataRecFlag;
    
    //-----------------------------------------
    // ���ϴ���
//    if(s_BackMsg->st_dataErrNum != ERR_NONE)
//    {
//        ctrl_data = FRAM_CTRL_DATA_ERR; // �������λ
//        
//        
//        s_BackMsg->st_dataBuf[0] = s_BackMsg->st_dataErrPos;
//        s_BackMsg->st_dataBuf[1] = s_BackMsg->st_dataErrNum;
//        s_BackMsg->st_dataLen = 2;
//        if(s_BackMsg->st_dataBuf[1] > 200)
//        {
//            ctrl_data = FRAM_CTRL_DATA_WAR; // �������λ
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
    // ���ش�����Ϣ
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
    
    // ���ݿ���λ
    queue_in_byte(MidReBack_QueBuf,ctrl_data);
    
    queue_in_byte(MidReBack_QueBuf,(UINT8)(s_BackMsg->st_dataLen>>8));
    queue_in_byte(MidReBack_QueBuf,(UINT8)(s_BackMsg->st_dataLen));
    
    // �����
    queue_in_byte(MidReBack_QueBuf,s_BackMsg->st_dataCmd);
    
    // ���ݷ���
    for(data_num = 0; data_num < (s_BackMsg->st_dataLen-1); data_num++)
    {
        queue_in_byte(MidReBack_QueBuf,s_BackMsg->st_dataBuf[data_num]);
    }    
    
    // ����У��λ
    queue_in_byte(MidReBack_QueBuf,data_crc);
    
    // ����֡β
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
        // �ź���������Post���Σ��ͻ�Pend����
        OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,&ts,&err);
        
        printf("reback_task\r\n");
        //----------------------------------------------------------------------
        // CAN�ڽ�������
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
