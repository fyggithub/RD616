#ifndef __MID_C__
#define __MID_C__

#include <My_includes.h>

UINT8 set_debug = 0;

UINT8 Sys_Errcode = 0;// ϵͳ������
UINT8 Sys_Errpos  = 0;       // Ĭ�ϴ���λ��

UINT8 Mid_usart_buf[MSG_BUF_LEN] = {0};     // ���ڽ������ݻ���
UINT8 Mid_can_buf[MSG_BUF_LEN] = {0};       // Can�������ݻ���

UINT8 RecieveBuf[MSG_BUF_LEN] = {0};        // ���ݻ���

UINT8 ReBack_que_buf[REBACK_BUF_LEN] = {0}; // �������ݻ���

UINT8 Recieve_Flag = 0;                     // �������ݱ�ʾ

st_QueWaitPara s_MidWaitPata;

APPInfoType gApp;

/***************************************************************************************
** �������ƣ�UINT8 PL_CommandCheck(UINT8* pStr, UINT16 nLen, UINT8* pResult)
** �������ܣ�У������
** ��    ����*pStr: ��������
              nLen: ���ݳ���
             *pResult: ���ؽ��
** �� �� ֵ��e_true
***************************************************************************************/
UINT8 PL_CommandCheck(UINT8* pStr, UINT16 nLen, UINT8* pResult)
{
    UINT16 nCharCount = 0;
    UINT8  nCheckResult = 0;

    for(nCharCount = 0; nCharCount < nLen; nCharCount++)
    {
        // sum
        nCheckResult += (UINT8)(*(pStr + nCharCount)); 
    }
   
    *pResult = nCheckResult;

    return e_true;
}

/***************************************************************************************
** �������ƣ�void Sys_Init(void)
** �������ܣ�ϵͳ��Ϣ��Ӳ���ȳ�ʼ��
** ��    ������
** �� �� ֵ����
***************************************************************************************/
void Sys_Init(void)
{
    UINT8 i = 0;
    
    // ����Bootloadʹ��
#if IS_BOOTLOAD_ENABLE
    //=======================================================================
    // bootload������ȡ������Ϣ
    STMFLASH_Read(FLASH_APP_FLAG_ADDR,(unsigned long *)&gApp,sizeof(APPInfoType)>>2);         // ���±�־
    // �ж�ID���Ƿ���ȷ������������д����
    if(gApp.ID != CAN_ID)
    {
        gApp.Flag = APP_IS_ID_ERR;     // ���򲻴���
        gApp.ByteCounts = 0;
        
        STMFLASH_Write(FLASH_APP_FLAG_ADDR,(unsigned long *)&gApp,sizeof(APPInfoType)>>2);         // ���±�־
        Sys_Delay_ms(10);
        SoftReset();
    }
    gApp.Flag = APP_IS_VALID;     // ������Ч
    gApp.ByteCounts = 0;
    STMFLASH_Write(FLASH_APP_FLAG_ADDR,(unsigned long *)&gApp,sizeof(APPInfoType)>>2);         // ���±�־
#endif
    
    // �������ó�ʼ��
    Com_Initialization(MAIN_COM,Mid_usart_buf,MSG_BUF_LEN);
    
    // Can���ó�ʼ��
    Can_Initialization(Mid_can_buf,MSG_BUF_LEN);
    
    // LED��ʼ��
    Led_Initialization();
    LK_PinAllInit();
    Waste_Light_PinInit();
    // �����ʼ��
    Mtx_Init(E_TIM_0);
//    Mtx_Init(E_TIM_1);
//    Mtx_Init(E_TIM_2);
//    Mtx_Init(E_TIM_3);
    Enter_Cup_Check_Light();
    Spring_Pin_Init();
    // �ⲿ�洢��ʼ��
    FRAME_INIT();
    
    printf("Bulider Wang\r\n");
    printf("Proble\r\n");

    // ��ģ����Ϣ��ʼ��
    Msg_Init();
    
    // ����ϵͳ����
    Load_SysPara();
    
}


/***************************************************************************************
** �������ƣ�UINT8 Que_WaitFram(UINT8* Que_Msgbuf,UINT8 *RecFram,UINT16 ts)
** �������ܣ��Ӷ����л�ȡ����
** ��    ����*Que_Msgbuf�����е�ַ
             *RecBuf���������ݻ���
             ts���ȴ�ʱ��
** �� �� ֵ����
***************************************************************************************/
UINT16 Que_WaitFram(UINT8* Que_Msgbuf,UINT8 *RecBuf,st_QueWaitPara *sWaitPara, UINT16 ts)
{
    UINT8 ch = 0;
    UINT8 ret = ERR_NONE;
    UINT8 check_result = 0;
    
    
    while(queue_out_byte(Que_Msgbuf,&ch,ts))
    {

        printf("%c",ch);
        // ���յ�����״̬
        switch(sWaitPara->RecStaue)
        {
            // ���տ���״̬
            case Idel:
            {
                sWaitPara->RecStaue = Fram_Head;
                sWaitPara->get_len = 0;
                sWaitPara->data_len = 0;
                sWaitPara->len = 0;
                
            }
            // ��������ͷ
            case Fram_Head:
            {
                if(ch == FRAM_HEAD_DATA)
                {
                    RecBuf[sWaitPara->len++] = ch;
                    sWaitPara->RecStaue = Fram_Ctrl;
                }
                else
                {
                    ret = ERR_FRAM_HEAD;
                    sWaitPara->RecStaue = Idel;
                }
                break;
            }
            // ���տ���λ
            case Fram_Ctrl:
            {
                if(ch&0xF0)
                    
                {
                    RecBuf[sWaitPara->len++] = ch;
                    sWaitPara->RecStaue = Fram_Len;
                }
                else
                {
                    ret = ERR_FRAM_CTRL;
                    sWaitPara->RecStaue = Idel;
                }
                break;
            }
            // �������ݳ���
            case Fram_Len:
            {
                RecBuf[sWaitPara->len++] = ch;
//                get_len |= get_len<<0x08 | ch;
                sWaitPara->get_len = FunUnionTwoByte(sWaitPara->get_len,ch);
//                printf("g%d",get_len);
                sWaitPara->data_len++;
                if(sWaitPara->data_len == 2)
                {
                    sWaitPara->RecStaue = Fram_Data;
                    sWaitPara->data_len = 0;
                }
                break;
            }
            // ��������
            case Fram_Data:
            {
                if(sWaitPara->get_len != 0)
                {
                    RecBuf[sWaitPara->len++] = ch;
                    sWaitPara->data_len++;
                    if(sWaitPara->data_len == sWaitPara->get_len)
                    {
                        sWaitPara->RecStaue = Fram_Crc;
                        sWaitPara->data_len = 0;
                        sWaitPara->get_len = 0;
                    }
                    break;
                }
            }
            // ����У����
            case Fram_Crc:
            {
                RecBuf[sWaitPara->len++] = ch;
                sWaitPara->data_len++;
// ��ʱ����
#if 0
                //=====================================================
                // ����У׼
                //=====================================================
                PL_CommandCheck(&RecBuf[4],get_len,&check_result);
                if(check_result == ch)
                {
                    RecStaue = Fram_Tail;
                    data_len = 0;
                }
                else
                {
                    ret = ERR_CMD_CHECK;
                    RecStaue   = Idel;
                }
#endif
                if(sWaitPara->data_len == 1)
                {
                    sWaitPara->RecStaue = Fram_Tail;
                    sWaitPara->data_len = 0;
                }
                
                break;
            }
            // ��������β
            case Fram_Tail:
            {
                if(ch == FRAM_TAIL_DATA)
                {
                    RecBuf[sWaitPara->len++] = ch;
                    sWaitPara->RecStaue   = Idel;
                    return sWaitPara->len;
                }
                else
                {
                    ret = ERR_FRAM_TAIL;
                    sWaitPara->RecStaue   = Idel;
                }
                break;
            }
            default:
                break;
        }
    }

    return ret;
//#endif
}

/***************************************************************************************
** �������ƣ�void Mid_Task(void *p_arg)
** �������ܣ��м䴦�����񣬽����������ʹ��
** ��    ����p_arg: ���ڴ���������ʱ���ݵ��β�
** �� �� ֵ����
***************************************************************************************/
void Mid_Task(void *p_arg)
{
    OS_ERR err;
    UINT16 len = 0;
    UINT8 *Mid_msg_buf;
    OS_TICK ts;
    UINT32 i = 0;
    (void)p_arg;
    s_MidWaitPata.data_len = 0;
    s_MidWaitPata.get_len = 0;
    s_MidWaitPata.len = 0;
    s_MidWaitPata.RecStaue = Idel;
    while(1)
    {
        // �ź���������Post���Σ��ͻ�Pend����
        OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,&ts,&err);
        
        //-------------------------------------------------------------
        //  ���ڽ�������
        if(queue_len_query(sComCtrl.sComConfig[MAIN_COM].Com_MemBuf))
        {
            Recieve_Flag = COM_RECIEVE_FLAG;
            Mid_msg_buf = sComCtrl.sComConfig[MAIN_COM].Com_MemBuf;
        }
#ifdef TCP_IP_ENABLE 
        //  ���ڽ�������
        else if(queue_len_query(Mid_eth_buf))
        {
            Recieve_Flag = ETH_RECIEVE_FLAG;
            Mid_msg_buf = Mid_eth_buf;
        }
#endif
        

#if 1
        // CAN�ڽ������ݱ�־
        else if(queue_len_query(sCanCtrl.sCanConfig.canRecieveBuf))
        {
            Recieve_Flag = CAN_RECIEVE_FLAG;
            Mid_msg_buf = sCanCtrl.sCanConfig.canRecieveBuf;
        }
        
#endif
//        printf("send:");
        //==============================================================
        // �����������
//        OSMutexPend (&TEST_MUTEX,0,OS_OPT_PEND_BLOCKING,0,&err);	//���󻥳��ź���
        while(queue_len_query(Mid_msg_buf))
        {
            // �ȴ��������ݣ����յȴ�ʱ��Ϊ50ms��can���յ�ʱ���������ӳ٣���֮��������ִ�к��жϣ�����ʱΪ50ms
            if((len = Que_WaitFram(Mid_msg_buf,RecieveBuf,&s_MidWaitPata,1)) != 0)
            {
                if(RecieveBuf[1] & FRAM_CTRL_DATA_ERR)
                {
//                RecieveBuf[1] |= Recieve_Flag;
//                sCanCtrl.sCanFunList.canSendBuf(RecieveBuf,len);
                    // ����������Ϣ
                    Handle_Msg(RecieveBuf,len);
                }

            }
#if 0
            else
            {
                sys_msg.err = ERR_CMD;
                sys_msg.err_pos = Sys_Errpos;

                Mid_ReBack_Fill(&sys_msg,FRAM_CTRL_DATA_S_M);
            }
#endif            
        }
//        OSMutexPost(&TEST_MUTEX,OS_OPT_POST_NONE,&err);				//�ͷŻ����ź���

    }   
}

#endif
