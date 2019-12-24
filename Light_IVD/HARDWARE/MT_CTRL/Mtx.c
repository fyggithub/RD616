/*********************************************************************************
**********************************************************************************
**********************************************************************************
* 文件名称: Mtx.c                                                          *
* 文件简述：                                                      *
* 创建日期：2014.10.03                                                          *
* 版    本：V1.13                                                               *
* 作    者：LY Chen                                                             *
* 说    明： 步进电机算法                                         * 
**********************************************************************************
*********************************************************************************/
#ifndef __MTX_C__
#define __MTX_C__

#include "My_includes.h"
UINT8 mtRunIndex = 0x00;

UINT8 Mtx_DriverCtrl(UINT8 MtxId, UINT8 state)
{
    
    if(state == e_true)
    {
        if(Mtx_Phase_Idel(MtxId))
            return ERR_MOTOR_PARA;
        if(Mtx_Phase_Load(MtxId))
            return ERR_MOTOR_PARA;
        MtxFunList[MtxId].pinDriverUnlock(MtxId);
        MtxFunList[MtxId].tmrCountOn(MtxId);
        mtRunIndex = 0x1<<MtxId;
        //======================================
        // 开启电机的脉冲发送定时器
        //======================================
//       TIM_Cmd(TIM6, ENABLE);
        
    }
    else
    {
        MtxFunList[MtxId].pinDriverLock(MtxId);
        MtxFunList[MtxId].tmrCountOff(MtxId);
    }
    return ERR_NONE;
}

// 计算加速度和减加速的值
UINT8 MtxCaculateAccDec(UINT8 MtxId)
{
    UINT32 nDel = 0;
    UINT32 nH   = 0;

    // 计算加速阶段加速度变化值
    if(MtxPara[MtxId].nFreqMax <= MtxPara[MtxId].nFreqMin)
    {
        MtxPara[MtxId].nAccMax = 0;
        MtxPara[MtxId].nAccAcc = 0;
        MtxPara[MtxId].nAccDec = 0;
        MtxPara[MtxId].nDecMax = 0;
        MtxPara[MtxId].nDecAcc = 0;
        MtxPara[MtxId].nDecDec = 0;
    }
    else
    {
        nDel = MtxPara[MtxId].nFreqMax - MtxPara[MtxId].nFreqMin;

        if((MtxPara[MtxId].nStepsT1 == 0) &&
           (MtxPara[MtxId].nStepsT2 == 0) && 
           (MtxPara[MtxId].nStepsT3 == 0))
        {
            MtxPara[MtxId].nAccMax = 0;
            MtxPara[MtxId].nAccAcc = 0;
            MtxPara[MtxId].nAccDec = 0;
        }
        else
        {
            // 最大加速值
            nH = (nDel<<11)/(2*MtxPara[MtxId].nStepsT2 + MtxPara[MtxId].nStepsT1 + MtxPara[MtxId].nStepsT3);
            MtxPara[MtxId].nAccMax = nH;

            // 加加速阶段加速的比值
            if(MtxPara[MtxId].nStepsT1 != 0)
                MtxPara[MtxId].nAccAcc = MtxPara[MtxId].nAccMax/MtxPara[MtxId].nStepsT1;
            else
                MtxPara[MtxId].nAccAcc = 0;

            // 加减速阶段加速度的比值
            if(MtxPara[MtxId].nStepsT3 != 0)
                MtxPara[MtxId].nAccDec = MtxPara[MtxId].nAccMax/MtxPara[MtxId].nStepsT3;
            else
                MtxPara[MtxId].nAccDec = 0;
        }

        // 计算减速阶段的减速值
        if((MtxPara[MtxId].nStepsT5 == 0) &&
           (MtxPara[MtxId].nStepsT6 == 0) && 
           (MtxPara[MtxId].nStepsT7 == 0))
        {
            MtxPara[MtxId].nDecMax = 0;
            MtxPara[MtxId].nDecAcc = 0;
            MtxPara[MtxId].nDecDec = 0;
        }
        else
        {
            // 最大减速值
            nH = (nDel<<11)/(2*MtxPara[MtxId].nStepsT6 + MtxPara[MtxId].nStepsT5 + MtxPara[MtxId].nStepsT7);
            MtxPara[MtxId].nDecMax = nH;

            // 减加速阶段的减速比值
            if(MtxPara[MtxId].nStepsT5 != 0)
                MtxPara[MtxId].nDecAcc = MtxPara[MtxId].nDecMax/MtxPara[MtxId].nStepsT5;
            else
                MtxPara[MtxId].nDecAcc = 0;

            // 减减速阶段的减速比值
            if(MtxPara[MtxId].nStepsT7 != 0)
                MtxPara[MtxId].nDecDec = MtxPara[MtxId].nDecMax/MtxPara[MtxId].nStepsT7;
            else
                MtxPara[MtxId].nDecDec = 0;
        }
    }
    if(set_debug)
    {
        printf("Mt%d nAccMax = %d  nAccAcc = %d  nAccDec = %d\r\n",MtxId,MtxPara[MtxId].nAccMax,MtxPara[MtxId].nAccAcc,MtxPara[MtxId].nAccDec);
        printf("Mt%d nDecMax = %d  nDecAcc = %d  nDecDec = %d\r\n",MtxId,MtxPara[MtxId].nDecMax,MtxPara[MtxId].nDecAcc,MtxPara[MtxId].nDecDec);
    }
    return e_true;
}


/****************************************************
    |       _______
    |      /       \
    |     /         \
    |    /           \
    |   /             \
    |  /               \
    | /                 \
    |/___________________\
******************************************************/
// 计算加减速的频率
UINT16 MtxCountFreqAccDec(UINT8 MtxId, UINT16 *AccSteps, UINT16 *DecSteps)
{
    UINT32 TCount = 0, t = 1, Count = 0, Hacc = 0;
    UINT16 CurFreq = MtxPara[MtxId].nFreqMin;
    UINT32 AddFreq = 0, MaxFreq = 0;

    UINT16 T1 = MtxCtrl[MtxId].mSteps1;
    UINT16 T2 = MtxCtrl[MtxId].mSteps1 + MtxCtrl[MtxId].mSteps2;
    UINT16 T3 = MtxCtrl[MtxId].mSteps1 + MtxCtrl[MtxId].mSteps2 + MtxCtrl[MtxId].mSteps3;
    UINT32 HaccMax = T1*MtxPara[MtxId].nAccAcc;

    UINT16 T5 = MtxCtrl[MtxId].mSteps5;
    UINT16 T6 = MtxCtrl[MtxId].mSteps5 + MtxCtrl[MtxId].mSteps6;
    UINT16 T7 = MtxCtrl[MtxId].mSteps5 + MtxCtrl[MtxId].mSteps6 + MtxCtrl[MtxId].mSteps7;
    UINT32 HdecMax = T5*MtxPara[MtxId].nDecAcc;
    if(set_debug)
    {
        printf("T1 = %d\r\n",T1);
        printf("T2 = %d\r\n",T2);
        printf("T3 = %d\r\n",T3);
        printf("T5 = %d\r\n",T5);
        printf("T6 = %d\r\n",T6);
        printf("T7 = %d\r\n",T7);
        printf("HaccMax = %d\r\n",HaccMax);
        printf("HdecMax = %d\r\n",HdecMax);
        printf("Mtx_Timer_Freq = %d\r\n",TimConfig[MtxId].Mtx_Timer_Freq);
    }
    // 计算加速过程的频率变化
    while(1)
    {
        if(t < T1)
        {
            Hacc += MtxPara[MtxId].nAccAcc;
            AddFreq = Hacc>>10;
            CurFreq += AddFreq;
        }
        else if(t < T2)
        {
            CurFreq += AddFreq;
        }
        else if(t < T3)
        {
            if(Hacc < MtxPara[MtxId].nAccDec)
            {
                Hacc = 0;
            }
            else
                Hacc -= MtxPara[MtxId].nAccDec;
            AddFreq = Hacc>>10;
            CurFreq += AddFreq;
        }
        else
            break;
        
        
        // 先把ARR 值计算完成，后面直接加载进入
        MtxPara[MtxId].nAccTimArr[Count] = TimConfig[MtxId].Mtx_Timer_Freq / CurFreq;
        if(set_debug)
        {
            printf("%d freq %d  arr %d\r\n",Count,CurFreq,MtxPara[MtxId].nAccTimArr[Count]);
        }
        t++;
        Count++;
        
        // 判断加减速的步数是否大于存储空间
//        if(Count > ARR_BUF_LEN)
//        {
//            return ERR_MOTOR_ARR_BUF_LEN | MtxId<<0x08;
//        }
        
    }
    // 加速步数赋值
    *AccSteps = Count;
    Count = 0;
    t = 1;
    // 减速的最大频率为加速后的频率
    MaxFreq = CurFreq;

    // 计算加速过程的频率变化
    while(1)
    {
        if(t < T5)
        {
            Hacc += MtxPara[MtxId].nDecAcc;
            AddFreq = Hacc>>10;
            CurFreq -= AddFreq;
        }
        else if(t < T6)
        {
            CurFreq -= AddFreq;
        }
        else if(t < T7)
        {
            if(Hacc < MtxPara[MtxId].nDecDec)
            {
                Hacc = 0;
            }
            else
                Hacc -= MtxPara[MtxId].nDecDec;
            AddFreq = Hacc>>10;
            CurFreq -= AddFreq;
        }
        else
            break;
        
        // 先把ARR 值计算完成，后面直接加载进入
        MtxPara[MtxId].nDecTimArr[Count] = TimConfig[MtxId].Mtx_Timer_Freq / CurFreq;
        if(set_debug)
        {
            printf("%d freq %d  arr %d\r\n",Count,CurFreq,MtxPara[MtxId].nDecTimArr[Count]);
        }
        t++;
        Count++;
        
        // 判断加减速的步数是否大于存储空间
//        if(Count > ARR_BUF_LEN)
//        {
//            return ERR_MOTOR_ARR_BUF_LEN | MtxId<<0x08;
//        }
    }
    *DecSteps = Count;
    return ERR_NONE;
}

void Mtx_Pwm_ISR(UINT8 MtxId)
{
    MtxFunList[MtxId].tmrInterruptOff(MtxId);
    MtxFunList[MtxId].tmrInterruptClear(MtxId);

    switch(MtxCtrl[MtxId].ePhase)
    {
        case eMtx_Phase_ACC:
            if(Mtx_Phase_Acc(MtxId) != TRUE)
                break;
        case eMtx_Phase_EQU:
            if(Mtx_Phase_Equ(MtxId) != TRUE)
                break;
        case eMtx_Phase_DEC:
            if(Mtx_Phase_Dec(MtxId) != TRUE)
                break;
        case eMtx_Phase_FIN:
            if(Mtx_Phase_Fin(MtxId) != TRUE)
                break;
        default:
            MtxCtrl[MtxId].mErrCode = ERR_MOTOR_PARA;
            MtxCtrl[MtxId].mRunFlag = 0;
            break;
    }
    MtPhasePwm(MtxId);
    MtxFunList[MtxId].tmrInterruptOn(MtxId);
    
}

BOOL Mtx_Phase_Idel(UINT8 MtxId)
{
    MtxFlag[MtxId].mt_Clk = 0;
    if(MtxCtrl[MtxId].mRunFlag == 0)
        return FALSE;
    if((MtxCtrl[MtxId].mStepsCur >= MtxPara[MtxId].nMaxPos) && (MtxCtrl[MtxId].mRunMode != RST_TIGGER_RUN_MODE)
        && (MtxCtrl[MtxId].eDirection == eMtx_Dir_NEGA))
    {
        MtxCtrl[MtxId].mErrCode = ERR_MOTOR_OVER_DISTANCE;
        MtxCtrl[MtxId].mRunFlag = 0;
        return FALSE;
    }
    else if((MtxCtrl[MtxId].mStepsCur == 0) && (MtxCtrl[MtxId].mRunMode != RST_TIGGER_RUN_MODE)
        && (MtxCtrl[MtxId].eDirection == eMtx_Dir_POSI))
    {
        MtxCtrl[MtxId].mErrCode = ERR_MOTOR_OVER_DISTANCE;
        MtxCtrl[MtxId].mRunFlag = 0;
        return FALSE;
    }
    return TRUE;
}

BOOL Mtx_Phase_Load(UINT8 MtxId)
{
    MtxCtrl[MtxId].ePhase         = eMtx_Phase_ACC;

    MtxCtrl[MtxId].mStepsRun      = 0;
    MtxCtrl[MtxId].mTimeLoad      = MtxPara[MtxId].nFreqMin<<10;
    MtxCtrl[MtxId].mStepsTiger    = 0;
    MtxCtrl[MtxId].mTimeLoadIndex = 0;
    
    // 考虑用一个定时器用做脉冲触发，先预留，等查看资源是否足够，如果不够就改为直接在定时器中断中实现脉冲
//    MT_CLK_EANBLE();
    //获取复位光耦状态
    MtxFlag[MtxId].mtRstFlag = MtxFunList[MtxId].mtResetRead(MtxId);
    if(MtxCtrl[MtxId].mRunMode == RST_TIGGER_RUN_MODE)
        MtxCtrl[MtxId].mStepsCur = 0;

    // 码盘检测
    if(MtxPara[MtxId].nLightCheck & MT_CODE_LIGHT_ENABLE)
    {
        MtxFlag[MtxId].mtCodeFlag  = MtxFunList[MtxId].mtCodeRead(MtxId);
        MtxFlag[MtxId].mtCodeCount = 0;
    }

    // 到位光耦的检测
    if(MtxPara[MtxId].nLightCheck & MT_ARRIVE_LIGHT_ENABLE)
    {
        if((MtxCtrl[MtxId].eDirection == eMtx_Dir_NEGA) && (MtxFunList[MtxId].mtArriveRead(MtxId)))
        {
            MtxCtrl[MtxId].mRunFlag = 0;
            return FALSE;
        }
        MtxFlag[MtxId].mtArriveCount = 0;
    }
    
    // 碰撞光耦的检测
    if(MtxPara[MtxId].nLightCheck & MT_CRASH_LIGHT_ENABLE)
    {
        if((MtxCtrl[MtxId].eDirection == eMtx_Dir_NEGA) && (!MtxFunList[MtxId].mtCrashRead(MtxId)))
        {
            MtxCtrl[MtxId].mRunFlag = 0;
            return FALSE;
        }
        MtxFlag[MtxId].mtCrashCount = 0;
    }
    MtxFunList[MtxId].tmrLoad(MtxId, MtxPara[MtxId].nAccTimArr[MtxCtrl[MtxId].mTimeLoadIndex]);
    return TRUE;
        
}

BOOL Mtx_Phase_Acc(UINT8 MtxId)
{
    if((MtxCtrl[MtxId].mStepsRun < MtxCtrl[MtxId].mStepsAcc) && (MtxCtrl[MtxId].mStepsTotal > MtxCtrl[MtxId].mStepsAcc))
    {
        MtxCtrl[MtxId].mTimeLoad = MtxPara[MtxId].nAccTimArr[MtxCtrl[MtxId].mTimeLoadIndex];
        MtxCtrl[MtxId].mTimeLoadIndex++;
        
        return FALSE;
    }
    else
    {
        MtxCtrl[MtxId].ePhase         = eMtx_Phase_EQU;
        MtxCtrl[MtxId].mTimeLoadIndex = 0;
        return TRUE;
    }
}

BOOL Mtx_Phase_Equ(UINT8 MtxId)
{
    if((MtxCtrl[MtxId].mStepsTotal - MtxCtrl[MtxId].mStepsRun) > MtxCtrl[MtxId].mStepsDec)
    {
        
        MtxCtrl[MtxId].mTimeLoad = MtxCtrl[MtxId].mTimeLoad;
        return FALSE;
    }
    else
    {
        MtxCtrl[MtxId].ePhase         = eMtx_Phase_DEC;
        MtxCtrl[MtxId].mTimeLoadIndex = 0;
        return TRUE;
    }
}

BOOL Mtx_Phase_Dec(UINT8 MtxId)
{
    if((MtxCtrl[MtxId].mStepsTotal - MtxCtrl[MtxId].mStepsRun))
    {
        MtxCtrl[MtxId].mTimeLoad = MtxPara[MtxId].nDecTimArr[MtxCtrl[MtxId].mTimeLoadIndex];
        MtxCtrl[MtxId].mTimeLoadIndex++;
        
        return FALSE;
    }
    else
    {
        MtxCtrl[MtxId].ePhase         = eMtx_Phase_FIN;
        MtxCtrl[MtxId].mTimeLoadIndex = 0;
        return TRUE;
    }
}

BOOL Mtx_Phase_Fin(UINT8 MtxId)
{
    #if SYSTEM_SUPPORT_OS
    OS_ERR err;
    #endif
    if(MtxCtrl[MtxId].mRunMode == RST_TIGGER_RUN_MODE)
        if((MtxCtrl[MtxId].mStepsTiger == 0) && (!MtxCtrl[MtxId].mErrCode))
            MtxCtrl[MtxId].mErrCode = ERR_MOTOR_NOT_RESET;
    #if (TIM_MTx_PWM==0)
    MtxFunList[MtxId].pinPulseLow(MtxId);
    #endif
    MtxFunList[MtxId].mtDriverCtrl(MtxId,e_false);
    MtxCtrl[MtxId].ePhase = eMtx_Phase_IDEL; 
//        MtxFunList[MtxId].mtTimPlusCtrl(DISABLE);

    #if SYSTEM_SUPPORT_OS
    OSFlagPost(&MtxEventFlags,1<<MtxId,OS_OPT_POST_FLAG_SET,&err);//设置对应的信号量为1
    #endif
    return FALSE;
}

_STA_ UINT16 MtxCountDecArrNum(UINT8 MtxId,UINT16 accArr)
{
    UINT16 accFreq = 0;
    UINT16 freqValve = MtxPara[MtxId].nFreqMax - MtxPara[MtxId].nFreqMin;
    UINT16 decAccFreq,conFreq,decDecFreq;
    UINT16 perFreq;
    UINT16 loadIndex = 0;
    
    perFreq = (freqValve<<1)/MtxCtrl[MtxId].mStepsDec;

    decAccFreq = perFreq*MtxCtrl[MtxId].mSteps5;
    conFreq = perFreq*MtxCtrl[MtxId].mSteps6;
    decDecFreq = perFreq*MtxCtrl[MtxId].mSteps7;
    
    
    accFreq = TimConfig[MtxId].Mtx_Timer_Freq / accArr - MtxPara[MtxId].nFreqMin;
    
    if(accFreq < decDecFreq)
    {
        loadIndex = accFreq /  perFreq;
        loadIndex += MtxCtrl[MtxId].mSteps5+MtxCtrl[MtxId].mSteps6;
    }
    else if(accFreq < (decDecFreq + conFreq))
    {
        loadIndex = (accFreq - decDecFreq)/perFreq;
        loadIndex += MtxCtrl[MtxId].mSteps5;
    }
    else
    {
        loadIndex = (accFreq - decDecFreq - conFreq)/perFreq;
    }
    return loadIndex;

}

BOOL MtPhasePwm(UINT8 MtxId)
{
    
    
    if(MtxCtrl[MtxId].mRunFlag)
    {
        #if (TIM_MTx_PWM==0)
        MtxFunList[MtxId].pinPulseHigh(MtxId);
        #endif
//        MtxFlag[MtxId].mt_Clk = 1;
        // 更新Arr值
        MtxFunList[MtxId].tmrLoad(MtxId, MtxCtrl[MtxId].mTimeLoad);
        MtxCtrl[MtxId].mStepsRun++;
//        MtxCtrl[MtxId].mStepsCur++;

        // 运行步数的判断
        if(MtxCtrl[MtxId].mRunMode != RST_TIGGER_RUN_MODE)
        {
            if(MtxCtrl[MtxId].eDirection == eMtx_Dir_NEGA)
            {
                MtxCtrl[MtxId].mStepsCur++;
                if(MtxCtrl[MtxId].mStepsCur >= MtxPara[MtxId].nMaxPos)
                {
                    MtxCtrl[MtxId].mErrCode = ERR_MOTOR_OVER_DISTANCE;
                    MtxCtrl[MtxId].mRunFlag = 0;
                }
            }
            else if(MtxCtrl[MtxId].eDirection == eMtx_Dir_POSI)
            {
                if(MtxCtrl[MtxId].mStepsCur == 0)
                {
                    MtxCtrl[MtxId].mErrCode = ERR_MOTOR_OVER_DISTANCE;
                    MtxCtrl[MtxId].mRunFlag = 0;
                }
                else
                    MtxCtrl[MtxId].mStepsCur--;
            }
        }

//        #if 0
        // 复位光耦处理，进过复位光耦后走的步数
        if(MtxFunList[MtxId].mtResetRead(MtxId) != MtxFlag[MtxId].mtRstFlag)
            MtxCtrl[MtxId].mStepsTiger++;
        else
        {
            // 过了触发信号后还发现光耦检测出错，则光耦异常或者电机运动异常(可能电机运行方向错乱)(盘电机除外)
            if(MtxCtrl[MtxId].mStepsTiger > MT_TIGGER_CHECK_TIME)
            {
                if(MtxPara[MtxId].nIsCircle)
                    MtxCtrl[MtxId].mStepsTiger++;
                else
                {
                    MtxCtrl[MtxId].mErrCode = ERR_MOTOR_RESET_TIGGER;
                    MtxCtrl[MtxId].mRunFlag = 0;
                }
            }
            else
                MtxCtrl[MtxId].mStepsTiger = 0;
        }

        //----------------------------------------------------------
        // 复位动作，触发光耦后走的步数，最好大于减速的步数
        if(MtxCtrl[MtxId].mRunMode == RST_TIGGER_RUN_MODE)
        {
            if(MtxCtrl[MtxId].mStepsTiger == MT_TIGGER_CHECK_TIME)
                MtxCtrl[MtxId].mStepsTotal = MtxCtrl[MtxId].mStepsRun + MtxCtrl[MtxId].mRunTigerPost - MT_TIGGER_CHECK_TIME;
//            if(MtxCtrl[MtxId].ePhase == eMtx_Phase_ACC)
//            {
//                MtxCtrl[MtxId].ePhase = eMtx_Phase_DEC;
//                MtxCtrl[MtxId].mTimeLoadIndex = MtxCountDecArrNum(MtxId,MtxCtrl[MtxId].mTimeLoad);
//            }
            
        }

        //-----------------------------------------------------------
        // 到位光耦检测，向到位光耦运行才会进行检测，复位运动不进行检测
        if(MtxPara[MtxId].nLightCheck & MT_ARRIVE_LIGHT_ENABLE)
        {
            if(MtxCtrl[MtxId].eDirection == eMtx_Dir_NEGA)
            {
                if(MtxFunList[MtxId].mtArriveRead(MtxId))
                {
                    MtxFlag[MtxId].mtArriveCount++;
                    if(MtxFlag[MtxId].mtArriveCount == MT_TIGGER_CHECK_TIME)
                        MtxCtrl[MtxId].mStepsTotal = MtxCtrl[MtxId].mStepsRun + MtxPara[MtxId].nArrive - MT_TIGGER_CHECK_TIME;
                    
                }
                else
                {
                    //已经触发后还检测到无到位光耦，则出错。
                    if(MtxFlag[MtxId].mtArriveCount > MT_TIGGER_CHECK_TIME)
                    {
                        MtxCtrl[MtxId].mErrCode = ERR_MOTOR_ARRIVE_TIGGER;
                        MtxCtrl[MtxId].mRunFlag = 0;
                    }
                    else
                        MtxFlag[MtxId].mtArriveCount = 0;
                }
            }
        }

        //-----------------------------------------------------------
        // 码盘光耦检测
        if(MtxPara[MtxId].nLightCheck & MT_CODE_LIGHT_ENABLE)
        {
            if(MtxFlag[MtxId].mtCodeFlag != MtxFunList[MtxId].mtCodeRead(MtxId))
            {
                MtxFlag[MtxId].mtCodeCount++;
                MtxFlag[MtxId].mtCodeNum++;
                if(MtxFlag[MtxId].mtCodeNum == MT_TIGGER_CHECK_TIME)
                {
                    MtxFlag[MtxId].mtCodeCount = 0;
                    MtxFlag[MtxId].mtCodeFlag = MtxFunList[MtxId].mtCodeRead(MtxId);

                    if(MtxFlag[MtxId].mtCodeFlag == 0)
                        MtxFlag[MtxId].mtCodeNum = 0;
                    else if(MtxFlag[MtxId].mtCodeFlag == 1)
                        MtxFlag[MtxId].mtCodeNum = 1;
                }
            }
            else
            {
                MtxFlag[MtxId].mtCodeCount = 0;
            }
            //已经触发后还检测到无到位光耦，则出错。
            if(MtxFlag[MtxId].mtCodeCount > MtxPara[MtxId].nCode)
            {
                MtxCtrl[MtxId].mErrCode = ERR_MOTOR_CODE_TIGGER;
                MtxCtrl[MtxId].mRunFlag = 0;
            }
        }
        
        //-----------------------------------------------------------
        // 碰撞光耦检测，向碰撞光耦运行才会进行检测，复位运动不进行检测
        if(MtxPara[MtxId].nLightCheck & MT_CRASH_LIGHT_ENABLE)
        {
            if(MtxCtrl[MtxId].eDirection == eMtx_Dir_NEGA)
            {
                if(!MtxFunList[MtxId].mtCrashRead(MtxId))
                {
                    MtxFlag[MtxId].mtCrashCount++;
                    if(MtxFlag[MtxId].mtCrashCount == MT_TIGGER_CHECK_TIME)
                    {
                        MtxCtrl[MtxId].mErrCode = ERR_MOTOR_CRASH;
                        MtxCtrl[MtxId].mRunFlag = 0;
                    }
                }
            }
        }
        #if (TIM_MTx_PWM==0)
        MtxFunList[MtxId].pinPulseLow(MtxId);
        #endif
        if(MtxCtrl[MtxId].mStepsTotal == MtxCtrl[MtxId].mStepsRun)
            MtxCtrl[MtxId].mRunFlag = 0;
//        #endif
    }
    if(!MtxCtrl[MtxId].mRunFlag)
    {
        Mtx_Phase_Fin(MtxId);
    }
    return TRUE;
}

#endif
