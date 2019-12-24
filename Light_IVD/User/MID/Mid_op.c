#ifndef __MID_OP_C__
#define __MID_OP_C__

#include <My_includes.h>

st_DataMsg sys_msg;           // 系统命令参数信息
st_DataMsg io_msg;            // 泵阀命令参数信息
st_DataMsg ec_msg;            // 试剂盘命令参数信息
st_DataMsg para_msg;          // 存储数据命令参数信息

tMtxPara    MtxPara[MTx_NUM];
tMtxFunList MtxFunList[MTx_NUM];
tMtxCtrl    MtxCtrl[MTx_NUM];
tMtxFlag    MtxFlag[MTx_NUM];
tMtxCtrlPinConfig MtxCtrlPin[MTx_NUM];

UINT8 Sys_Msg_Data[16] = {0};       // 系统命令数据存储
UINT8 Io_Msg_Data[16] = {0};        // 泵阀命令数据存储
UINT8 Ec_Msg_Data[32] = {0};        // 试剂盘命令数据存储
UINT8 Para_Msg_Data[64] = {0};      // 存储数据命令数据存储

/***************************************************************************************
** 函数名称：void SoftReset(void)
** 函数功能：系统复位
** 参    数：无
** 返 回 值：无
***************************************************************************************/
void SoftReset(void)
{
    __set_FAULTMASK(1);         // 关闭所有中断
    NVIC_SystemReset();         // 复位
}


// 参数信息初始化
void Msg_Init(void)
{
//    sys_msg.err_pos = SYS_CMD;
//    pv_msg.err_pos = PV_CMD;
//    sp_msg.err_pos = SP_CMD;
//    para_msg.err_pos = PARA_CMD;
    
    sys_msg.st_dataBuf = Sys_Msg_Data;
    io_msg.st_dataBuf = Io_Msg_Data;
    ec_msg.st_dataBuf = Ec_Msg_Data;
    para_msg.st_dataBuf = Para_Msg_Data;
}

/***************************************************************************************
** 函数名称：void Task_Call(st_TaskCtrlType *s_taskctrl,UINT8 *send_buf,UINT16 send_len,UINT8 event_Type,UINT8 rec_Flag)
** 函数功能：任务调用函数
** 参    数：s_taskctrl 任务信息结构体
            send_buf 发送的数据
            send_len 发送的数据长度
            event_Type 数据类型
            rec_Flag 接收标志
** 返 回 值：无
***************************************************************************************/
void Task_Call(st_TaskCtrlType *s_taskctrl,UINT8 *send_buf,UINT16 send_len,UINT8 event_Type,UINT8 rec_Flag)
{
    OS_ERR os_err;
    // 控制位增加接收标志
    send_buf[1] = (send_buf[1] & 0xF0) | rec_Flag;
    // 事件类型
    send_buf[2] = event_Type;
    // 数据入队
    queue_in(s_taskctrl->st_taskQueBuf,send_buf,send_len);
    // 调用任务
    OSTaskSemPost(s_taskctrl->st_taskTcb,OS_OPT_POST_NONE,&os_err);
}

BOOL Get_DataMsg(UINT8 *que_buf,st_DataMsg * s_dataM, st_QueWaitPara *sWaitPara)
{
    UINT8 ch = 0;
    UINT8 ret = ERR_NONE;
    UINT8 check_result = 0;
    
//    return queue_out(Que_Msgbuf,RecBuf,256,1);
//    
//#if 0
    while(queue_out_byte(que_buf,&ch,1))
    {
//        if(set_debug)
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
                s_dataM->st_dataSendId = 0;
            }
            // 接收数据头
            case Fram_Head:
            {
                if(ch == FRAM_HEAD_DATA)
                {
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
                    s_dataM->st_dataSendId = ch & 0x0F;    // 获取接收ID号
                    sWaitPara->RecStaue = Fram_Len;
                    s_dataM->st_dataLen = 0;
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
                s_dataM->st_dataLen = FunUnionTwoByte(s_dataM->st_dataLen,ch);
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
                if(s_dataM->st_dataLen)
                {
                    if(!sWaitPara->data_len)
                    {
                        s_dataM->st_dataCmd = ch;
                    }
                    else
                        s_dataM->st_dataBuf[sWaitPara->data_len-1] = ch;
                    sWaitPara->data_len++;
                    if(sWaitPara->data_len == s_dataM->st_dataLen)
                    {
                        sWaitPara->RecStaue = Fram_Crc;
                        sWaitPara->data_len = 0;
                    }
                    break;
                }
            }
            // 接收校验码
            case Fram_Crc:
            {
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
                    sWaitPara->RecStaue   = Idel;
                    s_dataM->st_dataErrNum = ERR_NONE;
                    return TRUE;
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
    s_dataM->st_dataErrNum = ret;
    return FALSE;
}

// 处理系统命令
UINT8 Handle_Sys_Cmd(void)
{
    UINT8 ret;
    UINT32 t = 0;
    switch(sys_msg.st_dataCmd)
    {
        case CMD_SYS_HANDLE:
        {
            if(sys_msg.st_dataLen != 0)
            {
                sys_msg.st_dataErrNum = ERR_FRAM_LENGTH;
                sys_msg.st_dataLen = 2;
                break;
            }
            sys_msg.st_dataBuf[0] = SYSTEM_VERSION;
            sys_msg.st_dataLen = 1;
            break;
        }
        // 系统版本号
        case CMD_SYS_VERSION:
        {
            if(sys_msg.st_dataLen != 0)
            {
                sys_msg.st_dataErrNum = ERR_FRAM_LENGTH;
                sys_msg.st_dataLen = 0;
                break;
            }
            sys_msg.st_dataBuf[0] = VERSION_NAME;
            sys_msg.st_dataBuf[1] = SYSTEM_VERSION;
            sys_msg.st_dataLen = 2;
            break;
        }

        case CMD_ENTER_BOOTLOAD:
        {
            if(sys_msg.st_dataLen != 0)
            {
                sys_msg.st_dataErrNum = ERR_FRAM_LENGTH;
                sys_msg.st_dataLen = 2;
                break;
            }
            gApp.ByteCounts = 0;
            gApp.Flag = APP_IS_INVALID;
            sys_msg.st_dataLen = 0;
            STMFLASH_Write(FLASH_APP_FLAG_ADDR,(unsigned long *)&gApp,sizeof(APPInfoType)>>2); 
            Sys_Delay_ms(10);
            SoftReset();
            break;
        }
        case CMD_SYS_DEBUG:
        {
            if(sys_msg.st_dataLen != 1)
            {
                sys_msg.st_dataErrNum = ERR_FRAM_LENGTH;
                sys_msg.st_dataLen = 2;
                break;
            }
            set_debug = sys_msg.st_dataBuf[0];
            sys_msg.st_dataLen = 0;
            break;
        }
            
        default:
            break;
    }
    Mid_ReBack_Fill(&sys_msg,FRAM_CTRL_DATA_S_M);
    return ret;
}

UINT8 Handle_Io_Cmd(void)
{
    UINT8 ret;
    UINT32 t = 0;
    switch(io_msg.st_dataCmd)
    {
        case CMD_LIGHT_STATE_CHECK:
        {
            if(io_msg.st_dataLen != 1)
            {
                io_msg.st_dataErrNum = ERR_FRAM_LENGTH;
                io_msg.st_dataLen = 2;
                break;
            }
            switch(io_msg.st_dataBuf[0])
            {
                case e_EC_udRest:
                {
                    io_msg.st_dataBuf[1] = Fun_MtxResetGetState(E_TIM_0);
                    break;
                }
                case e_EC_upCheck:
                {
                    io_msg.st_dataBuf[1] = Get_UpLight_State();
                    break;
                }
                case e_EC_downCheck:
                {
                    io_msg.st_dataBuf[1] = Get_DowmLight_State();
                    break;
                }
                case e_EC_moveCheck:
                {
                    io_msg.st_dataBuf[1] = Get_MoveLight_State();
                    break;
                }
                case e_WC_1_haveCheck:
                {
                    io_msg.st_dataBuf[1] = Get_WasteCup_Light_State(0);
                    break;
                }
                case e_WC_2_haveCheck:
                {
                    io_msg.st_dataBuf[1] = Get_WasteCup_Light_State(1);
                    break;
                }
                case e_WC_1_fullCheck:
                {
                    io_msg.st_dataBuf[1] = Get_WasteCup_Light_State(2);
                    break;
                }
                case e_WC_2_fullCheck:
                {
                    io_msg.st_dataBuf[1] = Get_WasteCup_Light_State(3);
                    break;
                }
            }
            
            io_msg.st_dataLen = 2;
            break;
        }
        case CMD_LED_STATE_CTRL:
        {
             if(io_msg.st_dataLen != 2)
            {
                io_msg.st_dataErrNum = ERR_FRAM_LENGTH;
                io_msg.st_dataLen = 2;
                break;
            }
            s_lkFlag.led_state[io_msg.st_dataBuf[0]] = io_msg.st_dataBuf[1];
            io_msg.st_dataLen = 0;
            break;
        }
        default:
            break;
    }
    Mid_ReBack_Fill(&io_msg,FRAM_CTRL_DATA_S_M);
    return ret;
}

/***************************************************************************************
** 函数名称：UINT8 Handle_Msg(UINT8 *Recieve_buf, UINT16 msg_len)
** 函数功能：处理和分配命令
** 参    数：Recieve_buf：命令缓存地址
              msg_len：命令数据长度
** 返 回 值：无
***************************************************************************************/
UINT8 Handle_Msg(UINT8 *Recieve_buf, UINT16 msg_len)
{
    OS_ERR err;
    UINT16 data_len = 0;
    UINT8  Cmd = 0;
    UINT8  op_cmd = 0;
    UINT8  ret = ERR_NONE;
    //==================================================
    // 接收数据的判断，如果接收的数据参数小于7，则为命令接收错误
    //==================================================
    if(msg_len<7)
    {
        ret = msg_len;
    }
    else
    {
        data_len = (Recieve_buf[2]<<8) | Recieve_buf[3];
        Cmd = Recieve_buf[4];
        op_cmd = Cmd>>4;
        switch(op_cmd)
        {
            //=======================================
            // ÏµÍ³¿ØÖÆÃüÁî
            //=======================================
            case SYS_CMD:
            {
                sys_msg.st_dataCmd = Cmd;
                sys_msg.st_dataLen = data_len-1;
                sys_msg.st_dataSendId = Recieve_buf[1] & 0x0F;      // 获取ID号
                memcpy(sys_msg.st_dataBuf,&Recieve_buf[5],sys_msg.st_dataLen);
                Handle_Sys_Cmd();
                break;
            }
            case IO_CMD:
            {
                io_msg.st_dataCmd = Cmd;
                io_msg.st_dataLen = data_len-1;
                io_msg.st_dataSendId = Recieve_buf[1] & 0x0F;      // 获取ID号
                memcpy(io_msg.st_dataBuf,&Recieve_buf[5],io_msg.st_dataLen);
                Handle_Io_Cmd();
                break;
            }
            case PARA_CMD:
            {
                para_msg.st_dataCmd = Cmd;
                para_msg.st_dataLen = data_len-1;
                para_msg.st_dataSendId = Recieve_buf[1] & 0x0F;      // 获取ID号
                memcpy(para_msg.st_dataBuf,&Recieve_buf[5],para_msg.st_dataLen);
                Handle_Para_Cmd();
                break;
            }
            // 进杯器命令
            case EC_CMD:
            {
                queue_in(EC_QueBuf,Recieve_buf,msg_len);
#if SYSTEM_SUPPORT_OS

                OSTaskSemPost(&EnterCupCtrlTaskTCB,OS_OPT_POST_NONE,&err);             
#else
                sp_msg.st_dataCmd = Cmd;
                sp_msg.st_dataLen = data_len-1;
                memcpy(sp_msg.st_dataBuf,&Recieve_buf[5],sp_msg.st_dataLen);
    //                Handle_RT_Cmd();
                Handle_EC_Cmd();
#endif
                break;
            }
            default:
                break;
        }
      
    }
    return e_true;
}



#endif

