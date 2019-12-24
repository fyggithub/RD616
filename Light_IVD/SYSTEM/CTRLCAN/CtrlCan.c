#ifndef __CTRLCAN_C__
#define __CTRLCAN_C__
/********************************************************************************
writer : ZH Wang
time   : 2017-10-31
*********************************************************************************/
#include "CONFIG\My_define.h"
#include "CONFIG\My_Type.h"

// �ж��Ƿ�֧��OSϵͳ
#if SYSTEM_SUPPORT_OS
#include "includes.h"
#include  <os.h>
#include "CONFIG\My_cfg.h"
#endif

#include "STM_SYS\Stm_Sys.h"
#include "CTRLCAN\CtrlCan.h"
#include "stdio.h"
#include "QUEUE\queue.h"
#include "LED\led.h"


st_CanCtrlModule  sCanCtrl;


UINT32 FUN_CAN_Get_ID(void)
{
    sCanCtrl.sCanConfig.canID = CAN_ID;

    return sCanCtrl.sCanConfig.canID;
}


UINT8 FUN_CAN_Init(void)
{
    RCC_ClocksTypeDef       RCC_Clocks;
    GPIO_InitTypeDef 		GPIO_InitStructure; 
	CAN_InitTypeDef        	CAN_InitStructure;
	CAN_FilterInitTypeDef  	CAN_FilterInitStructure;
	NVIC_InitTypeDef  		NVIC_InitStructure;
    
#if defined (STM32F40_41xxx)
    RCC_AHB1PeriphClockCmd(CAN1_TX_GPIO_CLK | CAN1_RX_GPIO_CLK, ENABLE);//ʹ��PORTAʱ��	                   											 
#else
    RCC_APB2PeriphClockCmd(CAN1_TX_GPIO_CLK | CAN1_RX_GPIO_CLK, ENABLE);//ʹ��PORTAʱ��	                   											 
#endif
    
	RCC_APB1PeriphClockCmd(CAN1_CLK, ENABLE);//ʹ��CAN1ʱ��	

    // CAN �ŵ�ʹ�ܽ�
#if CAN_EN_SET
    RCC_APB2PeriphClockCmd(CAN1_EN_GPIO_CLK, ENABLE);
#if defined (STM32F40_41xxx)
    GPIO_InitStructure.GPIO_Pin = CAN1_EN_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//����
    GPIO_Init(CAN1_EN_GPIO_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(CAN1_EN_GPIO_PORT,CAN1_EN_GPIO_PIN);
#else	
    GPIO_InitStructure.GPIO_Pin = CAN1_EN_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(CAN1_EN_GPIO_PORT, &GPIO_InitStructure);
    CAN1_EN_GPIO_PORT->BRR = CAN1_EN_GPIO_PIN;  
#endif
#endif

    GPIO_InitStructure.GPIO_Pin = CAN1_TX_GPIO_PIN;
#if defined (STM32F40_41xxx)
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
#else
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//��������
#endif
	GPIO_Init(CAN1_TX_GPIO_PORT, &GPIO_InitStructure);			//��ʼ��IO

	GPIO_InitStructure.GPIO_Pin = CAN1_RX_GPIO_PIN;
#if defined (STM32F40_41xxx)
    
#else
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//��������
#endif
	GPIO_Init(CAN1_RX_GPIO_PORT, &GPIO_InitStructure);			//��ʼ��IO
    
#if defined (STM32F40_41xxx)
    //���Ÿ���ӳ������
    GPIO_PinAFConfig(CAN1_RX_GPIO_PORT,CAN1_RX_PINSOURCE,GPIO_AF_CAN1); //GPIOA11����ΪCAN1
    GPIO_PinAFConfig(CAN1_TX_GPIO_PORT,CAN1_TX_PINSOURCE,GPIO_AF_CAN1); //GPIOA12����ΪCAN1
#else
    // configuie AF pins
    if(CAN1_PINREMAP_EN)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
        GPIO_PinRemapConfig(GPIO_Remap1_CAN1, ENABLE);
    }
#endif
    
    //CAN��Ԫ����
	CAN_InitStructure.CAN_TTCM=DISABLE;			//��ʱ�䴥��ͨ��ģʽ  
	CAN_InitStructure.CAN_ABOM=ENABLE;			//����Զ����߹���	 
	CAN_InitStructure.CAN_AWUM=DISABLE;			//˯��ģʽͨ���������(���CAN->MCR��SLEEPλ)
	CAN_InitStructure.CAN_NART=DISABLE;			//��ֹ�����Զ����� 
	CAN_InitStructure.CAN_RFLM=ENABLE;		 	//���Ĳ�����,�µĸ��Ǿɵ�  
	CAN_InitStructure.CAN_TXFP=ENABLE;			//���ȼ��ɱ��ı�ʶ������ 
	CAN_InitStructure.CAN_Mode= CAN_Mode_Normal;	        //ģʽ���ã� mode:0,��ͨģʽ;1,�ػ�ģʽ; 

    //���ò�����   36000/[(CAN_BS1+CAN_BS2+1)*CAN_Prescaler]
	CAN_InitStructure.CAN_SJW=CAN1_SJW;				//����ͬ����Ծ���(Tsjw)Ϊtsjw+1��ʱ�䵥λ  CAN_SJW_1tq	 CAN_SJW_2tq CAN_SJW_3tq CAN_SJW_4tq
	CAN_InitStructure.CAN_BS1=CAN1_BS1; 			//Tbs1=tbs1+1��ʱ�䵥λCAN_BS1_1tq ~CAN_BS1_16tq
	CAN_InitStructure.CAN_BS2=CAN1_BS2;				//Tbs2=tbs2+1��ʱ�䵥λCAN_BS2_1tq ~	CAN_BS2_8tq
	CAN_InitStructure.CAN_Prescaler=CAN1_BPR;        //��Ƶϵ��(Fdiv)Ϊbrp+1	
	CAN_Init(CTRLCAN1, &CAN_InitStructure);        	//��ʼ��CAN1 
    
    // ����CAN��Ƶ��
    RCC_GetClocksFreq(&RCC_Clocks);
    sCanCtrl.sCanConfig.canBpr = (CAN1_BS1+1+CAN1_BS2+1+1)*CAN1_BPR;
    sCanCtrl.sCanConfig.canBpr = (RCC_Clocks.PCLK1_Frequency)/sCanCtrl.sCanConfig.canBpr;
    
    // �ж�æ״̬ ����11�����Ե�ƽ����Ϊ��æ���жϺĵ�ָ����24����ѭ�������������¡�
    sCanCtrl.sCanConfig.canBusyNum = (RCC_Clocks.SYSCLK_Frequency/sCanCtrl.sCanConfig.canBpr)*11/24;
    printf("Sys_Clk = %d, Pclk1 = %d\r\n",RCC_Clocks.SYSCLK_Frequency,RCC_Clocks.PCLK1_Frequency);
    printf("Can_Bpr = %d\r\n",sCanCtrl.sCanConfig.canBpr);
    printf("CAN_BusyNum = %d\r\n",sCanCtrl.sCanConfig.canBusyNum);
    
//    FUN_CAN_Get_ID();
    // filter channels can1:1~19 , can2:20~27
    CAN_SlaveStartBank(1);
    
    CAN_FilterInitStructure.CAN_FilterNumber=0;	//������0
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 	//����λģʽ
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; 	//32λ�� 
	CAN_FilterInitStructure.CAN_FilterIdHigh=(UINT16)((CAN_ID << 5) | 0x0000);	//32λID
	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0xFFE0;//32λMASK
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//������0������FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;//���������0

	CAN_FilterInit(&CAN_FilterInitStructure);			//�˲�����ʼ��
	
    // �����������ж�
#if 0
    // �����ж�
	CAN_ITConfig(CTRLCAN1,CAN_IT_TME,ENABLE);				//FIFO0��Ϣ�Һ��ж�����.		    

	NVIC_InitStructure.NVIC_IRQChannel = USB_HP_CAN1_TX_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;     // �����ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;            // �����ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif
    // ---FMP0
    CAN_ITConfig(CTRLCAN1,CAN_IT_FMP0,ENABLE);				//FIFO0��Ϣ�Һ��ж�����.		    
    CAN_ITConfig(CTRLCAN1,CAN_IT_FF0,ENABLE);
    CAN_ITConfig(CTRLCAN1,CAN_IT_FOV0,ENABLE);
    
#if defined (STM32F40_41xxx)
    NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
#else
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
#endif
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;     // �����ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;            // �����ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
    
    // ---SCE
    CAN_ITConfig(CTRLCAN1,CAN_IT_EWG,ENABLE);				//FIFO0��Ϣ�Һ��ж�����.		    
    CAN_ITConfig(CTRLCAN1,CAN_IT_EPV,ENABLE);
    CAN_ITConfig(CTRLCAN1,CAN_IT_BOF,ENABLE);
    CAN_ITConfig(CTRLCAN1,CAN_IT_LEC,ENABLE);
    CAN_ITConfig(CTRLCAN1,CAN_IT_ERR,ENABLE);
    
	NVIC_InitStructure.NVIC_IRQChannel = CAN1_SCE_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;     // �����ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;            // �����ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
    return e_true;
}

UINT8 FUN_CAN_ClearFlagTI(void)
{
    CAN_ClearITPendingBit(CTRLCAN1,CAN_IT_TME);
    return e_true;
}

UINT8 FUN_CAN_ClearFlagRI(void)
{
//    CAN_ClearITPendingBit(CTRLCAN1,CAN_IT_FMP0);
    return e_true;
}

UINT8 FUN_CAN_CheckFlagTI(void)
{
    return (RESET == CAN_GetITStatus(CTRLCAN1, CAN_IT_TME))? 0: 1;
}

UINT8 FUN_CAN_CheckFlagRI(void)
{
    return (RESET == CAN_GetITStatus(CTRLCAN1, CAN_IT_FMP0))? 0: 1;
}

UINT8 FUN_CAN_CheckFlagTXE(void)
{
    return (((CTRLCAN1->TSR&CAN_TSR_TME0) == CAN_TSR_TME0) ||\
            ((CTRLCAN1->TSR&CAN_TSR_TME1) == CAN_TSR_TME1) ||\
            ((CTRLCAN1->TSR&CAN_TSR_TME2) == CAN_TSR_TME2))? 0: 1;
}

UINT8 FUN_CAN_IntTxEnable(void)
{
    CAN_ITConfig(CTRLCAN1,CAN_IT_TME,ENABLE);
    return e_true;
}


UINT8 FUN_CAN_IntRxEnable(void)
{
    CAN_ITConfig(CTRLCAN1,CAN_IT_FMP0,ENABLE);				//FIFO0��Ϣ�Һ��ж�����.		    
    CAN_ITConfig(CTRLCAN1,CAN_IT_FF0,ENABLE);
    CAN_ITConfig(CTRLCAN1,CAN_IT_FOV0,ENABLE);
    return e_true;
}

UINT8 FUN_CAN_IntTxDisable(void)
{
    CAN_ITConfig(CTRLCAN1,CAN_IT_TME,DISABLE);
    return e_true;
}

UINT8 FUN_CAN_IntRxDisable(void)
{
    CAN_ITConfig(CTRLCAN1,CAN_IT_FMP0,DISABLE);				//FIFO0��Ϣ�Һ��ж�����.		    
    CAN_ITConfig(CTRLCAN1,CAN_IT_FF0,DISABLE);
    CAN_ITConfig(CTRLCAN1,CAN_IT_FOV0,DISABLE);
    return e_true;
}

UINT8 FUN_CAN_MsgTx(tCANMsgTx *ptMsgTx)
{
    UINT8 ch;
    CanTxMsg TxMessage;
    
	TxMessage.StdId=ptMsgTx->Id;			// ��׼��ʶ�� 
	TxMessage.ExtId=ptMsgTx->Id;			// ������չ��ʾ�� 
	TxMessage.IDE=CAN_Id_Standard; 	// ��׼֡
	TxMessage.RTR=CAN_RTR_Data;		// ����֡
	TxMessage.DLC=ptMsgTx->DLC;				// Ҫ���͵����ݳ���
	for(ch=0;ch<8;ch++)
        TxMessage.Data[ch]=ptMsgTx->Data[ch];			          
	ch= CAN_Transmit(CAN1, &TxMessage);   

	return e_true;	 
}

UINT8 FUN_CAN_MsgRx(tCANMsgRx *ptMsgRx)
{
    UINT8 ch;
	CanRxMsg RxMessage;
    
    CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);//��ȡ����	
    
    ptMsgRx->Id = RxMessage.ExtId;
    ptMsgRx->DLC = RxMessage.DLC;
    ptMsgRx->FMI = RxMessage.FMI;
    
    for(ch=0;ch<8;ch++)
        ptMsgRx->Data[ch]=RxMessage.Data[ch];  
	return e_true;	
}

UINT8 FUN_CAN_MonitorRXP(void)
{
    //--------------------------------------------------------------
    // overrun
    if(SET == CAN_GetITStatus(CTRLCAN1, CAN_IT_FOV0))
    {
        CAN_ClearITPendingBit(CTRLCAN1, CAN_IT_FOV0);
    }
    
    //--------------------------------------------------------------
    // full
    if(SET == CAN_GetITStatus(CTRLCAN1, CAN_IT_FF0))
    {
        CAN_ClearITPendingBit(CTRLCAN1, CAN_IT_FF0);
    }
    return e_true;
}

UINT8 FUN_CAN_MonitorSCE(void)
{
    printf("\r\n");
    printf("---LEC:%d---\r\n",CAN_GetLastErrorCode(CTRLCAN1)>>4);
    printf("---REC:%d---\r\n",CAN_GetReceiveErrorCounter(CTRLCAN1));
    printf("---TEC:%d---\r\n",CAN_GetLSBTransmitErrorCounter(CTRLCAN1));
    printf("\r\n");
    
    // ���CAN�жϱ������IT_ERR���߻�һֱ��������ж���
    CAN_ITConfig(CTRLCAN1,CAN_IT_ERR,DISABLE);

    // WKU
    if(SET == CAN_GetITStatus(CTRLCAN1, CAN_IT_WKU))
    {
        CAN_ClearITPendingBit(CTRLCAN1, CAN_IT_WKU);
        printf("INT-WKU\r\n");
    }
    
    // WKU
    if(SET == CAN_GetITStatus(CTRLCAN1, CAN_IT_SLK))
    {
        CAN_ClearITPendingBit(CTRLCAN1, CAN_IT_SLK);
        printf("INT-SLK\r\n");
    }
    
    // EWG
    if(SET == CAN_GetITStatus(CTRLCAN1, CAN_IT_EWG))
    {
        CAN_ClearITPendingBit(CTRLCAN1, CAN_IT_EWG);
        printf("INT-EWG\r\n");
    }
    
    // EPV
    if(SET == CAN_GetITStatus(CTRLCAN1, CAN_IT_EPV))
    {
        CAN_ClearITPendingBit(CTRLCAN1, CAN_IT_EPV);
        printf("INT-EPV\r\n");
    }
    
    // BOF
    if(SET == CAN_GetITStatus(CTRLCAN1, CAN_IT_BOF))
    {
        CAN_ClearITPendingBit(CTRLCAN1, CAN_IT_BOF);
        printf("INT-BOF\r\n");
    }
    
    // LEC
    if(SET == CAN_GetITStatus(CTRLCAN1, CAN_IT_LEC))
    {
        CAN_ClearITPendingBit(CTRLCAN1, CAN_IT_LEC);
        printf("INT-LEC\r\n");
    }
    
    // ERR
    if(SET == CAN_GetITStatus(CTRLCAN1, CAN_IT_ERR))
    {
        CAN_ClearITPendingBit(CTRLCAN1, CAN_IT_ERR);
        printf("INT-ERR\r\n");
    }
    return e_true;
}

void Config_Can_Fun(void)
{
    sCanCtrl.sCanFunList.canInit            = FUN_CAN_Init;
    sCanCtrl.sCanFunList.canCheckFlagRI     = FUN_CAN_CheckFlagRI;
    sCanCtrl.sCanFunList.canCheckFlagTI     = FUN_CAN_CheckFlagTI;
    sCanCtrl.sCanFunList.canCheckFlagTXE    = FUN_CAN_CheckFlagTXE;
    sCanCtrl.sCanFunList.canClearFlagRI     = FUN_CAN_ClearFlagRI;
    sCanCtrl.sCanFunList.canClearFlagTI     = FUN_CAN_ClearFlagTI;
    sCanCtrl.sCanFunList.canIntRxDisable    = FUN_CAN_IntRxDisable;
    sCanCtrl.sCanFunList.canIntRxEnable     = FUN_CAN_IntRxEnable;
    sCanCtrl.sCanFunList.canIntTxDisable    = FUN_CAN_IntTxDisable;
    sCanCtrl.sCanFunList.canIntTxEnable     = FUN_CAN_IntTxEnable;
    sCanCtrl.sCanFunList.canMonitorRXP      = FUN_CAN_MonitorRXP;
    sCanCtrl.sCanFunList.canMonitorSCE      = FUN_CAN_MonitorSCE;
    sCanCtrl.sCanFunList.canMsgRx           = FUN_CAN_MsgRx;
    sCanCtrl.sCanFunList.canMsgTx           = FUN_CAN_MsgTx;
    sCanCtrl.sCanFunList.canSendBuf         = FUN_CAN_SendBuf;
}

BOOL Can_Initialization(UINT8 *can_buf,UINT16 can_len)
{
    Config_Can_Fun();
    sCanCtrl.sCanConfig.canRecieveBuf = can_buf;
    sCanCtrl.sCanConfig.canRecieveLen = can_len;
    sCanCtrl.sCanFunList.canInit();
    
    queue_inital(sCanCtrl.sCanConfig.canRecieveBuf, sCanCtrl.sCanConfig.canRecieveLen);
    return TRUE;
}


void CAN_IsrRXPHandling(void)
{
    UINT8 chReturn = e_true;
    UINT8 chNum = 0;
    _STA_ UINT32 i = 0;
    tCANMsgRx RxMessage;
#if SYSTEM_SUPPORT_OS
    OS_ERR err;
    CPU_SR_ALLOC();
    
    OS_CRITICAL_ENTER();	//�����ٽ���
    
#endif
    sCanCtrl.sCanFunList.canMonitorRXP();

    while(1 == sCanCtrl.sCanFunList.canCheckFlagRI())
    {
        sCanCtrl.sCanFunList.canClearFlagRI();
        sCanCtrl.sCanFunList.canIntRxDisable();
        
        sCanCtrl.sCanFunList.canMsgRx(&RxMessage);
//        i++;
//        printf("i=%d\r\n",i);
        chReturn = queue_in(sCanCtrl.sCanConfig.canRecieveBuf, (UINT8 *)RxMessage.Data,RxMessage.DLC);
        sCanCtrl.sCanFunList.canIntRxEnable();
    }

#if SYSTEM_SUPPORT_OS

    // �ȴ�����Ϊ׼��״̬
    if(MAIN_CAN_REC_TASKTCB.TaskState == OS_TASK_STATE_PEND)
    {
        // �����ź���
        sCanCtrl.sCanConfig.can_Receive_Flag = 1;
        OSTaskSemPost(&MAIN_CAN_REC_TASKTCB,OS_OPT_POST_NONE,&err);
        if(err)
            printf("err %d",err);
        
    }
//    
    OS_CRITICAL_EXIT();	        //�˳��ٽ���
//    printf("St3 %d\r\n",MAIN_CAN_REC_TASKTCB.TaskState);
#else
    sCanCtrl.sCanConfig.can_Receive_Flag = 1;
#endif
}

UINT8 Can_BusyCheck(void)
{
    UINT16 time = 0,t = 0;
    UINT32 tx = 0,rx = 0;
#if CAN_BUSY_DEBUG
    LED0 = 1;
#endif
//    for(time = 0;time<sCanCtrl.sCanConfig.canBusyNum;time++)                // for��ѭ��һ��Ϊ8��ָ��
    {
        if(!((CAN1_TX_GPIO_PORT->IDR) && (0x1800)))
            printf("tx ODR 0x%x, tx IDR 0x%x, rx ODR 0x%x, rx IDR0x%x\r\n",(CAN1_TX_GPIO_PORT->ODR),(CAN1_TX_GPIO_PORT->IDR),(CAN1_RX_GPIO_PORT->ODR),(CAN1_RX_GPIO_PORT->IDR));
//        if(!(((CAN1_TX_GPIO_PORT->ODR & CAN1_TX_GPIO_PIN)?1:0) && ((CAN1_RX_GPIO_PORT->IDR & CAN1_RX_GPIO_PIN)?1:0)))       // 11
//        {
//            t++;                                // 1
//            time = 0;                           // 1
//            if(t==0xFFF)                        // 3
//            {
//                printf("The Can bus is busy\r\n");
//                return e_false;
//            }
//        }
    }
#if CAN_BUSY_DEBUG
    LED0 = 0;
#endif
    
    return e_true;
}

UINT8 FUN_CAN_SendBuf(UINT8* CanSendBuf, UINT16 len)
{
    UINT8 ch = 0;
    UINT8 chBreak = 0;
    UINT8 chReturn = e_true;
    UINT8 chNum = 0;
    UINT8 *pBuf = CanSendBuf;
    tCANMsgTx tTxMessage;
    
    // ��ȡҪ���͵�ID��
    if(sCanCtrl.sCanConfig.canID == 0)
        sCanCtrl.sCanConfig.canID = MAIN_CAN_ID;
    tTxMessage.Id  = sCanCtrl.sCanConfig.canID;//FunUnionTwoByte(CanSendBuf[3],CanSendBuf[4]);
    
//    CanSendBuf[3] = (sCanCtrl.sCanConfig.canID>>8) & 0xff;
//    CanSendBuf[4] = sCanCtrl.sCanConfig.canID & 0xff;
    sCanCtrl.sCanFunList.canIntTxDisable();
//    chBreak = 0;
    
    // �ж��Ƿ�Ϊæ״̬
//    if(Can_BusyCheck() != e_true)
//        return FALSE;
    while(len)
    {
        chNum = 0;
        while(chNum<8)
        {
            ch = *pBuf++;
            tTxMessage.Data[chNum++] = ch;
            len--;
            if(len == 0)
                break;
        }
        if(chNum)
        {
            // wait empty
            while(sCanCtrl.sCanFunList.canCheckFlagTXE());

            
            tTxMessage.DLC = chNum;
            chReturn = sCanCtrl.sCanFunList.canMsgTx(&tTxMessage);
            
//            Fun_LedToggle(led_1);
        }
    }
    return TRUE;
}


void CAN1_SCE_IRQHandler(void)
{
    sCanCtrl.sCanFunList.canMonitorSCE();
}

#if defined (STM32F40_41xxx)
void CAN1_RX0_IRQHandler(void)
#else
void USB_LP_CAN1_RX0_IRQHandler(void)
#endif
{
#if SYSTEM_SUPPORT_OS
    OSIntEnter();
#endif
    CAN_IsrRXPHandling();
#if SYSTEM_SUPPORT_OS
    OSIntExit();
#endif
//    Fun_LedToggle(led_1);
}

#endif

