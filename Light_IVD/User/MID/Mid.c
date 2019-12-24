#ifndef __MID_C__
#define __MID_C__

#include <My_includes.h>

UINT8 set_debug = 0;

UINT8 Sys_Errcode = 0;// 系统错误码
UINT8 Sys_Errpos  = 0;       // 默认错误位置

UINT8 Mid_usart_buf[MSG_BUF_LEN] = {0};     // 串口接收数据缓存
UINT8 Mid_can_buf[MSG_BUF_LEN] = {0};       // Can接收数据缓存

UINT8 RecieveBuf[MSG_BUF_LEN] = {0};        // 数据缓存

UINT8 ReBack_que_buf[REBACK_BUF_LEN] = {0}; // 返回数据缓存

UINT8 Recieve_Flag = 0;                     // 接收数据表示

st_QueWaitPara s_MidWaitPata;

APPInfoType gApp;

/***************************************************************************************
** 函数名称：UINT8 PL_CommandCheck(UINT8* pStr, UINT16 nLen, UINT8* pResult)
** 函数功能：校验命令
** 参    数：*pStr: 输入数据
              nLen: 数据长度
             *pResult: 返回结果
** 返 回 值：e_true
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
** 函数名称：void Sys_Init(void)
** 函数功能：系统信息、硬件等初始化
** 参    数：无
** 返 回 值：无
***************************************************************************************/
void Sys_Init(void)
{
    UINT8 i = 0;
    
    // 开启Bootload使用
#if IS_BOOTLOAD_ENABLE
    //=======================================================================
    // bootload启动读取更新信息
    STMFLASH_Read(FLASH_APP_FLAG_ADDR,(unsigned long *)&gApp,sizeof(APPInfoType)>>2);         // 更新标志
    // 判断ID号是否正确，否则重新烧写程序
    if(gApp.ID != CAN_ID)
    {
        gApp.Flag = APP_IS_ID_ERR;     // 程序不存在
        gApp.ByteCounts = 0;
        
        STMFLASH_Write(FLASH_APP_FLAG_ADDR,(unsigned long *)&gApp,sizeof(APPInfoType)>>2);         // 更新标志
        Sys_Delay_ms(10);
        SoftReset();
    }
    gApp.Flag = APP_IS_VALID;     // 程序生效
    gApp.ByteCounts = 0;
    STMFLASH_Write(FLASH_APP_FLAG_ADDR,(unsigned long *)&gApp,sizeof(APPInfoType)>>2);         // 更新标志
#endif
    
    // 串口配置初始化
    Com_Initialization(MAIN_COM,Mid_usart_buf,MSG_BUF_LEN);
    
    // Can配置初始化
    Can_Initialization(Mid_can_buf,MSG_BUF_LEN);
    
    // LED初始化
    Led_Initialization();
    LK_PinAllInit();
    Waste_Light_PinInit();
    // 电机初始化
    Mtx_Init(E_TIM_0);
//    Mtx_Init(E_TIM_1);
//    Mtx_Init(E_TIM_2);
//    Mtx_Init(E_TIM_3);
    Enter_Cup_Check_Light();
    Spring_Pin_Init();
    // 外部存储初始化
    FRAME_INIT();
    
    printf("Bulider Wang\r\n");
    printf("Proble\r\n");

    // 各模块信息初始化
    Msg_Init();
    
    // 加载系统参数
    Load_SysPara();
    
}


/***************************************************************************************
** 函数名称：UINT8 Que_WaitFram(UINT8* Que_Msgbuf,UINT8 *RecFram,UINT16 ts)
** 函数功能：从队列中获取命令
** 参    数：*Que_Msgbuf：队列地址
             *RecBuf：命令数据缓存
             ts：等待时间
** 返 回 值：无
***************************************************************************************/
UINT16 Que_WaitFram(UINT8* Que_Msgbuf,UINT8 *RecBuf,st_QueWaitPara *sWaitPara, UINT16 ts)
{
    UINT8 ch = 0;
    UINT8 ret = ERR_NONE;
    UINT8 check_result = 0;
    
    
    while(queue_out_byte(Que_Msgbuf,&ch,ts))
    {

        printf("%c",ch);
        // 接收的数据状态
        switch(sWaitPara->RecStaue)
        {
            // 接收空闲状态
            case Idel:
            {
                sWaitPara->RecStaue = Fram_Head;
                sWaitPara->get_len = 0;
                sWaitPara->data_len = 0;
                sWaitPara->len = 0;
                
            }
            // 接收数据头
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
            // 接收控制位
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
            // 接收数据长度
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
            // 接收数据
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
            // 接收校验码
            case Fram_Crc:
            {
                RecBuf[sWaitPara->len++] = ch;
                sWaitPara->data_len++;
// 暂时屏蔽
#if 0
                //=====================================================
                // 增加校准
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
            // 接收数据尾
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
** 函数名称：void Mid_Task(void *p_arg)
** 函数功能：中间处理任务，进行任务分配使用
** 参    数：p_arg: 是在创建该任务时传递的形参
** 返 回 值：无
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
        // 信号量被处理Post几次，就会Pend几次
        OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,&ts,&err);
        
        //-------------------------------------------------------------
        //  串口接收数据
        if(queue_len_query(sComCtrl.sComConfig[MAIN_COM].Com_MemBuf))
        {
            Recieve_Flag = COM_RECIEVE_FLAG;
            Mid_msg_buf = sComCtrl.sComConfig[MAIN_COM].Com_MemBuf;
        }
#ifdef TCP_IP_ENABLE 
        //  网口接收数据
        else if(queue_len_query(Mid_eth_buf))
        {
            Recieve_Flag = ETH_RECIEVE_FLAG;
            Mid_msg_buf = Mid_eth_buf;
        }
#endif
        

#if 1
        // CAN口接收数据标志
        else if(queue_len_query(sCanCtrl.sCanConfig.canRecieveBuf))
        {
            Recieve_Flag = CAN_RECIEVE_FLAG;
            Mid_msg_buf = sCanCtrl.sCanConfig.canRecieveBuf;
        }
        
#endif
//        printf("send:");
        //==============================================================
        // 处理接收数据
//        OSMutexPend (&TEST_MUTEX,0,OS_OPT_PEND_BLOCKING,0,&err);	//请求互斥信号量
        while(queue_len_query(Mid_msg_buf))
        {
            // 等待队列数据，接收等待时间为50ms，can接收的时候有数据延迟，加之其他任务执行和中断，故延时为50ms
            if((len = Que_WaitFram(Mid_msg_buf,RecieveBuf,&s_MidWaitPata,1)) != 0)
            {
                if(RecieveBuf[1] & FRAM_CTRL_DATA_ERR)
                {
//                RecieveBuf[1] |= Recieve_Flag;
//                sCanCtrl.sCanFunList.canSendBuf(RecieveBuf,len);
                    // 处理命令信息
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
//        OSMutexPost(&TEST_MUTEX,OS_OPT_POST_NONE,&err);				//释放互斥信号量

    }   
}

#endif
