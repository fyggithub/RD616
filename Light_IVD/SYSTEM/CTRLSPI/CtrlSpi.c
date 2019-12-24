#ifndef __CTRLSPI_C__
#define __CTRLSPI_C__

#include "CONFIG\My_Type.h"
#include "STM_SYS\Stm_Sys.h" 
#include "CTRLSPI\CtrlSpi.h"

tSpiConfig SpiConfig[SPI_NUM];

void ConfigSpiPara(UINT8 eSpiId)
{
    SPII(eSpiId,SpiConfig[eSpiId].Ctrl_Spi);
    SPI_CLK(eSpiId,SpiConfig[eSpiId].Ctrl_Spi_Clk);
    
    SPI_SCK_GPIO_CLK(eSpiId,SpiConfig[eSpiId].Spi_sck_clk);
    SPI_SCK_GPIO_PORT(eSpiId,SpiConfig[eSpiId].Spi_sck_port);
    SPI_SCK_GPIO_PIN(eSpiId,SpiConfig[eSpiId].Spi_sck_pin);

#if defined (STM32F40_41xxx)
    GetPinSource(SpiConfig[eSpiId].Spi_sck_pin,SpiConfig[eSpiId].Spi_sck_pinsource);
#endif
    
    SPI_MISO_GPIO_CLK(eSpiId,SpiConfig[eSpiId].Spi_miso_clk);
    SPI_MISO_GPIO_PORT(eSpiId,SpiConfig[eSpiId].Spi_miso_port);
    SPI_MISO_GPIO_PIN(eSpiId,SpiConfig[eSpiId].Spi_miso_pin);
#if defined (STM32F40_41xxx)
    GetPinSource(SpiConfig[eSpiId].Spi_miso_pin,SpiConfig[eSpiId].Spi_miso_pinsource);
#endif
    
    SPI_MOSI_GPIO_CLK(eSpiId,SpiConfig[eSpiId].Spi_mosi_clk);
    SPI_MOSI_GPIO_PORT(eSpiId,SpiConfig[eSpiId].Spi_mosi_port);
    SPI_MOSI_GPIO_PIN(eSpiId,SpiConfig[eSpiId].Spi_mosi_pin);
#if defined (STM32F40_41xxx)
    SPI_AF(eSpiId, SpiConfig[eSpiId].Spi_af);
    GetPinSource(SpiConfig[eSpiId].Spi_mosi_pin,SpiConfig[eSpiId].Spi_mosi_pinsource);
#endif
    
    SPI_BRP(eSpiId,SpiConfig[eSpiId].Spi_bpr);
    
}


//以下是SPI模块的初始化代码，配置成主机模式，访问SD Card/W25Q64/NRF24L01						  
//SPI口初始化
//这里针是对SPI2的初始化
void Fun_SPI_Init(UINT8 eSpiId)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef  SPI_InitStructure;
    
    ConfigSpiPara(eSpiId);
    
    if(SpiConfig[eSpiId].Ctrl_Spi == SPI1)
    {
        RCC_APB2PeriphClockCmd(	SpiConfig[eSpiId].Ctrl_Spi_Clk,  ENABLE );//SPI1时钟使能 	
    }
    else
        RCC_APB1PeriphClockCmd(	SpiConfig[eSpiId].Ctrl_Spi_Clk,  ENABLE );//SPI2时钟使能
#if defined (STM32F40_41xxx)
    RCC_AHB1PeriphClockCmd(	SpiConfig[eSpiId].Spi_sck_clk | SpiConfig[eSpiId].Spi_miso_clk | \
                            SpiConfig[eSpiId].Spi_mosi_clk, ENABLE );//PORTB时钟使能 
#else
    RCC_APB2PeriphClockCmd(	SpiConfig[eSpiId].Spi_sck_clk | SpiConfig[eSpiId].Spi_miso_clk | \
                            SpiConfig[eSpiId].Spi_mosi_clk, ENABLE );//PORTB时钟使能 
#endif
    
    GPIO_InitStructure.GPIO_Pin = SpiConfig[eSpiId].Spi_sck_pin;
#if defined (STM32F40_41xxx)
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    
#else
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PB13/14/15复用推挽输出 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
#endif
    GPIO_Init(SpiConfig[eSpiId].Spi_sck_port, &GPIO_InitStructure);//初始化GPIOB
    

    GPIO_InitStructure.GPIO_Pin = SpiConfig[eSpiId].Spi_miso_pin;
	GPIO_Init(SpiConfig[eSpiId].Spi_miso_port, &GPIO_InitStructure);//初始化GPIOB
   
    GPIO_InitStructure.GPIO_Pin = SpiConfig[eSpiId].Spi_mosi_pin;
	GPIO_Init(SpiConfig[eSpiId].Spi_mosi_port, &GPIO_InitStructure);//初始化GPIOB
    
#if defined (STM32F40_41xxx)
    GPIO_PinAFConfig(SpiConfig[eSpiId].Spi_sck_port,SpiConfig[eSpiId].Spi_sck_pinsource,SpiConfig[eSpiId].Spi_af);
    GPIO_PinAFConfig(SpiConfig[eSpiId].Spi_miso_port,SpiConfig[eSpiId].Spi_miso_pinsource,SpiConfig[eSpiId].Spi_af);
    GPIO_PinAFConfig(SpiConfig[eSpiId].Spi_mosi_port,SpiConfig[eSpiId].Spi_mosi_pinsource,SpiConfig[eSpiId].Spi_af);
#endif
//    GPIO_SetBits(SpiConfig[eSpiId].Spi_sck_port,SpiConfig[eSpiId].Spi_sck_pin);
//    GPIO_SetBits(SpiConfig[eSpiId].Spi_miso_port,SpiConfig[eSpiId].Spi_miso_pin);  //PB13/14/15上拉
//    GPIO_SetBits(SpiConfig[eSpiId].Spi_mosi_port,SpiConfig[eSpiId].Spi_mosi_pin);  //PB13/14/15上拉

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//串行同步时钟的空闲状态为高电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//串行同步时钟的第二个跳变沿（上升或下降）数据被采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SpiConfig[eSpiId].Spi_bpr;		//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SpiConfig[eSpiId].Ctrl_Spi, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
 
	SPI_Cmd(SpiConfig[eSpiId].Ctrl_Spi, ENABLE); //使能SPI外设
	
	SPI_ReadWriteByte(eSpiId,0xff);//启动传输		 
}   

//SPIx 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
UINT8 SPI_ReadWriteByte(UINT8 eSpiId,UINT8 TxData)
{
    while (SPI_I2S_GetFlagStatus(SpiConfig[eSpiId].Ctrl_Spi, SPI_I2S_FLAG_TXE) == RESET){}//等待发送区空  
	
	SPI_I2S_SendData(SpiConfig[eSpiId].Ctrl_Spi, TxData); //通过外设SPIx发送一个byte  数据
		
    while (SPI_I2S_GetFlagStatus(SpiConfig[eSpiId].Ctrl_Spi, SPI_I2S_FLAG_RXNE) == RESET){} //等待接收完一个byte  
 
	return SPI_I2S_ReceiveData(SpiConfig[eSpiId].Ctrl_Spi); //返回通过SPIx最近接收的数据	

}

#endif
