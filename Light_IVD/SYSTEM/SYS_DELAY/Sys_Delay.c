#ifndef __SYS_DELAY_C__
#define __SYS_DELAY_C__

#include "stm32f10x.h"
#include "CONFIG\My_define.h"
#include "SYS_DELAY\Sys_Delay.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//如果需要使用OS,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif
#include "stdio.h"


tSysTick_Count SysTick_Count;

//仅作UCOSII和UCOSIII的支持,其他OS,请自行参考着移植
//支持UCOSII
#ifdef 	OS_CRITICAL_METHOD						//OS_CRITICAL_METHOD定义了,说明要支持UCOSII				
#define delay_osrunning		OSRunning			//OS是否运行标记,0,不运行;1,在运行
#define delay_ostickspersec	OS_TICKS_PER_SEC	//OS时钟节拍,即每秒调度次数
#define delay_osintnesting 	OSIntNesting		//中断嵌套级别,即中断嵌套次数
#endif

//支持UCOSIII
#ifdef 	CPU_CFG_CRITICAL_METHOD					//CPU_CFG_CRITICAL_METHOD定义了,说明要支持UCOSIII	
#define delay_osrunning		OSRunning			//OS是否运行标记,0,不运行;1,在运行
#define delay_ostickspersec	OSCfg_TickRate_Hz	//OS时钟节拍,即每秒调度次数
#define delay_osintnesting 	OSIntNestingCtr		//中断嵌套级别,即中断嵌套次数
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
#if SYSTEM_SUPPORT_OS  							        //如果需要支持OS.
	reload=tClockTree.SYSCLK_Frequency/delay_ostickspersec; //每秒钟的计数次数 单位为K	   

#else
    reload = tClockTree.SYSCLK_Frequency/1000;              // 默认为1000，为1ms中断一次
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
 
//systick中断服务函数,使用ucos时用到
void SysTick_Handler(void)
{	
#if SYSTEM_SUPPORT_OS  
	if(delay_osrunning==1)						//OS开始跑了,才执行正常的调度处理
	{
		OSIntEnter();							//进入中断
		OSTimeTick();       					//调用ucos的时钟服务程序      
#if (TEMP_ADC_ENABLE==1)
        TempTickHandler();     
#endif
		OSIntExit();       	 					//触发任务切换软中断
        
	}
#else
    // system time delay counter
	SysTick_Count.m_nSysTicks++;
#endif
}

// 延时指定时间
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

// 获得计数值
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
// 设置延时时间
void Sys_SetDlyms(unsigned long nDlyMs)
{
    SysTick_Count.m_nSysTicksEnd = SysTick_Count.m_nSysTicks + nDlyMs;
}

// 检测延时是否完成
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


