#ifndef __MOTOR_C__
#define __MOTOR_C__

#include "My_includes.h"

/********************************************************************************
 ** 函数功能:判定不同计算步数规则
 ** 输入参数:参数一：枚举类型相当于整数与电机定时器相关联（0-3）-->eMtx_ID_X(1234)
             参数二：要走的总步数（传参进来）
 ** 返 回 值: 无
 ** 说    明：无
 *******************************************************************************/
//计算步数规则
UINT8 MT_CalculateStepRuler(eMtxID MtxId, UINT32 nSteps)
{
    UINT16 adsteps, adSteps0, adSteps1, adSteps2;
    UINT16 acc_freq, dec_freq;

    MtxCtrl[MtxId].mStepsTotal = nSteps;

    // 计算曲线加减速所需要的总步数
    adsteps = MtxPara[MtxId].nStepsT1 + MtxPara[MtxId].nStepsT2 + MtxPara[MtxId].nStepsT3 + 
              MtxPara[MtxId].nStepsT5 + MtxPara[MtxId].nStepsT6 + MtxPara[MtxId].nStepsT7;

    if(nSteps >= adsteps)
    {
        MtxCtrl[MtxId].mSteps1 = MtxPara[MtxId].nStepsT1;
        MtxCtrl[MtxId].mSteps2 = MtxPara[MtxId].nStepsT2;
        MtxCtrl[MtxId].mSteps3 = MtxPara[MtxId].nStepsT3;
        MtxCtrl[MtxId].mSteps5 = MtxPara[MtxId].nStepsT5;
        MtxCtrl[MtxId].mSteps6 = MtxPara[MtxId].nStepsT6;
        MtxCtrl[MtxId].mSteps7 = MtxPara[MtxId].nStepsT7;
    }
    // 加减速的步数不够就使用5阶加速法。
    else
    {
        adSteps0 = MtxPara[MtxId].nStepsT1 +  MtxPara[MtxId].nStepsT3 +
                   MtxPara[MtxId].nStepsT5 + MtxPara[MtxId].nStepsT7;
        if(nSteps >= adSteps0)
        {
            MtxCtrl[MtxId].mSteps1 = MtxPara[MtxId].nStepsT1;
            MtxCtrl[MtxId].mSteps2 = 0;
            MtxCtrl[MtxId].mSteps3 = MtxPara[MtxId].nStepsT3;
            MtxCtrl[MtxId].mSteps5 = MtxPara[MtxId].nStepsT5;
            MtxCtrl[MtxId].mSteps6 = 0;
            MtxCtrl[MtxId].mSteps7 = MtxPara[MtxId].nStepsT7;
        }
        else
        {
            adSteps1 = (nSteps * (MtxPara[MtxId].nStepsT1 + MtxPara[MtxId].nStepsT3)) / adSteps0;//5阶加速步数
            adSteps2 = (nSteps * (MtxPara[MtxId].nStepsT5 + MtxPara[MtxId].nStepsT7)) / adSteps0;//5阶减速步数
            
            MtxCtrl[MtxId].mSteps1 = (adSteps1 * MtxPara[MtxId].nStepsT1) / (MtxPara[MtxId].nStepsT1 + MtxPara[MtxId].nStepsT3);
            MtxCtrl[MtxId].mSteps2 = 0;
            MtxCtrl[MtxId].mSteps3 = (adSteps1 * MtxPara[MtxId].nStepsT3) / (MtxPara[MtxId].nStepsT1 + MtxPara[MtxId].nStepsT3);
            MtxCtrl[MtxId].mSteps5 = (adSteps1 * MtxPara[MtxId].nStepsT5) / (MtxPara[MtxId].nStepsT5 + MtxPara[MtxId].nStepsT7);
            MtxCtrl[MtxId].mSteps6 = 0;
            MtxCtrl[MtxId].mSteps7 = (adSteps1 * MtxPara[MtxId].nStepsT7) / (MtxPara[MtxId].nStepsT5 + MtxPara[MtxId].nStepsT7);
        }
    }
    if(set_debug)
    {
        //函数功能:计算加减速的频率    其实相当于求面积
        printf("mSteps1 = %d\r\n",MtxCtrl[MtxId].mSteps1);
        printf("mSteps2 = %d\r\n",MtxCtrl[MtxId].mSteps2);
        printf("mSteps3 = %d\r\n",MtxCtrl[MtxId].mSteps3);
        printf("mSteps5 = %d\r\n",MtxCtrl[MtxId].mSteps5);
        printf("mSteps6 = %d\r\n",MtxCtrl[MtxId].mSteps6);
        printf("mSteps7 = %d\r\n",MtxCtrl[MtxId].mSteps7);
       
    }
    MtxCountFreqAccDec(MtxId, &MtxCtrl[MtxId].mStepsAcc , &MtxCtrl[MtxId].mStepsDec);
    if(set_debug)
    {
        printf("mStepsAcc %d , mStepsDec %d\r\n",MtxCtrl[MtxId].mStepsAcc,MtxCtrl[MtxId].mStepsDec);
        printf("mStepsTotal %d\r\n",MtxCtrl[MtxId].mStepsTotal);
    }
}


/********************************************************************************
 ** 函数功能:延时检测  需要考虑是延时还是UcosIII 还是定时器中断的模式。
 ** 输入参数:参数一：枚举类型相当于整数与电机定时器相关联（0-3）-->eMtx_ID_X(1234)
             参数二：超时计数
 ** 返 回 值:错误代码
 ** 说    明：无
 *******************************************************************************/
// 需要考虑是延时还是UcosIII 还是定时器中断的模式。
#if SYSTEM_SUPPORT_OS
UINT8 Mtx_Waiting(eMtxID MtxId, UINT32 timeov)
{
    UINT32 t = 0;
    OS_ERR err;
    OS_TICK ts1,ts2;
    ts1 = Sys_GetTicks();
    OSFlagPend((OS_FLAG_GRP*)&MtxEventFlags,
               (OS_FLAGS    )0x01<<MtxId,
               (OS_TICK     )timeov,
               (OS_OPT      )OS_OPT_PEND_FLAG_SET_ALL|OS_OPT_PEND_FLAG_CONSUME,
               (CPU_TS*     )0,
               (OS_ERR*     )&err);//等待信号量
    if(MtxCtrl[MtxId].mRunFlag != 0)
    {
        ts2 = Sys_GetTicks() - ts1;
        printf("ts %d\r\n",ts2);
        MtxCtrl[MtxId].mRunFlag = 0;
        // 超时关闭需要在Pend一次
        OSFlagPend((OS_FLAG_GRP*)&MtxEventFlags,
               (OS_FLAGS    )0x01<<MtxId,
               (OS_TICK     )timeov,
               (OS_OPT      )OS_OPT_PEND_FLAG_SET_ALL|OS_OPT_PEND_FLAG_CONSUME,
               (CPU_TS*     )0,
               (OS_ERR*     )&err);//等待信号量
        if(set_debug)
        {
            printf("timer ov\r\n");
        }
        return ERR_MOTOT_TIME_OVER;
    }
    else
        return MtxCtrl[MtxId].mErrCode;
}
#else
UINT8 Mtx_Waiting(eMtxID MtxId, UINT32 timeov)
{
    UINT32 t = 0;
    while(1)
    {
        Sys_Sys_Sys_Delay_ms(10);
        t++;
        if(t >= (timeov/10))
        {
            MtxCtrl[MtxId].mRunFlag = 0;
            if(set_debug)
            {
                printf("timer ov\r\n");
            }
            return ERR_MOTOT_TIME_OVER;
        }
        if(MtxCtrl[MtxId].mRunFlag == 0)
            return MtxCtrl[MtxId].mErrCode;
    }
}
#endif


/********************************************************************************
 ** 函数功能:电机x运行多少步
 ** 输入参数:参数一：枚举类型相当于整数与电机定时器相关联（0-3）-->eMtx_ID_X(1234)
             参数二：要走的总步数（传参进来）
             参数三：方向(步数减小方向eMtx_Dir_POSI /步数增大方向eMtx_Dir_NEGA
             参数四：超时计数
 ** 返 回 值:错误代码
 ** 说    明：无
 *******************************************************************************/
UINT8 Mtx_Move(eMtxID MtxId, UINT32 nSteps, eMtxDir eDir, UINT32 timeov)
{
    UINT8 ret = ERR_NONE;
    // 默认超时时间为10s
    if(timeov == 0)
        timeov = 10000;
    if(!nSteps)
        return ret;
    MT_CalculateStepRuler(MtxId, nSteps);

    // 正常运行模式
    MtxCtrl[MtxId].mRunMode = MODEL_RUN_MODE;
    MtxCtrl[MtxId].eDirection = eDir;
    if(eMtx_Dir_NEGA == eDir)
        MtxFunList[MtxId].pinDirectionNega(MtxId);
    else
        MtxFunList[MtxId].pinDirectionPosi(MtxId);

    MtxCtrl[MtxId].mRunFlag = 1;
    ret = MtxFunList[MtxId].mtDriverCtrl(MtxId, e_true);
    if(ret)
        return ret;
    ret = Mtx_Waiting(MtxId,timeov);
    if(set_debug)
    {
        printf("mStepsCur %d\r\n",MtxCtrl[MtxId].mStepsCur);
    }
    return ret;
}

/********************************************************************************
 ** 函数功能:电机x运行多少步
 ** 输入参数:参数一：枚举类型相当于整数与电机定时器相关联（0-3）-->eMtx_ID_X(1234)
             参数二：要走的总步数（传参进来）
             参数三：方向(步数减小方向eMtx_Dir_POSI /步数增大方向eMtx_Dir_NEGA
             参数四：超时计数
 ** 返 回 值:错误代码
 ** 说    明：无
 *******************************************************************************/
UINT8 Mtx_Move_NoWait(eMtxID MtxId, UINT32 nSteps, eMtxDir eDir)
{
    UINT8 ret = ERR_NONE;
    // 默认超时时间为10s

    if(!nSteps)
        return ret;
    MT_CalculateStepRuler(MtxId, nSteps);

    // 正常运行模式
    MtxCtrl[MtxId].mRunMode = MODEL_RUN_MODE;
    MtxCtrl[MtxId].eDirection = eDir;
    if(eMtx_Dir_NEGA == eDir)
        MtxFunList[MtxId].pinDirectionNega(MtxId);
    else
        MtxFunList[MtxId].pinDirectionPosi(MtxId);

    MtxCtrl[MtxId].mRunFlag = 1;
    ret = MtxFunList[MtxId].mtDriverCtrl(MtxId, e_true);
    if(ret)
        return ret;
    if(set_debug)
    {
        printf("mStepsCur %d\r\n",MtxCtrl[MtxId].mStepsCur);
    }
    return ret;
}

/********************************************************************************
 ** 函数功能:电机x运行到第几步
 ** 输入参数:参数一：枚举类型相当于整数与电机定时器相关联（0-3）-->    eMtx_ID_X(1234)
             参数二：当前的步数
             参数三：超时计数
             
 ** 返 回 值:错误代码
 ** 说    明：无
 *******************************************************************************/
UINT8 Mtx_Move_To(eMtxID MtxId, UINT32 nSteps, UINT32 timeov)
{
    UINT16 adSteps;
    eMtxDir eDir;
    
    if(MtxCtrl[MtxId].mStepsCur == nSteps)
        return ERR_NONE;
    if(MtxCtrl[MtxId].mStepsCur > nSteps)
    {
        adSteps = MtxCtrl[MtxId].mStepsCur - nSteps;
        eDir = eMtx_Dir_POSI;
    }
    else
    {
        adSteps =  nSteps - MtxCtrl[MtxId].mStepsCur;
        eDir = eMtx_Dir_NEGA;
    }
    return Mtx_Move(MtxId ,adSteps ,eDir ,timeov);
}


/********************************************************************************
 ** 函数功能:直到电机触发时候
 ** 输入参数:参数一：枚举类型相当于整数与电机定时器相关联（0-3）-->    eMtx_ID_X(1234)
             参数二：触发光耦后要走的步数
             参数三：方向(步数减小方向eMtx_Dir_POSI /步数增大方向eMtx_Dir_NEGA
             参数四：超时计数
 ** 返 回 值:ERR_MOTOT_TIME_OVER电机超时  10/错误代码
 ** 说    明：无
 *******************************************************************************/
UINT8 Mtx_UntilTegger(eMtxID MtxId, UINT32 tiggerpost, eMtxDir eDir, UINT32 timeov)
{
    UINT8 ret = 0;
    UINT32 maxpos = 0;
    if(timeov == 0)
        timeov = 10000;
    // 使用触发模式是用最大步数去计算的
    if(MtxPara[MtxId].nIsCircle)
    {
        maxpos = MtxPara[MtxId].nCircleSteps + MtxPara[MtxId].nRst;
    }
    else
        maxpos = MtxPara[MtxId].nMaxPos;
    MT_CalculateStepRuler(MtxId, maxpos);
    MtxCtrl[MtxId].mRunTigerPost = tiggerpost;
    
    MtxCtrl[MtxId].eDirection = eDir;
    if(eMtx_Dir_NEGA == eDir)
        MtxFunList[MtxId].pinDirectionNega(MtxId);
    else
        MtxFunList[MtxId].pinDirectionPosi(MtxId);

    MtxCtrl[MtxId].mRunFlag = 1;
    ret = MtxFunList[MtxId].mtDriverCtrl(MtxId, e_true);
    if(ret)
        return ret;
    ret = Mtx_Waiting(MtxId,timeov);
    if(set_debug)
    {
        printf("trigger %d\r\n",MtxCtrl[MtxId].mStepsTiger);
        printf("run pos %d\r\n",MtxCtrl[MtxId].mStepsRun);
    }
    return ret;
}


/********************************************************************************
 ** 函数功能:电机复位动作
 ** 输入参数:参数一：枚举类型相当于整数与电机定时器相关联（0-3）-->    eMtx_ID_X(1234)
             参数二：超时计数         
 ** 返 回 值:错误代码
 ** 说    明：无
 *******************************************************************************/
UINT8 Mtx_Rst(eMtxID MtxId,UINT32 timeov)
{
    UINT32 nSteps;
    UINT8 ret = ERR_NONE;
    eMtxDir eDir;

    MtxCtrl[MtxId].mErrCode = ERR_NONE;
    MtxCtrl[MtxId].mRunMode= RST_TIGGER_RUN_MODE;

    // 盘复位只有正向复位
    if(MtxPara[MtxId].nIsCircle)
    {
        eDir = eMtx_Dir_NEGA;

        nSteps = MtxPara[MtxId].nRst;
        ret = Mtx_UntilTegger(MtxId, nSteps, eDir, timeov);
        if(ret)
            return ret;
        Sys_Delay_ms(10);

        nSteps = MtxPara[MtxId].nRst;
        ret = Mtx_UntilTegger(MtxId, nSteps, eDir, timeov);
        if(ret)
            return ret;
    }
    else
    {
        // 检测到光耦被挡住 正向运动
        if(MtxPara[MtxId].nRst_LightCheck == MtxFunList[MtxId].mtResetRead(MtxId))
        {
            eDir = eMtx_Dir_NEGA;
            nSteps = MtxPara[MtxId].nRRst;
            ret = Mtx_UntilTegger(MtxId, nSteps, eDir, timeov);
            if(ret)
                return ret;
            Sys_Delay_ms(10);
        }

        if(MtxPara[MtxId].nRst_LightCheck == MtxFunList[MtxId].mtResetRead(MtxId))
            return ERR_MOTOT_LIGHT;
        eDir = eMtx_Dir_POSI;
        nSteps = MtxPara[MtxId].nRst;
        ret = Mtx_UntilTegger(MtxId, nSteps, eDir, timeov);
        if(ret)
            return ret;
        Sys_Delay_ms(10);

        if(MtxPara[MtxId].nRst_LightCheck != MtxFunList[MtxId].mtResetRead(MtxId))
            return ERR_MOTOT_LIGHT;
        
        eDir = eMtx_Dir_NEGA;
        nSteps = MtxPara[MtxId].nRRst;
        ret = Mtx_UntilTegger(MtxId, nSteps, eDir, timeov);
        if(ret)
            return ret;
        Sys_Delay_ms(10);
        
        if(MtxPara[MtxId].nRst_LightCheck == MtxFunList[MtxId].mtResetRead(MtxId))
            return ERR_MOTOT_LIGHT;

        eDir = eMtx_Dir_POSI;
        nSteps = MtxPara[MtxId].nRst;
        ret = Mtx_UntilTegger(MtxId, nSteps, eDir, timeov);
        if(ret)
            return ret;
        
    }
    return ret;
    
}


/********************************************************************************
 ** 函数功能:电机快速复位动作
 ** 输入参数:参数一：枚举类型相当于整数与电机定时器相关联（0-3）-->    eMtx_ID_X(1234)
             参数二：超时计数         
 ** 返 回 值:错误代码
 ** 说    明：无
 *******************************************************************************/
UINT8 Mtx_FastRst(eMtxID MtxId,UINT32 timeov)
{
    UINT32 nSteps;
    UINT8 ret = ERR_NONE;
    eMtxDir eDir;
    
    MtxCtrl[MtxId].mErrCode = ERR_NONE;
    MtxCtrl[MtxId].mRunMode= RST_TIGGER_RUN_MODE;

    // 盘复位只有正向复位
    if(MtxPara[MtxId].nIsCircle)
    {
        eDir = eMtx_Dir_NEGA;

        nSteps = MtxPara[MtxId].nRst;
        ret = Mtx_UntilTegger(MtxId, nSteps, eDir, timeov);
        if(ret)
            return ret;
    }
    else
    {
        // 检测到光耦被挡住 正向运动
        if(MtxPara[MtxId].nRst_LightCheck == MtxFunList[MtxId].mtResetRead(MtxId))
        {
            eDir = eMtx_Dir_NEGA;
            ret = Mtx_Move_To(MtxId,0,timeov);

            if(ret)
                return ret;
        }
        else
        {
            eDir = eMtx_Dir_POSI;
            nSteps = MtxPara[MtxId].nRst;
            ret = Mtx_UntilTegger(MtxId, nSteps, eDir, timeov);
            if(ret)
            return ret;
        }
    }
    return ret;
    
}
UINT8 Mtx_Cup_Move_To(eMtxID MtxId, UINT32 nSteps, UINT32 timeov)
{
    UINT16 adSteps;
    eMtxDir eDir;
    
    if(MtxCtrl[MtxId].mStepsCur == nSteps)
        return ERR_NONE;
    if(MtxCtrl[MtxId].mStepsCur > nSteps)
    {
        adSteps = MtxCtrl[MtxId].mStepsCur - nSteps;
        eDir = eMtx_Dir_POSI;
        
        if(adSteps > MtxPara[MtxId].nCircleSteps/2)
        {
            adSteps = MtxCtrl[MtxId].mStepsCur - nSteps;            
            eDir = eMtx_Dir_NEGA;
        }
        
    }
    else
    {
        adSteps =  nSteps - MtxCtrl[MtxId].mStepsCur;
        eDir = eMtx_Dir_NEGA;
        
        if(adSteps > MtxPara[MtxId].nCircleSteps/2)
        {
            adSteps = MtxCtrl[MtxId].mStepsCur - nSteps;            
            eDir = eMtx_Dir_POSI;
        }
        
    }
    return Mtx_Move(MtxId ,adSteps ,eDir ,timeov);
}

UINT8 MT_Cup_Move(eMtxID MtxId,UINT8 cup,UINT16 offset_pos,UINT8 mode,UINT16 time)
{
    UINT8 ret = ERR_NONE;
    UINT16 steps = 0;
    UINT16 pos = 0,pos2 = 0;
    eMtxDir eDir;
    
    if(cup>0 && cup<=MtxPara[MtxId].nCircleCups)
    {
        steps = ((cup-1) * MtxPara[MtxId].nCirclePerSteps + offset_pos) % MtxPara[MtxId].nCircleSteps;
        pos = MtxCtrl[MtxId].mStepsCur % MtxPara[MtxId].nCircleSteps;
        MtxCtrl[MtxId].mStepsCur = pos;
        if(steps != pos)
        {
            if(steps>pos)
            {
                pos2 = steps-pos;
                if(pos2>(MtxPara[MtxId].nCircleSteps >> 1))
                {
                    eDir = eMtx_Dir_POSI;
                    pos2 = MtxPara[MtxId].nCircleSteps - pos2;
                    if(MtxCtrl[MtxId].mStepsCur < pos2)
                        MtxCtrl[MtxId].mStepsCur += MtxPara[MtxId].nCircleSteps;
                }
                else
                {
                    eDir = eMtx_Dir_NEGA;
                }
            }
            else
            {
                pos2 = pos-steps;
                if(pos2>(MtxPara[MtxId].nCircleSteps >> 1))
                {
                    eDir = eMtx_Dir_NEGA;
                    pos2 = MtxPara[MtxId].nCircleSteps - pos2;
                }
                else
                {
                    eDir = eMtx_Dir_POSI;
                }
                
            }
            ret = Mtx_Move(MtxId ,pos2 ,eDir ,time);
        }
        else
        {
            MtxCtrl[MtxId].mStepsCur = pos;
            return ret;
        }
        
    }
    else
        return ERR_FRAM_DATA;
    return ret;
}

UINT8 MT_Cup_Move_One_Dir(eMtxID MtxId,UINT8 cup,UINT16 offset_pos,UINT8 mode,UINT16 time)
{
    UINT8 ret = ERR_NONE;
    UINT16 steps = 0;
    UINT16 pos = 0,pos2 = 0;
    eMtxDir eDir;
    
    if(cup>0 && cup<=MtxPara[MtxId].nCircleCups)
    {
        steps = ((cup-1)*MtxPara[MtxId].nCirclePerSteps + offset_pos) % MtxPara[MtxId].nCircleSteps;
        pos = MtxCtrl[MtxId].mStepsCur % MtxPara[MtxId].nCircleSteps;
        MtxCtrl[MtxId].mStepsCur = pos;
        if(steps != pos)
        {
            if(steps>pos)
            {
                pos2 = steps-pos;
                if(pos2>(MtxPara[MtxId].nCircleSteps >> 1))
                {
                    eDir = eMtx_Dir_POSI;
                    pos2 = MtxPara[MtxId].nCircleSteps - pos2;
                    if(MtxCtrl[MtxId].mStepsCur < pos2)
                        MtxCtrl[MtxId].mStepsCur += MtxPara[MtxId].nCircleSteps;
                }
                else
                {
                    eDir = eMtx_Dir_NEGA;
                }
            }
            else
            {
                pos2 = pos-steps;
                if(pos2>(MtxPara[MtxId].nCircleSteps >> 1))
                {
                    eDir = eMtx_Dir_NEGA;
                    pos2 = MtxPara[MtxId].nCircleSteps - pos2;
                }
                else
                {
                    eDir = eMtx_Dir_POSI;
                }
                
            }
            if(eDir == eMtx_Dir_POSI)
            {
                eDir = eMtx_Dir_NEGA;
                pos2 = MtxPara[MtxId].nCircleSteps - pos2;
            }
            ret = Mtx_Move(MtxId ,pos2 ,eDir ,time);
        }
        else
        {
            MtxCtrl[MtxId].mStepsCur = pos;
            return ret;
        }
        
    }
    else
        return ERR_FRAM_DATA;
    return ret;
 
}
#endif

