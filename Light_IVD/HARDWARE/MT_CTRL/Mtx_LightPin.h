#ifndef __MTX_LIGHTPIN_H__
#define __MTX_LIGHTPIN_H__
#include "Mtx.h"

#define MT_RESET_LIGHT_ENABLE   0x01        // 复位光耦使能
#define MT_ARRIVE_LIGHT_ENABLE  0x02        // 到位光耦使能
#define MT_CODE_LIGHT_ENABLE    0x04        // 码盘光耦使能
#define MT_CRASH_LIGHT_ENABLE   0x08        // 碰撞光耦使能

//--------------------------进杯上下电机光耦------------------------------------
#define MT_1_RESET_LIGHT_PIN        GPIO_Pin_3
#define MT_1_RESET_LIGHT_PORT       GPIOD
#define MT_1_RESET_LIGHT_CLK        RCC_APB2Periph_GPIOD

#define MT_1_ARRIVE_LIGHT_PIN       0
#define MT_1_ARRIVE_LIGHT_PORT      NULL
#define MT_1_ARRIVE_LIGHT_CLK       0

#define MT_1_CODE_LIGHT_PIN         0
#define MT_1_CODE_LIGHT_PORT        NULL
#define MT_1_CODE_LIGHT_CLK         0

#define MT_1_CRASH_LIGHT_PIN        0
#define MT_1_CRASH_LIGHT_PORT       NULL
#define MT_1_CRASH_LIGHT_CLK        0

//--------------------------吸液上下电机--------------------------------------
#define MT_2_RESET_LIGHT_PIN        GPIO_Pin_2
#define MT_2_RESET_LIGHT_PORT       GPIOE
#define MT_2_RESET_LIGHT_CLK        RCC_APB2Periph_GPIOE

#define MT_2_ARRIVE_LIGHT_PIN       0
#define MT_2_ARRIVE_LIGHT_PORT      NULL
#define MT_2_ARRIVE_LIGHT_CLK       0

#define MT_2_CODE_LIGHT_PIN         0
#define MT_2_CODE_LIGHT_PORT        NULL
#define MT_2_CODE_LIGHT_CLK         0

#define MT_2_CRASH_LIGHT_PIN        0
#define MT_2_CRASH_LIGHT_PORT       NULL
#define MT_2_CRASH_LIGHT_CLK        0

//----------------------------底物注射1电机------------------------------------
#define MT_3_RESET_LIGHT_PIN        GPIO_Pin_3
#define MT_3_RESET_LIGHT_PORT       GPIOE
#define MT_3_RESET_LIGHT_CLK        RCC_APB2Periph_GPIOE

#define MT_3_ARRIVE_LIGHT_PIN       0
#define MT_3_ARRIVE_LIGHT_PORT      NULL
#define MT_3_ARRIVE_LIGHT_CLK       0

#define MT_3_CODE_LIGHT_PIN         0
#define MT_3_CODE_LIGHT_PORT        NULL
#define MT_3_CODE_LIGHT_CLK         0

#define MT_3_CRASH_LIGHT_PIN        0
#define MT_3_CRASH_LIGHT_PORT       NULL
#define MT_3_CRASH_LIGHT_CLK        0

//------------------------底物注射2电机----------------------------------------
#define MT_4_RESET_LIGHT_PIN        GPIO_Pin_4
#define MT_4_RESET_LIGHT_PORT       GPIOE
#define MT_4_RESET_LIGHT_CLK        RCC_APB2Periph_GPIOE

#define MT_4_ARRIVE_LIGHT_PIN       0
#define MT_4_ARRIVE_LIGHT_PORT      NULL
#define MT_4_ARRIVE_LIGHT_CLK       0

#define MT_4_CODE_LIGHT_PIN         0
#define MT_4_CODE_LIGHT_PORT        NULL
#define MT_4_CODE_LIGHT_CLK         0

#define MT_4_CRASH_LIGHT_PIN        0
#define MT_4_CRASH_LIGHT_PORT       NULL
#define MT_4_CRASH_LIGHT_CLK        0


//================================================================================
#define MTx_RESET_LIGHT_PIN(x)			MT_##x##_RESET_LIGHT_PIN
#define MTx_RESET_LIGHT_PORT(x)         MT_##x##_RESET_LIGHT_PORT
#define MTx_RESET_LIGHT_CLK(x)			MT_##x##_RESET_LIGHT_CLK

#define MTx_ARRIVE_LIGHT_PIN(x)			MT_##x##_ARRIVE_LIGHT_PIN
#define MTx_ARRIVE_LIGHT_PORT(x)        MT_##x##_ARRIVE_LIGHT_PORT
#define MTx_ARRIVE_LIGHT_CLK(x)			MT_##x##_ARRIVE_LIGHT_CLK

#define MTx_CODE_LIGHT_PIN(x)			MT_##x##_CODE_LIGHT_PIN
#define MTx_CODE_LIGHT_PORT(x)          MT_##x##_CODE_LIGHT_PORT
#define MTx_CODE_LIGHT_CLK(x)			MT_##x##_CODE_LIGHT_CLK

#define MTx_CRASH_LIGHT_PIN(x)			MT_##x##_CRASH_LIGHT_PIN
#define MTx_CRASH_LIGHT_PORT(x)         MT_##x##_CRASH_LIGHT_PORT
#define MTx_CRASH_LIGHT_CLK(x)			MT_##x##_CRASH_LIGHT_CLK

//================================================================================
#define MT_RESET_LIGHT_PIN(x,y)		    do{  if(x==0) y = MTx_RESET_LIGHT_PIN(1);\
                                        else if(x==1) y = MTx_RESET_LIGHT_PIN(2);\
                                        else if(x==2) y = MTx_RESET_LIGHT_PIN(3);\
                                        else if(x==3) y = MTx_RESET_LIGHT_PIN(4);\
                                        }while(0)
                                        
#define MT_RESET_LIGHT_PORT(x,y)        do{  if(x==0) y = MTx_RESET_LIGHT_PORT(1);\
                                        else if(x==1) y = MTx_RESET_LIGHT_PORT(2);\
                                        else if(x==2) y = MTx_RESET_LIGHT_PORT(3);\
                                        else if(x==3) y = MTx_RESET_LIGHT_PORT(4);\
                                        }while(0)
                                        
#define MT_RESET_LIGHT_CLK(x,y)		    do{  if(x==0) y = MTx_RESET_LIGHT_CLK(1);\
                                        else if(x==1) y = MTx_RESET_LIGHT_CLK(2);\
                                        else if(x==2) y = MTx_RESET_LIGHT_CLK(3);\
                                        else if(x==3) y = MTx_RESET_LIGHT_CLK(4);\
                                        }while(0)
                                        
//-------------------------------------------------------------------
#define MT_ARRIVE_LIGHT_PIN(x,y)	    do{  if(x==0) y = MTx_ARRIVE_LIGHT_PIN(1);\
                                        else if(x==1) y = MTx_ARRIVE_LIGHT_PIN(2);\
                                        else if(x==2) y = MTx_ARRIVE_LIGHT_PIN(3);\
                                        else if(x==3) y = MTx_ARRIVE_LIGHT_PIN(4);\
                                        }while(0)
                                                                                
#define MT_ARRIVE_LIGHT_PORT(x,y)       do{  if(x==0) y = MTx_ARRIVE_LIGHT_PORT(1);\
                                        else if(x==1) y = MTx_ARRIVE_LIGHT_PORT(2);\
                                        else if(x==2) y = MTx_ARRIVE_LIGHT_PORT(3);\
                                        else if(x==3) y = MTx_ARRIVE_LIGHT_PORT(4);\
                                        }while(0)
                                        
#define MT_ARRIVE_LIGHT_CLK(x,y)	    do{  if(x==0) y = MTx_ARRIVE_LIGHT_CLK(1);\
                                        else if(x==1) y = MTx_ARRIVE_LIGHT_CLK(2);\
                                        else if(x==2) y = MTx_ARRIVE_LIGHT_CLK(3);\
                                        else if(x==3) y = MTx_ARRIVE_LIGHT_CLK(4);\
                                        }while(0)

//------------------------------------------------------------------
#define MT_CODE_LIGHT_PIN(x,y)	        do{  if(x==0) y = MTx_CODE_LIGHT_PIN(1);\
                                        else if(x==1) y = MTx_CODE_LIGHT_PIN(2);\
                                        else if(x==2) y = MTx_CODE_LIGHT_PIN(3);\
                                        else if(x==3) y = MTx_CODE_LIGHT_PIN(4);\
                                        }while(0)

#define MT_CODE_LIGHT_PORT(x,y)         do{  if(x==0) y = MTx_CODE_LIGHT_PORT(1);\
                                        else if(x==1) y = MTx_CODE_LIGHT_PORT(2);\
                                        else if(x==2) y = MTx_CODE_LIGHT_PORT(3);\
                                        else if(x==3) y = MTx_CODE_LIGHT_PORT(4);\
                                        }while(0)

#define MT_CODE_LIGHT_CLK(x,y)	        do{  if(x==0) y = MTx_CODE_LIGHT_CLK(1);\
                                        else if(x==1) y = MTx_CODE_LIGHT_CLK(2);\
                                        else if(x==2) y = MTx_CODE_LIGHT_CLK(3);\
                                        else if(x==3) y = MTx_CODE_LIGHT_CLK(4);\
                                        }while(0)

//-------------------------------------------------------------------
#define MT_CRASH_LIGHT_PIN(x,y)	        do{  if(x==0) y = MTx_CRASH_LIGHT_PIN(1);\
                                        else if(x==1) y = MTx_CRASH_LIGHT_PIN(2);\
                                        else if(x==2) y = MTx_CRASH_LIGHT_PIN(3);\
                                        else if(x==3) y = MTx_CRASH_LIGHT_PIN(4);\
                                        }while(0)

#define MT_CRASH_LIGHT_PORT(x,y)        do{  if(x==0) y = MTx_CRASH_LIGHT_PORT(1);\
                                        else if(x==1) y = MTx_CRASH_LIGHT_PORT(2);\
                                        else if(x==2) y = MTx_CRASH_LIGHT_PORT(3);\
                                        else if(x==3) y = MTx_CRASH_LIGHT_PORT(4);\
                                        }while(0)

#define MT_CRASH_LIGHT_CLK(x,y)	        do{  if(x==0) y = MTx_CRASH_LIGHT_CLK(1);\
                                        else if(x==1) y = MTx_CRASH_LIGHT_CLK(2);\
                                        else if(x==2) y = MTx_CRASH_LIGHT_CLK(3);\
                                        else if(x==3) y = MTx_CRASH_LIGHT_CLK(4);\
                                        }while(0)

typedef struct
{
    GPIO_TypeDef*   Reset_Light_Port;
    UINT32          Reset_Light_Clk;
    UINT16          Reset_Light_Pin;

    UINT16          Arrive_Light_Pin;
    UINT32          Arrive_Light_Clk;
    GPIO_TypeDef*   Arrive_Light_Port;

    GPIO_TypeDef*   Code_Light_Port;
    UINT32          Code_Light_Clk;
    UINT16          Code_Light_Pin;

    UINT16          Crash_Light_Pin;
    UINT32          Crash_Light_Clk;
    GPIO_TypeDef*   Crash_Light_Port;
}tMtxLightConfig;

UINT8 Fun_Mtx_LightInit(UINT8 MtxId);
UINT8 Fun_MtxResetGetState(UINT8 MtxId);
UINT8 Fun_MtxArriveGetState(UINT8 MtxId);
UINT8 Fun_MtxCodeGetState(UINT8 MtxId);
UINT8 Fun_MtxCrashGetState(UINT8 MtxId);
#endif

