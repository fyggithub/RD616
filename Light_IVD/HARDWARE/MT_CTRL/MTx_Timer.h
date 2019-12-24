#ifndef __MTx_TIMER_H__
#define __MTx_TIMER_H__

#include "stm32f10x.h"
#include "My_Type.h"

#define TIM_MTx_PWM         1

// 电机的个数
#define MTx_NUM             1

#define MT0_TIME                TIM4
#define MT0_TIME_CLK            RCC_APB1Periph_TIM4
#define MT0_TIME_IRQn           TIM4_IRQn

#if TIM_MTx_PWM
#define MT0_TIME_PWM_PIN        GPIO_Pin_6
#define MT0_TIME_PWM_PORT       GPIOB
#define MT0_TIME_PWM_CLK        RCC_APB2Periph_GPIOB
#define MT0_TIME_PWM_PINREMAP   0
#define MT0_TIME_PWM_CH         1               // 通道
#endif


#define MT1_TIME                TIM5
#define MT1_TIME_CLK            RCC_APB1Periph_TIM5
#define MT1_TIME_IRQn           TIM5_IRQn

#if TIM_MTx_PWM
#define MT1_TIME_PWM_PIN        GPIO_Pin_2
#define MT1_TIME_PWM_PORT       GPIOA
#define MT1_TIME_PWM_CLK        RCC_APB2Periph_GPIOA
#define MT1_TIME_PWM_PINREMAP   0
#define MT1_TIME_PWM_CH         3               // 通道
#endif

#define MT2_TIME                TIM3
#define MT2_TIME_CLK            RCC_APB1Periph_TIM3
#define MT2_TIME_IRQn           TIM3_IRQn

#if TIM_MTx_PWM
#define MT2_TIME_PWM_PIN        GPIO_Pin_0
#define MT2_TIME_PWM_PORT       GPIOB
#define MT2_TIME_PWM_CLK        RCC_APB2Periph_GPIOB
#define MT2_TIME_PWM_PINREMAP   0
#define MT2_TIME_PWM_CH         3               // 通道
#endif

#define MT3_TIME                TIM4
#define MT3_TIME_CLK            RCC_APB1Periph_TIM4
#define MT3_TIME_IRQn           TIM4_IRQn

#if TIM_MTx_PWM
#define MT3_TIME_PWM_PIN        GPIO_Pin_6
#define MT3_TIME_PWM_PORT       GPIOB
#define MT3_TIME_PWM_CLK        RCC_APB2Periph_GPIOB
#define MT3_TIME_PWM_PINREMAP   0
#define MT3_TIME_PWM_CH         1               // 通道
#endif



#define MTx_TIME(x)                     MT##x##_TIME
#define MTx_TIME_CLK(x)					MT##x##_TIME_CLK
#define MTx_TIME_IRQn(x)                MT##x##_TIME_IRQn

#if TIM_MTx_PWM
#define MTx_TIME_PWM_PIN(x)			    MT##x##_TIME_PWM_PIN
#define MTx_TIME_PWM_PORT(x)            MT##x##_TIME_PWM_PORT
#define MTx_TIME_PWM_CLK(x)			    MT##x##_TIME_PWM_CLK
#define MTx_TIME_PWM_PINREMAP(x)        MT##x##_TIME_PWM_PINREMAP
#define MTx_TIME_PWM_CH(x)			    MT##x##_TIME_PWM_CH

#define MTx_TIM_OCInit(x,tim,ocs)               TIM_OC##x##Init(tim,ocs)
#define MTx_TIM_OCPreloadConfig(x,tim,oce)      TIM_OC##x##PreloadConfig(tim,oce)

#endif

#define MT_TIME(x,y)                    do{     if(x==0) y = MTx_TIME(0);\
                                           else if(x==1) y = MTx_TIME(1);\
                                           else if(x==2) y = MTx_TIME(2);\
                                           else if(x==3) y = MTx_TIME(3);\
                                           }while(0)

#define MT_TIME_CLK(x,y)                do{     if(x==0) y = MTx_TIME_CLK(0);\
                                           else if(x==1) y = MTx_TIME_CLK(1);\
                                           else if(x==2) y = MTx_TIME_CLK(2);\
                                           else if(x==3) y = MTx_TIME_CLK(3);\
                                           }while(0)
                                           
#define MT_TIME_IRQn(x,y)               do{     if(x==0) y = MTx_TIME_IRQn(0);\
                                           else if(x==1) y = MTx_TIME_IRQn(1);\
                                           else if(x==2) y = MTx_TIME_IRQn(2);\
                                           else if(x==3) y = MTx_TIME_IRQn(3);\
                                           }while(0)

#if TIM_MTx_PWM
#define MT_TIME_PWM_PIN(x,y)		    do{  if(x==0) y = MTx_TIME_PWM_PIN(0);\
                                        else if(x==1) y = MTx_TIME_PWM_PIN(1);\
                                        else if(x==2) y = MTx_TIME_PWM_PIN(2);\
                                        else if(x==3) y = MTx_TIME_PWM_PIN(3);\
                                        }while(0)
                                        
#define MT_TIME_PWM_PORT(x,y)           do{  if(x==0) y = MTx_TIME_PWM_PORT(0);\
                                        else if(x==1) y = MTx_TIME_PWM_PORT(1);\
                                        else if(x==2) y = MTx_TIME_PWM_PORT(2);\
                                        else if(x==3) y = MTx_TIME_PWM_PORT(3);\
                                        }while(0)
                                        
#define MT_TIME_PWM_CLK(x,y)		    do{  if(x==0) y = MTx_TIME_PWM_CLK(0);\
                                        else if(x==1) y = MTx_TIME_PWM_CLK(1);\
                                        else if(x==2) y = MTx_TIME_PWM_CLK(2);\
                                        else if(x==3) y = MTx_TIME_PWM_CLK(3);\
                                        }while(0)

#define MT_TIME_PWM_PINREMAP(x,y)	    do{  if(x==0) y = MTx_TIME_PWM_PINREMAP(0);\
                                        else if(x==1) y = MTx_TIME_PWM_PINREMAP(1);\
                                        else if(x==2) y = MTx_TIME_PWM_PINREMAP(2);\
                                        else if(x==3) y = MTx_TIME_PWM_PINREMAP(3);\
                                        }while(0)

#define MT_TIME_PWM_CH(x,y)		        do{  if(x==0) y = MTx_TIME_PWM_CH(0);\
                                        else if(x==1) y = MTx_TIME_PWM_CH(1);\
                                        else if(x==2) y = MTx_TIME_PWM_CH(2);\
                                        else if(x==3) y = MTx_TIME_PWM_CH(3);\
                                        }while(0)

#define MT_TIM_OCInit(x,tim,ocs)        do{  if(x==1) MTx_TIM_OCInit(1,tim,ocs);\
                                        else if(x==2) MTx_TIM_OCInit(2,tim,ocs);\
                                        else if(x==3) MTx_TIM_OCInit(3,tim,ocs);\
                                        else if(x==4) MTx_TIM_OCInit(4,tim,ocs);\
                                        }while(0)

#define MT_TIM_OCPreloadConfig(x,tim,ocs)        do{  if(x==1) MTx_TIM_OCPreloadConfig(1,tim,ocs);\
                                                 else if(x==2) MTx_TIM_OCPreloadConfig(2,tim,ocs);\
                                                 else if(x==3) MTx_TIM_OCPreloadConfig(3,tim,ocs);\
                                                 else if(x==4) MTx_TIM_OCPreloadConfig(4,tim,ocs);\
                                                 }while(0)

#endif

typedef enum
{
    E_TIM_0 = 0,
    E_TIM_1 = 1,
    E_TIM_2 = 2,
    E_TIM_3 = 3
}eTimType;

// 定时器配置结构体
typedef struct
{
    TIM_TypeDef*    Tim;
    UINT32          Tim_clk;
    UINT16          Tim_irqn;
    UINT16          Tim_arr;
    UINT32          Mtx_Timer_Freq;
    #if TIM_MTx_PWM
    
    UINT8           Mtx_Tim_Pwm_Ch;
    UINT16          Mtx_Tim_Pwm_Pin;
    GPIO_TypeDef*   Mtx_Tim_Pwm_Port;
    UINT32          Mtx_Tim_Pwm_Clk;
    
    UINT32          MTx_Tim_Pwm_Pinremap;
    #endif
}tTIMConfig;
_EXT_ tTIMConfig TimConfig[MTx_NUM];

UINT8 Fun_Time_Init(UINT8 eTim);
UINT8 Fun_Mtx_TimInterruptON(UINT8 eTim);
UINT8 Fun_Mtx_TimInterruptOFF(UINT8 eTim);
UINT8 Fun_Mtx_TimInterruptClear(UINT8 eTim);
UINT8 Fun_Mtx_TimCountON(UINT8 eTim);
UINT8 Fun_Mtx_TimCountOFF(UINT8 eTim);
UINT8 Fun_Mtx_TimLoad(UINT8 eTim, UINT16 nLoad);
UINT8 Fun_Mtx_TimPlusCtrl(UINT8 NewState);
UINT8 Time6_Init(void);
#endif
