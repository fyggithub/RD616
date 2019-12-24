#ifndef __SYS_DELAY_C__
#define __SYS_DELAY_C__

#include "stm32f10x.h"
#include "CONFIG\My_define.h"
#include "SYS_DELAY\Sys_Delay.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//�����Ҫʹ��OS,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ʹ��	  
#endif
#include "stdio.h"


tSysTick_Count SysTick_Count;

//����UCOSII��UCOSIII��֧��,����OS,�����вο�����ֲ
//֧��UCOSII
#ifdef 	OS_CRITICAL_METHOD						//OS_CRITICAL_METHOD������,˵��Ҫ֧��UCOSII				
#define delay_osrunning		OSRunning			//OS�Ƿ����б��,0,������;1,������
#define delay_ostickspersec	OS_TICKS_PER_SEC	//OSʱ�ӽ���,��ÿ����ȴ���
#define delay_osintnesting 	OSIntNesting		//�ж�Ƕ�׼���,���ж�Ƕ�״���
#endif

//֧��UCOSIII
#ifdef 	CPU_CFG_CRITICAL_METHOD					//CPU_CFG_CRITICAL_METHOD������,˵��Ҫ֧��UCOSIII	
#define delay_osrunning		OSRunning			//OS�Ƿ����б��,0,������;1,������
#define delay_ostickspersec	OSCfg_TickRate_Hz	//OSʱ�ӽ���,��ÿ����ȴ���
#define delay_osintnesting 	OSIntNestingCtr		//�ж�Ƕ�׼���,���ж�Ƕ�״���
#endif

void Sys_delay_init(void)
{
    unsigned long reload;            
    RCC_ClocksTypeDef tClockTree;
    
    SystemCoreClockUpdate();
	RCC_GetClocksFreq(&tClockTree);
#if 0
	// printf("\r\n--- SystemCoreClock = %d ---\r\n", SystemCoreClock);
	printf("\r\n sys-clk = %d", tClockTree.SYSCLK_Frequency);
	printf("\r\n AHB-clk = %d", tClockTree.HCLK_Frequency);
	printf("\r\n APB1-clk = %d", tClockTree.PCLK1_Frequency);
	printf("\r\n APB2-clk = %d", tClockTree.PCLK2_Frequency);
#endif
#if SYSTEM_SUPPORT_OS  							        //�����Ҫ֧��OS.
	reload=tClockTree.SYSCLK_Frequency/delay_ostickspersec; //ÿ���ӵļ������� ��λΪK	   

#else
    reload = tClockTree.SYSCLK_Frequency/1000;              // Ĭ��Ϊ1000��Ϊ1ms�ж�һ��
#endif
    //-------------------------------------------
    // 4. Initialize and start the SysTick counter and its interrupt. 
    //    take attention: when want to use IRQ_DelayMs(), this must be called first !!!
	if(SysTick_Config(reload))   // Setup SysTick Timer for 1 millisecond interrupts. 
    { 
#if 0
        // Capture error / 
	    printf("\r\nSysTick_Config error\r\n");
#endif
        while (1);
    }
    // attentio: set to the higher priority
	NVIC_SetPriority(SysTick_IRQn, 0);
    SysTick_Count.m_nSysTicks = 0;
    SysTick_Count.m_nSysTicksEnd = 0;

}

#if (TEMP_ADC_ENABLE==1)
static void TempTickHandler(void)
{
  uint16_t i;
  for(i=0; i<TEMPERATURE_NUM-1; i++)
   {
//     if( IS_TEMP_CTL(&TempCtl[i]) )
//      {
        if(TempCtl[i].on_ticks > 0)
        {
          TempCtl[i].on_ticks -= 1;
          if(TempCtl[i].on_ticks == 0)
           {
             TempCtl[i].TurnHeat(i,0);
           }
        }
        
//      }
   }
}
#endif
 
//systick�жϷ�����,ʹ��ucosʱ�õ�
void SysTick_Handler(void)
{	
#if SYSTEM_SUPPORT_OS  
	if(delay_osrunning==1)						//OS��ʼ����,��ִ�������ĵ��ȴ���
	{
		OSIntEnter();							//�����ж�
		OSTimeTick();       					//����ucos��ʱ�ӷ������      
#if (TEMP_ADC_ENABLE==1)
        TempTickHandler();     
#endif
		OSIntExit();       	 					//���������л����ж�
        
	}
#else
    // system time delay counter
	SysTick_Count.m_nSysTicks++;
#endif
}

// ��ʱָ��ʱ��
void Sys_Delay_ms(unsigned long nDlyMs)
{
#if SYSTEM_SUPPORT_OS 
    OS_ERR err;    
    OSTimeDly(nDlyMs,OS_OPT_TIME_DLY,&err);
    if(err)
    {
        printf("Delay_ERR %d\r\n",err);
    }
#else
    SysTick_Count.m_nSysTicksEnd = SysTick_Count.m_nSysTicks + nDlyMs;
	while(SysTick_Count.m_nSysTicks < SysTick_Count.m_nSysTicksEnd);
#endif
}

// ��ü���ֵ
unsigned long Sys_GetTicks(void)
{
#if SYSTEM_SUPPORT_OS 
    OS_ERR err;    
    return OSTimeGet(&err);
#else
    return SysTick_Count.m_nSysTicks;
#endif
}

#if (SYSTEM_SUPPORT_OS == 0)
// ������ʱʱ��
void Sys_SetDlyms(unsigned long nDlyMs)
{
    SysTick_Count.m_nSysTicksEnd = SysTick_Count.m_nSysTicks + nDlyMs;
}

// �����ʱ�Ƿ����
unsigned char Sys_CheckDlyFin(void)
{
    if( SysTick_Count.m_nSysTicks >= SysTick_Count.m_nSysTicksEnd)
    {
        return 1;
	}
	else
	{
        return 0;
	}
}
#endif

#endif


