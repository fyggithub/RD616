#ifndef _LED_KEY_CTRL_H__
#define _LED_KEY_CTRL_H__

//==================================================
// °´¼üµÆIO

// °´¼üµÆ¸öÊý
#define LED_KEY_CTRL_NUM        6

#define WASTE_LIGHT_NUM             4

//==================================================
// °´¼üµÆ1  Ñù±¾ÅÌ×´Ì¬µÆ
#define LK_KEY_1_CTRL_PIN       GPIO_Pin_8
#define LK_KEY_1_CTRL_PORT      GPIOD
#define LK_KEY_1_CTRL_CLK       RCC_APB2Periph_GPIOD

#define LK_LED_1_CTRL_PIN       GPIO_Pin_9
#define LK_LED_1_CTRL_PORT      GPIOD
#define LK_LED_1_CTRL_CLK       RCC_APB2Periph_GPIOD


//==================================================
// °´¼üµÆ2   ÊÔ¼ÁÅÌ×´Ì¬µÆ

#define LK_KEY_2_CTRL_PIN       GPIO_Pin_10
#define LK_KEY_2_CTRL_PORT      GPIOD
#define LK_KEY_2_CTRL_CLK       RCC_APB2Periph_GPIOD

#define LK_LED_2_CTRL_PIN       GPIO_Pin_11
#define LK_LED_2_CTRL_PORT      GPIOD
#define LK_LED_2_CTRL_CLK       RCC_APB2Periph_GPIOD


//==================================================
// °´¼üµÆ3    µ×Îï1×´Ì¬µÆ

#define LK_KEY_3_CTRL_PIN       GPIO_Pin_12
#define LK_KEY_3_CTRL_PORT      GPIOD
#define LK_KEY_3_CTRL_CLK       RCC_APB2Periph_GPIOD

#define LK_LED_3_CTRL_PIN       GPIO_Pin_13
#define LK_LED_3_CTRL_PORT      GPIOD
#define LK_LED_3_CTRL_CLK       RCC_APB2Periph_GPIOD


//==================================================
// °´¼üµÆ4    µ×Îï2×´Ì¬µÆ

#define LK_KEY_4_CTRL_PIN       GPIO_Pin_14
#define LK_KEY_4_CTRL_PORT      GPIOD
#define LK_KEY_4_CTRL_CLK       RCC_APB2Periph_GPIOD

#define LK_LED_4_CTRL_PIN       GPIO_Pin_15
#define LK_LED_4_CTRL_PORT      GPIOD
#define LK_LED_4_CTRL_CLK       RCC_APB2Periph_GPIOD

//==================================================
// °´¼üµÆ5    ·ÏÁÏÏä1×´Ì¬µÆ
#define LK_KEY_5_CTRL_PIN       GPIO_Pin_6
#define LK_KEY_5_CTRL_PORT      GPIOC
#define LK_KEY_5_CTRL_CLK       RCC_APB2Periph_GPIOC

#define LK_LED_5_CTRL_PIN       GPIO_Pin_7
#define LK_LED_5_CTRL_PORT      GPIOC
#define LK_LED_5_CTRL_CLK       RCC_APB2Periph_GPIOC


//==================================================
// °´¼üµÆ6    ·ÏÁÏÏä2×´Ì¬µÆ

#define LK_KEY_6_CTRL_PIN       GPIO_Pin_8
#define LK_KEY_6_CTRL_PORT      GPIOC
#define LK_KEY_6_CTRL_CLK       RCC_APB2Periph_GPIOC

#define LK_LED_6_CTRL_PIN       GPIO_Pin_9
#define LK_LED_6_CTRL_PORT      GPIOC
#define LK_LED_6_CTRL_CLK       RCC_APB2Periph_GPIOC


//==================================================
// °´¼üµÆ7

#define LK_KEY_7_CTRL_PIN       GPIO_Pin_6
#define LK_KEY_7_CTRL_PORT      GPIOD
#define LK_KEY_7_CTRL_CLK       RCC_APB2Periph_GPIOD

#define LK_LED_7_CTRL_PIN       GPIO_Pin_7
#define LK_LED_7_CTRL_PORT      GPIOD
#define LK_LED_7_CTRL_CLK       RCC_APB2Periph_GPIOD


//==================================================
// °´¼üµÆ8

#define LK_KEY_8_CTRL_PIN       GPIO_Pin_3
#define LK_KEY_8_CTRL_PORT      GPIOB
#define LK_KEY_8_CTRL_CLK       RCC_APB2Periph_GPIOB

#define LK_LED_8_CTRL_PIN       GPIO_Pin_4
#define LK_LED_8_CTRL_PORT      GPIOB
#define LK_LED_8_CTRL_CLK       RCC_APB2Periph_GPIOB

//=====================================================
#define LK_KEYx_CTRL_PIN(x)         LK_KEY_##x##_CTRL_PIN
#define LK_KEYx_CTRL_PORT(x)        LK_KEY_##x##_CTRL_PORT
#define LK_KEYx_CTRL_CLK(x)         LK_KEY_##x##_CTRL_CLK

#define LK_LEDx_CTRL_PIN(x)			LK_LED_##x##_CTRL_PIN
#define LK_LEDx_CTRL_PORT(x)        LK_LED_##x##_CTRL_PORT
#define LK_LEDx_CTRL_CLK(x)			LK_LED_##x##_CTRL_CLK



//-------------------------------------------------------------------
#define LK_KEY_CTRL_PIN(x,y)            do{  if(x==0) y = LK_KEYx_CTRL_PIN(1);\
                                        else if(x==1) y = LK_KEYx_CTRL_PIN(2);\
                                        else if(x==2) y = LK_KEYx_CTRL_PIN(3);\
                                        else if(x==3) y = LK_KEYx_CTRL_PIN(4);\
                                        else if(x==4) y = LK_KEYx_CTRL_PIN(5);\
                                        else if(x==5) y = LK_KEYx_CTRL_PIN(6);\
                                        else if(x==6) y = LK_KEYx_CTRL_PIN(7);\
                                        else if(x==7) y = LK_KEYx_CTRL_PIN(8);\
                                        }while(0)
                                                                                
#define LK_KEY_CTRL_PORT(x,y)          do{  if(x==0) y = LK_KEYx_CTRL_PORT(1);\
                                        else if(x==1) y = LK_KEYx_CTRL_PORT(2);\
                                        else if(x==2) y = LK_KEYx_CTRL_PORT(3);\
                                        else if(x==3) y = LK_KEYx_CTRL_PORT(4);\
                                        else if(x==4) y = LK_KEYx_CTRL_PORT(5);\
                                        else if(x==5) y = LK_KEYx_CTRL_PORT(6);\
                                        else if(x==6) y = LK_KEYx_CTRL_PORT(7);\
                                        else if(x==7) y = LK_KEYx_CTRL_PORT(8);\
                                        }while(0)
                                        
#define LK_KEY_CTRL_CLK(x,y)           do{  if(x==0) y = LK_KEYx_CTRL_CLK(1);\
                                        else if(x==1) y = LK_KEYx_CTRL_CLK(2);\
                                        else if(x==2) y = LK_KEYx_CTRL_CLK(3);\
                                        else if(x==3) y = LK_KEYx_CTRL_CLK(4);\
                                        else if(x==4) y = LK_KEYx_CTRL_CLK(5);\
                                        else if(x==5) y = LK_KEYx_CTRL_CLK(6);\
                                        else if(x==6) y = LK_KEYx_CTRL_CLK(7);\
                                        else if(x==7) y = LK_KEYx_CTRL_CLK(8);\
                                        }while(0)

//------------------------------------------------------------------
#define LK_LED_CTRL_PIN(x,y)            do{  if(x==0) y = LK_LEDx_CTRL_PIN(1);\
                                        else if(x==1) y = LK_LEDx_CTRL_PIN(2);\
                                        else if(x==2) y = LK_LEDx_CTRL_PIN(3);\
                                        else if(x==3) y = LK_LEDx_CTRL_PIN(4);\
                                        else if(x==4) y = LK_LEDx_CTRL_PIN(5);\
                                        else if(x==5) y = LK_LEDx_CTRL_PIN(6);\
                                        else if(x==6) y = LK_LEDx_CTRL_PIN(7);\
                                        else if(x==7) y = LK_LEDx_CTRL_PIN(8);\
                                        }while(0)

#define LK_LED_CTRL_PORT(x,y)           do{  if(x==0) y = LK_LEDx_CTRL_PORT(1);\
                                        else if(x==1) y = LK_LEDx_CTRL_PORT(2);\
                                        else if(x==2) y = LK_LEDx_CTRL_PORT(3);\
                                        else if(x==3) y = LK_LEDx_CTRL_PORT(4);\
                                        else if(x==4) y = LK_LEDx_CTRL_PORT(5);\
                                        else if(x==5) y = LK_LEDx_CTRL_PORT(6);\
                                        else if(x==6) y = LK_LEDx_CTRL_PORT(7);\
                                        else if(x==7) y = LK_LEDx_CTRL_PORT(8);\
                                        }while(0)

#define LK_LED_CTRL_CLK(x,y)            do{  if(x==0) y = LK_LEDx_CTRL_CLK(1);\
                                        else if(x==1) y = LK_LEDx_CTRL_CLK(2);\
                                        else if(x==2) y = LK_LEDx_CTRL_CLK(3);\
                                        else if(x==3) y = LK_LEDx_CTRL_CLK(4);\
                                        else if(x==4) y = LK_LEDx_CTRL_CLK(5);\
                                        else if(x==5) y = LK_LEDx_CTRL_CLK(6);\
                                        else if(x==6) y = LK_LEDx_CTRL_CLK(7);\
                                        else if(x==7) y = LK_LEDx_CTRL_CLK(8);\
                                        }while(0)

                                        
typedef enum
{
    e_ledkey_0 = 0,
    e_ledkey_1 = 1,
    e_ledkey_2 = 2,
    e_ledkey_3 = 3,
    e_ledkey_4 = 4,
    e_ledkey_5 = 5,
    e_ledkey_6 = 6,
    e_ledkey_7 = 7,
}e_ledkeyType;

                                       
typedef UINT8  (*funlkOne)   (e_ledkeyType);
typedef UINT8  (*funlkTwo)   (UINT8,UINT16);
typedef UINT32 (*funlkThree) (UINT8);
typedef UINT8  (*funlkFour)  (UINT32);
typedef UINT16 (*funlkFive)  (UINT8, UINT8);

typedef struct
{
    UINT16          lk_keyctrl_Pin;
    UINT32          lk_keyctrl_Clk;
    GPIO_TypeDef*   lk_keyctrl_Port;

    GPIO_TypeDef*   lk_ledctrl_Port;
    UINT32          lk_ledctrl_Clk;
    UINT16          lk_ledctrl_Pin;

}st_ledkeyPinConfig;

typedef struct
{
    funlkOne   lk_pinInit;
    funlkOne   lk_ledon;
    funlkOne   lk_ledoff;
    funlkOne   lk_keyget;

}st_ledkeyCtrlFunList;


typedef struct
{
    st_ledkeyPinConfig  s_lkpin[LED_KEY_CTRL_NUM];
    st_ledkeyCtrlFunList s_lkfun;
}st_lkCtrlType;

typedef struct
{
    UINT8 led_state[LED_KEY_CTRL_NUM];
    UINT8 key_state[LED_KEY_CTRL_NUM];
    UINT8 wastelight_state[WASTE_LIGHT_NUM];

}st_lkFlagType;


_EXT_ st_lkCtrlType s_lkCtrl;
_EXT_ st_lkFlagType s_lkFlag;

UINT8 LK_LedPinInit(e_ledkeyType LkId);
UINT8 LK_Led_On(e_ledkeyType LkId);
UINT8 LK_Led_Off(e_ledkeyType LkId);
UINT8 LK_KeyGetState(e_ledkeyType LkId);
void LK_PinAllInit(void);
UINT8 Waste_Light_PinInit(void);

//===============================================================
// ¹âñîIO


UINT8 Get_WasteCup_Light_State(UINT8 lightid);

#endif

