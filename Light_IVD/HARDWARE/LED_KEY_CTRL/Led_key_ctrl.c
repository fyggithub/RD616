#ifndef _LED_KEY_CTRL_C__
#define _LED_KEY_CTRL_C__

#include "My_includes.h"


st_lkCtrlType s_lkCtrl;

st_lkFlagType s_lkFlag;

void LK_FUn_Config(void)
{
    s_lkCtrl.s_lkfun.lk_pinInit = LK_LedPinInit;
    s_lkCtrl.s_lkfun.lk_ledon   = LK_Led_On;
    s_lkCtrl.s_lkfun.lk_ledoff  = LK_Led_Off;
    s_lkCtrl.s_lkfun.lk_keyget  = LK_KeyGetState;
}

void LK_Pin_ConfigStruct(e_ledkeyType lkId)
{
    LK_KEY_CTRL_PIN(lkId,s_lkCtrl.s_lkpin[lkId].lk_keyctrl_Pin);
    LK_KEY_CTRL_PORT(lkId,s_lkCtrl.s_lkpin[lkId].lk_keyctrl_Port);
    LK_KEY_CTRL_CLK(lkId,s_lkCtrl.s_lkpin[lkId].lk_keyctrl_Clk);

    LK_LED_CTRL_PIN(lkId,s_lkCtrl.s_lkpin[lkId].lk_ledctrl_Pin);
    LK_LED_CTRL_PORT(lkId,s_lkCtrl.s_lkpin[lkId].lk_ledctrl_Port);
    LK_LED_CTRL_CLK(lkId,s_lkCtrl.s_lkpin[lkId].lk_ledctrl_Clk);

}

UINT8 LK_LedPinInit(e_ledkeyType LkId)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    LK_Pin_ConfigStruct(LkId);
    
    RCC_APB2PeriphClockCmd(s_lkCtrl.s_lkpin[LkId].lk_keyctrl_Clk | \
                           s_lkCtrl.s_lkpin[LkId].lk_ledctrl_Clk,ENABLE);
   
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin   = s_lkCtrl.s_lkpin[LkId].lk_ledctrl_Pin;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(s_lkCtrl.s_lkpin[LkId].lk_ledctrl_Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin   = s_lkCtrl.s_lkpin[LkId].lk_keyctrl_Pin;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(s_lkCtrl.s_lkpin[LkId].lk_keyctrl_Port, &GPIO_InitStructure);
    
    return e_true;
}

void LK_PinAllInit(void)
{
    UINT8 i = 0;
    
    LK_FUn_Config();
    for(i=0;i<LED_KEY_CTRL_NUM;i++)
    {
        s_lkCtrl.s_lkfun.lk_pinInit(i);
    }
    
}

UINT8 LK_Led_On(e_ledkeyType LkId)
{
    s_lkCtrl.s_lkpin[LkId].lk_ledctrl_Port->BSRR = s_lkCtrl.s_lkpin[LkId].lk_ledctrl_Pin;
}

UINT8 LK_Led_Off(e_ledkeyType LkId)
{
    s_lkCtrl.s_lkpin[LkId].lk_ledctrl_Port->BRR = s_lkCtrl.s_lkpin[LkId].lk_ledctrl_Pin;
}

void LK_Led_Toggle(e_ledkeyType LkId)
{
    s_lkCtrl.s_lkpin[LkId].lk_ledctrl_Port->ODR ^= s_lkCtrl.s_lkpin[LkId].lk_ledctrl_Pin;
}

UINT8 LK_KeyGetState(e_ledkeyType LkId)
{
//    return GPIO_ReadInputDataBit(MtxLightConfig[MtxId].Reset_Light_Port,MtxLightConfig[MtxId].Reset_Light_Pin);
    if(s_lkCtrl.s_lkpin[LkId].lk_keyctrl_Port->IDR & s_lkCtrl.s_lkpin[LkId].lk_keyctrl_Pin)
        return Bit_SET;
    else
        return Bit_RESET;
}

// ·ÏÁÏºÐ¹âñî¼ì²â
UINT16 Waste_Light_Pin[WASTE_LIGHT_NUM] = 
{
    GPIO_Pin_5,     // ·ÏÒººÐ¹âñî1
    GPIO_Pin_4,     // ·ÏÒººÐ¹âñî2
    GPIO_Pin_3,     // ·ÏÒºÂú¹âñî1
    GPIO_Pin_2      // ·ÏÒºÂú¹âñî2
};

UINT32 Waste_Light_Clk[WASTE_LIGHT_NUM] = 
{
    RCC_APB2Periph_GPIOE,     // ·ÏÒººÐ¹âñî1
    RCC_APB2Periph_GPIOE,     // ·ÏÒººÐ¹âñî2
    RCC_APB2Periph_GPIOE,     // ·ÏÒºÂú¹âñî1
    RCC_APB2Periph_GPIOE      // ·ÏÒºÂú¹âñî2
};

GPIO_TypeDef* Waste_Light_Port[WASTE_LIGHT_NUM] = 
{
    GPIOE,     // ·ÏÒººÐ¹âñî1
    GPIOE,     // ·ÏÒººÐ¹âñî2
    GPIOE,     // ·ÏÒºÂú¹âñî1
    GPIOE      // ·ÏÒºÂú¹âñî2
};

UINT8 Waste_Light_PinInit(void)
{
    UINT8 i = 0;
    GPIO_InitTypeDef  GPIO_InitStructure;

    for(i = 0;i<WASTE_LIGHT_NUM; i++)
    {
        RCC_APB2PeriphClockCmd(Waste_Light_Clk[i],ENABLE);
       
        GPIO_InitStructure.GPIO_Pin   = Waste_Light_Pin[i];
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
        GPIO_Init(Waste_Light_Port[i], &GPIO_InitStructure);
    }
    
    return e_true;
}

UINT8 Get_WasteCup_Light_State(UINT8 lightid)
{
    if(Waste_Light_Port[lightid]->IDR & Waste_Light_Pin[lightid])
        return Bit_SET;
    else
        return Bit_RESET;
}

UINT8 key_ret = 0;

void LedKey_Ctrl(void)
{
    _STA_ UINT8 key_check_time[LED_KEY_CTRL_NUM] = {0};
    _STA_ UINT8 led_delay[LED_KEY_CTRL_NUM] = {0};
    UINT8 i = 0;
    UINT8 int_key = 0;
    // °´¼üµÆ×´Ì¬ÏÔÊ¾
    for(i = 0; i<LED_KEY_CTRL_NUM; i++)
    {
        led_delay[i]++;
        if(led_delay[i] == 25)
        {
            led_delay[i] = 0;
            
            if(s_lkFlag.led_state[i] == 0)
            {
                LK_Led_Off(i);
            }
            else if(s_lkFlag.led_state[i] == 1)
            {
                LK_Led_On(i);
            }
            else
            {
                LK_Led_Toggle(i);
            }
        }
        
        // °´¼ü¼ì²â
        int_key = LK_KeyGetState(i);
        if((s_lkFlag.key_state[i] == 1 && int_key) && (key_ret))
        {
            // ·µ»Ø°´¼ü°´ÏÂ
            key_check_time[i]++;
            
            if(i == 0 || i == 1)
            {
                if(key_check_time[i] == 5)
                {
                    // ÐÞ¸ÄµÆ×´Ì¬
                    if(s_lkFlag.led_state[i] >= 2)
                        s_lkFlag.led_state[i] = 0;
                    else
                        s_lkFlag.led_state[i]++;
                    
                    key_check_time[i] = 0;
                    s_lkFlag.key_state[i] = 0;
                    
                    if(i == 0)
                        io_msg.st_dataCmd = 0x59;//CMD_KEY_STATE_BACK;
                    else
                        io_msg.st_dataCmd = 0x26;//CMD_KEY_STATE_BACK;
                    io_msg.st_dataBuf[0] = 0;//+i;
//                    io_msg.st_dataBuf[1] = 1;
                    io_msg.st_dataLen = 1;
                    io_msg.st_dataSendId = 0x04;
                    Mid_ReBack_Fill(&io_msg,FRAM_CTRL_DATA_S_S);
                }
            }
            else
            {
                
                if(key_check_time[i] == 5)
                {
                    key_check_time[i] = 0;
                    // ÊÔ¼ÁÅÌ°´ÏÂÐý×ª
                    // ÐÞ¸ÄµÆ×´Ì¬
                    if(s_lkFlag.led_state[i] >= 2)
                        s_lkFlag.led_state[i] = 0;
                    else
                        s_lkFlag.led_state[i]++;
                    
#if 0
                    io_msg.st_dataCmd = CMD_KEY_STATE_BACK;
                    io_msg.st_dataBuf[0] = 1+i;
                    io_msg.st_dataBuf[1] = 1;
                    io_msg.st_dataLen = 2;
                    
                    Mid_ReBack_Fill(&io_msg,FRAM_CTRL_DATA_S_M);
#endif
                    s_lkFlag.key_state[i] = 0;
                }
            }
            
        }
        else if((s_lkFlag.key_state[i] == 0) && (!int_key))
        {
            key_ret = 1;
            // ·µ»Ø°´¼ü°´ÏÂ
            key_check_time[i]++;
            
            if(i == 0 || i == 1)
            {
                
                if(key_check_time[i] == 150)
                {
                    key_check_time[i] = 0;
                    s_lkFlag.key_state[i] = 1;
                    
                    if(i == 0)
                        io_msg.st_dataCmd = 0x59;//CMD_KEY_STATE_BACK;
                    else
                        io_msg.st_dataCmd = 0x26;//CMD_KEY_STATE_BACK;
                    io_msg.st_dataBuf[0] = 1;//+i;
//                    io_msg.st_dataBuf[1] = 1;
                    io_msg.st_dataLen = 1;
                    io_msg.st_dataSendId = 0x04;
                    Mid_ReBack_Fill(&io_msg,FRAM_CTRL_DATA_S_S);
                }
            }
            else
            {
                if(key_check_time[i] == 5)
                {
                    key_check_time[i] = 0;
                    // ÊÔ¼ÁÅÌ°´ÏÂÐý×ª
                    
                    
#if 0
                    io_msg.st_dataCmd = CMD_KEY_STATE_BACK;
                    io_msg.st_dataBuf[0] = 1+i;
                    io_msg.st_dataBuf[1] = 1;
                    io_msg.st_dataLen = 2;
                    
                    Mid_ReBack_Fill(&io_msg,FRAM_CTRL_DATA_S_M);
#endif
                    s_lkFlag.key_state[i] = 1;
                }
            }
            
            
//            if(key_check_time[i] == 5)
//            {
//                key_check_time[i] = 0;
//                if(s_lkFlag.led_state[i] == 2)
//                    s_lkFlag.led_state[i] = 0;
//                else
//                    s_lkFlag.led_state[i]++;
//#if 0
//                io_msg.st_dataCmd = CMD_KEY_STATE_BACK;
//                io_msg.st_dataBuf[0] = 1+i;
//                io_msg.st_dataBuf[1] = 1;
//                io_msg.st_dataLen = 2;
//                
//                Mid_ReBack_Fill(&io_msg);
//#endif
//                s_lkFlag.key_state[i] = 0;
//            }
        }
        else
        {
            key_check_time[i] = 0;
        }
        
    }
}

void WasteCup_Light_Check(void)
{
    UINT8 int_light = 0;
    UINT8 i = 0;
    _STA_ UINT8 light_check_time[WASTE_LIGHT_NUM] = {0};
    
    for(i=0;i<WASTE_LIGHT_NUM;i++)
    {
        // °´¼ü¼ì²â
        int_light = Get_WasteCup_Light_State(i);
        if(s_lkFlag.wastelight_state[i] == 0 && int_light)
        {
            // ·µ»Ø¹âñî°´ÏÂ
            light_check_time[i]++;
            if(light_check_time[i] == 5)
            {
                light_check_time[i] = 0;
                io_msg.st_dataCmd = CMD_LIGHT_STATE_BACK;
                io_msg.st_dataBuf[0] = e_WC_1_haveCheck+i;
                io_msg.st_dataBuf[1] = 1;
                io_msg.st_dataLen = 2;
                Mid_ReBack_Fill(&io_msg,FRAM_CTRL_DATA_S_M);
                s_lkFlag.wastelight_state[i] = 1;
            }
        }
        else if((s_lkFlag.wastelight_state[i] == 1) && (!int_light))
        {
            // ·µ»Ø¹âñî°´ÏÂ
            light_check_time[i]++;
            if(light_check_time[i] == 5)
            {
                light_check_time[i] = 0;
                io_msg.st_dataCmd = CMD_LIGHT_STATE_BACK;
                io_msg.st_dataBuf[0] = e_WC_1_haveCheck+i;
                io_msg.st_dataBuf[1] = 0;
                io_msg.st_dataLen = 2;
                Mid_ReBack_Fill(&io_msg,FRAM_CTRL_DATA_S_M);
                s_lkFlag.wastelight_state[i] = 0;
            }
        }
        else
        {
            light_check_time[i] = 0;
        }
    }
}

void LedKey_Ctrl_Task(void *p_arg)
{
    OS_ERR      err;
    
	UINT8 ARPTimer = 0;
    UINT8 i = 0;
    UINT8 key_check_time[LED_KEY_CTRL_NUM] = {0};
    UINT8 led_delay[LED_KEY_CTRL_NUM] = {0};
    
	(void)p_arg;
    for(i = 0; i<LED_KEY_CTRL_NUM; i++)
    {
        s_lkFlag.key_state[i] = 0;
        s_lkFlag.led_state[i] = 2;
    }
    for(i = 0; i<WASTE_LIGHT_NUM; i++)
    {
        s_lkFlag.wastelight_state[i] = 0;
    }
    Sys_Delay_ms(1000);
    while(1)
    {
        
        LedKey_Ctrl();
        
        WasteCup_Light_Check();
        
        
        Sys_Delay_ms(20);
    }
}


#endif


