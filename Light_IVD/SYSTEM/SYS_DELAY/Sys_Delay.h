#ifndef __SYS_DELAY_H__
#define __SYS_DELAY_H__
		   
typedef struct
{
    unsigned long m_nSysTicks;
    unsigned long m_nSysTicksEnd;
}tSysTick_Count;

void Sys_delay_init(void);
void Sys_Delay_ms(unsigned long nDlyMs);
unsigned long Sys_GetTicks(void);

void Sys_SetDlyms(unsigned long nDlyMs);
unsigned char Sys_CheckDlyFin(void);

#endif
