#ifndef __MTX_LIGHTPIN_C__
#define __MTX_LIGHTPIN_C__
#include "Mtx.h"
#include "ConfigMtx.h"
#include "Mtx_LightPin.h"

tMtxLightConfig MtxLightConfig[MTx_NUM];

void Fun_ConfigMtxLightStruct(eTimType MtxId)
{
    MT_RESET_LIGHT_PIN(MtxId,MtxLightConfig[MtxId].Reset_Light_Pin);
    MT_RESET_LIGHT_PORT(MtxId,MtxLightConfig[MtxId].Reset_Light_Port);
    MT_RESET_LIGHT_CLK(MtxId,MtxLightConfig[MtxId].Reset_Light_Clk);

    MT_ARRIVE_LIGHT_PIN(MtxId,MtxLightConfig[MtxId].Arrive_Light_Pin);
    MT_ARRIVE_LIGHT_PORT(MtxId,MtxLightConfig[MtxId].Arrive_Light_Port);
    MT_ARRIVE_LIGHT_CLK(MtxId,MtxLightConfig[MtxId].Arrive_Light_Clk);

    MT_CODE_LIGHT_PIN(MtxId,MtxLightConfig[MtxId].Code_Light_Pin);
    MT_CODE_LIGHT_PORT(MtxId,MtxLightConfig[MtxId].Code_Light_Port);
    MT_CODE_LIGHT_CLK(MtxId,MtxLightConfig[MtxId].Code_Light_Clk);

    MT_CRASH_LIGHT_PIN(MtxId,MtxLightConfig[MtxId].Crash_Light_Pin);
    MT_CRASH_LIGHT_PORT(MtxId,MtxLightConfig[MtxId].Crash_Light_Port);
    MT_CRASH_LIGHT_CLK(MtxId,MtxLightConfig[MtxId].Crash_Light_Clk);
}

UINT8 Fun_Mtx_LightInit(UINT8 MtxId)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    Fun_ConfigMtxLightStruct(MtxId);
    

    RCC_APB2PeriphClockCmd(MtxLightConfig[MtxId].Reset_Light_Clk ,ENABLE);
    GPIO_InitStructure.GPIO_Pin   = MtxLightConfig[MtxId].Reset_Light_Pin;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(MtxLightConfig[MtxId].Reset_Light_Port, &GPIO_InitStructure);
//    #if 0
    if(MtxPara[MtxId].nLightCheck & MT_ARRIVE_LIGHT_ENABLE)
    {
        RCC_APB2PeriphClockCmd(MtxLightConfig[MtxId].Arrive_Light_Clk ,ENABLE);
        GPIO_InitStructure.GPIO_Pin   = MtxLightConfig[MtxId].Arrive_Light_Pin;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
        GPIO_Init(MtxLightConfig[MtxId].Arrive_Light_Port, &GPIO_InitStructure);
    }

    if(MtxPara[MtxId].nLightCheck & MT_CODE_LIGHT_ENABLE)
    {
        RCC_APB2PeriphClockCmd(MtxLightConfig[MtxId].Code_Light_Clk ,ENABLE);
        GPIO_InitStructure.GPIO_Pin   = MtxLightConfig[MtxId].Code_Light_Pin;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
        GPIO_Init(MtxLightConfig[MtxId].Code_Light_Port, &GPIO_InitStructure);
    }

    if(MtxPara[MtxId].nLightCheck & MT_CRASH_LIGHT_ENABLE)
    {
        RCC_APB2PeriphClockCmd(MtxLightConfig[MtxId].Crash_Light_Clk ,ENABLE);
        GPIO_InitStructure.GPIO_Pin   = MtxLightConfig[MtxId].Crash_Light_Pin;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
        GPIO_Init(MtxLightConfig[MtxId].Crash_Light_Port, &GPIO_InitStructure);
    }
    return e_true;  
//    #endif
}

UINT8 Fun_MtxResetGetState(UINT8 MtxId)
{
//    return GPIO_ReadInputDataBit(MtxLightConfig[MtxId].Reset_Light_Port,MtxLightConfig[MtxId].Reset_Light_Pin);
    if(MtxLightConfig[MtxId].Reset_Light_Port->IDR & MtxLightConfig[MtxId].Reset_Light_Pin)
        return Bit_SET;
    else
        return Bit_RESET;
}

UINT8 Fun_MtxArriveGetState(UINT8 MtxId)
{
    if(MtxLightConfig[MtxId].Arrive_Light_Port->IDR & MtxLightConfig[MtxId].Arrive_Light_Pin)
        return Bit_SET;
    else
        return Bit_RESET;
}

UINT8 Fun_MtxCodeGetState(UINT8 MtxId)
{
    if(MtxLightConfig[MtxId].Code_Light_Port->IDR & MtxLightConfig[MtxId].Code_Light_Pin)
        return Bit_SET;
    else
        return Bit_RESET;
}

UINT8 Fun_MtxCrashGetState(UINT8 MtxId)
{
    if(MtxLightConfig[MtxId].Crash_Light_Port->IDR & MtxLightConfig[MtxId].Crash_Light_Pin)
        return Bit_SET;
    else
        return Bit_RESET;
}

#endif

