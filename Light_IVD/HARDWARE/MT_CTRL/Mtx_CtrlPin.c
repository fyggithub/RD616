#ifndef __MTX_CTRLPIN_C__
#define __MTX_CTRLPIN_C__

#include "My_includes.h"

/********************************************************************************
 ** 函数功能:选择电机控制（clk，EN，DIR）的引脚以及挂载时钟
 ** 输入参数: 参数一：电机编号0-3对应motor1234
 ** 返 回 值: 无
 ** 说    明：无
 *******************************************************************************/
void Fun_ConfigMtxCtrlPinStruct(eTimType MtxId)
{
    #if (TIM_MTx_PWM == 0)
    MT_OUT_CLK_PIN(MtxId,MtxCtrlPin[MtxId].Mt_Clk_Ctrl_Pin);
    MT_OUT_CLK_PORT(MtxId,MtxCtrlPin[MtxId].Mt_Clk_Ctrl_Port);
    MT_OUT_CLK_CLK(MtxId,MtxCtrlPin[MtxId].Mt_Clk_Ctrl_Clk);
    #endif
    
    MT_OUT_EN_PIN(MtxId,MtxCtrlPin[MtxId].Mt_En_Ctrl_Pin);
    MT_OUT_EN_PORT(MtxId,MtxCtrlPin[MtxId].Mt_En_Ctrl_Port);
    MT_OUT_EN_CLK(MtxId,MtxCtrlPin[MtxId].Mt_En_Ctrl_Clk);

    MT_OUT_DIR_PIN(MtxId,MtxCtrlPin[MtxId].Mt_Dir_Ctrl_Pin);
    MT_OUT_DIR_PORT(MtxId,MtxCtrlPin[MtxId].Mt_Dir_Ctrl_Port);
    MT_OUT_DIR_CLK(MtxId,MtxCtrlPin[MtxId].Mt_Dir_Ctrl_Clk);

}

UINT8 Mtx_CtrlPinInit(UINT8 MtxId)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    Fun_ConfigMtxCtrlPinStruct(MtxId);
    #if (TIM_MTx_PWM == 0)
    RCC_APB2PeriphClockCmd(MtxCtrlPin[MtxId].Mt_Clk_Ctrl_Clk ,ENABLE);
    #endif
    RCC_APB2PeriphClockCmd(MtxCtrlPin[MtxId].Mt_En_Ctrl_Clk | \
                           MtxCtrlPin[MtxId].Mt_Dir_Ctrl_Clk,ENABLE);
    #if (TIM_MTx_PWM == 0)
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin   = MtxCtrlPin[MtxId].Mt_Clk_Ctrl_Pin;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(MtxCtrlPin[MtxId].Mt_Clk_Ctrl_Port, &GPIO_InitStructure);
    GPIO_SetBits(MtxCtrlPin[MtxId].Mt_Clk_Ctrl_Port,MtxCtrlPin[MtxId].Mt_Clk_Ctrl_Pin);
    Fun_MtxClkLow(MtxId);
    #endif
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin   = MtxCtrlPin[MtxId].Mt_Dir_Ctrl_Pin;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(MtxCtrlPin[MtxId].Mt_Dir_Ctrl_Port, &GPIO_InitStructure);
    GPIO_SetBits(MtxCtrlPin[MtxId].Mt_Dir_Ctrl_Port,MtxCtrlPin[MtxId].Mt_Dir_Ctrl_Pin);
    
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin   = MtxCtrlPin[MtxId].Mt_En_Ctrl_Pin;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(MtxCtrlPin[MtxId].Mt_En_Ctrl_Port, &GPIO_InitStructure);
    Fun_MtxUnLock(MtxId);
    return e_true;
}

#if (TIM_MTx_PWM == 0)
// 步进电机步进
UINT8 Fun_MtxClkHigh(UINT8 MtxId)
{
    MtxCtrlPin[MtxId].Mt_Clk_Ctrl_Port->BSRR = MtxCtrlPin[MtxId].Mt_Clk_Ctrl_Pin;
    return e_true;
}

// 步进电机步进
UINT8 Fun_MtxClkLow(UINT8 MtxId)
{
    MtxCtrlPin[MtxId].Mt_Clk_Ctrl_Port->BRR = MtxCtrlPin[MtxId].Mt_Clk_Ctrl_Pin;
    return e_true;
}
#endif

// 步进电机锁住
UINT8 Fun_MtxLock(UINT8 MtxId)
{
    MtxCtrlPin[MtxId].Mt_En_Ctrl_Port->BSRR = MtxCtrlPin[MtxId].Mt_En_Ctrl_Pin;
    return e_true;
}

// 步进电机解锁
UINT8 Fun_MtxUnLock(UINT8 MtxId)
{
    MtxCtrlPin[MtxId].Mt_En_Ctrl_Port->BRR = MtxCtrlPin[MtxId].Mt_En_Ctrl_Pin;
    return e_true;
}

// 步进电机正向
UINT8 Fun_MtxDirectionPosi(UINT8 MtxId)
{
    MtxCtrl[MtxId].eDirection = eMtx_Dir_POSI;
    if(MTx_Pin_Low == MtxPara[MtxId].nLevelPosi)
        MtxCtrlPin[MtxId].Mt_Dir_Ctrl_Port->BRR = MtxCtrlPin[MtxId].Mt_Dir_Ctrl_Pin;
    else
        MtxCtrlPin[MtxId].Mt_Dir_Ctrl_Port->BSRR = MtxCtrlPin[MtxId].Mt_Dir_Ctrl_Pin;
    return e_true;
}

// 步进电机反向运动
UINT8 Fun_MtxDirectionNega(UINT8 MtxId)
{
    MtxCtrl[MtxId].eDirection = eMtx_Dir_NEGA;
    if(MTx_Pin_Low == MtxPara[MtxId].nLevelPosi)
        MtxCtrlPin[MtxId].Mt_Dir_Ctrl_Port->BSRR = MtxCtrlPin[MtxId].Mt_Dir_Ctrl_Pin;
    else
        MtxCtrlPin[MtxId].Mt_Dir_Ctrl_Port->BRR = MtxCtrlPin[MtxId].Mt_Dir_Ctrl_Pin;
    return e_true;
}

#endif

