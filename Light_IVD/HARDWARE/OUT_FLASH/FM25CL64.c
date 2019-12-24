#ifndef __FM25CL64_C__
#define __FM25CL64_C__	

#include "My_includes.h"

#define FM25CL64_WREN_INST   0x06   //дʹ��
#define FM25CL64_WRDI_INST   0x04   //дʧ��
#define FM25CL64_RDSR_INST   0x05   //��״̬�Ĵ���
#define FM25CL64_WRSR_INST   0x01   //д״̬�Ĵ���
#define FM25CL64_WRITE_INST  0x02   //д����
#define FM25CL64_READ_INST   0x03   //������
#define FM25CL64_STATUS_REG  0x00   //״̬�Ĵ������ݡ����
#define FM25CL64_INIT_INST   0x09   //

static UINT8 Frame_Test(void)
{
    UINT16 i = 0;
    UINT8 buf[256] = {0};
    for(i = 0;i<256;i++)
    {
        buf[i] = i;
        printf("%c",buf[i]);
    }
    printf("\r\n");
    Write_FM25CL64_nByte(0x0000,buf,i);
    Sys_Delay_ms(100);
    Read_FM25CL64_nByte(0x0000,buf,i);
    for(i = 0;i<256;i++)
    {
        printf("%c",buf[i]);
    }
    printf("%d \r\n",i);
}
static UINT8 Check_Start(void);
//-------------------------------------
void FRAME_INIT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
#if defined (STM32F40_41xxx)
    RCC_AHB1PeriphClockCmd(	FRAME_CS_GPIO_CLK, ENABLE );//PORTBʱ��ʹ�� 
#else
    RCC_APB2PeriphClockCmd(	FRAME_CS_GPIO_CLK, ENABLE );//PORTBʱ��ʹ�� 
#endif
    // cs
    GPIO_InitStructure.GPIO_Pin = FRAME_CS_GPIO_PIN;  // PB12 ���� 
#if defined (STM32F40_41xxx)
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//���
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//����
#else
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
#endif
 	GPIO_Init(FRAME_CS_GPIO_PORT, &GPIO_InitStructure);
 	GPIO_SetBits(FRAME_CS_GPIO_PORT,FRAME_CS_GPIO_PIN);
    
    Fun_SPI_Init(FRAME_SPI);
//    i = Check_Start();
//    printf("Frame_State %d\r\n",i);
//    Frame_Test();
}



//1. ��״̬
static UINT8 ReadState(void)
{
    UINT8 r;
    FM25CL64_CS_LOW();
    r=SPI_ReadWriteByte(FRAME_SPI,FM25CL64_RDSR_INST);
    r=SPI_ReadWriteByte(FRAME_SPI,0);
    FM25CL64_CS_HIGH();
    return(r);
}

//2. ����״̬
static UINT8 Check_Start(void)
{
    UINT8 i=0,r=255;
    do 
    {
        r=ReadState();
        i++;
    } while ((r&0x01)&& i<254);
    if (i>=254)
        return 1;
  
    return 0;
}

//3. дʹ��
static void WriteEnable(void)
{
    FM25CL64_CS_LOW();
//    Sys_Delay_ms(1);
    SPI_ReadWriteByte(FRAME_SPI,FM25CL64_WREN_INST);
    FM25CL64_CS_HIGH();
}

//4. д״̬<----������ûʲô��
static void WriteState(void)
{
    WriteEnable();
    FM25CL64_CS_LOW();
    SPI_ReadWriteByte(FRAME_SPI,FM25CL64_WRSR_INST);
    SPI_ReadWriteByte(FRAME_SPI,FM25CL64_STATUS_REG);
    FM25CL64_CS_HIGH();
    Check_Start();
}

//5. ���洢�������ӳ���
UINT8 Read_FM25CL64_Byte(UINT16 addre)
{
    UINT8 dat;
    FM25CL64_CS_LOW();
//    Sys_Delay_ms(1);
    SPI_ReadWriteByte(FRAME_SPI,FM25CL64_READ_INST);
    SPI_ReadWriteByte(FRAME_SPI,(addre&0xFF00)>>8);
    dat=SPI_ReadWriteByte(FRAME_SPI,(addre&0x00FF));
    dat=SPI_ReadWriteByte(FRAME_SPI,0);
    FM25CL64_CS_HIGH();
    return(dat);
}

//6. ���洢�������ӳ���(���ֽ�)
UINT8 Read_FM25CL64_nByte(UINT16 addre,UINT8* buff,UINT16 len)
{
    UINT8 dat=0;
    UINT16 i = 0;
    FM25CL64_CS_LOW();
    SPI_ReadWriteByte(FRAME_SPI,FM25CL64_READ_INST);
    SPI_ReadWriteByte(FRAME_SPI,(addre&0xFF00)>>8);
    SPI_ReadWriteByte(FRAME_SPI,(addre&0x00FF));
    for (i=0;i<len;i++)
    {
        buff[i]=SPI_ReadWriteByte(FRAME_SPI,0);
    }
    FM25CL64_CS_HIGH();
    return(dat);
}
//7. д�洢�������ӳ���
UINT8 Write_FM25CL64_Byte(UINT16 addre,UINT8 dat)
{
    WriteEnable();
//    Sys_Delay_ms(1);
    FM25CL64_CS_LOW();
//    Sys_Delay_ms(1);
    SPI_ReadWriteByte(FRAME_SPI,FM25CL64_WRITE_INST);
    SPI_ReadWriteByte(FRAME_SPI,(addre&0xFF00)>>8);
    SPI_ReadWriteByte(FRAME_SPI,(addre&0x00FF));
    SPI_ReadWriteByte(FRAME_SPI,dat);
    FM25CL64_CS_HIGH();
    return(dat);
}


//8. д�洢�������ӳ���(���ֽ�)
void Write_FM25CL64_nByte(UINT16 addre,UINT8* buff,UINT16 len)
{
    UINT16 i = 0;
    WriteEnable();
    FM25CL64_CS_LOW();
    SPI_ReadWriteByte(FRAME_SPI,FM25CL64_WRITE_INST);
    SPI_ReadWriteByte(FRAME_SPI,(addre&0xFF00)>>8);
    SPI_ReadWriteByte(FRAME_SPI,(addre&0x00FF));
    for (i=0;i<len;i++)
    {
        SPI_ReadWriteByte(FRAME_SPI,buff[i]);
    }
    FM25CL64_CS_HIGH();
}
#endif
