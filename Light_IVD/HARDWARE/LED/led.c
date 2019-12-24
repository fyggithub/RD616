#ifndef _LED_C__
#define _LED_C__


#include "My_includes.h"

st_Led_CtrlModule sLed_Ctrl;

void Led_Initialization(void)
{
    Led_Config_init();
    sLed_Ctrl.sLed_FunList.fun_Led_All_Reset();
}    

void Led_Config_init(void)
{
    // LED Clk init
    sLed_Ctrl.sLed_Gpio.Led_Gpio_Clk[em_led_01]     = LED_01_CLK;
#if 0
    sLed_Ctrl.sLed_Gpio.Led_Gpio_Clk[em_led_02]     = LED_02_CLK;
    sLed_Ctrl.sLed_Gpio.Led_Gpio_Clk[em_led_03]     = LED_03_CLK;
#endif
    // LED Port init
    sLed_Ctrl.sLed_Gpio.Led_Gpio_Port[em_led_01]    = LED_01_PORT;
#if 0
    sLed_Ctrl.sLed_Gpio.Led_Gpio_Port[em_led_02]    = LED_02_PORT;
    sLed_Ctrl.sLed_Gpio.Led_Gpio_Port[em_led_03]    = LED_03_PORT;
#endif
    // LED Pin init
    sLed_Ctrl.sLed_Gpio.Led_Gpio_Pin[em_led_01]     = LED_01_PIN;
#if 0
    sLed_Ctrl.sLed_Gpio.Led_Gpio_Pin[em_led_02]     = LED_02_PIN;
    sLed_Ctrl.sLed_Gpio.Led_Gpio_Pin[em_led_03]     = LED_03_PIN;
#endif
    // LED function config
    sLed_Ctrl.sLed_FunList.fun_Led_Pin_Init         = LED_Pin_Init;
    sLed_Ctrl.sLed_FunList.fun_Led_All_Reset        = LED_All_Reset;
    sLed_Ctrl.sLed_FunList.fun_Led_Set              = LED_Set;
    sLed_Ctrl.sLed_FunList.fun_Led_Reset            = LED_Reset;
    sLed_Ctrl.sLed_FunList.fun_Led_Toggle           = LED_Toggle;
}

void LED_Pin_Init(em_Led_Number led_id)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(sLed_Ctrl.sLed_Gpio.Led_Gpio_Clk[led_id],ENABLE);
	
	GPIO_InitStructure.GPIO_Pin     = sLed_Ctrl.sLed_Gpio.Led_Gpio_Pin[led_id];
    
#if defined (STM32F40_41xxx)
	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_OUT;//输出
	GPIO_InitStructure.GPIO_OType   = GPIO_OType_PP;  //推挽输出
	GPIO_InitStructure.GPIO_PuPd    = GPIO_PuPd_NOPULL;  //上拉输出
	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_100MHz; //高速GPIO
    
#else													   

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
#endif

    
	GPIO_Init(sLed_Ctrl.sLed_Gpio.Led_Gpio_Port[led_id],&GPIO_InitStructure);
	
	GPIO_SetBits(sLed_Ctrl.sLed_Gpio.Led_Gpio_Port[led_id],sLed_Ctrl.sLed_Gpio.Led_Gpio_Pin[led_id]); //GPIOF 高电平
}

void LED_All_Reset(void)
{
    em_Led_Number led_id;
    for(led_id = 0; led_id < LED_NUM; led_id++)
    {
        sLed_Ctrl.sLed_FunList.fun_Led_Pin_Init(led_id);
    }
}

void LED_Set(em_Led_Number led_id)
{
    sLed_Ctrl.sLed_Gpio.Led_Gpio_Port[led_id]->BSRR = sLed_Ctrl.sLed_Gpio.Led_Gpio_Pin[led_id];
}

void LED_Reset(em_Led_Number led_id)
{
    sLed_Ctrl.sLed_Gpio.Led_Gpio_Port[led_id]->BRR = sLed_Ctrl.sLed_Gpio.Led_Gpio_Pin[led_id];
}

void LED_Toggle(em_Led_Number led_id)
{
    sLed_Ctrl.sLed_Gpio.Led_Gpio_Port[led_id]->ODR ^= sLed_Ctrl.sLed_Gpio.Led_Gpio_Pin[led_id];
}


void Led_Task(void *p_arg)
{
    OS_ERR      err;
    _STA_ UINT8 int_key = 0;
	UINT8 ARPTimer = 0;
    
	(void)p_arg;
        
    while(1)
    {
        sLed_Ctrl.sLed_FunList.fun_Led_Reset(em_led_01);
        Sys_Delay_ms(500);
        sLed_Ctrl.sLed_FunList.fun_Led_Set(em_led_01);
        Sys_Delay_ms(500);

    }
}

#endif


