/*********************************************************************************
**********************************************************************************
**********************************************************************************
* 文件名称: Mtx_Timer.c                                                         *
* 文件简述：                                                                    *
* 创建日期：2014.10.03                                                          *
* 版    本：V1.13                                                               *
* 作    者：LY Chen                                                             *
* 说    明：主要初始化pwm，发送脉冲                                            * 
**********************************************************************************
*********************************************************************************/
#ifndef __MTx_TIMER_C__
#define __MTx_TIMER_C__


#include "My_includes.h"

tTIMConfig TimConfig[MTx_NUM];
UINT8 Time2_Irq_Num = 0;
UINT8 Time3_Irq_Num = 0;
UINT8 Time4_Irq_Num = 0;
UINT8 Time5_Irq_Num = 0;

void Fun_ConfigTimStruct(eTimType eTim)
{
    MT_TIME(eTim,TimConfig[eTim].Tim);
    MT_TIME_CLK(eTim,TimConfig[eTim].Tim_clk);
    MT_TIME_IRQn(eTim,TimConfig[eTim].Tim_irqn);
    
    // 相应的定时器对应的电机
    if(TimConfig[eTim].Tim == TIM2)
    {
        Time2_Irq_Num = eTim;
    }
    else if(TimConfig[eTim].Tim == TIM3)
    {
        Time3_Irq_Num = eTim;
    }
    else if(TimConfig[eTim].Tim == TIM4)
    {
        Time4_Irq_Num = eTim;
    }
    else if(TimConfig[eTim].Tim == TIM5)
    {
        Time5_Irq_Num = eTim;
    }
    
    #if TIM_MTx_PWM
    MT_TIME_PWM_PIN(eTim,TimConfig[eTim].Mtx_Tim_Pwm_Pin);
    MT_TIME_PWM_PORT(eTim,TimConfig[eTim].Mtx_Tim_Pwm_Port);
    MT_TIME_PWM_CLK(eTim,TimConfig[eTim].Mtx_Tim_Pwm_Clk);
    MT_TIME_PWM_PINREMAP(eTim,TimConfig[eTim].MTx_Tim_Pwm_Pinremap);
    MT_TIME_PWM_CH(eTim,TimConfig[eTim].Mtx_Tim_Pwm_Ch);
    #endif
}


//通用定时器3中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
UINT8 Fun_Time_Init(UINT8 eTim)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef        NVIC_InitStructure;
    RCC_ClocksTypeDef       mtTimerClock;
    #if TIM_MTx_PWM
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    #endif
    // 配置定时器参数
    Fun_ConfigTimStruct(eTim);
    
    TIM_DeInit(TimConfig[eTim].Tim);

    RCC_APB1PeriphClockCmd(TimConfig[eTim].Tim_clk, ENABLE); //时钟使能
    #if TIM_MTx_PWM
    RCC_APB2PeriphClockCmd(TimConfig[eTim].Mtx_Tim_Pwm_Clk  | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟
	
    if(TimConfig[eTim].MTx_Tim_Pwm_Pinremap)
        GPIO_PinRemapConfig(TimConfig[eTim].MTx_Tim_Pwm_Pinremap, ENABLE); //Timer3部分重映射  TIM3_CH2->PB5    
 
   //设置该引脚为复用输出功能,输出TIM3 CH2的PWM脉冲波形	GPIOB.5
	GPIO_InitStructure.GPIO_Pin = TimConfig[eTim].Mtx_Tim_Pwm_Pin; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(TimConfig[eTim].Mtx_Tim_Pwm_Port, &GPIO_InitStructure);//初始化GPIO
    #endif
    //----------------------------------------------------------------------
    //定时器初始化
    TIM_TimeBaseStructure.TIM_Period = 3000; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值  
    TIM_TimeBaseStructure.TIM_Prescaler =7; //设置用来作为TIMx时钟频率除数的预分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down;  //TIM向上计数模式
    TIM_TimeBaseInit(TimConfig[eTim].Tim, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位

    #if TIM_MTx_PWM
	//初始化TIM3 Channel2 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode      = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity  = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性高
	TIM_OCInitStructure.TIM_Pulse       = 200;
    MT_TIM_OCInit(TimConfig[eTim].Mtx_Tim_Pwm_Ch,TimConfig[eTim].Tim, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC2
    #endif
	
    
    //----------------------------------------------------------------------
    //中断优先级NVIC设置
    NVIC_InitStructure.NVIC_IRQChannel = TimConfig[eTim].Tim_irqn;  //TIM3中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级0级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = eTim;  //从优先级3级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器

    //-----------------------------------------------------------------------
    // 初始化设置
    TIM_ARRPreloadConfig(TimConfig[eTim].Tim, ENABLE);
    TIM_ClearFlag(TimConfig[eTim].Tim, TIM_FLAG_Update);
    
    TIM_ITConfig(TimConfig[eTim].Tim,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断
    
    #if TIM_MTx_PWM
    MT_TIM_OCPreloadConfig(TimConfig[eTim].Mtx_Tim_Pwm_Ch,TimConfig[eTim].Tim, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
    #endif
    
    TIM_Cmd(TimConfig[eTim].Tim, DISABLE);  //使能TIMx   

    RCC_GetClocksFreq(&mtTimerClock);
    if(RCC->CFGR & 0xF00)
    {
        TimConfig[eTim].Mtx_Timer_Freq = (mtTimerClock.PCLK1_Frequency<<1)/(TIM_TimeBaseStructure.TIM_Prescaler+1);
        if(set_debug)
        {
            printf("Mtx Freqent %d\r\n",TimConfig[eTim].Mtx_Timer_Freq);
            printf("PCLK1_Frequency %d\r\n",mtTimerClock.PCLK1_Frequency);
        }
    }
    return e_true;
}

UINT8 Fun_Mtx_TimInterruptON(UINT8 eTim)
{
    TIM_ITConfig(TimConfig[eTim].Tim, TIM_IT_Update, ENABLE);
    return e_true;
}

UINT8 Fun_Mtx_TimInterruptOFF(UINT8 eTim)
{
    TIM_ITConfig(TimConfig[eTim].Tim, TIM_IT_Update, DISABLE);
    return e_true;
}

UINT8 Fun_Mtx_TimInterruptClear(UINT8 eTim)
{
    TIM_ClearITPendingBit(TimConfig[eTim].Tim, TIM_FLAG_Update);
    return e_true;
}

UINT8 Fun_Mtx_TimCountON(UINT8 eTim)
{
    TIM_Cmd(TimConfig[eTim].Tim, ENABLE);
    return e_true;
}

UINT8 Fun_Mtx_TimCountOFF(UINT8 eTim)
{
    TIM_Cmd(TimConfig[eTim].Tim, DISABLE);
    return e_true;
}

UINT8 Fun_Mtx_TimLoad(UINT8 eTim, UINT16 nLoad)
{
    TimConfig[eTim].Tim->ARR = nLoad;
    return e_true;
}

UINT8 Fun_Mtx_TimPlusCtrl(UINT8 NewState)
{
#if SYSTEM_SUPPORT_OS
    OSIntEnter();							//进入中断
#endif
    TIM_Cmd(TIM6, NewState);
#if SYSTEM_SUPPORT_OS
    OSIntExit();       	 					//触发任务切换软中断
#endif
}

//定时器3中断服务程序
void TIM2_IRQHandler(void)   //TIM3中断
{
#if SYSTEM_SUPPORT_OS
    OSIntEnter();							//进入中断
#endif
    Mtx_Pwm_ISR(Time2_Irq_Num);
#if SYSTEM_SUPPORT_OS
    OSIntExit();       	 					//触发任务切换软中断
#endif
}

//定时器3中断服务程序
void TIM3_IRQHandler(void)   //TIM3中断
{
#if SYSTEM_SUPPORT_OS
    OSIntEnter();							//进入中断
#endif
    Mtx_Pwm_ISR(Time3_Irq_Num);
#if SYSTEM_SUPPORT_OS
    OSIntExit();       	 					//触发任务切换软中断
#endif
}

//定时器3中断服务程序
void TIM4_IRQHandler(void)   //TIM3中断
{
#if SYSTEM_SUPPORT_OS
    OSIntEnter();							//进入中断
#endif
    Mtx_Pwm_ISR(Time4_Irq_Num);
#if SYSTEM_SUPPORT_OS
    OSIntExit();       	 					//触发任务切换软中断
#endif    
}

//定时器3中断服务程序
void TIM5_IRQHandler(void)   //TIM3中断
{
#if SYSTEM_SUPPORT_OS
    OSIntEnter();       	 					//触发任务切换软中断
#endif    
    Mtx_Pwm_ISR(Time5_Irq_Num);
#if SYSTEM_SUPPORT_OS
    OSIntExit();       	 					//触发任务切换软中断
#endif    
}



#endif










