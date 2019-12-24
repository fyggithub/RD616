#ifndef __MTX_CTRLPIN_H__
#define __MTX_CTRLPIN_H__
#include "Mtx.h"


//========================进杯上下电机==========================
// motor 1 ctrl gpio
#if (TIM_MTx_PWM == 0)
#define MT_1_OUT_CLK_PIN        GPIO_Pin_6
#define MT_1_OUT_CLK_PORT       GPIOB
#define MT_1_OUT_CLK_CLK        RCC_APB2Periph_GPIOB
#endif
#define MT_1_OUT_EN_PIN         GPIO_Pin_14
#define MT_1_OUT_EN_PORT        GPIOE
#define MT_1_OUT_EN_CLK         RCC_APB2Periph_GPIOE

#define MT_1_OUT_DIR_PIN        GPIO_Pin_15
#define MT_1_OUT_DIR_PORT       GPIOE
#define MT_1_OUT_DIR_CLK        RCC_APB2Periph_GPIOE


//=======================吸液上下电机===========================
// motor 2 ctrl gpio
#if (TIM_MTx_PWM == 0)
#define MT_2_OUT_CLK_PIN        GPIO_Pin_2
#define MT_2_OUT_CLK_PORT       GPIOA
#define MT_2_OUT_CLK_CLK        RCC_APB2Periph_GPIOA
#endif
#define MT_2_OUT_EN_PIN         GPIO_Pin_10
#define MT_2_OUT_EN_PORT        GPIOE
#define MT_2_OUT_EN_CLK         RCC_APB2Periph_GPIOE

#define MT_2_OUT_DIR_PIN        GPIO_Pin_11
#define MT_2_OUT_DIR_PORT       GPIOE
#define MT_2_OUT_DIR_CLK        RCC_APB2Periph_GPIOE


//======================底物注射电机============================
// motor 3 ctrl gpio
#if (TIM_MTx_PWM == 0)
#define MT_3_OUT_CLK_PIN        GPIO_Pin_0
#define MT_3_OUT_CLK_PORT       GPIOB
#define MT_3_OUT_CLK_CLK        RCC_APB2Periph_GPIOB
#endif
#define MT_3_OUT_EN_PIN         GPIO_Pin_12
#define MT_3_OUT_EN_PORT        GPIOE
#define MT_3_OUT_EN_CLK         RCC_APB2Periph_GPIOE

#define MT_3_OUT_DIR_PIN        GPIO_Pin_13
#define MT_3_OUT_DIR_PORT       GPIOE
#define MT_3_OUT_DIR_CLK        RCC_APB2Periph_GPIOE


//=====================预留电机=============================
// motor 4 ctrl gpio
#if (TIM_MTx_PWM == 0)
#define MT_4_OUT_CLK_PIN        GPIO_Pin_6
#define MT_4_OUT_CLK_PORT       GPIOB
#define MT_4_OUT_CLK_CLK        RCC_APB2Periph_GPIOB
#endif
#define MT_4_OUT_EN_PIN         GPIO_Pin_14
#define MT_4_OUT_EN_PORT        GPIOE
#define MT_4_OUT_EN_CLK         RCC_APB2Periph_GPIOE

#define MT_4_OUT_DIR_PIN        GPIO_Pin_15
#define MT_4_OUT_DIR_PORT       GPIOE
#define MT_4_OUT_DIR_CLK        RCC_APB2Periph_GPIOE


//=====================================================
#if (TIM_MTx_PWM == 0)
#define MTx_OUT_CLK_PIN(x)			MT_##x##_OUT_CLK_PIN
#define MTx_OUT_CLK_PORT(x)         MT_##x##_OUT_CLK_PORT
#define MTx_OUT_CLK_CLK(x)			MT_##x##_OUT_CLK_CLK
#endif
#define MTx_OUT_EN_PIN(x)			MT_##x##_OUT_EN_PIN
#define MTx_OUT_EN_PORT(x)          MT_##x##_OUT_EN_PORT
#define MTx_OUT_EN_CLK(x)			MT_##x##_OUT_EN_CLK

#define MTx_OUT_DIR_PIN(x)			MT_##x##_OUT_DIR_PIN
#define MTx_OUT_DIR_PORT(x)         MT_##x##_OUT_DIR_PORT
#define MTx_OUT_DIR_CLK(x)			MT_##x##_OUT_DIR_CLK


//======================================================
#if (TIM_MTx_PWM == 0)
#define MT_OUT_CLK_PIN(x,y)		        do{  if(x==0) y = MTx_OUT_CLK_PIN(1);\
                                        else if(x==1) y = MTx_OUT_CLK_PIN(2);\
                                        else if(x==2) y = MTx_OUT_CLK_PIN(3);\
                                        else if(x==3) y = MTx_OUT_CLK_PIN(4);\
                                        }while(0)
                                        
#define MT_OUT_CLK_PORT(x,y)            do{  if(x==0) y = MTx_OUT_CLK_PORT(1);\
                                        else if(x==1) y = MTx_OUT_CLK_PORT(2);\
                                        else if(x==2) y = MTx_OUT_CLK_PORT(3);\
                                        else if(x==3) y = MTx_OUT_CLK_PORT(4);\
                                        }while(0)
                                        
#define MT_OUT_CLK_CLK(x,y)		        do{  if(x==0) y = MTx_OUT_CLK_CLK(1);\
                                        else if(x==1) y = MTx_OUT_CLK_CLK(2);\
                                        else if(x==2) y = MTx_OUT_CLK_CLK(3);\
                                        else if(x==3) y = MTx_OUT_CLK_CLK(4);\
                                        }while(0)
#endif                                        
//-------------------------------------------------------------------
#define MT_OUT_EN_PIN(x,y)	            do{  if(x==0) y = MTx_OUT_EN_PIN(1);\
                                        else if(x==1) y = MTx_OUT_EN_PIN(2);\
                                        else if(x==2) y = MTx_OUT_EN_PIN(3);\
                                        else if(x==3) y = MTx_OUT_EN_PIN(4);\
                                        }while(0)
                                                                                
#define MT_OUT_EN_PORT(x,y)             do{  if(x==0) y = MTx_OUT_EN_PORT(1);\
                                        else if(x==1) y = MTx_OUT_EN_PORT(2);\
                                        else if(x==2) y = MTx_OUT_EN_PORT(3);\
                                        else if(x==3) y = MTx_OUT_EN_PORT(4);\
                                        }while(0)
                                        
#define MT_OUT_EN_CLK(x,y)	            do{  if(x==0) y = MTx_OUT_EN_CLK(1);\
                                        else if(x==1) y = MTx_OUT_EN_CLK(2);\
                                        else if(x==2) y = MTx_OUT_EN_CLK(3);\
                                        else if(x==3) y = MTx_OUT_EN_CLK(4);\
                                        }while(0)

//------------------------------------------------------------------
#define MT_OUT_DIR_PIN(x,y)	            do{  if(x==0) y = MTx_OUT_DIR_PIN(1);\
                                        else if(x==1) y = MTx_OUT_DIR_PIN(2);\
                                        else if(x==2) y = MTx_OUT_DIR_PIN(3);\
                                        else if(x==3) y = MTx_OUT_DIR_PIN(4);\
                                        }while(0)

#define MT_OUT_DIR_PORT(x,y)            do{  if(x==0) y = MTx_OUT_DIR_PORT(1);\
                                        else if(x==1) y = MTx_OUT_DIR_PORT(2);\
                                        else if(x==2) y = MTx_OUT_DIR_PORT(3);\
                                        else if(x==3) y = MTx_OUT_DIR_PORT(4);\
                                        }while(0)

#define MT_OUT_DIR_CLK(x,y)	            do{  if(x==0) y = MTx_OUT_DIR_CLK(1);\
                                        else if(x==1) y = MTx_OUT_DIR_CLK(2);\
                                        else if(x==2) y = MTx_OUT_DIR_CLK(3);\
                                        else if(x==3) y = MTx_OUT_DIR_CLK(4);\
                                        }while(0)

typedef struct
{
    #if (TIM_MTx_PWM == 0)
    GPIO_TypeDef*   Mt_Clk_Ctrl_Port;
    UINT32          Mt_Clk_Ctrl_Clk;
    UINT16          Mt_Clk_Ctrl_Pin;
    #endif
    UINT16          Mt_Dir_Ctrl_Pin;
    UINT32          Mt_Dir_Ctrl_Clk;
    GPIO_TypeDef*   Mt_Dir_Ctrl_Port;

    GPIO_TypeDef*   Mt_En_Ctrl_Port;
    UINT32          Mt_En_Ctrl_Clk;
    UINT16          Mt_En_Ctrl_Pin;

}tMtxCtrlPinConfig;

_EXT_ tMtxCtrlPinConfig MtxCtrlPin[MTx_NUM];

UINT8 Mtx_CtrlPinInit(UINT8 MtxId);
#if (TIM_MTx_PWM == 0)
UINT8 Fun_MtxClkHigh(UINT8 MtxId);
UINT8 Fun_MtxClkLow(UINT8 MtxId);
#endif
UINT8 Fun_MtxLock(UINT8 MtxId);
UINT8 Fun_MtxUnLock(UINT8 MtxId);
UINT8 Fun_MtxDirectionPosi(UINT8 MtxId);
UINT8 Fun_MtxDirectionNega(UINT8 MtxId);

#endif

