
#ifndef __DMA_C__
#define __DMA_C__

/********************************************************************************
writer : ZH Wang
time   : 2018-11-29
*********************************************************************************/
#include "CONFIG\My_Type.h"
#include "STM_SYS\Stm_Sys.h" 

/*******************************************************
�������ֻ����ϴ���ʹ�õģ�������ȷ���Ƿ���Ҫ���ļ���
*******************************************************/

DMA_InitTypeDef DMA_InitStructure;
UINT16 dma_transfer_len;

/***************************************************************************************
��������: void DMA_Config(DMA_Channel_TypeDef* DMA_CHx, 
                        u32 peripheral_addr,
                        u32 mem_addr, 
                        u32 dma_type, 
                        u16 cnt)
��������: ����DMA����
�������: DMA_CHx --- DMAͨ��CHx
          peripheral_addr --- �����ַ
          mem_addr --- �洢����ַ
          _type --- DMA���ͣ� DMA_DIR_PeripheralDST ���ڴ浽����
                              DMA_DIR_PeripheralSRC �����赽�ڴ�
          cnt --- ��������
�� �� ֵ: 
***************************************************************************************/
#if defined (STM32F40_41xxx)
void DMA_Config(DMA_Stream_TypeDef *DMA_CHx, 
                UINT32             DMA_Channel, 
                UINT32             peripheral_addr, 
                UINT32             mem_addr,
                UINT32             dma_type, 
                UINT16             cnt)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1 | RCC_AHB1Periph_DMA2, ENABLE);	        // ʹ��DMA����
#else
void DMA_Config(DMA_Channel_TypeDef* DMA_CHx, 
                UINT32               peripheral_addr,
                UINT32               mem_addr, 
                UINT32               dma_type, 
                UINT16               cnt)

{
    
 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 | RCC_AHBPeriph_DMA2, ENABLE);	        // ʹ��DMA����
#endif
    DMA_DeInit(DMA_CHx);    // ��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
    
    //==================================================================================
    //==================================================================================
	dma_transfer_len = cnt;
    #if defined (STM32F40_41xxx)
    /* ���� DMA Stream */
    DMA_InitStructure.DMA_Channel = DMA_Channel;                                // ͨ��ѡ��
    DMA_InitStructure.DMA_Memory0BaseAddr = mem_addr;                       // DMA �洢��0��ַ
//    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;                 // ���赽�洢��
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;                   // �е����ȼ�
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;             // �洢��ͻ�����δ���
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;     // ����ͻ�����δ���
    
    #else
    
	DMA_InitStructure.DMA_MemoryBaseAddr     = mem_addr;            // DMA�ڴ����ַ
//	DMA_InitStructure.DMA_DIR                = dma_type;                       // ������Ϊ���ݴ����Ŀ�ĵ�

	DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;                // DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��

    #endif
    
    DMA_InitStructure.DMA_PeripheralBaseAddr = peripheral_addr; // DMA�������ַ
    DMA_InitStructure.DMA_DIR                = dma_type;                       // ������Ϊ���ݴ����Ŀ�ĵ�
    DMA_InitStructure.DMA_BufferSize         = cnt;                     // DMAͨ����DMA����Ĵ�С
    // �����ַ�Ĵ�������
	DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
    // �ڴ��ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; // ���ݿ��Ϊ8λ
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;         // ���ݿ��Ϊ8λ
    DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;               // ��������������ģʽ
    DMA_InitStructure.DMA_Priority           = DMA_Priority_Medium;       // DMAͨ�� xӵ�������ȼ� 
    
    DMA_Init(DMA_CHx, &DMA_InitStructure);                  // ����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ������ʶ�ļĴ���
}


/***************************************************************************************
��������: void dma_enable(DMA_Channel_TypeDef *DMA_CHx)
��������: ����һ��DMA����
�������: DMA_CHx --- DMAͨ��CHx
�� �� ֵ: 
***************************************************************************************/
#if defined (STM32F40_41xxx)
void DMA_Enable(DMA_Stream_TypeDef *DMA_CHx)
#else
void DMA_Enable(DMA_Channel_TypeDef *DMA_CHx)
#endif
{
	DMA_Cmd(DMA_CHx, DISABLE);  
    
    #if defined (STM32F40_41xxx)
    while (DMA_GetCmdStatus(DMA_CHx) != DISABLE){}	//ȷ��DMA���Ա�����  
	#endif
        
    DMA_SetCurrDataCounter(DMA_CHx,dma_transfer_len);          //���ݴ�����  
 	DMA_Cmd(DMA_CHx, ENABLE);                               // ʹ��USART1 TX DMA1 ��ָʾ��ͨ�� 
}

#endif

