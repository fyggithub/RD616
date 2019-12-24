/*********************************************************************************
**********************************************************************************
**********************************************************************************
* �ļ�����: Mtx_Timer.c                                                         *
* �ļ�������                                                                    *
* �������ڣ�2014.10.03                                                          *
* ��    ����V1.13                                                               *
* ��    �ߣ�LY Chen                                                             *
* ˵    ������Ҫ��ʼ��pwm����������                                            * 
**********************************************************************************�B
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
    
    // ��Ӧ�Ķ�ʱ����Ӧ�ĵ��
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


//ͨ�ö�ʱ��3�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
UINT8 Fun_Time_Init(UINT8 eTim)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef        NVIC_InitStructure;
    RCC_ClocksTypeDef       mtTimerClock;
    #if TIM_MTx_PWM
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    #endif
    // ���ö�ʱ������
    Fun_ConfigTimStruct(eTim);
    
    TIM_DeInit(TimConfig[eTim].Tim);

    RCC_APB1PeriphClockCmd(TimConfig[eTim].Tim_clk, ENABLE); //ʱ��ʹ��
    #if TIM_MTx_PWM
    RCC_APB2PeriphClockCmd(TimConfig[eTim].Mtx_Tim_Pwm_Clk  | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��
	
    if(TimConfig[eTim].MTx_Tim_Pwm_Pinremap)
        GPIO_PinRemapConfig(TimConfig[eTim].MTx_Tim_Pwm_Pinremap, ENABLE); //Timer3������ӳ��  TIM3_CH2->PB5    
 
   //���ø�����Ϊ�����������,���TIM3 CH2��PWM���岨��	GPIOB.5
	GPIO_InitStructure.GPIO_Pin = TimConfig[eTim].Mtx_Tim_Pwm_Pin; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(TimConfig[eTim].Mtx_Tim_Pwm_Port, &GPIO_InitStructure);//��ʼ��GPIO
    #endif
    //----------------------------------------------------------------------
    //��ʱ����ʼ��
    TIM_TimeBaseStructure.TIM_Period = 3000; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ  
    TIM_TimeBaseStructure.TIM_Prescaler =7; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down;  //TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TimConfig[eTim].Tim, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

    #if TIM_MTx_PWM
	//��ʼ��TIM3 Channel2 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode      = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity  = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ը�
	TIM_OCInitStructure.TIM_Pulse       = 200;
    MT_TIM_OCInit(TimConfig[eTim].Mtx_Tim_Pwm_Ch,TimConfig[eTim].Tim, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC2
    #endif
	
    
    //----------------------------------------------------------------------
    //�ж����ȼ�NVIC����
    NVIC_InitStructure.NVIC_IRQChannel = TimConfig[eTim].Tim_irqn;  //TIM3�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�0��
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = eTim;  //�����ȼ�3��
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
    NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���

    //-----------------------------------------------------------------------
    // ��ʼ������
    TIM_ARRPreloadConfig(TimConfig[eTim].Tim, ENABLE);
    TIM_ClearFlag(TimConfig[eTim].Tim, TIM_FLAG_Update);
    
    TIM_ITConfig(TimConfig[eTim].Tim,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�
    
    #if TIM_MTx_PWM
    MT_TIM_OCPreloadConfig(TimConfig[eTim].Mtx_Tim_Pwm_Ch,TimConfig[eTim].Tim, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���
    #endif
    
    TIM_Cmd(TimConfig[eTim].Tim, DISABLE);  //ʹ��TIMx   

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
    OSIntEnter();							//�����ж�
#endif
    TIM_Cmd(TIM6, NewState);
#if SYSTEM_SUPPORT_OS
    OSIntExit();       	 					//���������л����ж�
#endif
}

//��ʱ��3�жϷ������
void TIM2_IRQHandler(void)   //TIM3�ж�
{
#if SYSTEM_SUPPORT_OS
    OSIntEnter();							//�����ж�
#endif
    Mtx_Pwm_ISR(Time2_Irq_Num);
#if SYSTEM_SUPPORT_OS
    OSIntExit();       	 					//���������л����ж�
#endif
}

//��ʱ��3�жϷ������
void TIM3_IRQHandler(void)   //TIM3�ж�
{
#if SYSTEM_SUPPORT_OS
    OSIntEnter();							//�����ж�
#endif
    Mtx_Pwm_ISR(Time3_Irq_Num);
#if SYSTEM_SUPPORT_OS
    OSIntExit();       	 					//���������л����ж�
#endif
}

//��ʱ��3�жϷ������
void TIM4_IRQHandler(void)   //TIM3�ж�
{
#if SYSTEM_SUPPORT_OS
    OSIntEnter();							//�����ж�
#endif
    Mtx_Pwm_ISR(Time4_Irq_Num);
#if SYSTEM_SUPPORT_OS
    OSIntExit();       	 					//���������л����ж�
#endif    
}

//��ʱ��3�жϷ������
void TIM5_IRQHandler(void)   //TIM3�ж�
{
#if SYSTEM_SUPPORT_OS
    OSIntEnter();       	 					//���������л����ж�
#endif    
    Mtx_Pwm_ISR(Time5_Irq_Num);
#if SYSTEM_SUPPORT_OS
    OSIntExit();       	 					//���������л����ж�
#endif    
}



#endif










