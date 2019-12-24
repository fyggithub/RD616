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


//������SPIģ��ĳ�ʼ�����룬���ó�����ģʽ������SD Card/W25Q64/NRF24L01						  
//SPI�ڳ�ʼ��
//�������Ƕ�SPI2�ĳ�ʼ��
void Fun_SPI_Init(UINT8 eSpiId)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef  SPI_InitStructure;
    
    ConfigSpiPara(eSpiId);
    
    if(SpiConfig[eSpiId].Ctrl_Spi == SPI1)
    {
        RCC_APB2PeriphClockCmd(	SpiConfig[eSpiId].Ctrl_Spi_Clk,  ENABLE );//SPI1ʱ��ʹ�� 	
    }
    else
        RCC_APB1PeriphClockCmd(	SpiConfig[eSpiId].Ctrl_Spi_Clk,  ENABLE );//SPI2ʱ��ʹ��
#if defined (STM32F40_41xxx)
    RCC_AHB1PeriphClockCmd(	SpiConfig[eSpiId].Spi_sck_clk | SpiConfig[eSpiId].Spi_miso_clk | \
                            SpiConfig[eSpiId].Spi_mosi_clk, ENABLE );//PORTBʱ��ʹ�� 
#else
    RCC_APB2PeriphClockCmd(	SpiConfig[eSpiId].Spi_sck_clk | SpiConfig[eSpiId].Spi_miso_clk | \
                            SpiConfig[eSpiId].Spi_mosi_clk, ENABLE );//PORTBʱ��ʹ�� 
#endif
    
    GPIO_InitStructure.GPIO_Pin = SpiConfig[eSpiId].Spi_sck_pin;
#if defined (STM32F40_41xxx)
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    
#else
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PB13/14/15����������� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
#endif
    GPIO_Init(SpiConfig[eSpiId].Spi_sck_port, &GPIO_InitStructure);//��ʼ��GPIOB
    

    GPIO_InitStructure.GPIO_Pin = SpiConfig[eSpiId].Spi_miso_pin;
	GPIO_Init(SpiConfig[eSpiId].Spi_miso_port, &GPIO_InitStructure);//��ʼ��GPIOB
   
    GPIO_InitStructure.GPIO_Pin = SpiConfig[eSpiId].Spi_mosi_pin;
	GPIO_Init(SpiConfig[eSpiId].Spi_mosi_port, &GPIO_InitStructure);//��ʼ��GPIOB
    
#if defined (STM32F40_41xxx)
    GPIO_PinAFConfig(SpiConfig[eSpiId].Spi_sck_port,SpiConfig[eSpiId].Spi_sck_pinsource,SpiConfig[eSpiId].Spi_af);
    GPIO_PinAFConfig(SpiConfig[eSpiId].Spi_miso_port,SpiConfig[eSpiId].Spi_miso_pinsource,SpiConfig[eSpiId].Spi_af);
    GPIO_PinAFConfig(SpiConfig[eSpiId].Spi_mosi_port,SpiConfig[eSpiId].Spi_mosi_pinsource,SpiConfig[eSpiId].Spi_af);
#endif
//    GPIO_SetBits(SpiConfig[eSpiId].Spi_sck_port,SpiConfig[eSpiId].Spi_sck_pin);
//    GPIO_SetBits(SpiConfig[eSpiId].Spi_miso_port,SpiConfig[eSpiId].Spi_miso_pin);  //PB13/14/15����
//    GPIO_SetBits(SpiConfig[eSpiId].Spi_mosi_port,SpiConfig[eSpiId].Spi_mosi_pin);  //PB13/14/15����

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SpiConfig[eSpiId].Spi_bpr;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
	SPI_Init(SpiConfig[eSpiId].Ctrl_Spi, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
 
	SPI_Cmd(SpiConfig[eSpiId].Ctrl_Spi, ENABLE); //ʹ��SPI����
	
	SPI_ReadWriteByte(eSpiId,0xff);//��������		 
}   

//SPIx ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
UINT8 SPI_ReadWriteByte(UINT8 eSpiId,UINT8 TxData)
{
    while (SPI_I2S_GetFlagStatus(SpiConfig[eSpiId].Ctrl_Spi, SPI_I2S_FLAG_TXE) == RESET){}//�ȴ���������  
	
	SPI_I2S_SendData(SpiConfig[eSpiId].Ctrl_Spi, TxData); //ͨ������SPIx����һ��byte  ����
		
    while (SPI_I2S_GetFlagStatus(SpiConfig[eSpiId].Ctrl_Spi, SPI_I2S_FLAG_RXNE) == RESET){} //�ȴ�������һ��byte  
 
	return SPI_I2S_ReceiveData(SpiConfig[eSpiId].Ctrl_Spi); //����ͨ��SPIx������յ�����	

}

#endif
