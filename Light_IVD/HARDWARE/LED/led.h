#ifndef _LED_H__
#define _LED_H__

// LED的个数
#define LED_NUM             1

#define LED_01_PORT         GPIOC
#define LED_01_CLK          RCC_APB2Periph_GPIOC
#define LED_01_PIN          GPIO_Pin_2

#if 0
#define LED_02_PORT         GPIOF
#define LED_02_CLK          RCC_APB2Periph_GPIOF
#define LED_02_PIN          GPIO_Pin_7

#define LED_03_PORT         GPIOF
#define LED_03_CLK          RCC_APB2Periph_GPIOF
#define LED_03_PIN          GPIO_Pin_8
#endif
typedef enum
{
    em_led_01 = 0,
    em_led_02 = 1,
    em_led_03 = 2
}em_Led_Number;

// 定义函数类型
typedef void (*FunLedOne)(void);
typedef void (*FunLedTwo)(em_Led_Number);



typedef struct
{
    GPIO_TypeDef* Led_Gpio_Port[LED_NUM];
    UINT32        Led_Gpio_Clk[LED_NUM];
    UINT16        Led_Gpio_Pin[LED_NUM];
}st_Led_GpioType;

typedef struct
{
    FunLedTwo   fun_Led_Pin_Init;
    FunLedOne   fun_Led_All_Reset;
    FunLedTwo   fun_Led_Set;
    FunLedTwo   fun_Led_Reset;
    FunLedTwo   fun_Led_Toggle;
}st_Led_FunListType;

typedef struct
{
    em_Led_Number       eLed_id;
    st_Led_GpioType     sLed_Gpio; 
    st_Led_FunListType  sLed_FunList;
    
}st_Led_CtrlModule;

_EXT_ st_Led_CtrlModule sLed_Ctrl;

void Led_Initialization(void);
void Led_Config_init(void);
void LED_All_Reset(void);
void LED_Pin_Init(em_Led_Number led_id);
void LED_Set(em_Led_Number led_id);
void LED_Reset(em_Led_Number led_id);
void LED_Toggle(em_Led_Number led_id);
#endif
