#ifndef __CTRLCOM_H__
#define __CTRLCOM_H__
/********************************************************************************
writer : ZH Wang
time   : 2017-10-30
*********************************************************************************/
// 支持UCOSIII
#if SYSTEM_SUPPORT_OS
#define MAIN_USART_REC_TASKTCB       MIDTaskTCB

#endif

// 最多可以开启5个串口，串口的参数可以配置，配置后就可使用

// Uart num   串口个数
#define UART_NUM					1
// 主串口，默认是第一个串口
#define MAIN_COM					0

// 扫描头端口
#define SCAN_COM                    1

#define UART_DMA_RX_LEN             128

// 有多少个串口，相应要修改这里
typedef enum
{
    eUsart_Com1 = 0,
}em_Usart_Com;

#define COMM1						USART1
#define COMM1_CLK					RCC_APB2Periph_USART1

#if defined (STM32F40_41xxx)
#define COMM1_GPIO_AF_USART         GPIO_AF_USART1
#endif

// TX 端口
#define COMM1_TX_GPIO_CLK			RCC_APB2Periph_GPIOA
#define COMM1_TX_GPIO_PORT			GPIOA
#define COMM1_TX_GPIO_PIN			GPIO_Pin_9

#if defined (STM32F40_41xxx)
#define COMM1_TX_PINSOURCE          GPIO_PinSource9
#endif

// RX 端口
#define COMM1_RX_GPIO_CLK			RCC_APB2Periph_GPIOA
#define COMM1_RX_GPIO_PORT			GPIOA
#define COMM1_RX_GPIO_PIN			GPIO_Pin_10

#if defined (STM32F40_41xxx)
#define COMM1_RX_PINSOURCE          GPIO_PinSource10
#endif

#define COMM1_IRQn                  USART1_IRQn

#define COMM1_IRQn_IDLE             ENABLE

#define COMM1_BPR                   115200
#define COMM1_WORDLENGTH            USART_WordLength_8b
#define COMM1_STOPBITS              USART_StopBits_1
#define COMM1_PARITY                USART_Parity_No
#define COMM1_FLOWCONTROL           USART_HardwareFlowControl_None


#if (UART_NUM>1)
#define COMM2						USART3
#define COMM2_CLK					RCC_APB1Periph_USART3
#if defined (STM32F40_41xxx)
#define COMM2_GPIO_AF_USART         GPIO_AF_USART3
#endif

#define COMM2_TX_GPIO_CLK			RCC_APB2Periph_GPIOB
#define COMM2_TX_GPIO_PORT			GPIOB
#define COMM2_TX_GPIO_PIN			GPIO_Pin_10
#if defined (STM32F40_41xxx)
#define COMM2_TX_PINSOURCE          GPIO_PinSource10
#endif

#define COMM2_RX_GPIO_CLK			RCC_APB2Periph_GPIOB
#define COMM2_RX_GPIO_PORT			GPIOB
#define COMM2_RX_GPIO_PIN			GPIO_Pin_11
#if defined (STM32F40_41xxx)
#define COMM2_RX_PINSOURCE          GPIO_PinSource11
#endif

#define COMM2_IRQn                  USART3_IRQn
#define COMM2_IRQn_IDLE             ENABLE

#define COMM2_BPR                   9600
#define COMM2_WORDLENGTH            USART_WordLength_8b
#define COMM2_STOPBITS              USART_StopBits_1
#define COMM2_PARITY                USART_Parity_No
#define COMM2_FLOWCONTROL           USART_HardwareFlowControl_None

#elif (UART_NUM>2)
#define COMM3						USART1
#define COMM3_CLK					RCC_APB2Periph_USART1
#if defined (STM32F40_41xxx)
#define COMM3_GPIO_AF_USART         GPIO_AF_USART1
#endif

#define COMM3_TX_GPIO_CLK			RCC_APB2Periph_GPIOA
#define COMM3_TX_GPIO_PORT			GPIOA
#define COMM3_TX_GPIO_PIN			GPIO_Pin_9
#if defined (STM32F40_41xxx)
#define COMM3_TX_PINSOURCE          GPIO_PinSource9
#endif

#define COMM3_RX_GPIO_CLK			RCC_APB2Periph_GPIOA
#define COMM3_RX_GPIO_PORT			GPIOA
#define COMM3_RX_GPIO_PIN			GPIO_Pin_10
#if defined (STM32F40_41xxx)
#define COMM3_RX_PINSOURCE          GPIO_PinSource10
#endif

#define COMM3_IRQn                  USART1_IRQn
#define COMM3_IRQn_IDLE             ENABLE

#define COMM3_BPR                   115200
#define COMM3_WORDLENGTH            USART_WordLength_8b
#define COMM3_STOPBITS              USART_StopBits_1
#define COMM3_PARITY                USART_Parity_No
#define COMM3_FLOWCONTROL           USART_HardwareFlowControl_None

#elif (UART_NUM>3)
#define COMM4						USART1
#define COMM4_CLK					RCC_APB2Periph_USART1
#if defined (STM32F40_41xxx)
#define COMM4_GPIO_AF_USART         GPIO_AF_USART1
#endif

#define COMM4_TX_GPIO_CLK			RCC_APB2Periph_GPIOA
#define COMM4_TX_GPIO_PORT			GPIOA
#define COMM4_TX_GPIO_PIN			GPIO_Pin_9
#if defined (STM32F40_41xxx)
#define COMM4_TX_PINSOURCE          GPIO_PinSource9
#endif

#define COMM4_RX_GPIO_CLK			RCC_APB2Periph_GPIOA
#define COMM4_RX_GPIO_PORT			GPIOA
#define COMM4_RX_GPIO_PIN			GPIO_Pin_10
#if defined (STM32F40_41xxx)
#define COMM4_RX_PINSOURCE          GPIO_PinSource10
#endif

#define COMM4_IRQn                  USART1_IRQn
#define COMM4_IRQn_IDLE             ENABLE

#define COMM4_BPR                   115200
#define COMM4_WORDLENGTH            USART_WordLength_8b
#define COMM4_STOPBITS              USART_StopBits_1
#define COMM4_PARITY                USART_Parity_No
#define COMM4_FLOWCONTROL           USART_HardwareFlowControl_None


#elif (UART_NUM>4)
#define COMM5						USART1
#define COMM5_CLK					RCC_APB2Periph_USART1
#if defined (STM32F40_41xxx)
#define COMM5_GPIO_AF_USART         GPIO_AF_USART1
#endif

#define COMM5_TX_GPIO_CLK			RCC_APB2Periph_GPIOA
#define COMM5_TX_GPIO_PORT			GPIOA
#define COMM5_TX_GPIO_PIN			GPIO_Pin_9
#if defined (STM32F40_41xxx)
#define COMM5_TX_PINSOURCE          GPIO_PinSource9
#endif

#define COMM5_RX_GPIO_CLK			RCC_APB2Periph_GPIOA
#define COMM5_RX_GPIO_PORT			GPIOA
#define COMM5_RX_GPIO_PIN			GPIO_Pin_10
#if defined (STM32F40_41xxx)
#define COMM5_RX_PINSOURCE          GPIO_PinSource10
#endif

#define COMM5_IRQn                  USART1_IRQn
#define COMM5_IRQn_IDLE             ENABLE

#define COMM5_BPR                   115200
#define COMM5_WORDLENGTH            USART_WordLength_8b
#define COMM5_STOPBITS              USART_StopBits_1
#define COMM5_PARITY                USART_Parity_No
#define COMM5_FLOWCONTROL           USART_HardwareFlowControl_None

#endif

/* 使用拼接符## 拼接前后字符串，生成新的宏
   注：拼接出来只可以拼接字符，不可以拼接变量
*/
#define COMM(x)                     COMM##x
#define COMM_CLK(x)					COMM##x##_CLK
#if defined (STM32F40_41xxx)
#define COMM_GPIO_AF_USART(x)       COMM##x##_GPIO_AF_USART
#endif

#define COMM_TX_GPIO_CLK(x)			COMM##x##_TX_GPIO_CLK
#define COMM_TX_GPIO_PORT(x)        COMM##x##_TX_GPIO_PORT
#define COMM_TX_GPIO_PIN(x)			COMM##x##_TX_GPIO_PIN
#if defined (STM32F40_41xxx)
#define COMM_TX_PINSOURCE(x)        COMM##x##_TX_PINSOURCE
#endif

#define COMM_RX_GPIO_CLK(x)			COMM##x##_RX_GPIO_CLK
#define COMM_RX_GPIO_PORT(x)        COMM##x##_RX_GPIO_PORT
#define COMM_RX_GPIO_PIN(x)			COMM##x##_RX_GPIO_PIN
#if defined (STM32F40_41xxx)
#define COMM_RX_PINSOURCE(x)        COMM##x##_RX_PINSOURCE
#endif

#define COMM_IRQn(x)                COMM##x##_IRQn
#define COMM_IRQn_IDLE(x)           COMM##x##_IRQn_IDLE

#define COMM_BPR(x)                 COMM##x##_BPR
#define COMM_WORDLENGTH(x)          COMM##x##_WORDLENGTH
#define COMM_STOPBITS(x)            COMM##x##_STOPBITS
#define COMM_PARITY(x)              COMM##x##_PARITY
#define COMM_FLOWCONTROL(x)         COMM##x##_FLOWCONTROL

//=========================================================================
// 一个串口
#if (UART_NUM==1)

#define COM(x,y)                    do{if(x==0) y = COMM(1);\
                                    }while(0)

#define COM_CLK(x,y)                do{if(x==0) y = COMM_CLK(1);\
                                    }while(0)
#if defined (STM32F40_41xxx)
#define COM_GPIO_AF_USART(x,y)      do{if(x==0) y = COMM_GPIO_AF_USART(1);\
                                    }while(0)
#endif
                                    
#define COM_TX_GPIO_CLK(x,y)		do{if(x==0) y = COMM_TX_GPIO_CLK(1);\
                                    }while(0)

#define COM_TX_GPIO_PORT(x,y)       do{if(x==0) y = COMM_TX_GPIO_PORT(1);\
                                    }while(0)

#define COM_TX_GPIO_PIN(x,y)		do{if(x==0) y = COMM_TX_GPIO_PIN(1);\
                                    }while(0)

#if defined (STM32F40_41xxx)
#define COM_TX_PINSOURCE(x,y)		do{if(x==0) y = COMM_TX_PINSOURCE(1);\
                                    }while(0)
#endif
                                    
#define COM_RX_GPIO_CLK(x,y)		do{if(x==0) y = COMM_RX_GPIO_CLK(1);\
                                    }while(0)

#define COM_RX_GPIO_PORT(x,y)       do{if(x==0) y = COMM_RX_GPIO_PORT(1);\
                                    }while(0)

#define COM_RX_GPIO_PIN(x,y)		do{if(x==0) y = COMM_RX_GPIO_PIN(1);\
                                    }while(0)

#if defined (STM32F40_41xxx)
#define COM_RX_PINSOURCE(x,y)		do{if(x==0) y = COMM_RX_PINSOURCE(1);\
                                    }while(0)
#endif
                                    
#define COM_IRQn(x,y)               do{if(x==0) y = COMM_IRQn(1);\
                                    }while(0)

#define COM_IRQn_IDLE(x,y)          do{if(x==0) y = COMM_IRQn(1);\
                                    }while(0)
                                    
#define COM_BPR(x,y)                do{if(x==0) y = COMM_BPR(1);\
                                    }while(0)

#define COM_WORDLENGTH(x,y)         do{if(x==0) y = COMM_WORDLENGTH(1);\
                                    }while(0)

#define COM_STOPBITS(x,y)           do{if(x==0) y = COMM_STOPBITS(1);\
                                    }while(0)

#define COM_PARITY(x,y)             do{if(x==0) y = COMM_PARITY(1);\
                                    }while(0)

#define COM_FLOWCONTROL(x,y)       do{if(x==0) y = COMM_FLOWCONTROL(1);\
                                    }while(0)

//=========================================================================
// 两个串口
#elif (UART_NUM==2)

#define COM(x,y)                    do{if(x==0) y = COMM(1);\
                                       else if(x==1) y = COMM(2);\
                                    }while(0)

#define COM_CLK(x,y)                do{if(x==0) y = COMM_CLK(1);\
                                       else if(x==1) y = COMM_CLK(2);\
                                    }while(0)

#if defined (STM32F40_41xxx)
#define COM_GPIO_AF_USART(x,y)      do{if(x==0) y = COMM_GPIO_AF_USART(1);\
                                       else if(x==1) y = COMM_GPIO_AF_USART(2);\
                                    }while(0)
#endif
                                    
#define COM_TX_GPIO_CLK(x,y)		do{if(x==0) y = COMM_TX_GPIO_CLK(1);\
                                       else if(x==1) y = COMM_TX_GPIO_CLK(2);\
                                    }while(0)

#define COM_TX_GPIO_PORT(x,y)       do{if(x==0) y = COMM_TX_GPIO_PORT(1);\
                                       else if(x==1) y = COMM_TX_GPIO_PORT(2);\
                                    }while(0)

#define COM_TX_GPIO_PIN(x,y)		do{if(x==0) y = COMM_TX_GPIO_PIN(1);\
                                       else if(x==1) y = COMM_TX_GPIO_PIN(2);\
                                    }while(0)

#if defined (STM32F40_41xxx)
#define COM_TX_PINSOURCE(x,y)		do{if(x==0) y = COMM_TX_PINSOURCE(1);\
                                       else if(x==1) y = COMM_TX_PINSOURCE(2);\
                                    }while(0)
#endif

#define COM_RX_GPIO_CLK(x,y)		do{if(x==0) y = COMM_RX_GPIO_CLK(1);\
                                       else if(x==1) y = COMM_RX_GPIO_CLK(2);\
                                    }while(0)

#define COM_RX_GPIO_PORT(x,y)       do{if(x==0) y = COMM_RX_GPIO_PORT(1);\
                                       else if(x==1) y = COMM_RX_GPIO_PORT(2);\
                                    }while(0)

#define COM_RX_GPIO_PIN(x,y)		do{if(x==0) y = COMM_RX_GPIO_PIN(1);\
                                       else if(x==1) y = COMM_RX_GPIO_PIN(2);\
                                    }while(0)

#if defined (STM32F40_41xxx)
#define COM_RX_PINSOURCE(x,y)		do{if(x==0) y = COMM_RX_PINSOURCE(1);\
                                       else if(x==1) y = COMM_RX_PINSOURCE(2);\
                                    }while(0)
#endif

#define COM_IRQn(x,y)               do{if(x==0) y = COMM_IRQn(1);\
                                       else if(x==1) y = COMM_IRQn(2);\
                                    }while(0)

#define COM_IRQn_IDLE(x,y)          do{if(x==0) y = COMM_IRQn(1);\
                                       else if(x==1) y = COMM_IRQn(2);\
                                    }while(0)
                                    
#define COM_BPR(x,y)                do{if(x==0) y = COMM_BPR(1);\
                                       else if(x==1) y = COMM_BPR(2);\
                                    }while(0)

#define COM_WORDLENGTH(x,y)         do{if(x==0) y = COMM_WORDLENGTH(1);\
                                       else if(x==1) y = COMM_WORDLENGTH(2);\
                                    }while(0)

#define COM_STOPBITS(x,y)           do{if(x==0) y = COMM_STOPBITS(1);\
                                       else if(x==1) y = COMM_STOPBITS(2);\
                                    }while(0)

#define COM_PARITY(x,y)             do{if(x==0) y = COMM_PARITY(1);\
                                       else if(x==1) y = COMM_PARITY(2);\
                                    }while(0)

#define COM_FLOWCONTROL(x,y)       do{if(x==0) y = COMM_FLOWCONTROL(1);\
                                      else if(x==1) y = COMM_FLOWCONTROL(2);\
                                    }while(0)


//=========================================================================
// 三个串口
#elif (UART_NUM==3)

#define COM(x,y)                    do{if(x==0) y = COMM(1);\
                                       else if(x==1) y = COMM(2);\
                                       else if(x==2) y = COMM(3);\
                                    }while(0)

#define COM_CLK(x,y)                do{if(x==0) y = COMM_CLK(1);\
                                       else if(x==1) y = COMM_CLK(2);\
                                       else if(x==2) y = COMM_CLK(3);\
                                    }while(0)

#if defined (STM32F40_41xxx)
#define COM_GPIO_AF_USART(x,y)      do{if(x==0) y = COMM_GPIO_AF_USART(1);\
                                       else if(x==1) y = COMM_GPIO_AF_USART(2);\
                                       else if(x==2) y = COMM_GPIO_AF_USART(3);\
                                    }while(0)
#endif

#define COM_TX_GPIO_CLK(x,y)		do{if(x==0) y = COMM_TX_GPIO_CLK(1);\
                                       else if(x==1) y = COMM_TX_GPIO_CLK(2);\
                                       else if(x==2) y = COMM_TX_GPIO_CLK(3);\
                                    }while(0)

#define COM_TX_GPIO_PORT(x,y)       do{if(x==0) y = COMM_TX_GPIO_PORT(1);\
                                       else if(x==1) y = COMM_TX_GPIO_PORT(2);\
                                       else if(x==2) y = COMM_TX_GPIO_PORT(3);\
                                    }while(0)

#define COM_TX_GPIO_PIN(x,y)		do{if(x==0) y = COMM_TX_GPIO_PIN(1);\
                                       else if(x==1) y = COMM_TX_GPIO_PIN(2);\
                                       else if(x==2) y = COMM_TX_GPIO_PIN(3);\
                                    }while(0)

#if defined (STM32F40_41xxx)
#define COM_TX_PINSOURCE(x,y)		do{if(x==0) y = COMM_TX_PINSOURCE(1);\
                                       else if(x==1) y = COMM_TX_PINSOURCE(2);\
                                       else if(x==2) y = COMM_TX_PINSOURCE(3);\
                                    }while(0)
#endif

#define COM_RX_GPIO_CLK(x,y)		do{if(x==0) y = COMM_RX_GPIO_CLK(1);\
                                       else if(x==1) y = COMM_RX_GPIO_CLK(2);\
                                       else if(x==2) y = COMM_RX_GPIO_CLK(3);\
                                    }while(0)

#define COM_RX_GPIO_PORT(x,y)       do{if(x==0) y = COMM_RX_GPIO_PORT(1);\
                                       else if(x==1) y = COMM_RX_GPIO_PORT(2);\
                                       else if(x==2) y = COMM_RX_GPIO_PORT(3);\
                                    }while(0)

#define COM_RX_GPIO_PIN(x,y)		do{if(x==0) y = COMM_RX_GPIO_PIN(1);\
                                       else if(x==1) y = COMM_RX_GPIO_PIN(2);\
                                       else if(x==2) y = COMM_RX_GPIO_PIN(3);\
                                    }while(0)

#if defined (STM32F40_41xxx)
#define COM_RX_PINSOURCE(x,y)		do{if(x==0) y = COMM_RX_PINSOURCE(1);\
                                       else if(x==1) y = COMM_RX_PINSOURCE(2);\
                                       else if(x==2) y = COMM_RX_PINSOURCE(3);\
                                    }while(0)
#endif

#define COM_IRQn(x,y)               do{if(x==0) y = COMM_IRQn(1);\
                                       else if(x==1) y = COMM_IRQn(2);\
                                       else if(x==2) y = COMM_IRQn(3);\
                                    }while(0)

#define COM_IRQn_IDLE(x,y)          do{if(x==0) y = COMM_IRQn(1);\
                                       else if(x==1) y = COMM_IRQn(2);\
                                       else if(x==2) y = COMM_IRQn(3);\
                                    }while(0)

#define COM_BPR(x,y)                do{if(x==0) y = COMM_BPR(1);\
                                       else if(x==1) y = COMM_BPR(2);\
                                       else if(x==2) y = COMM_BPR(3);\
                                    }while(0)

#define COM_WORDLENGTH(x,y)         do{if(x==0) y = COMM_WORDLENGTH(1);\
                                       else if(x==1) y = COMM_WORDLENGTH(2);\
                                       else if(x==2) y = COMM_WORDLENGTH(3);\
                                    }while(0)

#define COM_STOPBITS(x,y)           do{if(x==0) y = COMM_STOPBITS(1);\
                                       else if(x==1) y = COMM_STOPBITS(2);\
                                       else if(x==2) y = COMM_STOPBITS(3);\
                                    }while(0)

#define COM_PARITY(x,y)             do{if(x==0) y = COMM_PARITY(1);\
                                       else if(x==1) y = COMM_PARITY(2);\
                                       else if(x==2) y = COMM_PARITY(3);\
                                    }while(0)

#define COM_FLOWCONTROL(x,y)       do{if(x==0) y = COMM_FLOWCONTROL(1);\
                                      else if(x==1) y = COMM_FLOWCONTROL(2);\
                                      else if(x==2) y = COMM_FLOWCONTROL(3);\
                                    }while(0)
                                    

//=========================================================================
// 四个串口
#elif (UART_NUM==4)

#define COM(x,y)                    do{if(x==0) y = COMM(1);\
                                       else if(x==1) y = COMM(2);\
                                       else if(x==2) y = COMM(3);\
                                       else if(x==3) y = COMM(4);\
                                    }while(0)

#define COM_CLK(x,y)                do{if(x==0) y = COMM_CLK(1);\
                                       else if(x==1) y = COMM_CLK(2);\
                                       else if(x==2) y = COMM_CLK(3);\
                                       else if(x==3) y = COMM_CLK(4);\
                                    }while(0)

#if defined (STM32F40_41xxx)
#define COM_GPIO_AF_USART(x,y)      do{if(x==0) y = COMM_GPIO_AF_USART(1);\
                                       else if(x==1) y = COMM_GPIO_AF_USART(2);\
                                       else if(x==2) y = COMM_GPIO_AF_USART(3);\
                                       else if(x==3) y = COMM_GPIO_AF_USART(4);\
                                    }while(0)
#endif

#define COM_TX_GPIO_CLK(x,y)		do{if(x==0) y = COMM_TX_GPIO_CLK(1);\
                                       else if(x==1) y = COMM_TX_GPIO_CLK(2);\
                                       else if(x==2) y = COMM_TX_GPIO_CLK(3);\
                                       else if(x==3) y = COMM_TX_GPIO_CLK(4);\
                                    }while(0)

#define COM_TX_GPIO_PORT(x,y)       do{if(x==0) y = COMM_TX_GPIO_PORT(1);\
                                       else if(x==1) y = COMM_TX_GPIO_PORT(2);\
                                       else if(x==2) y = COMM_TX_GPIO_PORT(3);\
                                       else if(x==3) y = COMM_TX_GPIO_PORT(4);\
                                    }while(0)

#define COM_TX_GPIO_PIN(x,y)		do{if(x==0) y = COMM_TX_GPIO_PIN(1);\
                                       else if(x==1) y = COMM_TX_GPIO_PIN(2);\
                                       else if(x==2) y = COMM_TX_GPIO_PIN(3);\
                                       else if(x==3) y = COMM_TX_GPIO_PIN(4);\
                                    }while(0)

#if defined (STM32F40_41xxx)
#define COM_TX_PINSOURCE(x,y)		do{if(x==0) y = COMM_TX_PINSOURCE(1);\
                                       else if(x==1) y = COMM_TX_PINSOURCE(2);\
                                       else if(x==2) y = COMM_TX_PINSOURCE(3);\
                                       else if(x==3) y = COMM_TX_PINSOURCE(4);\
                                    }while(0)
#endif

#define COM_RX_GPIO_CLK(x,y)		do{if(x==0) y = COMM_RX_GPIO_CLK(1);\
                                       else if(x==1) y = COMM_RX_GPIO_CLK(2);\
                                       else if(x==2) y = COMM_RX_GPIO_CLK(3);\
                                       else if(x==3) y = COMM_RX_GPIO_CLK(4);\
                                    }while(0)

#define COM_RX_GPIO_PORT(x,y)       do{if(x==0) y = COMM_RX_GPIO_PORT(1);\
                                       else if(x==1) y = COMM_RX_GPIO_PORT(2);\
                                       else if(x==2) y = COMM_RX_GPIO_PORT(3);\
                                       else if(x==3) y = COMM_RX_GPIO_PORT(4);\
                                    }while(0)

#define COM_RX_GPIO_PIN(x,y)		do{if(x==0) y = COMM_RX_GPIO_PIN(1);\
                                       else if(x==1) y = COMM_RX_GPIO_PIN(2);\
                                       else if(x==2) y = COMM_RX_GPIO_PIN(3);\
                                       else if(x==3) y = COMM_RX_GPIO_PIN(4);\
                                    }while(0)

#if defined (STM32F40_41xxx)
#define COM_RX_PINSOURCE(x,y)		do{if(x==0) y = COMM_RX_PINSOURCE(1);\
                                       else if(x==1) y = COMM_RX_PINSOURCE(2);\
                                       else if(x==2) y = COMM_RX_PINSOURCE(3);\
                                       else if(x==3) y = COMM_RX_PINSOURCE(4);\
                                    }while(0)
#endif

#define COM_IRQn(x,y)               do{if(x==0) y = COMM_IRQn(1);\
                                       else if(x==1) y = COMM_IRQn(2);\
                                       else if(x==2) y = COMM_IRQn(3);\
                                       else if(x==3) y = COMM_IRQn(4);\
                                    }while(0)

#define COM_IRQn_IDLE(x,y)          do{if(x==0) y = COMM_IRQn(1);\
                                       else if(x==1) y = COMM_IRQn(2);\
                                       else if(x==2) y = COMM_IRQn(3);\
                                       else if(x==3) y = COMM_IRQn(4);\
                                    }while(0)
                                    
#define COM_BPR(x,y)                do{if(x==0) y = COMM_BPR(1);\
                                       else if(x==1) y = COMM_BPR(2);\
                                       else if(x==2) y = COMM_BPR(3);\
                                       else if(x==3) y = COMM_BPR(4);\
                                    }while(0)

#define COM_WORDLENGTH(x,y)         do{if(x==0) y = COMM_WORDLENGTH(1);\
                                       else if(x==1) y = COMM_WORDLENGTH(2);\
                                       else if(x==2) y = COMM_WORDLENGTH(3);\
                                       else if(x==3) y = COMM_WORDLENGTH(4);\
                                    }while(0)

#define COM_STOPBITS(x,y)           do{if(x==0) y = COMM_STOPBITS(1);\
                                       else if(x==1) y = COMM_STOPBITS(2);\
                                       else if(x==2) y = COMM_STOPBITS(3);\
                                       else if(x==3) y = COMM_STOPBITS(4);\
                                    }while(0)

#define COM_PARITY(x,y)             do{if(x==0) y = COMM_PARITY(1);\
                                       else if(x==1) y = COMM_PARITY(2);\
                                       else if(x==2) y = COMM_PARITY(3);\
                                       else if(x==3) y = COMM_PARITY(4);\
                                    }while(0)

#define COM_FLOWCONTROL(x,y)       do{if(x==0) y = COMM_FLOWCONTROL(1);\
                                      else if(x==1) y = COMM_FLOWCONTROL(2);\
                                      else if(x==2) y = COMM_FLOWCONTROL(3);\
                                      else if(x==3) y = COMM_FLOWCONTROL(4);\
                                    }while(0)
                                    

//=========================================================================
// 五个串口
#elif (UART_NUM==5)

#define COM(x,y)                    do{if(x==0) y = COMM(1);\
                                       else if(x==1) y = COMM(2);\
                                       else if(x==2) y = COMM(3);\
                                       else if(x==3) y = COMM(4);\
                                       else if(x==4) y = COMM(5);\
                                    }while(0)

#define COM_CLK(x,y)                do{if(x==0) y = COMM_CLK(1);\
                                       else if(x==1) y = COMM_CLK(2);\
                                       else if(x==2) y = COMM_CLK(3);\
                                       else if(x==3) y = COMM_CLK(4);\
                                       else if(x==4) y = COMM_CLK(5);\
                                    }while(0)

#if defined (STM32F40_41xxx)
#define COM_GPIO_AF_USART(x,y)      do{if(x==0) y = COMM_GPIO_AF_USART(1);\
                                       else if(x==1) y = COMM_GPIO_AF_USART(2);\
                                       else if(x==2) y = COMM_GPIO_AF_USART(3);\
                                       else if(x==3) y = COMM_GPIO_AF_USART(4);\
                                       else if(x==4) y = COMM_GPIO_AF_USART(5);\
                                    }while(0)
#endif
                                    
#define COM_TX_GPIO_CLK(x,y)		do{if(x==0) y = COMM_TX_GPIO_CLK(1);\
                                       else if(x==1) y = COMM_TX_GPIO_CLK(2);\
                                       else if(x==2) y = COMM_TX_GPIO_CLK(3);\
                                       else if(x==3) y = COMM_TX_GPIO_CLK(4);\
                                       else if(x==4) y = COMM_TX_GPIO_CLK(5);\
                                    }while(0)

#define COM_TX_GPIO_PORT(x,y)       do{if(x==0) y = COMM_TX_GPIO_PORT(1);\
                                       else if(x==1) y = COMM_TX_GPIO_PORT(2);\
                                       else if(x==2) y = COMM_TX_GPIO_PORT(3);\
                                       else if(x==3) y = COMM_TX_GPIO_PORT(4);\
                                       else if(x==4) y = COMM_TX_GPIO_PORT(5);\
                                    }while(0)

#define COM_TX_GPIO_PIN(x,y)		do{if(x==0) y = COMM_TX_GPIO_PIN(1);\
                                       else if(x==1) y = COMM_TX_GPIO_PIN(2);\
                                       else if(x==2) y = COMM_TX_GPIO_PIN(3);\
                                       else if(x==3) y = COMM_TX_GPIO_PIN(4);\
                                       else if(x==4) y = COMM_TX_GPIO_PIN(5);\
                                    }while(0)

#if defined (STM32F40_41xxx)
#define COM_TX_PINSOURCE(x,y)		do{if(x==0) y = COMM_TX_PINSOURCE(1);\
                                       else if(x==1) y = COMM_TX_PINSOURCE(2);\
                                       else if(x==2) y = COMM_TX_PINSOURCE(3);\
                                       else if(x==3) y = COMM_TX_PINSOURCE(4);\
                                       else if(x==4) y = COMM_TX_PINSOURCE(5);\
                                    }while(0)
#endif

#define COM_RX_GPIO_CLK(x,y)		do{if(x==0) y = COMM_RX_GPIO_CLK(1);\
                                       else if(x==1) y = COMM_RX_GPIO_CLK(2);\
                                       else if(x==2) y = COMM_RX_GPIO_CLK(3);\
                                       else if(x==3) y = COMM_RX_GPIO_CLK(4);\
                                       else if(x==4) y = COMM_RX_GPIO_CLK(5);\
                                    }while(0)

#define COM_RX_GPIO_PORT(x,y)       do{if(x==0) y = COMM_RX_GPIO_PORT(1);\
                                       else if(x==1) y = COMM_RX_GPIO_PORT(2);\
                                       else if(x==2) y = COMM_RX_GPIO_PORT(3);\
                                       else if(x==3) y = COMM_RX_GPIO_PORT(4);\
                                       else if(x==4) y = COMM_RX_GPIO_PORT(5);\
                                    }while(0)

#define COM_RX_GPIO_PIN(x,y)		do{if(x==0) y = COMM_RX_GPIO_PIN(1);\
                                       else if(x==1) y = COMM_RX_GPIO_PIN(2);\
                                       else if(x==2) y = COMM_RX_GPIO_PIN(3);\
                                       else if(x==3) y = COMM_RX_GPIO_PIN(4);\
                                       else if(x==4) y = COMM_RX_GPIO_PIN(5);\
                                    }while(0)

#if defined (STM32F40_41xxx)
#define COM_RX_PINSOURCE(x,y)		do{if(x==0) y = COMM_RX_PINSOURCE(1);\
                                       else if(x==1) y = COMM_RX_PINSOURCE(2);\
                                       else if(x==2) y = COMM_RX_PINSOURCE(3);\
                                       else if(x==3) y = COMM_RX_PINSOURCE(4);\
                                       else if(x==4) y = COMM_RX_PINSOURCE(5);\
                                    }while(0)
#endif

#define COM_IRQn(x,y)               do{if(x==0) y = COMM_IRQn(1);\
                                       else if(x==1) y = COMM_IRQn(2);\
                                       else if(x==2) y = COMM_IRQn(3);\
                                       else if(x==3) y = COMM_IRQn(4);\
                                       else if(x==4) y = COMM_IRQn(5);\
                                    }while(0)

#define COM_IRQn_IDLE(x,y)          do{if(x==0) y = COMM_IRQn(1);\
                                       else if(x==1) y = COMM_IRQn(2);\
                                       else if(x==2) y = COMM_IRQn(3);\
                                       else if(x==3) y = COMM_IRQn(4);\
                                       else if(x==4) y = COMM_IRQn(5);\
                                    }while(0)
                                    
#define COM_BPR(x,y)                do{if(x==0) y = COMM_BPR(1);\
                                       else if(x==1) y = COMM_BPR(2);\
                                       else if(x==2) y = COMM_BPR(3);\
                                       else if(x==3) y = COMM_BPR(4);\
                                       else if(x==4) y = COMM_BPR(5);\
                                    }while(0)

#define COM_WORDLENGTH(x,y)         do{if(x==0) y = COMM_WORDLENGTH(1);\
                                       else if(x==1) y = COMM_WORDLENGTH(2);\
                                       else if(x==2) y = COMM_WORDLENGTH(3);\
                                       else if(x==3) y = COMM_WORDLENGTH(4);\
                                       else if(x==4) y = COMM_WORDLENGTH(5);\
                                    }while(0)

#define COM_STOPBITS(x,y)           do{if(x==0) y = COMM_STOPBITS(1);\
                                       else if(x==1) y = COMM_STOPBITS(2);\
                                       else if(x==2) y = COMM_STOPBITS(3);\
                                       else if(x==3) y = COMM_STOPBITS(4);\
                                       else if(x==4) y = COMM_STOPBITS(5);\
                                    }while(0)

#define COM_PARITY(x,y)             do{if(x==0) y = COMM_PARITY(1);\
                                       else if(x==1) y = COMM_PARITY(2);\
                                       else if(x==2) y = COMM_PARITY(3);\
                                       else if(x==3) y = COMM_PARITY(4);\
                                       else if(x==4) y = COMM_STOPBITS(5);\
                                    }while(0)

#define COM_FLOWCONTROL(x,y)       do{if(x==0) y = COMM_FLOWCONTROL(1);\
                                      else if(x==1) y = COMM_FLOWCONTROL(2);\
                                      else if(x==2) y = COMM_FLOWCONTROL(3);\
                                      else if(x==3) y = COMM_FLOWCONTROL(4);\
                                      else if(x==4) y = COMM_FLOWCONTROL(5);\
                                    }while(0)

                                    
#endif


// 串口配置结构体
typedef struct
{
    em_Usart_Com            eCom;
    USART_TypeDef*          Com_usart;
    UINT8                   Com_gpio_af_usart;
    GPIO_TypeDef*           Com_tx_port;
    GPIO_TypeDef*           Com_rx_port;
    UINT16                  Com_tx_pin;
    UINT16                  Com_rx_pin;
    UINT32                  Com_usart_clk;
    UINT32                  Com_tx_gpio_clk;
    UINT32                  Com_rx_gpio_clk;
    UINT8                   Com_tx_pinsource;
    UINT8                   Com_rx_pinsource;
    UINT16                  Com_irqn;
    UINT16                  Com_irqn_idle;
    UINT32                  Com_bpr;
    UINT16                  Com_wordlength;
    UINT16                  Com_stopbits;
    UINT16                  Com_parity;
    UINT16                  Com_flowControl;
#if CTRL_COM_DMA_ENABLE
#if defined (STM32F40_41xxx)
    DMA_Stream_TypeDef*     Com_dma_Channel;        // 在407的DMA流有点类似103的通道，所以写为通道
    UINT32                  Com_dma_Stream_Channel;
#else
    DMA_Channel_TypeDef*    Com_dma_Channel;        // 这里只采用了串口接收DMA
#endif
#endif
    UINT32                  Com_dma_ITTC;
    UINT32                  Com_dma_bpr;
    UINT16                  Com_BufLen;
    UINT8*                  Com_MemBuf;
    UINT8                   Com_Receive_Flag;       // 接收完成标志
}st_ComConfigType;

typedef UINT8 (*funCOMOne)  (void);
typedef UINT8 (*funCOMTwo)  (em_Usart_Com);
typedef UINT8 (*funCOMTree) (em_Usart_Com,UINT8);
typedef UINT8 (*funCOMFour) (em_Usart_Com,UINT32);
typedef UINT8 (*funCOMFive) (em_Usart_Com,UINT8 *,UINT16);
typedef struct
{
    funCOMTwo       FunComInit;
    
    funCOMTwo       FunComClearFlagTI;
    funCOMTwo       FunComClearFlagRI;
    funCOMTwo       FunComCheckFlagTI;
    funCOMTwo       FunComCheckFlagRI;
    funCOMTwo       FunComCheckFlagTXE;
    funCOMTwo       FunComInitTxEnable;
    funCOMTwo       FunComInitRxEnable;
    funCOMTwo       FunComInitTxDisable;
    funCOMTwo       FunComInitRxDisable;
    funCOMTree      FunComBufSet;
    funCOMTwo       FunComBufGet;
    
    funCOMFive       FunComSendString;
}st_ComFunListType;

typedef struct
{
    st_ComConfigType    sComConfig[UART_NUM];
    st_ComFunListType   sComFunList;
}st_ComCtrlModule;


typedef struct
{
    UINT8 USART_Com_ID[5];              // 最多支持USART1-USART5
}st_UsartComId;

_EXT_ st_UsartComId   sUsartComId;                     // 串口对应ID好结构体
_EXT_ st_ComCtrlModule sComCtrl;

BOOL  Com_Initialization(em_Usart_Com eCom,UINT8 *com_buf,UINT16 com_len);
UINT8 Usart_ComInit(em_Usart_Com eCom);
UINT8 Usart_ComCheckFlagTI(em_Usart_Com eCom);
UINT8 Usart_ComCheckFlagRI(em_Usart_Com eCom);
UINT8 Usart_ComCheckFlagTXE(em_Usart_Com eCom);
UINT8 Usart_ComIntTxEnable(em_Usart_Com eCom);
UINT8 Usart_ComIntRxEnable(em_Usart_Com eCom);
UINT8 Usart_ComIntTxDisable(em_Usart_Com eCom);
UINT8 Usart_ComIntRxDisable(em_Usart_Com eCom);
UINT8 Usart_ComBufSet(em_Usart_Com eCom, UINT8 ch);
UINT8 Usart_ComBufGet(em_Usart_Com eCom);

UINT8 Usart_ComSendString(em_Usart_Com eCom, UINT8 *SendBuf, UINT16 SendLen);
void USART_IRQReceive(em_Usart_Com ComId);
#endif


