#ifndef __CTRLSPI_H__
#define __CTRLSPI_H__

#define SPI_NUM                         1

#if defined (STM32F40_41xxx)
#define SPI_AF                          ((uint8_t)0x05)
#endif

#define CTRL_SPI1						SPI1
#define CTRL_SPI1_CLK					RCC_APB2Periph_SPI1

#define CTRL_SPI1_SCK_GPIO_CLK			RCC_APB2Periph_GPIOA
#define CTRL_SPI1_SCK_GPIO_PORT			GPIOA
#define CTRL_SPI1_SCK_GPIO_PIN			GPIO_Pin_5
#if defined (STM32F40_41xxx)
#define CTRL_SPI1_SCK_GPIO_SOURCE       GPIO_PinSource5
#endif

#define CTRL_SPI1_MISO_GPIO_CLK			RCC_APB2Periph_GPIOA
#define CTRL_SPI1_MISO_GPIO_PORT        GPIOA
#define CTRL_SPI1_MISO_GPIO_PIN			GPIO_Pin_6

#define CTRL_SPI1_MOSI_GPIO_CLK			RCC_APB2Periph_GPIOA
#define CTRL_SPI1_MOSI_GPIO_PORT        GPIOA
#define CTRL_SPI1_MOSI_GPIO_PIN			GPIO_Pin_7

// CS 由其他调用的单元进行定义

#define  CTRL_SPI1_AF                   GPIO_AF_SPI1

#define CTRL_SPI1_BRP                   SPI_BaudRatePrescaler_64

#if (SPI_NUM>1)
#define CTRL_SPI2						SPI3
#define CTRL_SPI2_CLK					RCC_APB1Periph_SPI3

//#define CTRL_SPI2_CS_GPIO_CLK			RCC_APB2Periph_GPIOC
//#define CTRL_SPI2_CS_GPIO_PORT			GPIOC
//#define CTRL_SPI2_CS_GPIO_PIN			GPIO_Pin_10

#define CTRL_SPI2_SCK_GPIO_CLK			RCC_AHB1Periph_GPIOC
#define CTRL_SPI2_SCK_GPIO_PORT			GPIOC
#define CTRL_SPI2_SCK_GPIO_PIN			GPIO_Pin_10

#define CTRL_SPI2_MISO_GPIO_CLK			RCC_AHB1Periph_GPIOC
#define CTRL_SPI2_MISO_GPIO_PORT        GPIOC
#define CTRL_SPI2_MISO_GPIO_PIN			GPIO_Pin_11

#define CTRL_SPI2_MOSI_GPIO_CLK			RCC_AHB1Periph_GPIOC
#define CTRL_SPI2_MOSI_GPIO_PORT        GPIOC
#define CTRL_SPI2_MOSI_GPIO_PIN			GPIO_Pin_12

#define  CTRL_SPI2_AF                   GPIO_AF_SPI3

#define CTRL_SPI2_BRP                   SPI_BaudRatePrescaler_256
#endif


#define CTRL_SPI(x)						CTRL_SPI##x
#define CTRL_SPI_CLK(x)					CTRL_SPI##x##_CLK

//#define CTRL_SPI_CS_GPIO_CLK(x)			CTRL_SPI##x##_CS_GPIO_CLK
//#define CTRL_SPI_CS_GPIO_PORT(x)        CTRL_SPI##x##_CS_GPIO_PORT
//#define CTRL_SPI_CS_GPIO_PIN(x)			CTRL_SPI##x##_CS_GPIO_PIN

#define CTRL_SPI_SCK_GPIO_CLK(x)        CTRL_SPI##x##_SCK_GPIO_CLK
#define CTRL_SPI_SCK_GPIO_PORT(x)       CTRL_SPI##x##_SCK_GPIO_PORT
#define CTRL_SPI_SCK_GPIO_PIN(x)        CTRL_SPI##x##_SCK_GPIO_PIN

#define CTRL_SPI_MISO_GPIO_CLK(x)       CTRL_SPI##x##_MISO_GPIO_CLK
#define CTRL_SPI_MISO_GPIO_PORT(x)      CTRL_SPI##x##_MISO_GPIO_PORT
#define CTRL_SPI_MISO_GPIO_PIN(x)       CTRL_SPI##x##_MISO_GPIO_PIN

#define CTRL_SPI_MOSI_GPIO_CLK(x)       CTRL_SPI##x##_MOSI_GPIO_CLK
#define CTRL_SPI_MOSI_GPIO_PORT(x)      CTRL_SPI##x##_MOSI_GPIO_PORT
#define CTRL_SPI_MOSI_GPIO_PIN(x)       CTRL_SPI##x##_MOSI_GPIO_PIN

#define  CTRL_SPI_AF(x)                 CTRL_SPI##x##_AF

#define CTRL_SPI_BRP(x)                 CTRL_SPI##x##_BRP

#if (SPI_NUM == 1)

#define SPII(x,y)                       do{if(x==0) y = CTRL_SPI(1);\
                                        }while(0)

#define SPI_CLK(x,y)                    do{if(x==0) y = CTRL_SPI_CLK(1);\
                                        }while(0)

#define SPI_SCK_GPIO_CLK(x,y)		    do{if(x==0) y = CTRL_SPI_SCK_GPIO_CLK(1);\
                                        }while(0)

#define SPI_SCK_GPIO_PORT(x,y)           do{if(x==0) y = CTRL_SPI_SCK_GPIO_PORT(1);\
                                        }while(0)

#define SPI_SCK_GPIO_PIN(x,y)		    do{if(x==0) y = CTRL_SPI_SCK_GPIO_PIN(1);\
                                        }while(0)

#define SPI_MISO_GPIO_CLK(x,y)		    do{if(x==0) y = CTRL_SPI_MISO_GPIO_CLK(1);\
                                        }while(0)

#define SPI_MISO_GPIO_PORT(x,y)         do{if(x==0) y = CTRL_SPI_MISO_GPIO_PORT(1);\
                                        }while(0)

#define SPI_MISO_GPIO_PIN(x,y)		    do{if(x==0) y = CTRL_SPI_MISO_GPIO_PIN(1);\
                                        }while(0)

#define SPI_MOSI_GPIO_CLK(x,y)		    do{if(x==0) y = CTRL_SPI_MOSI_GPIO_CLK(1);\
                                        }while(0)

#define SPI_MOSI_GPIO_PORT(x,y)         do{if(x==0) y = CTRL_SPI_MOSI_GPIO_PORT(1);\
                                        }while(0)

#define SPI_MOSI_GPIO_PIN(x,y)		    do{if(x==0) y = CTRL_SPI_MOSI_GPIO_PIN(1);\
                                        }while(0)

#define SPI_AF(x,y)		                do{if(x==0) y = CTRL_SPI_AF(1);\
                                        }while(0)
                                        
#define SPI_BRP(x,y)		            do{if(x==0) y = CTRL_SPI_BRP(1);\
                                        }while(0)
#elif (UART_NUM==2)

#define SPII(x,y)                       do{if(x==0) y = CTRL_SPI(1);\
                                      else if(x==1) y = CTRL_SPI(2);\
                                        }while(0)

#define SPI_CLK(x,y)                    do{if(x==0) y = CTRL_SPI_CLK(1);\
                                      else if(x==1) y = CTRL_SPI_CLK(2);\
                                        }while(0)


#define SPI_SCK_GPIO_CLK(x,y)		    do{if(x==0) y = CTRL_SPI_SCK_GPIO_CLK(1);\
                                      else if(x==1) y = CTRL_SPI_SCK_GPIO_CLK(2);\
                                        }while(0)

#define SPI_SCK_GPIO_PORT(x,y)           do{if(x==0) y = CTRL_SPI_SCK_GPIO_PORT(1);\
                                      else if(x==1) y = CTRL_SPI_SCK_GPIO_PORT(2);\
                                        }while(0)

#define SPI_SCK_GPIO_PIN(x,y)		    do{if(x==0) y = CTRL_SPI_SCK_GPIO_PIN(1);\
                                      else if(x==1) y = CTRL_SPI_SCK_GPIO_PIN(2);\
                                        }while(0)

#define SPI_MISO_GPIO_CLK(x,y)		    do{if(x==0) y = CTRL_SPI_MISO_GPIO_CLK(1);\
                                      else if(x==1) y = CTRL_SPI_MISO_GPIO_CLK(2);\
                                        }while(0)

#define SPI_MISO_GPIO_PORT(x,y)         do{if(x==0) y = CTRL_SPI_MISO_GPIO_PORT(1);\
                                      else if(x==1) y = CTRL_SPI_MISO_GPIO_PORT(2);\
                                        }while(0)

#define SPI_MISO_GPIO_PIN(x,y)		    do{if(x==0) y = CTRL_SPI_MISO_GPIO_PIN(1);\
                                      else if(x==1) y = CTRL_SPI_MISO_GPIO_PIN(2);\
                                        }while(0)

#define SPI_MOSI_GPIO_CLK(x,y)		    do{if(x==0) y = CTRL_SPI_MOSI_GPIO_CLK(1);\
                                      else if(x==1) y = CTRL_SPI_MOSI_GPIO_CLK(2);\
                                        }while(0)

#define SPI_MOSI_GPIO_PORT(x,y)         do{if(x==0) y = CTRL_SPI_MOSI_GPIO_PORT(1);\
                                      else if(x==1) y = CTRL_SPI_MOSI_GPIO_PORT(2);\
                                        }while(0)

#define SPI_MOSI_GPIO_PIN(x,y)		    do{if(x==0) y = CTRL_SPI_MOSI_GPIO_PIN(1);\
                                      else if(x==1) y = CTRL_SPI_MOSI_GPIO_PIN(2);\
                                        }while(0)

#define SPI_AF(x,y)		                do{if(x==0) y = CTRL_SPI_AF(1);\
                                      else if(x==1) y = CTRL_SPI_AF(2);\
                                        }while(0)
                                        
#define SPI_BRP(x,y)		            do{if(x==0) y = CTRL_SPI_BRP(1);\
                                      else if(x==1) y = CTRL_SPI_BRP(2);\
                                        }while(0)
#endif
// 串口配置结构体
typedef struct
{
    SPI_TypeDef*    Ctrl_Spi;
    UINT32          Ctrl_Spi_Clk;
    
    GPIO_TypeDef*   Spi_sck_port;
    UINT16          Spi_sck_pin;
    UINT32          Spi_sck_clk;
#if defined (STM32F40_41xxx)
    UINT8           Spi_sck_pinsource;
#endif   

    GPIO_TypeDef*   Spi_miso_port;
    UINT16          Spi_miso_pin;
    UINT32          Spi_miso_clk;
    
#if defined (STM32F40_41xxx)
    UINT8           Spi_miso_pinsource;
#endif
    
    GPIO_TypeDef*   Spi_mosi_port;
    UINT16          Spi_mosi_pin;
    UINT32          Spi_mosi_clk;
#if defined (STM32F40_41xxx)
    UINT8           Spi_af;
    UINT8           Spi_mosi_pinsource;
#endif
    
    UINT16          Spi_bpr;
}tSpiConfig;

typedef UINT8 (*funSPIOne)   (void);
typedef UINT8 (*funSPITwo)   (UINT8);
typedef UINT8 (*funSPITree)  (UINT8,UINT8);
typedef UINT8 (*funSPIFour)  (UINT8,UINT32);

enum eSpi_type
{
	eSpi_1 = 0,
	eSpi_2 = 1
};

//typedef struct
//{
//    enum eCom_type  eCom;
//    funSPIOne      ComInit;
//    funSPITwo       ComClearFlagTI;

//}tSpiFunList;

UINT8 SPI_ReadWriteByte(UINT8 eSpiId,UINT8 TxData);
void Fun_SPI_Init(UINT8 eSpiId);

#endif
