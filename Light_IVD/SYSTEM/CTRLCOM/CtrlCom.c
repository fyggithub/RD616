#ifndef __USART_C__
#define __USART_C__
/********************************************************************************
writer : ZH Wang
time   : 2017-10-31
*********************************************************************************/
#include "CONFIG\My_define.h"
#include "CONFIG\My_Type.h"

// 判断是否支持OS系统
#if SYSTEM_SUPPORT_OS
#include "includes.h"
#include  <os.h>
#include "CONFIG\My_cfg.h"
#endif

#include "STM_SYS\Stm_Sys.h"
#include "CTRLCOM\CtrlCom.h"

#if CTRL_COM_DMA_ENABLE
#include "CTRLDMA\CtrlDma.h"
#endif

#include "QUEUE\queue.h"
#include "stdio.h"

/*
支持UCOSIII,支持stm32f103和stm32f407
*/

//tCOMConfig sComCtrl.sComConfig[UART_NUM];             // 串口参数配置结构体
//tComFunList ComFunList[UART_NUM];           // 串口函数列表结构体
st_UsartComId   sUsarteCom;                     // 串口对应ID好结构体

st_ComCtrlModule sComCtrl;

#if CTRL_COM_DMA_ENABLE
// 串口接收DMA缓存
uint8_t Uart_DMA_Rx[UART_DMA_RX_LEN] = {0};
#endif

//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((sComCtrl.sComConfig[MAIN_COM].Com_usart->SR&0X40)==0){};//循环发送,直到发送完毕   
    sComCtrl.sComConfig[MAIN_COM].Com_usart->DR = (u8) ch;      
	return ch;
}
#endif 



// 函数调用接口初始化
_STA_ void Config_Com_Fun(em_Usart_Com eCom)
{
    sComCtrl.sComFunList.FunComInit          = Usart_ComInit;

    sComCtrl.sComFunList.FunComCheckFlagRI   = Usart_ComCheckFlagRI;
    sComCtrl.sComFunList.FunComCheckFlagTI   = Usart_ComCheckFlagTI;
    sComCtrl.sComFunList.FunComCheckFlagTXE  = Usart_ComCheckFlagTXE;
    sComCtrl.sComFunList.FunComInitTxEnable  = Usart_ComIntTxEnable;
    sComCtrl.sComFunList.FunComInitRxEnable  = Usart_ComIntRxEnable;
    sComCtrl.sComFunList.FunComInitTxDisable = Usart_ComIntTxDisable;
    sComCtrl.sComFunList.FunComInitRxDisable = Usart_ComIntRxDisable;
    sComCtrl.sComFunList.FunComBufSet        = Usart_ComBufSet;
    sComCtrl.sComFunList.FunComBufGet        = Usart_ComBufGet;
    sComCtrl.sComFunList.FunComSendString    = Usart_ComSendString;
}
/***************************************************************************************
函数名称: void ConfigComPara(UINT8 eCom)
函数功能: 串口配置参数函数
输入参数: eCom --- 串口ID号
返 回 值: 
***************************************************************************************/
_STA_ void ComConfigInit(em_Usart_Com eCom)
{
    // Com参数定义
    COM(eCom                   , sComCtrl.sComConfig[eCom].Com_usart);
    COM_CLK(eCom               , sComCtrl.sComConfig[eCom].Com_usart_clk);
#if defined (STM32F40_41xxx)
    COM_GPIO_AF_USART(eCom     , sComCtrl.sComConfig[eCom].Com_gpio_af_usart);
#endif
    
    COM_TX_GPIO_CLK(eCom       , sComCtrl.sComConfig[eCom].Com_tx_gpio_clk);
    COM_TX_GPIO_PORT(eCom      , sComCtrl.sComConfig[eCom].Com_tx_port);
    COM_TX_GPIO_PIN(eCom       , sComCtrl.sComConfig[eCom].Com_tx_pin);
#if defined (STM32F40_41xxx)
    COM_TX_PINSOURCE(eCom      , sComCtrl.sComConfig[eCom].Com_tx_pinsource);
#endif
    
    COM_RX_GPIO_CLK(eCom       , sComCtrl.sComConfig[eCom].Com_rx_gpio_clk);
    COM_RX_GPIO_PORT(eCom      , sComCtrl.sComConfig[eCom].Com_rx_port);
    COM_RX_GPIO_PIN(eCom       , sComCtrl.sComConfig[eCom].Com_rx_pin);
#if defined (STM32F40_41xxx)
    COM_RX_PINSOURCE(eCom      , sComCtrl.sComConfig[eCom].Com_rx_pinsource);
#endif
    
    COM_IRQn(eCom              , sComCtrl.sComConfig[eCom].Com_irqn);
    COM_IRQn_IDLE(eCom         , sComCtrl.sComConfig[eCom].Com_irqn_idle);
    
    COM_BPR(eCom               , sComCtrl.sComConfig[eCom].Com_bpr);
    COM_WORDLENGTH(eCom        , sComCtrl.sComConfig[eCom].Com_wordlength);
    COM_STOPBITS(eCom          , sComCtrl.sComConfig[eCom].Com_stopbits);
    COM_PARITY(eCom            , sComCtrl.sComConfig[eCom].Com_parity);
    COM_FLOWCONTROL(eCom       , sComCtrl.sComConfig[eCom].Com_flowControl);
    
    // 获取串口对应的ID号  并配置DMA
    if(sComCtrl.sComConfig[eCom].Com_usart == USART1)
    {
        sUsarteCom.USART_Com_ID[0]              = eCom;
        
#if CTRL_COM_DMA_ENABLE
#if defined (STM32F40_41xxx)
        sComCtrl.sComConfig[eCom].Com_dma_Channel          = DMA2_Stream2;
        sComCtrl.sComConfig[eCom].Com_dma_Stream_Channel   = DMA_Channel_4;
        sComCtrl.sComConfig[eCom].Com_dma_bpr              = DMA2_Stream2_IRQn;
        sComCtrl.sComConfig[eCom].Com_dma_ITTC             = DMA_FLAG_TCIF2;
#else
        sComCtrl.sComConfig[eCom].Com_dma_Channel          = DMA1_Channel5;
        sComCtrl.sComConfig[eCom].Com_dma_bpr              = DMA1_Channel5_IRQn;
        sComCtrl.sComConfig[eCom].Com_dma_ITTC             = DMA1_IT_TC5;
#endif
#endif        
    }
    else if(sComCtrl.sComConfig[eCom].Com_usart == USART2)
    {
        sUsarteCom.USART_Com_ID[1]              = eCom;
#if CTRL_COM_DMA_ENABLE
#if defined (STM32F40_41xxx)
        sComCtrl.sComConfig[eCom].Com_dma_Channel          = DMA1_Stream5;
        sComCtrl.sComConfig[eCom].Com_dma_Stream_Channel   = DMA_Channel_4;
        sComCtrl.sComConfig[eCom].Com_dma_bpr              = DMA1_Stream5_IRQn;
        sComCtrl.sComConfig[eCom].Com_dma_ITTC             = DMA_FLAG_TCIF5;
#else
        sComCtrl.sComConfig[eCom].Com_dma_Channel          = DMA1_Channel6;
        sComCtrl.sComConfig[eCom].Com_dma_bpr              = DMA1_Channel6_IRQn;
        sComCtrl.sComConfig[eCom].Com_dma_ITTC             = DMA1_IT_TC6;
#endif
#endif        
    }
    else if(sComCtrl.sComConfig[eCom].Com_usart == USART3)
    {
        sUsarteCom.USART_Com_ID[2]              = eCom;
#if CTRL_COM_DMA_ENABLE        
#if defined (STM32F40_41xxx)
        sComCtrl.sComConfig[eCom].Com_dma_Channel          = DMA1_Stream1;
        sComCtrl.sComConfig[eCom].Com_dma_Stream_Channel   = DMA_Channel_4;
        sComCtrl.sComConfig[eCom].Com_dma_bpr              = DMA1_Stream1_IRQn;
        sComCtrl.sComConfig[eCom].Com_dma_ITTC             = DMA_FLAG_TCIF1;
#else
        sComCtrl.sComConfig[eCom].Com_dma_Channel          = DMA1_Channel3;
        sComCtrl.sComConfig[eCom].Com_dma_bpr              = DMA1_Channel3_IRQn;
        sComCtrl.sComConfig[eCom].Com_dma_ITTC             = DMA1_IT_TC3;
#endif
#endif        
    }
    else if(sComCtrl.sComConfig[eCom].Com_usart == UART4)
    {
        sUsarteCom.USART_Com_ID[3]              = eCom;
#if CTRL_COM_DMA_ENABLE        
#if defined (STM32F40_41xxx)
        sComCtrl.sComConfig[eCom].Com_dma_Channel          = DMA1_Stream2;
        sComCtrl.sComConfig[eCom].Com_dma_Stream_Channel   = DMA_Channel_4;
        sComCtrl.sComConfig[eCom].Com_dma_bpr              = DMA1_Stream2_IRQn;
        sComCtrl.sComConfig[eCom].Com_dma_ITTC             = DMA_FLAG_TCIF2;
#else
        sComCtrl.sComConfig[eCom].Com_dma_Channel          = DMA2_Channel3;
        sComCtrl.sComConfig[eCom].Com_dma_bpr              = DMA2_Channel3_IRQn;
        sComCtrl.sComConfig[eCom].Com_dma_ITTC             = DMA2_IT_TC3;
#endif
#endif
    }
    else if(sComCtrl.sComConfig[eCom].Com_usart == UART5)
    {
        sUsarteCom.USART_Com_ID[4]              = eCom;
#if CTRL_COM_DMA_ENABLE        
#if defined (STM32F40_41xxx)
        sComCtrl.sComConfig[eCom].Com_dma_Channel          = DMA1_Stream0;
        sComCtrl.sComConfig[eCom].Com_dma_Stream_Channel   = DMA_Channel_4;
        sComCtrl.sComConfig[eCom].Com_dma_bpr              = DMA1_Stream0_IRQn;
        sComCtrl.sComConfig[eCom].Com_dma_ITTC             = DMA_FLAG_TCIF0;
#else
        sComCtrl.sComConfig[eCom].Com_dma_Channel          = NULL;
        sComCtrl.sComConfig[eCom].Com_dma_bpr              = 0;
        sComCtrl.sComConfig[eCom].Com_dma_ITTC             = 0;
#endif
#endif        
    }
    Config_Com_Fun(eCom);
    // 
    
}

BOOL Com_Initialization(em_Usart_Com eCom,UINT8 *com_buf,UINT16 com_len)
{
    if(eCom > UART_NUM)
        return FALSE; 
    Config_Com_Fun(eCom);
    
    ComConfigInit(eCom);
    
    sComCtrl.sComConfig[eCom].Com_MemBuf = com_buf;
    sComCtrl.sComConfig[eCom].Com_BufLen = com_len;
    
    sComCtrl.sComFunList.FunComInit(eCom);

    queue_inital(sComCtrl.sComConfig[eCom].Com_MemBuf,sComCtrl.sComConfig[eCom].Com_BufLen);
    return TRUE;
    
}

UINT8 Usart_ComInit(em_Usart_Com eCom)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;
    
//    ConfigComPara(eCom);
    
    // eanble Gpio clock
#if defined (STM32F40_41xxx)
    RCC_AHB1PeriphClockCmd(sComCtrl.sComConfig[eCom].Com_rx_gpio_clk | sComCtrl.sComConfig[eCom].Com_tx_gpio_clk ,ENABLE);
#else
    RCC_APB2PeriphClockCmd(sComCtrl.sComConfig[eCom].Com_rx_gpio_clk | sComCtrl.sComConfig[eCom].Com_tx_gpio_clk ,ENABLE);
#endif
    if(sComCtrl.sComConfig[eCom].Com_usart == USART1)
    {
        RCC_APB2PeriphClockCmd(sComCtrl.sComConfig[eCom].Com_usart_clk ,ENABLE);
    }
    else
    {
        RCC_APB1PeriphClockCmd(sComCtrl.sComConfig[eCom].Com_usart_clk ,ENABLE);
    }
    
#if defined (STM32F40_41xxx)
    GPIO_PinAFConfig(sComCtrl.sComConfig[eCom].Com_tx_port,sComCtrl.sComConfig[eCom].Com_tx_pinsource,sComCtrl.sComConfig[eCom].Com_gpio_af_usart);  //
	GPIO_PinAFConfig(sComCtrl.sComConfig[eCom].Com_rx_port,sComCtrl.sComConfig[eCom].Com_rx_pinsource,sComCtrl.sComConfig[eCom].Com_gpio_af_usart); //
#endif
    // USART tx
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin   = sComCtrl.sComConfig[eCom].Com_tx_pin;
#if defined (STM32F40_41xxx)
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
#else
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
#endif
    GPIO_Init(sComCtrl.sComConfig[eCom].Com_tx_port, &GPIO_InitStructure);

    // USART rx
    GPIO_InitStructure.GPIO_Pin   = sComCtrl.sComConfig[eCom].Com_rx_pin;//PA10
#if defined (STM32F40_41xxx)
    // 不需要重新设置
#else
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
#endif
    GPIO_Init(sComCtrl.sComConfig[eCom].Com_rx_port, &GPIO_InitStructure);

    // USART configuration
    
    USART_InitStructure.USART_BaudRate            = sComCtrl.sComConfig[eCom].Com_bpr;
    USART_InitStructure.USART_WordLength          = sComCtrl.sComConfig[eCom].Com_wordlength;
    USART_InitStructure.USART_StopBits            = sComCtrl.sComConfig[eCom].Com_stopbits;
    USART_InitStructure.USART_Parity              = sComCtrl.sComConfig[eCom].Com_parity;
    USART_InitStructure.USART_HardwareFlowControl = sComCtrl.sComConfig[eCom].Com_flowControl;
    USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx; 
    USART_Init(sComCtrl.sComConfig[eCom].Com_usart, &USART_InitStructure);

    USART_Cmd(sComCtrl.sComConfig[eCom].Com_usart,ENABLE);
    USART_ITConfig(sComCtrl.sComConfig[eCom].Com_usart, USART_IT_TC, DISABLE);
    
    USART_ITConfig(sComCtrl.sComConfig[eCom].Com_usart, USART_IT_RXNE, ENABLE);
    USART_ITConfig(sComCtrl.sComConfig[eCom].Com_usart, USART_IT_IDLE, ENABLE);
    
    // MAIN_COM 采用DMA接收传输
    if(eCom == MAIN_COM)
    {
        
        //==================================================================================
        // DMA1通道 DMA1_Channel5, 外设为串口1, Uart_Rx, 长度
        //==================================================================================
#if CTRL_COM_DMA_ENABLE
        USART_ITConfig(sComCtrl.sComConfig[eCom].Com_usart, USART_IT_RXNE, DISABLE);
#if defined (STM32F40_41xxx)
        DMA_Config(sComCtrl.sComConfig[eCom].Com_dma_Channel,
                   sComCtrl.sComConfig[eCom].Com_dma_Stream_Channel,
                   (UINT32)&sComCtrl.sComConfig[eCom].Com_usart->DR,
                   (UINT32)Uart_DMA_Rx,
                   DMA_DIR_PeripheralToMemory,
                   UART_DMA_RX_LEN);
#else
        DMA_Config(sComCtrl.sComConfig[eCom].Com_dma_Channel,
                   (UINT32)&sComCtrl.sComConfig[eCom].Com_usart->DR,
                   (UINT32)Uart_DMA_Rx,
                   DMA_DIR_PeripheralSRC,
                   UART_DMA_RX_LEN);
#endif
        
        
#if 1
        // DMA发送中断设置
        NVIC_InitStructure.NVIC_IRQChannel =  sComCtrl.sComConfig[eCom].Com_dma_bpr;  
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = eCom;  
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
        NVIC_Init(&NVIC_InitStructure); 
#endif 
        
        USART_DMACmd(sComCtrl.sComConfig[eCom].Com_usart, USART_DMAReq_Rx, ENABLE);
        DMA_ITConfig(sComCtrl.sComConfig[eCom].Com_dma_Channel, DMA_IT_TC, ENABLE); //启动传输完成中断    
        
        // 开始DMA传输
        DMA_Enable(sComCtrl.sComConfig[eCom].Com_dma_Channel);
        
//        DMA_Cmd(sComCtrl.sComConfig[eCom].Com_dma_Channel, ENABLE);
#endif
    }
    
    NVIC_InitStructure.NVIC_IRQChannel                   = sComCtrl.sComConfig[eCom].Com_irqn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = eCom;
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
    return e_true;
}

UINT8 Usart_ComCheckFlagTI(em_Usart_Com eCom)
{
    return (RESET == USART_GetFlagStatus(sComCtrl.sComConfig[eCom].Com_usart,USART_FLAG_TC))?0:1;
}

UINT8 Usart_ComCheckFlagRI(em_Usart_Com eCom)
{
    return (RESET == USART_GetFlagStatus(sComCtrl.sComConfig[eCom].Com_usart,USART_FLAG_RXNE))?0:1;
}

UINT8 Usart_ComCheckFlagTXE(em_Usart_Com eCom)
{
    return (RESET == USART_GetFlagStatus(sComCtrl.sComConfig[eCom].Com_usart,USART_FLAG_TXE))?0:1;
}

UINT8 Usart_ComIntTxEnable(em_Usart_Com eCom)
{
    USART_ITConfig(sComCtrl.sComConfig[eCom].Com_usart,USART_IT_TC,ENABLE);
    return e_true;
}

UINT8 Usart_ComIntRxEnable(em_Usart_Com eCom)
{
    USART_ITConfig(sComCtrl.sComConfig[eCom].Com_usart,USART_IT_RXNE,ENABLE);
    return e_true;
}

UINT8 Usart_ComIntTxDisable(em_Usart_Com eCom)
{
    USART_ITConfig(sComCtrl.sComConfig[eCom].Com_usart,USART_IT_TC,DISABLE);
    return e_true;
}

UINT8 Usart_ComIntRxDisable(em_Usart_Com eCom)
{
    USART_ITConfig(sComCtrl.sComConfig[eCom].Com_usart,USART_IT_RXNE,DISABLE);
    return e_true;
}

UINT8 Usart_ComBufSet(em_Usart_Com eCom, UINT8 ch)
{
//    USART_SendData(sComCtrl.sComConfig[eCom].Com_usart,ch);
    sComCtrl.sComConfig[eCom].Com_usart->DR = (ch & (uint16_t)0x01FF);
    return e_true;
}

UINT8 Usart_ComBufGet(em_Usart_Com eCom)
{
    return (UINT8)USART_ReceiveData(sComCtrl.sComConfig[eCom].Com_usart);
}
	
UINT8 Usart_ComSendString(em_Usart_Com eCom, UINT8 *SendBuf, UINT16 SendLen)
{
    UINT8 ch;
    UINT8 *str = SendBuf;
    UINT16 ts = 0;
    
    while(SendLen)
    {
        if((str == NULL) || (ts==5000))
            return e_false;
        ch = *str++;
        ts = 0;
        while(((sComCtrl.sComConfig[eCom].Com_usart->SR&0X40)==0) && (ts<5000))ts++;//循环发送,直到发送完毕 
        sComCtrl.sComConfig[eCom].Com_usart->DR = (u8) ch;
        SendLen--;
    }
    return e_true;
}


void USART1_IRQHandler(void)
{
#if SYSTEM_SUPPORT_OS
    OSIntEnter();							//进入中断
#endif
    USART_IRQReceive(sUsarteCom.USART_Com_ID[eUsart_Com1]);
#if SYSTEM_SUPPORT_OS
    OSIntExit();       	 					//触发任务切换软中断
#endif
}

void USART2_IRQHandler(void)
{
#if SYSTEM_SUPPORT_OS
    OSIntEnter();							//进入中断
#endif
    USART_IRQReceive(sUsarteCom.USART_Com_ID[2]);
#if SYSTEM_SUPPORT_OS
    OSIntExit();       	 					//触发任务切换软中断
#endif
}

void USART_IRQReceive(em_Usart_Com eCom)
{
    UINT8  ch;
    UINT16 temp;
    _STA_ UINT32 i = 0;
#if SYSTEM_SUPPORT_OS
    OS_ERR err;
    CPU_SR_ALLOC();
    
    OS_CRITICAL_ENTER();	//进入临界区
#endif
    
    if(USART_GetITStatus(sComCtrl.sComConfig[eCom].Com_usart, USART_IT_RXNE) != RESET)
    {
        ch = sComCtrl.sComConfig[eCom].Com_usart->DR;
        queue_in_byte(sComCtrl.sComConfig[eCom].Com_MemBuf,ch);
    }
    
    
    if(USART_GetITStatus(sComCtrl.sComConfig[eCom].Com_usart,USART_IT_IDLE) != RESET)
    {
        // 清除串口超时中断
        ch = sComCtrl.sComConfig[eCom].Com_usart->SR;
        ch = sComCtrl.sComConfig[eCom].Com_usart->DR;
        // 传递信号量
        sComCtrl.sComConfig[eCom].Com_Receive_Flag = 1;
        
        // 只有主串口才采用DMA接收
        if(eCom == MAIN_COM)
        {
#if CTRL_COM_DMA_ENABLE            
            DMA_Cmd(sComCtrl.sComConfig[eCom].Com_dma_Channel,DISABLE);  

            // 查询剩余 DMA 接收数据长度
            temp = DMA_GetCurrDataCounter(sComCtrl.sComConfig[eCom].Com_dma_Channel);
            queue_in(sComCtrl.sComConfig[eCom].Com_MemBuf, Uart_DMA_Rx, UART_DMA_RX_LEN - temp);
            
            //设置传输数据长度   
            DMA_SetCurrDataCounter(sComCtrl.sComConfig[eCom].Com_dma_Channel,UART_DMA_RX_LEN);  
            //打开DMA   
            DMA_Cmd(sComCtrl.sComConfig[eCom].Com_dma_Channel,ENABLE);  
#endif            
#if SYSTEM_SUPPORT_OS
//            printf("%d ",i++);
            OSTaskSemPost(&MAIN_USART_REC_TASKTCB,OS_OPT_POST_NONE,&err);
#endif
        }

    }
#if SYSTEM_SUPPORT_OS
    OS_CRITICAL_EXIT();	//退出临界区
#endif
}

#if CTRL_COM_DMA_ENABLE    
/***************************************************************************************
函数名称: void DMA1_Channel5_IRQHandler(void)
函数功能: DMA通道接收完成中断服务函数
输入参数:  
返 回 值: 
***************************************************************************************/
void DMA_Channel_IRQHandler(em_Usart_Com eCom)
{
    uint32_t left_len;  
#if SYSTEM_SUPPORT_OS
    CPU_SR_ALLOC();
    
    OS_CRITICAL_ENTER();	//进入临界区
#endif
    
    //  传输完成中断
#if defined (STM32F40_41xxx)
    if(DMA_GetITStatus(sComCtrl.sComConfig[eCom].Com_dma_Channel,sComCtrl.sComConfig[eCom].Com_dma_ITTC))
#else
    if(DMA_GetITStatus(sComCtrl.sComConfig[eCom].Com_dma_ITTC))
#endif
    {
        DMA_Cmd(sComCtrl.sComConfig[eCom].Com_dma_Channel,DISABLE);
        // 查询剩余 DMA 接收数据长度
        left_len = DMA_GetCurrDataCounter(sComCtrl.sComConfig[eCom].Com_dma_Channel);
        queue_in(sComCtrl.sComConfig[eCom].Com_MemBuf, Uart_DMA_Rx, UART_DMA_RX_LEN - left_len);
        
        //设置传输数据长度   
        DMA_SetCurrDataCounter(sComCtrl.sComConfig[eCom].Com_dma_Channel,UART_DMA_RX_LEN);
        //打开DMA   
        DMA_Cmd(sComCtrl.sComConfig[eCom].Com_dma_Channel,ENABLE);
        printf("dma");
    }
#if defined (STM32F40_41xxx)
    DMA_ClearITPendingBit(sComCtrl.sComConfig[eCom].Com_dma_Channel,sComCtrl.sComConfig[eCom].Com_dma_ITTC);
#else
	// DMAy_IT_GLx 保存开头一位，后面的相对	DMAy_IT_TCx >> 1
    DMA_ClearITPendingBit((sComCtrl.sComConfig[eCom].Com_dma_ITTC&0x10000000) | ((sComCtrl.sComConfig[eCom].Com_dma_ITTC&0xFFFFFFF) >>1));
#endif
#if SYSTEM_SUPPORT_OS
    OS_CRITICAL_EXIT();	//退出临界区
#endif
}
#if defined (STM32F40_41xxx)
// 对应串口1 DMA
void DMA2_Stream2_IRQHandler(void)
{
#if SYSTEM_SUPPORT_OS
    OSIntEnter();							//进入中断
#endif
    DMA_Channel_IRQHandler(sUsarteCom.USART_Com_ID[0]);
#if SYSTEM_SUPPORT_OS
    OSIntExit();       	 					//触发任务切换软中断
#endif
}

void DMA1_Stream5_IRQHandler(void)
{
#if SYSTEM_SUPPORT_OS
    OSIntEnter();							//进入中断
#endif
    DMA_Channel_IRQHandler(sUsarteCom.USART_Com_ID[1]);
#if SYSTEM_SUPPORT_OS
    OSIntExit();       	 					//触发任务切换软中断
#endif
}
#else
void DMA1_Channel5_IRQHandler(void)
{
    #if SYSTEM_SUPPORT_OS
    OSIntEnter();							//进入中断
    #endif
    DMA_Channel_IRQHandler(sUsarteCom.USART_Com_ID[0]);
    #if SYSTEM_SUPPORT_OS
    OSIntExit();       	 					//触发任务切换软中断
    #endif
}

void DMA1_Channel6_IRQHandler(void)
{
    #if SYSTEM_SUPPORT_OS
    OSIntEnter();							//进入中断
    #endif
    DMA_Channel_IRQHandler(sUsarteCom.USART_Com_ID[1]);
    #if SYSTEM_SUPPORT_OS
    OSIntExit();       	 					//触发任务切换软中断
    #endif
}
#endif
#endif



#endif
