#ifndef __PUMP_VALVE_C__
#define __PUMP_VALVE_C__

#include <includes.h>
#include "My_includes.h"
//#if PUMP_VALVE_EN

tPV_Config PV_Config[PVx_NUM]; 

void Fun_Config_PV_Struct(ePV_Type ePV)
{
    PV_PIN(ePV,PV_Config[ePV].PV_Pin);
    PV_PORT(ePV,PV_Config[ePV].PV_Port);
    PV_CLK(ePV,PV_Config[ePV].PV_Clk);
}

UINT8 FUN_PV_INIT(ePV_Type ePV)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    
    Fun_Config_PV_Struct(ePV);
    RCC_APB2PeriphClockCmd(PV_Config[ePV].PV_Clk, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = PV_Config[ePV].PV_Pin;				 //LED0-->PB.5 端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
    
    GPIO_Init(PV_Config[ePV].PV_Port, &GPIO_InitStructure); 
    GPIO_ResetBits(PV_Config[ePV].PV_Port,PV_Config[ePV].PV_Pin); 
    return e_true;    
}


UINT8 Fun_PV_Open(ePV_Type PV_Type)
{
    GPIO_SetBits(PV_Config[PV_Type].PV_Port,PV_Config[PV_Type].PV_Pin); 
    return e_true;
}
    
UINT8 Fun_PV_Close(ePV_Type PV_Type)
{
    GPIO_ResetBits(PV_Config[PV_Type].PV_Port,PV_Config[PV_Type].PV_Pin);
    return e_true;    
}

UINT8 Fun_PVx_GetState(ePV_Type PV_Type)
{
    return GPIO_ReadOutputDataBit(PV_Config[PV_Type].PV_Port, PV_Config[PV_Type].PV_Pin);
}

UINT8 PV_AllInit(void)
{
    UINT8 i = 0;
    for(i = 0; i<PVx_NUM; i++)
    {
        FUN_PV_INIT(i);
    }
    return e_true;  
}

//#endif
#endif
