
#ifndef __DMA_C__
#define __DMA_C__

/********************************************************************************
writer : ZH Wang
time   : 2018-11-29
*********************************************************************************/
#include "CONFIG\My_Type.h"
#include "STM_SYS\Stm_Sys.h" 

/*******************************************************
这个函数只是配合串口使用的，其他不确定是否需要更改兼容
*******************************************************/

DMA_InitTypeDef DMA_InitStructure;
UINT16 dma_transfer_len;

/***************************************************************************************
函数名称: void DMA_Config(DMA_Channel_TypeDef* DMA_CHx, 
                        u32 peripheral_addr,
                        u32 mem_addr, 
                        u32 dma_type, 
                        u16 cnt)
函数功能: 配置DMA传输
输入参数: DMA_CHx --- DMA通道CHx
          peripheral_addr --- 外设地址
          mem_addr --- 存储器地址
          _type --- DMA类型， DMA_DIR_PeripheralDST 从内存到外设
                              DMA_DIR_PeripheralSRC 从外设到内存
          cnt --- 传输数量
返 回 值: 
***************************************************************************************/
#if defined (STM32F40_41xxx)
void DMA_Config(DMA_Stream_TypeDef *DMA_CHx, 
                UINT32             DMA_Channel, 
                UINT32             peripheral_addr, 
                UINT32             mem_addr,
                UINT32             dma_type, 
                UINT16             cnt)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1 | RCC_AHB1Periph_DMA2, ENABLE);	        // 使能DMA传输
#else
void DMA_Config(DMA_Channel_TypeDef* DMA_CHx, 
                UINT32               peripheral_addr,
                UINT32               mem_addr, 
                UINT32               dma_type, 
                UINT16               cnt)

{
    
 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 | RCC_AHBPeriph_DMA2, ENABLE);	        // 使能DMA传输
#endif
    DMA_DeInit(DMA_CHx);    // 将DMA的通道1寄存器重设为缺省值
    
    //==================================================================================
    //==================================================================================
	dma_transfer_len = cnt;
    #if defined (STM32F40_41xxx)
    /* 配置 DMA Stream */
    DMA_InitStructure.DMA_Channel = DMA_Channel;                                // 通道选择
    DMA_InitStructure.DMA_Memory0BaseAddr = mem_addr;                       // DMA 存储器0地址
//    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;                 // 外设到存储器
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;                   // 中等优先级
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;             // 存储器突发单次传输
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;     // 外设突发单次传输
    
    #else
    
	DMA_InitStructure.DMA_MemoryBaseAddr     = mem_addr;            // DMA内存基地址
//	DMA_InitStructure.DMA_DIR                = dma_type;                       // 外设作为数据传输的目的地

	DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;                // DMA通道x没有设置为内存到内存传输

    #endif
    
    DMA_InitStructure.DMA_PeripheralBaseAddr = peripheral_addr; // DMA外设基地址
    DMA_InitStructure.DMA_DIR                = dma_type;                       // 外设作为数据传输的目的地
    DMA_InitStructure.DMA_BufferSize         = cnt;                     // DMA通道的DMA缓存的大小
    // 外设地址寄存器不变
	DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
    // 内存地址寄存器递增
	DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; // 数据宽度为8位
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;         // 数据宽度为8位
    DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;               // 工作在正常缓存模式
    DMA_InitStructure.DMA_Priority           = DMA_Priority_Medium;       // DMA通道 x拥有中优先级 
    
    DMA_Init(DMA_CHx, &DMA_InitStructure);                  // 根据DMA_InitStruct中指定的参数初始化DMA的通道所标识的寄存器
}


/***************************************************************************************
函数名称: void dma_enable(DMA_Channel_TypeDef *DMA_CHx)
函数功能: 开启一次DMA传输
输入参数: DMA_CHx --- DMA通道CHx
返 回 值: 
***************************************************************************************/
#if defined (STM32F40_41xxx)
void DMA_Enable(DMA_Stream_TypeDef *DMA_CHx)
#else
void DMA_Enable(DMA_Channel_TypeDef *DMA_CHx)
#endif
{
	DMA_Cmd(DMA_CHx, DISABLE);  
    
    #if defined (STM32F40_41xxx)
    while (DMA_GetCmdStatus(DMA_CHx) != DISABLE){}	//确保DMA可以被设置  
	#endif
        
    DMA_SetCurrDataCounter(DMA_CHx,dma_transfer_len);          //数据传输量  
 	DMA_Cmd(DMA_CHx, ENABLE);                               // 使能USART1 TX DMA1 所指示的通道 
}

#endif

