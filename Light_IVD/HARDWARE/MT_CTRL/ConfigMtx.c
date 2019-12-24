/*********************************************************************************
**********************************************************************************
**********************************************************************************
* 文件名称: ConfigMtx.c                                                          *
* 文件简述：                                                      *
* 创建日期：2014.10.03                                                          *
* 版    本：V1.13                                                               *
* 作    者：LY Chen                                                             *
* 说    明：                        * 
**********************************************************************************
*********************************************************************************/
#ifndef __CONFIGMTX_C__
#define __CONFIGMTX_C__

#include "ConfigMtx.h"
#include "Mtx.h"
#include "Mtx_LightPin.h"
#include "Mtx_CtrlPin.h"

UINT16 AccTimArr_Buf[MTx_NUM][1024];
UINT16 DecTimArr_Buf[MTx_NUM][1024];

UINT8 MtxSetPara(UINT8 MtxId)
{
    MT_MAXFREQ(MtxId, MtxPara[MtxId].nFreqMax);
    MT_MINFREQ(MtxId, MtxPara[MtxId].nFreqMin);
    MT_MAXPOS(MtxId, MtxPara[MtxId].nMaxPos);
    MT_STEPS_T1(MtxId, MtxPara[MtxId].nStepsT1);
    MT_STEPS_T2(MtxId, MtxPara[MtxId].nStepsT2);
    MT_STEPS_T3(MtxId, MtxPara[MtxId].nStepsT3);
    MT_STEPS_T5(MtxId, MtxPara[MtxId].nStepsT5);
    MT_STEPS_T6(MtxId, MtxPara[MtxId].nStepsT6);
    MT_STEPS_T7(MtxId, MtxPara[MtxId].nStepsT7);
    
    MT_LIGHTCHECK(MtxId, MtxPara[MtxId].nLightCheck);

    MT_RSTPOS(MtxId, MtxPara[MtxId].nRst);
    MT_nRRSTPOS(MtxId, MtxPara[MtxId].nRRst);
    
    if(MtxPara[MtxId].nLightCheck & MT_CODE_LIGHT_ENABLE )
        MT_CODE(MtxId, MtxPara[MtxId].nCode);
    else
        MtxPara[MtxId].nCode = 0;
    if(MtxPara[MtxId].nLightCheck & MT_ARRIVE_LIGHT_ENABLE)
        MT_ARRIVE(MtxId, MtxPara[MtxId].nArrive);
    else
        MtxPara[MtxId].nArrive = 0;
    
    MT_ISCIRCLE(MtxId, MtxPara[MtxId].nIsCircle);
    if(MtxPara[MtxId].nIsCircle)
    {
        MT_CIRCLE_STEPS(MtxId, MtxPara[MtxId].nCircleSteps);
        MT_CIRCLE_CUPS(MtxId, MtxPara[MtxId].nCircleCups);
        MtxPara[MtxId].nCirclePerSteps = MtxPara[MtxId].nCircleSteps/MtxPara[MtxId].nCircleCups;
    }
    else
    {
        MtxPara[MtxId].nCircleSteps = 0;
        MtxPara[MtxId].nCircleCups = 0;
    }
    MT_ELEVEL_POSI(MtxId, MtxPara[MtxId].nLevelPosi);
    
    MT_RST_LIGHTCHECK(MtxId, MtxPara[MtxId].nRst_LightCheck);
    
    MtxPara[MtxId].nAccTimArr = AccTimArr_Buf[MtxId];
    MtxPara[MtxId].nDecTimArr = DecTimArr_Buf[MtxId];
    TIM_SetAutoreload(TimConfig[MtxId].Tim,TimConfig[MtxId].Mtx_Timer_Freq/MtxPara[MtxId].nFreqMin);
    return e_true;
}

UINT8 MtxFunListInit(UINT8 MtxId)
{
    MtxFunList[MtxId].tmrInit           = Fun_Time_Init;
    MtxFunList[MtxId].tmrInterruptOn    = Fun_Mtx_TimInterruptON;
    MtxFunList[MtxId].tmrInterruptOff   = Fun_Mtx_TimInterruptOFF;
    MtxFunList[MtxId].tmrInterruptClear = Fun_Mtx_TimInterruptClear;
    MtxFunList[MtxId].tmrCountOn        = Fun_Mtx_TimCountON;
    MtxFunList[MtxId].tmrCountOff       = Fun_Mtx_TimCountOFF;
    MtxFunList[MtxId].tmrLoad           = Fun_Mtx_TimLoad;

    MtxFunList[MtxId].mtLightPinInit    = Fun_Mtx_LightInit;
    MtxFunList[MtxId].mtResetRead       = Fun_MtxResetGetState;
    MtxFunList[MtxId].mtArriveRead      = Fun_MtxArriveGetState;
    MtxFunList[MtxId].mtCodeRead        = Fun_MtxCodeGetState;
    MtxFunList[MtxId].mtCrashRead       = Fun_MtxCrashGetState;

    MtxFunList[MtxId].pinInit           = Mtx_CtrlPinInit;
    #if (TIM_MTx_PWM == 0)
    MtxFunList[MtxId].pinPulseHigh      = Fun_MtxClkHigh;
    MtxFunList[MtxId].pinPulseLow       = Fun_MtxClkLow;
    #endif
    MtxFunList[MtxId].pinDriverLock     = Fun_MtxLock;
    MtxFunList[MtxId].pinDriverUnlock   = Fun_MtxUnLock;
    MtxFunList[MtxId].pinDirectionPosi  = Fun_MtxDirectionPosi;
    MtxFunList[MtxId].pinDirectionNega  = Fun_MtxDirectionNega;
    
    MtxFunList[MtxId].mtDriverCtrl      = Mtx_DriverCtrl;
    
    return e_true;
}

UINT8 Mtx_Init(UINT8 MtxId)
{
    MtxFunListInit(MtxId);
    MtxFunList[MtxId].tmrInit(MtxId);
    MtxSetPara(MtxId);
    
    MtxFunList[MtxId].pinInit(MtxId);
    MtxFunList[MtxId].mtLightPinInit(MtxId);
    MtxFunList[MtxId].mtDriverCtrl(MtxId,e_false);
    
    MtxCaculateAccDec(MtxId);
}

#endif

