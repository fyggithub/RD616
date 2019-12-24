#ifndef __CTRLDMA_H__
#define __CTRLDMA_H__

#if defined (STM32F40_41xxx)
void DMA_Config(DMA_Stream_TypeDef *DMA_CHx, 
                UINT32             DMA_Channel, 
                UINT32             peripheral_addr, 
                UINT32             mem_addr,
                UINT32             dma_type, 
                UINT16             cnt);	        // Ê¹ÄÜDMA´«Êä
#else
void DMA_Config(DMA_Channel_TypeDef* DMA_CHx, 
                UINT32               peripheral_addr,
                UINT32               mem_addr, 
                UINT32               dma_type, 
                UINT16               cnt);
#endif

#if defined (STM32F40_41xxx)
void DMA_Enable(DMA_Stream_TypeDef *DMA_CHx);
#else
void DMA_Enable(DMA_Channel_TypeDef *DMA_CHx);
#endif


#endif
