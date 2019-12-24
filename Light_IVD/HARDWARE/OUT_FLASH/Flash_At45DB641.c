#include "My_includes.h"
 #if 0
//AT45DB������������Buffer1��Buffer2 (SRAM),�˴�ֻʹ��buffer1

 
#ifdef TEST_AT45DB
#define TESTBASE	0X2000
u32 TestAddr = TESTBASE;		
u8 TestBuffer[256] ={0};
#endif


//=========================================================================================
//*Status Register Format:                                   *
//*   -----------------------------------------------------------------------  
//* bit7 bit6 bit5 bit4 bit3 bit2 bit1 bit0 *
//* -------- -------- -------- -------- -------- -------- -------- -------- *
//* RDY/BUSY COMP   0     1     1     1     X     X   *
//*   ----------------------------------------------------------------------- * 
//* bit7 - æ���,0Ϊæ1Ϊ��æ��                               * 
//*       ��Status Register��λ0�Ƴ�֮��,��������ʱ���������н�ʹSPI��������*
//*       �����µ�״̬�ֽ��ͳ���                               */ 
//* bit6 - ������һ��Main Memory Page��Buffer�ıȽϽ��,0��ͬ,1��ͬ��   *
//* bit5                                               * 
//* bit4                                               * 
//* bit3                                               *
//* bit2 - ��4λ������������ܶ�,����AT45DB041B,��4λӦ����0111,һ���ܱ�� 
//*       16�ֲ�ͬ�ܶȵ�������                               
//* bit1                                               
//* bit0 - ��2λ��ʱ��Ч                                     
//=======================================================================================
_STA_ UINT8 At45DB_ReadStatus(void)
{ 
    UINT8 i = 0;       
	
    AT45DB_CS_LOW();   
    SPI_ReadWriteByte(AT45DB_SPI,Status_Register_Opcode);
	i = SPI_ReadWriteByte(AT45DB_SPI,0xFF);/////////////////****************//////////////////
    AT45DB_CS_HIGH();
	
    return i;   
}

static UINT8 At45DB641_Test(void)
{
    UINT16 i = 0;
    UINT8 buf[256] = {0};
    for(i = 0;i<256;i++)
    {
        buf[i] = i;
        printf("%c",buf[i]);
    }
    printf("\r\n");
    At45DB_WritePage_WithErase(1,0x0000,buf,i);
    Sys_Delay_ms(100);
    At45DB_Read(0x0000,buf,i);
    for(i = 0;i<256;i++)
    {
        printf("%c",buf[i]);
    }
    printf("%d \r\n",i);
}

void At45DB641_INIT(void)
{
    UINT16 i = 0;
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(	AT45DB_CS_GPIO_CLK | AT45DB_RESET_GPIO_CLK, ENABLE );//PORTBʱ��ʹ�� 
    
    // cs
    GPIO_InitStructure.GPIO_Pin = AT45DB_CS_GPIO_PIN;
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//���
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//����
    
 	GPIO_Init(AT45DB_CS_GPIO_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = AT45DB_RESET_GPIO_PIN;
    GPIO_Init(AT45DB_RESET_GPIO_PORT, &GPIO_InitStructure);
    
    AT45DB_CS_HIGH();
    AT45DB_RESET_LOW();
    Sys_Delay_ms(1);
    AT45DB_RESET_HIGH();
    
    Fun_SPI_Init(AT45DB_SPI);
    
    printf("read AT45DB id \r\n");
    i = At45DB_ReadStatus();
    
    At45DB_Configer_Binary_Page_Size();

    At45DB_ReadIDInformation();
    printf("qiuyu %d \r\n",13%10);
    
    At45DB641_Test();
    
//    i = Check_Start();
    
//    Frame_Test();
}

//======================================================================
//�������ƣ�DF_Check_Busy_State(void)
//���룺
//�����
//�������ܣ���ȡFLASHæ��־λ(����ж�255��,���л��Ƿ����ҷ���0)
//======================================================================
UINT8 At45DB_Check_Busy_State(void)
{
  	UINT8 state = 0;
  	UINT8 i = 255;
	
  	while(i)
  	{
       	state = At45DB_ReadStatus();
	   	if(state & 0x80) break;		//��ȡ�����λ0ʱ����æ
	   	--i;
		Sys_Delay_ms(1);
 	}
    printf("state 0x%x\r\n",state);
  	return i;
}

//************************************************************
//*��ȡFLASH��ҳ��С
//*����1��ʾÿһҳ�Ĵ�СΪ256 bytes,����Ϊ264 bytes
//************************************************************
UINT8 At45DB_Check_Page_Size(void)
{
  	UINT8 Page_Size = 0;
	
  	Page_Size = At45DB_ReadStatus();
	printf("page size 0x%x\r\n",Page_Size);
  	if(Page_Size & 0x01) return 1;
	
  	return 0;
}

//======================================================================
//�������ƣ�DF_Deep_Power_Down(void)
//���룺
//�����
//�������ܣ�ʹFLASH����DeepPowerDownģʽ
//======================================================================
void At45DB_Deep_Power_Down(void)
{
	AT45DB_CS_LOW();   
	SPI_ReadWriteByte(AT45DB_SPI,Deep_Power_Down);
	AT45DB_CS_HIGH();
}

//======================================================================
//�������ƣ�DFLASH_Resume_from_Deep_Power_Down(void)
//���룺
//�����
//�������ܣ���FLASH��DeepPowerDownģʽ����
//======================================================================
void At45DB_Resume_from_Deep_Power_Down(void)
{
	AT45DB_CS_LOW();   
	SPI_ReadWriteByte(AT45DB_SPI,Resume_from_Deep_Power_Down);
	AT45DB_CS_HIGH();
}

//************************************************************
//*����FLASHҳ��СΪ1024bytes
//************************************************************
void At45DB_Configer_Binary_Page_Size(void)
{
	u8 i = 0, j = 0;
	u8 Power_of_Two_Page_Size_Command[4]={0x3d,0x2a,0x80,0xa6};
	while(j < 5)
	{
		if(!At45DB_Check_Page_Size())
		{//���ҳ��СΪ1056 bytes��������Ϊ1024 bytes
			At45DB_Check_Busy_State();
	  		AT45DB_CS_LOW();
	  		for(i=0;i<4;i++)
	  		{
		  		SPI_ReadWriteByte(AT45DB_SPI,Power_of_Two_Page_Size_Command[i]);//
	  		}
	  		
	  		AT45DB_CS_HIGH();	
			Sys_Delay_ms(10);
			At45DB_Check_Busy_State();
			Sys_Delay_ms(5);
			At45DB_Deep_Power_Down();
			Sys_Delay_ms(5);
			At45DB_Resume_from_Deep_Power_Down();
			Sys_Delay_ms(1);
		}
		else
		{//�Ѿ�Ϊ1024����������
			break;
		}
		j ++;
	}
}

//=========================================================================================
//�������ƣ�
//���룺
//�����
//��������:��ȡ����ID��ȷ���Ƿ���DATAFLASH����,���ڷ���1,���򷵻�0
//==========================================================================================
UINT8 At45DB_ReadIDInformation(void)
{
	UINT8 i = 0;
	UINT8 Idbuff[4] = {0};
	
	At45DB_Check_Busy_State();
    AT45DB_CS_LOW();
    SPI_ReadWriteByte(AT45DB_SPI,Device_ID_Opcode); 
    for(i = 0 ; i < 4 ; i ++)
    {
        Idbuff[i] = SPI_ReadWriteByte(AT45DB_SPI,0xFF);
        printf("%x ",Idbuff[i]);
    }
    AT45DB_CS_HIGH();	
	if((Idbuff[0] == 0x1f) && (Idbuff[1] == 0x28))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

UINT8 AT45BD_Read_HeadBuffer[AT45BD_PageSize];
UINT8 AT45BD_Read_TailBuffer[AT45BD_PageSize];

/**********************************************
��������:��ָ����λ�ÿ�ʼ�����������ݣ�ֱ��CS�źŵ�������
�������룺unsigned int PageAddr:ҳ��ַ��0~4095��
          unsigned int ByteAddr:�ֽڵ�ַ��0~527��
          unsigned char *DataBuffer:���ڴ�Ŷ���������
          unsigned long ByteNum:�������������ֽ��� 
��    �أ�PASSED����ȡ�ɹ� FAILED����ȡʧ��
***********************************************/
BOOL At45DB_Read(UINT32 ReadAddr,UINT8 *DataBuffer,UINT32 ByteNum)
{
    unsigned long i;
    if(ByteNum<=2162688)//����ȡ��������Ƭ�� ,��Χ��4096*528 Bytes
    {
        if(!At45DB_Check_Busy_State())
            return FALSE;
        AT45DB_CS_LOW();
        
        SPI_ReadWriteByte(AT45DB_SPI,Continuous_Array_Read_Cmd);
        SPI_ReadWriteByte(AT45DB_SPI,((ReadAddr>>16)&0xFF));
        SPI_ReadWriteByte(AT45DB_SPI,((ReadAddr<<8)&0xFF));
        SPI_ReadWriteByte(AT45DB_SPI,(ReadAddr & 0xFF));
        SPI_ReadWriteByte(AT45DB_SPI,0x00); //ʹ������03Hʱ���������������õ����磬������ΪE8H����4������0BH����1�������������������ɶ������ĳ�ʼ��
    
        for(i=0;i<ByteNum;i++)
        {
            DataBuffer[i] = SPI_ReadWriteByte(AT45DB_SPI,0xFF);
        }
        AT45DB_CS_HIGH(); 
    
        return  TRUE;
  }
  return FALSE;
}

BOOL  At45DB_WritePage_WithErase(UINT8 BufferNumber,UINT32 WriteAddr,UINT8 *Data,UINT32 ByteNum)
{
    unsigned int i;
    if((ByteNum<=(AT45BD_PageSize - WriteAddr%AT45BD_PageSize))&&(ByteNum>0))
    {
        if(At45DB_Check_Busy_State())
        {
            AT45DB_CS_LOW();
            switch(BufferNumber)
            {
                case 1: SPI_ReadWriteByte(AT45DB_SPI,Main_Memory_Page_Pro_Through_Buff1);break;
                case 2: SPI_ReadWriteByte(AT45DB_SPI,Main_Memory_Page_Pro_Through_Buff2);break;
            }
            SPI_ReadWriteByte(AT45DB_SPI,((WriteAddr>>16)&0xFF));
            SPI_ReadWriteByte(AT45DB_SPI,((WriteAddr<<8)&0xFF));
            SPI_ReadWriteByte(AT45DB_SPI,(WriteAddr & 0xFF));
            for(i=0;i<ByteNum;i++)
            {
                SPI_ReadWriteByte(AT45DB_SPI,Data[i]);
            }
            AT45DB_CS_HIGH();
            return TRUE;
        }
      
    }
  return FALSE; 
}
#if 0
//=========================================================================================
//�������ƣ�SaveDataToFlash(u32 *pBuffer, u32 cnt, u32 addr)
//���룺pBuffer-���ݻ�������ַ,cnt-д���ֽ���,WriteAddr-�����ַ
//���������ɹ����
//��������:�������ֽ�(С��0X800000)���ݱ�����FLASH�����ַ
//==========================================================================================
UINT8 At45DB_Erase_Write(UINT32 WriteAddr, UINT8 *pBuffer, UINT32 sizlen)
{
	UINT32 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;
	UINT16 paddr = 0;
	UINT16 StarPage = 0;
    UINT16 Taillen = 0;
    UINT32 TailAddr = 0;
    
	if(WriteAddr + sizlen >= AT45BD_FullSize) 
	{//��� ������ַ
		return 0;
	}
    
    StarPage = WriteAddr/AT45BD_PageSize;       // ����ҳ
  	Addr = WriteAddr % AT45BD_PageSize;         // ҳ�Ŀ�ʼ��ַ
  	count = AT45BD_PageSize - Addr;             // ҳʣ���ַ
    TailAddr = WriteAddr + sizlen;          // ������ַ
    if(sizlen <= count) 
    {
        Taillen = count - sizlen;               // β��ʣ��
        
        count = sizlen;
    }
//  	NumOfPage =  sizlen / AT45BD_PageSize;      // �洢����ҳ
//  	NumOfSingle = sizlen % AT45BD_PageSize;     // ʣ�೤��
	/* If WriteAddr is DF_PageSize aligned  */
    while(1) 
    {
        // ���濪ͷ�ֽ�
        if(count)
            At45DB_Read(StarPage*AT45BD_PageSize,AT45BD_Read_HeadBuffer,Addr);
        // �����β����
        if(sizlen == count)
        {
            At45DB_Read(TailAddr,AT45BD_Read_TailBuffer,Taillen);
        }
        
        
    }
    
  	if(Addr == 0) 
  	{//��ַΪ��ҳ��ʼ��ַ
    /* If sizlen < DF_PageSize */
    	if(NumOfPage == 0) 
    	{//����һҳ
      		paddr = (u16)((WriteAddr >> 8) & 0xfffc);//����ҳ��ַ
			DF_MainMemoryPagetoBufferTransfer(paddr);
			DF_MainMemoryPageProgramThroughBuffer(WriteAddr,pBuffer,NumOfSingle);
			DF_AutoPageRewriteThroughBuffer(paddr);	
    	}
    /* If sizlen > DF_PageSize */
    	else  
    	{//����һҳ
      		while(NumOfPage--)
      		{//��д��ҳ
        		paddr = (u16)((WriteAddr >> 8) & 0xfffc);//����ҳ��ַ
				DF_MainMemoryPagetoBufferTransfer(paddr);
				DF_MainMemoryPageProgramThroughBuffer(WriteAddr,pBuffer,DF_PageSize);
				DF_AutoPageRewriteThroughBuffer(paddr);	
        		WriteAddr +=  DF_PageSize;
        		pBuffer += DF_PageSize;
      		}//��д���²���
      		if(NumOfSingle!=0)
      		{
        		paddr = (u16)((WriteAddr >> 8) & 0xfffc);//����ҳ��ַ
				DF_MainMemoryPagetoBufferTransfer(paddr);
				DF_MainMemoryPageProgramThroughBuffer(WriteAddr,pBuffer,NumOfSingle);
				DF_AutoPageRewriteThroughBuffer(paddr);	
      		}
    	}
  	}
  	/* If WriteAddr is not DF_PageSize aligned  */
  	else 
  	{//��ʼ��ַ����ҳ��ַ
    	/* If sizlen < DF_PageSize */	
    	if(NumOfPage== 0) 
    	{//������һҳ
			if(NumOfSingle > count)
			{//������ҳ
				paddr = (u16)((WriteAddr >> 8) & 0xfffc);//����ҳ��ַ
				DF_MainMemoryPagetoBufferTransfer(paddr);
				DF_MainMemoryPageProgramThroughBuffer(WriteAddr,pBuffer,count);
				DF_AutoPageRewriteThroughBuffer(paddr);	
				NumOfSingle -= count;
				WriteAddr += count;
				pBuffer += count;
			   	paddr = (u16)((WriteAddr >> 8) & 0xfffc);//����ҳ��ַ
				DF_MainMemoryPagetoBufferTransfer(paddr);
				DF_MainMemoryPageProgramThroughBuffer(WriteAddr,pBuffer,NumOfSingle);
				DF_AutoPageRewriteThroughBuffer(paddr);	
			}
			else
			{//��������ҳ
	      		paddr = (u16)((WriteAddr >> 8) & 0xfffc);//����ҳ��ַ
				DF_MainMemoryPagetoBufferTransfer(paddr);
				DF_MainMemoryPageProgramThroughBuffer(WriteAddr,pBuffer,NumOfSingle);
				DF_AutoPageRewriteThroughBuffer(paddr);	
			}
    	}
    	/* If sizlen > DF_PageSize */
    	else
    	{//����һҳ
      		sizlen -= count;
      		NumOfPage =  sizlen / DF_PageSize;
      		NumOfSingle = sizlen % DF_PageSize;	
			if(count != 0)
	      	{  
	        	paddr = (u16)((WriteAddr >> 8) & 0xfffc);//����ҳ��ַ
				DF_MainMemoryPagetoBufferTransfer(paddr);
				DF_MainMemoryPageProgramThroughBuffer(WriteAddr,pBuffer,count);
				DF_AutoPageRewriteThroughBuffer(paddr);	
	        	WriteAddr += count;
	        	pBuffer += count;
	      	} 
	      	while(NumOfPage--)
	      	{
	        	paddr = (u16)((WriteAddr >> 8) & 0xfffc);//����ҳ��ַ
				DF_MainMemoryPagetoBufferTransfer(paddr);
				DF_MainMemoryPageProgramThroughBuffer(WriteAddr,pBuffer,DF_PageSize);
				DF_AutoPageRewriteThroughBuffer(paddr);	
	        	WriteAddr +=  DF_PageSize;
	        	pBuffer += DF_PageSize;  
	      	}
	      	if(NumOfSingle != 0)
	      	{
	        	paddr = (u16)((WriteAddr >> 8) & 0xfffc);//����ҳ��ַ
				DF_MainMemoryPagetoBufferTransfer(paddr);
				DF_MainMemoryPageProgramThroughBuffer(WriteAddr,pBuffer,NumOfSingle);
				DF_AutoPageRewriteThroughBuffer(paddr);	
	      	}
      	}
  	}
	return 1;
}
#endif

/**********************************************
���������������ȴ��䵽�������ٴ��������е�ָ��ҳ�����ڲ��߳���
�������룺unsigned char BufferNumberѡ�񻺳�����1��2��
          unsigned char PageAddr������ָ��ҳ(0~4095)
          unsigned char ByteAddrҳ��ָ���ֽڵ�ַ(0~527��
          unsigned char *Data��д�������
          unsigned char ByteNumд�������ֽ�����1~528��

��    �أ�PASSED:�ɹ�     FAILED��ʧ��
***********************************************/
BOOL  DataToPage_ViaBuffer_WithErase_OneWay(unsigned char BufferNumber,
                                           unsigned int PageAddr,unsigned int ByteAddr,
                                           unsigned char *Data,unsigned int ByteNum)
{
    if((ByteNum<=(528-ByteAddr))&&(ByteNum>0))
    {
        Buffer_Write(BufferNumber,ByteAddr,Data,ByteNum);
        BufferToMMPage_WithErase(BufferNumber,PageAddr);
        return TRUE;
    } 
    return FALSE;
}


BOOL  DataToPage_ViaBuffer_WithErase_SecWay(unsigned char BufferNumber,unsigned int PageAddr,unsigned int ByteAddr,
                                                   unsigned char *Data,unsigned int ByteNum)
 {
   unsigned int i;
   if((ByteNum<=(528-ByteAddr))&&(ByteNum>0))
   {
      if(At45DB_Check_Busy_State())
      {
          AT45DB_CS_LOW();
          switch(BufferNumber)
          {
          case 1: SPI_ReadWriteByte(AT45DB_SPI,Main_Memory_Page_Pro_Through_Buff1);break;
          case 2: SPI_ReadWriteByte(AT45DB_SPI,Main_Memory_Page_Pro_Through_Buff2);break;
          }
          SPI_ReadWriteByte(AT45DB_SPI,(unsigned char)(PageAddr>>6));
          SPI_ReadWriteByte(AT45DB_SPI,(unsigned char)((PageAddr<<2)|(ByteAddr>>8)));
          SPI_ReadWriteByte(AT45DB_SPI,(unsigned char)ByteAddr);
          for(i=0;i<ByteNum;i++)
          {
             SPI_ReadWriteByte(AT45DB_SPI,Data[i]);
          }
          AT45DB_CS_HIGH();
          return TRUE;
      }
      
  }
  return FALSE; 
}


/**********************************************
����������������д�뵽��������
�������룺unsigned char BufferNumberѡ�񻺳�����������1��2��
          unsigned int ByteAddr��������д�����ݵ���ʼ��ַ(0~527)
          unsigned char *Data��д�������
          unsigned int ByteNumд������ֽ���(1~528)

��    �أ�PASSED����ȡ�ɹ� FAILED����ȡʧ��
***********************************************/
BOOL  Buffer_Write(unsigned char BufferNumber,unsigned int ByteAddr,
                          unsigned char *Data,unsigned int ByteNum)
{
    unsigned int i;
    if(((528-ByteAddr)>=ByteNum)&&(ByteNum>0))//д����������ڻ�����ʣ��ķ�Χ��
    {
        if(At45DB_Check_Busy_State())
        {
            AT45DB_CS_LOW();
            switch(BufferNumber)
            {
                case 1: SPI_ReadWriteByte(AT45DB_SPI,Buffer1_Write_Cmd);break;
                case 2: SPI_ReadWriteByte(AT45DB_SPI,Buffer2_Write_Cmd);break;
            }
            SPI_ReadWriteByte(AT45DB_SPI,0x00);
            SPI_ReadWriteByte(AT45DB_SPI,(unsigned char)(ByteAddr>>8));
            SPI_ReadWriteByte(AT45DB_SPI,(unsigned char)ByteAddr);
        
            for(i=0;i<ByteNum;i++)
            {
                SPI_ReadWriteByte(AT45DB_SPI,Data[i]);
            }
            AT45DB_CS_HIGH();
            return TRUE;
        }
    }
    return FALSE;
}

/**********************************************
������������Ԥ�߳��Ļ�����ȫ�����ݿ�����������ָ��ҳ(û�б��߳���
�������룺unsigned char BufferNumberѡ�񻺳�����1��2��
          unsigned char PageAddr������ҳ��ַ(0~4095)

��    �أ�PASSED:�ɹ�     FAILED��ʧ��
***********************************************/
BOOL  BufferToMMPage_WithErase(unsigned char BufferNumber,unsigned int PageAddr)
{
    if(PageAddr<4096)
    {
        if(At45DB_Check_Busy_State())
        {
            AT45DB_CS_LOW();
            switch(BufferNumber)
            {
                case 1: SPI_ReadWriteByte(AT45DB_SPI,Buffer1ToMMPage_WithErase_Cmd);break;
                case 2: SPI_ReadWriteByte(AT45DB_SPI,Buffer2ToMMPage_WithErase_Cmd);break;
            }
            SPI_ReadWriteByte(AT45DB_SPI,(unsigned char)(PageAddr>>6));
            SPI_ReadWriteByte(AT45DB_SPI,(unsigned char)(PageAddr<<2));
            SPI_ReadWriteByte(AT45DB_SPI,0x00);
            AT45DB_CS_HIGH();
            return TRUE;
        }
    }
    return FALSE; 
}

/**********************************************
��������:��ָ����λ�ÿ�ʼ�����������ݣ�ֱ��CS�źŵ�������
�������룺unsigned int PageAddr:ҳ��ַ��0~4095��
          unsigned int ByteAddr:�ֽڵ�ַ��0~527��
          unsigned char *DataBuffer:���ڴ�Ŷ���������
          unsigned long ByteNum:�������������ֽ��� 
��    �أ�PASSED����ȡ�ɹ� FAILED����ȡʧ��
***********************************************/
BOOL Continuous_Array_Read(unsigned int PageAddr,unsigned int ByteAddr,
                           unsigned char *DataBuffer,unsigned long ByteNum)
{
    unsigned long i;
    if(ByteNum<=2162688)//����ȡ��������Ƭ�� ,��Χ��4096*528 Bytes
    {
        if(!At45DB_Check_Busy_State())
            return FALSE;
        AT45DB_CS_LOW();
        SPI_ReadWriteByte(AT45DB_SPI,Continuous_Array_Read_Cmd);
        SPI_ReadWriteByte(AT45DB_SPI,(unsigned char)(PageAddr>>6));
        SPI_ReadWriteByte(AT45DB_SPI,(unsigned char)((PageAddr<<2)|(ByteAddr>>8)));
        SPI_ReadWriteByte(AT45DB_SPI,(unsigned char)ByteAddr);
        SPI_ReadWriteByte(AT45DB_SPI,0x00); //ʹ������03Hʱ���������������õ����磬������ΪE8H����4������0BH����1�������������������ɶ������ĳ�ʼ��
    
        for(i=0;i<ByteNum;i++)
        {
            DataBuffer[i] = SPI_ReadWriteByte(AT45DB_SPI,0xFF);
        }
        AT45DB_CS_HIGH(); 
    
        return  TRUE;
  }
  return FALSE;
}

#if 0
//======================================================================
//�������ƣ�DF_SpiRxByte(void)
//���룺
//�����
//�������ܣ�ͨ��SPI�Ӵ�AT45DB��һ�ֽ�����
//======================================================================
u8 DF_SpiRxByte(void)
{
	u8 i = 0, rdat = 0; 
	
	for(i = 0; i < 8; i ++)
	{
		rdat <<= 1;
		GPIO_SetBits(GPIOC, FSCK);
		if(GPIO_ReadInputDataBit(GPIOC, FSDO))
		{
			rdat |= 0x01;
		}
		GPIO_ResetBits(GPIOC, FSCK);
		
	}
	
	return rdat;
}
//======================================================================
//�������ƣ�DF_SpiTxByte(u8 dat) 
//���룺data��д�������
//�����
//�������ܣ�ͨ��SPI�ӿ���AT45DBд��һ�ֽ�����
//======================================================================
void DF_SpiTxByte(u8 dat) 
{ 
  	u8 i = 0, sdat = 0; 
	
	sdat = dat;
	for(i = 0; i < 8; i ++)
	{
		if(sdat & 0x80)
		{
			GPIO_SetBits(GPIOC, FSDI);
		}
		else
		{
			GPIO_ResetBits(GPIOC, FSDI);
		}
		GPIO_SetBits(GPIOC, FSCK);
		sdat <<= 1;
		GPIO_ResetBits(GPIOC, FSCK);
	}
}
//======================================================================
//�������ƣ�Enable_DFLASH(void)
//���룺
//�����
//�������ܣ�ʹ��FLASH(�͵�ƽʹ��)
//======================================================================
void Enable_DFLASH(void)
{
   	u8 i = 5;
 
  	GPIO_ResetBits(GPIOC, FCS);
  	while(i --);	
}
//======================================================================
//�������ƣ�Disable_DFLASH(void)
//���룺
//�����
//�������ܣ���ֹFLASH(�ߵ�ƽ��ֹ)
//======================================================================
void Disable_DFLASH(void)
{
  	u8 i = 5;
	
  	GPIO_SetBits(GPIOC, FCS);
  	while(i --);			//�ӳ���һ�θߵ�ƽʱ��
}
//=========================================================================================
//*Status Register Format:                                   *
//*   -----------------------------------------------------------------------  
//* bit7 bit6 bit5 bit4 bit3 bit2 bit1 bit0 *
//* -------- -------- -------- -------- -------- -------- -------- -------- *
//* RDY/BUSY COMP   0     1     1     1     X     X   *
//*   ----------------------------------------------------------------------- * 
//* bit7 - æ���,0Ϊæ1Ϊ��æ��                               * 
//*       ��Status Register��λ0�Ƴ�֮��,��������ʱ���������н�ʹSPI��������*
//*       �����µ�״̬�ֽ��ͳ���                               */ 
//* bit6 - ������һ��Main Memory Page��Buffer�ıȽϽ��,0��ͬ,1��ͬ��   *
//* bit5                                               * 
//* bit4                                               * 
//* bit3                                               *
//* bit2 - ��4λ������������ܶ�,����AT45DB041B,��4λӦ����0111,һ���ܱ�� 
//*       16�ֲ�ͬ�ܶȵ�������                               
//* bit1                                               
//* bit0 - ��2λ��ʱ��Ч                                     
//=======================================================================================
u8 DF_ReadStatus(void)
{ 
    u8 i = 0;       
	
    Enable_DFLASH();   
    DF_SpiTxByte(Status_Register_Opcode);
	i = DF_SpiRxByte();/////////////////****************//////////////////
    Disable_DFLASH();
	
    return i;   
}
//======================================================================
//�������ƣ�DF_Check_Busy_State(void)
//���룺
//�����
//�������ܣ���ȡFLASHæ��־λ(����ж�255��,���л��Ƿ����ҷ���0)
//======================================================================
u8 DF_Check_Busy_State(void)
{
  	u8 state = 0;
  	u8 i = 255;
	
  	while(i)
  	{
       	state = DF_ReadStatus();
	   	if(state & 0x80) break;		//��ȡ�����λ0ʱ����æ
	   	--i;
		DelayNInstruct(100);
 	}
  	return i;
}
//************************************************************
//*��ȡFLASH��ҳ��С
//*����1��ʾÿһҳ�Ĵ�СΪ1024 bytes,����Ϊ1056 bytes
//************************************************************
u8 DF_Check_Page_Size(void)
{
  	u8 Page_Size = 0;
	
  	Page_Size = DF_ReadStatus();
	
  	if(Page_Size & 0x01) return 1;
	
  	return 0;
}
//======================================================================
//�������ƣ�DF_Deep_Power_Down(void)
//���룺
//�����
//�������ܣ�ʹFLASH����DeepPowerDownģʽ
//======================================================================
void DF_Deep_Power_Down(void)
{
	Enable_DFLASH();   
	DF_SpiTxByte(Deep_Power_Down);
	Disable_DFLASH();
}
//======================================================================
//�������ƣ�DFLASH_Resume_from_Deep_Power_Down(void)
//���룺
//�����
//�������ܣ���FLASH��DeepPowerDownģʽ����
//======================================================================
void DF_Resume_from_Deep_Power_Down(void)
{
	Enable_DFLASH();   
	DF_SpiTxByte(Resume_from_Deep_Power_Down);
	Disable_DFLASH();
}
//************************************************************
//*����FLASHҳ��СΪ1024bytes
//************************************************************
void DF_Configer_Binary_Page_Size(void)
{
	u8 i = 0, j = 0;
	u8 Power_of_Two_Page_Size_Command[4]={0x3d,0x2a,0x80,0xa6};
	while(j < 5)
	{
		if(!DF_Check_Page_Size())
		{//���ҳ��СΪ1056 bytes��������Ϊ1024 bytes
			DF_Check_Busy_State();
	  		Enable_DFLASH();
	  		for(i=0;i<4;i++)
	  		{
		  		DF_SpiTxByte(Power_of_Two_Page_Size_Command[i]);//
	  		}
	  		
	  		Disable_DFLASH();	
			DelayNmsTime(10);
			DF_Check_Busy_State();
			DelayNmsTime(5);
			DF_Deep_Power_Down();
			DelayNmsTime(5);
			DF_Resume_from_Deep_Power_Down();
			DelayNmsTime(1);
		}
		else
		{//�Ѿ�Ϊ1024����������
			break;
		}
		j ++;
	}
}
//************************************************************
//*ʹ����������
//************************************************************
void DF_Enable_Sector_Protection(void)
{
  	u8 Enable_Sector_Protection_Command[4]={0x3D,0x2A,0x7F,0xA9}; //ʹ����������������
  	u8 i;
 
  	DF_Check_Busy_State();
  	Enable_DFLASH();
  	for(i=0;i<4;i++)
  	{
	  	DF_SpiTxByte(Enable_Sector_Protection_Command[i]);//дʹ����������������
  	}
  	Disable_DFLASH();
}
//======================================================================
//�������ƣ�
//���룺
//�����
//�������ܣ���ֹ��������
//======================================================================
void DF_Disable_Sector_Protection(void)
{
  	u8 Disable_Sector_Protection_Command[4]={0x3D,0x2A,0x7F,0x9A};//��ֹ��������������
  	u8 i;
 
  	DF_Check_Busy_State();
  	Enable_DFLASH();
  	for(i=0;i<4;i++)
  	{
      	DF_SpiTxByte(Disable_Sector_Protection_Command[i]);//д��ֹ��������������
  	}
  	Disable_DFLASH();
}
//======================================================================
//�������ƣ�
//���룺������������0---unprotected 0xff---protected,32byte refer to 32 sector
//�����
//�������ܣ�������������  ע��:��ı�BUFFER1�е�����
//======================================================================
void DF_Program_Sector_Protection_Register(u8 *Sector_Protection_Register)
{
  	u8 Program_Sector_Protection_Command[4]={0x3D,0x2A,0x7F,0xFC};//������������������
  	u8 i;
 
  	DF_Check_Busy_State();
  	Enable_DFLASH();
  	for(i=0;i<4;i++)
  	{
      	DF_SpiTxByte(Program_Sector_Protection_Command[i]);//д������������������
  	}
  	for(i=0;i<32;i++)
  	{
      	DF_SpiTxByte(Sector_Protection_Register[i]);//д����������������
  	}
  	Disable_DFLASH();
}
 
//======================================================================
//�������ƣ�
//���룺
//�����
//�������ܣ���ȡ���������Ĵ�������(����32���ֽ�,��Ӧ32�����������)
//======================================================================
void DF_Read_Sector_Protection_Register(u8 *Sector_Protection_Register)
{
  	u8 Read_Sector_Protection_Register_Command[4]={0x32,0,0,0};
  	u8 i;
	
  	DF_Check_Busy_State();
  	Enable_DFLASH();
 
  	for(i=0;i<4;i++)//write
  	{
    	DF_SpiTxByte(Read_Sector_Protection_Register_Command[i]);
  	}
  	for(i=0;i<32;i++)//read
  	{
    	Sector_Protection_Register[i] = DF_SpiRxByte();
  	}
  	Disable_DFLASH();
}
//************************************************************
//*ȡ��������������
//*����1��ʾ�ɹ�ȡ���������Ա���
//************************************************************
u8 DF_Cancel_Sector_Protection(void)
{
  	u8 Sector_Protection_Register_for_Write[32]={0,0,0,0,0,0,0,0};//д��0Ϊȥ����
  	u8 Sector_Protection_Register_for_Read[32]={1,1,1,1,1,1,1,1};//��ֹĬ��ֵΪ0
  	u16 i;
  	u8 j=1;
  //ʹ����������
  	DF_Enable_Sector_Protection();
  //������������
 
  	DF_Program_Sector_Protection_Register(Sector_Protection_Register_for_Write);
  //��ȡ���������Ĵ�������
  	DF_Read_Sector_Protection_Register(Sector_Protection_Register_for_Read);
  //�ж����������Ĵ�������
  	for(i=0;i<8;i++)
  	{
    	if(Sector_Protection_Register_for_Read[i] != 0) j++;
  	}
  //��ֹ��������
  	DF_Disable_Sector_Protection();
 
  	return j;
}
//=============================================================================
//�������ƣ�DF_ContinusArrayRead(UIN32 readaddr,u8 *readbuff,u16 len)
//���룺readaddrָ��������FLASH�еĴ洢��ַ��ָ�����ݵ��׵�ַ,ָ�����ݵĳ��� 
//�����readbufָ�������ݴ������׵�ַ
//�������ܣ���FLASH����readaddr��ַ��������ȡlen�ֽ�����
//=============================================================================
void DF_ContinusArrayRead(u32 readaddr,u8 *readbuff,u16 len)
{
    u16 i;
	
    DF_Check_Busy_State();
    Enable_DFLASH();
    DF_SpiTxByte(Continuous_Array_Read_Command); 
    DF_SpiTxByte((u8)((readaddr >> 16) & 0xff)); 
    DF_SpiTxByte((u8)((readaddr >> 8) & 0xff)); 
    DF_SpiTxByte((u8)(readaddr & 0xff)); 
 
    for(i = 0 ; i < 4 ; i ++)
    {
        DF_SpiTxByte(0x00);
    }
    for(i = 0 ; i < len ; i ++)
    {
        readbuff[i] = DF_SpiRxByte();
    }
    Disable_DFLASH();
}
//=============================================================================
//�������ƣ�DF_BufferWrite(u8 bufferaddr,u8 *writebuff,u16 len)
//���룺BUFFER�е���ʼ��ַ, �������ݵ�ͷָ��,�������ݵĳ���1~1024
//�����
//�������ܣ���ָ������д���ĳ����ַ��0~1023����ʼ��BUFFER1��
//=============================================================================
void DF_BufferWrite(u16 bufferaddr,u8 *writebuff,u16 len)
{
    u16 i;
	
    DF_Check_Busy_State();
    Enable_DFLASH();
    DF_SpiTxByte(Write_Data_to_Buffer1); 
    DF_SpiTxByte(0x00);
    DF_SpiTxByte((u8)((bufferaddr >> 8) & 0x03));
    DF_SpiTxByte((u8)(bufferaddr & 0xff)); 
    for(i = 0 ; i < len ; i ++ )
    {
         DF_SpiTxByte(writebuff[i]); 
    }
    Disable_DFLASH();   
}
//=============================================================================
//�������ƣ�DF_BufferToMainMemoryWithErase(u16 pageaddr,u8 buffaddr,u8 *writebuff,u16 len)
//���룺BUFFER�е���ʼ��ַ, �������ݵ�ͷָ��,�������ݵĳ���1~1024
//�����
//�������ܣ���ָ������д���ĳ����ַ��0~1023����ʼ��BUFFER1��:����2������,���Ƚ�
//ָ������д�뵽BUFFER 1��,���п���ָ��BUFFER�е���ʼд���ַ,��д�붯����Ӱ��
//BUFFER��������ַ�е�����,Ȼ���ٽ�BUFFER�е���������д�뵽ĳָ��ҳ��(��Ԥ����)��                                      
//=============================================================================
void DF_BufferToMainMemoryWithErase(u16 pageaddr,u16 buffaddr,u8 *writebuff,u16 len)
{
    DF_BufferWrite(buffaddr,writebuff,len);
	DF_Check_Busy_State();
    Enable_DFLASH();
    DF_SpiTxByte(Write_Buffer1_to_Page_whin_Erase); 
    DF_SpiTxByte((u8)((pageaddr >> 8) & 0x7f)); 
    DF_SpiTxByte((u8)(pageaddr & 0xfc)); 
    DF_SpiTxByte(0x00);
	DelayNmsTime(40);
    Disable_DFLASH();
}
//======================================================================
//�������ƣ�DF_MainMemoryPageProgramThroughBuffer(u32 writeaddr, u8 *writebuff, u16 len)
//����:riteaddr---��д���ַ,writebuff����ָ��,len���ݳ���
//�����
//��������:ͨ��BUF1д���棬��������:�Ƚ�1-1024�ֽ�����д��BUF1��Ȼ�����
//BUF1������Ĵ��������
//==================================================================
void DF_MainMemoryPageProgramThroughBuffer(u32 writeaddr, u8 *writebuff, u16 len)
{
	u16 i = 0;
	
	DF_Check_Busy_State();
	Enable_DFLASH();
	DF_SpiTxByte(Main_Memory_Page_Pro_Through_Buff1); 
    DF_SpiTxByte((u8)((writeaddr >> 16) & 0xff));
    DF_SpiTxByte((u8)((writeaddr >> 8) & 0xff));
    DF_SpiTxByte((u8)(writeaddr & 0xff)); 
    for(i = 0 ; i < len ; i ++ )
    {
         DF_SpiTxByte(writebuff[i]); 
    }
	DelayNmsTime(40);
	Disable_DFLASH();
 
}
//========================================================================================
//�������ƣ�DF_MainMemoryPagetoBufferTransfer(u16 pageaddr)
//���룺pageaddr
//�����
//�������ܣ�
//==========================================================================================
void DF_MainMemoryPagetoBufferTransfer(u16 pageaddr)
{
	DF_Check_Busy_State();
	Enable_DFLASH();
	DF_SpiTxByte(Main_MemoryPage_to_Buffer1_Transfer); 
    DF_SpiTxByte((u8)((pageaddr >> 8) & 0x7f));
    DF_SpiTxByte((u8)(pageaddr & 0xfc));
    DF_SpiTxByte(0x00); 
	Disable_DFLASH();	
}
//========================================================================================
//�������ƣ�
//���룺
//�����
//�������ܣ�
//==========================================================================================
void DF_AutoPageRewriteThroughBuffer(u16 pageaddr)
{
	DF_Check_Busy_State();
	Enable_DFLASH();
	DF_SpiTxByte(Auto_Page_Rewrite_through_Buffer1); 
    DF_SpiTxByte((u8)((pageaddr >> 8) & 0x7f));
    DF_SpiTxByte((u8)(pageaddr & 0xfc));
    DF_SpiTxByte(0x00); 
	Disable_DFLASH();
 
	DelayNmsTime(18);
}
//=========================================================================================
//�������ƣ�DF_Chip_Erase(void)
//���룺
//�����
//��������:��Ƭ����FLASHȫ������
//==========================================================================================
void DF_Chip_Erase(void)
{
  	 u8 Chip_Erase_Command[4] = {0xC7,0x94,0x80,0x9A};//��Ƭ����������
  	 u8 i;
  	 DF_Check_Busy_State();
  	 Enable_DFLASH();
  	 for(i = 0 ; i < 4 ; i ++)
  	 {
	     DF_SpiTxByte(Chip_Erase_Command[i]);
	 }
  	 Disable_DFLASH();
}
//=========================================================================================
//�������ƣ�
//���룺
//�����
//��������:��ȡ����ID��ȷ���Ƿ���DATAFLASH����,���ڷ���1,���򷵻�0
//==========================================================================================
u8 DF_ReadManufactureIDInformation(void)
{
	u8 i = 0;
	u8 Idbuff[4] = {0};
	
	DF_Check_Busy_State();
    Enable_DFLASH();
    DF_SpiTxByte(Device_ID_Opcode); 
    for(i = 0 ; i < 4 ; i ++)
    {
        Idbuff[i] = DF_SpiRxByte();
    }
    Disable_DFLASH();	
	if((Idbuff[0] == 0x1f) && (Idbuff[1] == 0x28))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
//=========================================================================================
//�������ƣ�
//���룺
//�����
//��������:��ʼ��DataFlash���������ID������Ϊ1024�ֽ�ÿpage����ֹ��������
//==========================================================================================
void DF_Init(void)
{
	u8 i = 0;
	GPIO_SetBits(GPIOC, FCS);
	GPIO_ResetBits(GPIOC, FSCK);//AT45DBѡ��SPI��ʽ0
	GPIO_ResetBits(GPIOC, FSDI);
 
	i = DF_Check_Busy_State();
	if(DF_ReadManufactureIDInformation())
	{
		DF_Configer_Binary_Page_Size();
		i = DF_Cancel_Sector_Protection();
	}
	else
	{//δ��⵽DF���������ʾ������Ϣ
		i = 0;
		while(i < 50)
		{
			i ++;
		}
		BeepTim = 100;
	}	
}
 
//=========================================================================================
//�������ƣ�SaveDataToFlash(u32 *pBuffer, u32 cnt, u32 addr)
//���룺pBuffer-���ݻ�������ַ,cnt-д���ֽ���,WriteAddr-�����ַ
//���������ɹ����
//��������:�������ֽ�(С��0X800000)���ݱ�����FLASH�����ַ
//==========================================================================================
u8 SaveDataToFlash(u8 *pBuffer, u32 sizlen, u32 WriteAddr)
{
	u32 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;
	u16 paddr = 0;
	
	if(WriteAddr + sizlen >= DF_FullSize) 
	{//��� ������ַ
		return 0;
	}
  	Addr = WriteAddr % DF_PageSize;
  	count = DF_PageSize - Addr;
  	NumOfPage =  sizlen / DF_PageSize;
  	NumOfSingle = sizlen % DF_PageSize;
	/* If WriteAddr is DF_PageSize aligned  */
  	if(Addr == 0) 
  	{//��ַΪ��ҳ��ʼ��ַ
    /* If sizlen < DF_PageSize */
    	if(NumOfPage == 0) 
    	{//����һҳ
      		paddr = (u16)((WriteAddr >> 8) & 0xfffc);//����ҳ��ַ
			DF_MainMemoryPagetoBufferTransfer(paddr);
			DF_MainMemoryPageProgramThroughBuffer(WriteAddr,pBuffer,NumOfSingle);
			DF_AutoPageRewriteThroughBuffer(paddr);	
    	}
    /* If sizlen > DF_PageSize */
    	else  
    	{//����һҳ
      		while(NumOfPage--)
      		{//��д��ҳ
        		paddr = (u16)((WriteAddr >> 8) & 0xfffc);//����ҳ��ַ
				DF_MainMemoryPagetoBufferTransfer(paddr);
				DF_MainMemoryPageProgramThroughBuffer(WriteAddr,pBuffer,DF_PageSize);
				DF_AutoPageRewriteThroughBuffer(paddr);	
        		WriteAddr +=  DF_PageSize;
        		pBuffer += DF_PageSize;
      		}//��д���²���
      		if(NumOfSingle!=0)
      		{
        		paddr = (u16)((WriteAddr >> 8) & 0xfffc);//����ҳ��ַ
				DF_MainMemoryPagetoBufferTransfer(paddr);
				DF_MainMemoryPageProgramThroughBuffer(WriteAddr,pBuffer,NumOfSingle);
				DF_AutoPageRewriteThroughBuffer(paddr);	
      		}
    	}
  	}
  	/* If WriteAddr is not DF_PageSize aligned  */
  	else 
  	{//��ʼ��ַ����ҳ��ַ
    	/* If sizlen < DF_PageSize */	
    	if(NumOfPage== 0) 
    	{//������һҳ
			if(NumOfSingle > count)
			{//������ҳ
				paddr = (u16)((WriteAddr >> 8) & 0xfffc);//����ҳ��ַ
				DF_MainMemoryPagetoBufferTransfer(paddr);
				DF_MainMemoryPageProgramThroughBuffer(WriteAddr,pBuffer,count);
				DF_AutoPageRewriteThroughBuffer(paddr);	
				NumOfSingle -= count;
				WriteAddr += count;
				pBuffer += count;
			   	paddr = (u16)((WriteAddr >> 8) & 0xfffc);//����ҳ��ַ
				DF_MainMemoryPagetoBufferTransfer(paddr);
				DF_MainMemoryPageProgramThroughBuffer(WriteAddr,pBuffer,NumOfSingle);
				DF_AutoPageRewriteThroughBuffer(paddr);	
			}
			else
			{//��������ҳ
	      		paddr = (u16)((WriteAddr >> 8) & 0xfffc);//����ҳ��ַ
				DF_MainMemoryPagetoBufferTransfer(paddr);
				DF_MainMemoryPageProgramThroughBuffer(WriteAddr,pBuffer,NumOfSingle);
				DF_AutoPageRewriteThroughBuffer(paddr);	
			}
    	}
    	/* If sizlen > DF_PageSize */
    	else
    	{//����һҳ
      		sizlen -= count;
      		NumOfPage =  sizlen / DF_PageSize;
      		NumOfSingle = sizlen % DF_PageSize;	
			if(count != 0)
	      	{  
	        	paddr = (u16)((WriteAddr >> 8) & 0xfffc);//����ҳ��ַ
				DF_MainMemoryPagetoBufferTransfer(paddr);
				DF_MainMemoryPageProgramThroughBuffer(WriteAddr,pBuffer,count);
				DF_AutoPageRewriteThroughBuffer(paddr);	
	        	WriteAddr += count;
	        	pBuffer += count;
	      	} 
	      	while(NumOfPage--)
	      	{
	        	paddr = (u16)((WriteAddr >> 8) & 0xfffc);//����ҳ��ַ
				DF_MainMemoryPagetoBufferTransfer(paddr);
				DF_MainMemoryPageProgramThroughBuffer(WriteAddr,pBuffer,DF_PageSize);
				DF_AutoPageRewriteThroughBuffer(paddr);	
	        	WriteAddr +=  DF_PageSize;
	        	pBuffer += DF_PageSize;  
	      	}
	      	if(NumOfSingle != 0)
	      	{
	        	paddr = (u16)((WriteAddr >> 8) & 0xfffc);//����ҳ��ַ
				DF_MainMemoryPagetoBufferTransfer(paddr);
				DF_MainMemoryPageProgramThroughBuffer(WriteAddr,pBuffer,NumOfSingle);
				DF_AutoPageRewriteThroughBuffer(paddr);	
	      	}
      	}
  	}
	return 1;
}
//=========================================================================================
//�������ƣ�GetFlashData(u32 readdr, u8 *pBuffer, u16 sizlen)
//���룺readdr-��ȡ��ַ,pBuffer-��ȡ���ݻ����ַ,sizlen-�����ֽ���
//�����
//�������ܣ���FLASH����(0-0X7FFFFF)��ַ��ȡ�����ֽ�(0-65535)����
//==========================================================================================
u8 GetFlashData(u32 readdr, u8 *pBuffer, u16 sizlen)
{
	if(readdr + sizlen >= DF_FullSize) 
	{//��� ������ַ
		return 0;
	}
	DF_ContinusArrayRead(readdr,pBuffer,sizlen);
 
	return 1;
}
//=========================================================================================
//�������ƣ�
//���룺
//�����
//��������:���Զ�дDATAFLASH
//==========================================================================================
void Test_At45db642d(void)
{
#ifdef TEST_AT45DB
 
	u16 paddr = 0;
	u8 i = 0;
	
   	if(TestAddr < TESTBASE + 0x2000)
	{
		for(i = 0; i < 32; i ++)
		{
			TestBuffer[i] = i;
		}
		paddr = (u16)((TestAddr >> 8) & 0xfffc);
	
		DF_MainMemoryPagetoBufferTransfer(paddr);
		DF_MainMemoryPageProgramThroughBuffer(TestAddr,TestBuffer,32);
 
		DF_AutoPageRewriteThroughBuffer(paddr);
	   	for(i = 0; i < 250; i ++)
		{
			TestBuffer[i] = 0;
		}
		DF_ContinusArrayRead(TestAddr-32,TestBuffer,96);
		TestAddr += 32;
	}
#endif
}

#endif
#endif
