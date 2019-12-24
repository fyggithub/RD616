#include "My_includes.h"
 #if 0
//AT45DB有两个缓冲区Buffer1和Buffer2 (SRAM),此处只使用buffer1

 
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
//* bit7 - 忙标记,0为忙1为不忙。                               * 
//*       当Status Register的位0移出之后,接下来的时钟脉冲序列将使SPI器件继续*
//*       将最新的状态字节送出。                               */ 
//* bit6 - 标记最近一次Main Memory Page和Buffer的比较结果,0相同,1不同。   *
//* bit5                                               * 
//* bit4                                               * 
//* bit3                                               *
//* bit2 - 这4位用来标记器件密度,对于AT45DB041B,这4位应该是0111,一共能标记 
//*       16种不同密度的器件。                               
//* bit1                                               
//* bit0 - 这2位暂时无效                                     
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
    RCC_AHB1PeriphClockCmd(	AT45DB_CS_GPIO_CLK | AT45DB_RESET_GPIO_CLK, ENABLE );//PORTB时钟使能 
    
    // cs
    GPIO_InitStructure.GPIO_Pin = AT45DB_CS_GPIO_PIN;
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//输出
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//上拉
    
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
//函数名称：DF_Check_Busy_State(void)
//输入：
//输出：
//函数功能：读取FLASH忙标志位(最多判断255次,不行还是返回且返回0)
//======================================================================
UINT8 At45DB_Check_Busy_State(void)
{
  	UINT8 state = 0;
  	UINT8 i = 255;
	
  	while(i)
  	{
       	state = At45DB_ReadStatus();
	   	if(state & 0x80) break;		//读取的最高位0时器件忙
	   	--i;
		Sys_Delay_ms(1);
 	}
    printf("state 0x%x\r\n",state);
  	return i;
}

//************************************************************
//*读取FLASH的页大小
//*返回1表示每一页的大小为256 bytes,否则为264 bytes
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
//函数名称：DF_Deep_Power_Down(void)
//输入：
//输出：
//函数功能：使FLASH进入DeepPowerDown模式
//======================================================================
void At45DB_Deep_Power_Down(void)
{
	AT45DB_CS_LOW();   
	SPI_ReadWriteByte(AT45DB_SPI,Deep_Power_Down);
	AT45DB_CS_HIGH();
}

//======================================================================
//函数名称：DFLASH_Resume_from_Deep_Power_Down(void)
//输入：
//输出：
//函数功能：将FLASH从DeepPowerDown模式唤醒
//======================================================================
void At45DB_Resume_from_Deep_Power_Down(void)
{
	AT45DB_CS_LOW();   
	SPI_ReadWriteByte(AT45DB_SPI,Resume_from_Deep_Power_Down);
	AT45DB_CS_HIGH();
}

//************************************************************
//*配置FLASH页大小为1024bytes
//************************************************************
void At45DB_Configer_Binary_Page_Size(void)
{
	u8 i = 0, j = 0;
	u8 Power_of_Two_Page_Size_Command[4]={0x3d,0x2a,0x80,0xa6};
	while(j < 5)
	{
		if(!At45DB_Check_Page_Size())
		{//如果页大小为1056 bytes，则配置为1024 bytes
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
		{//已经为1024则不用再配置
			break;
		}
		j ++;
	}
}

//=========================================================================================
//函数名称：
//输入：
//输出：
//函数功能:读取制造ID，确认是否有DATAFLASH存在,存在返回1,否则返回0
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
功能描述:从指定的位置开始连续读出数据，直到CS信号的上升沿
参数输入：unsigned int PageAddr:页地址（0~4095）
          unsigned int ByteAddr:字节地址（0~527）
          unsigned char *DataBuffer:用于存放读出的数据
          unsigned long ByteNum:欲读出的数据字节数 
返    回：PASSED：读取成功 FAILED：读取失败
***********************************************/
BOOL At45DB_Read(UINT32 ReadAddr,UINT8 *DataBuffer,UINT32 ByteNum)
{
    unsigned long i;
    if(ByteNum<=2162688)//欲读取的数据在片内 ,范围：4096*528 Bytes
    {
        if(!At45DB_Check_Busy_State())
            return FALSE;
        AT45DB_CS_LOW();
        
        SPI_ReadWriteByte(AT45DB_SPI,Continuous_Array_Read_Cmd);
        SPI_ReadWriteByte(AT45DB_SPI,((ReadAddr>>16)&0xFF));
        SPI_ReadWriteByte(AT45DB_SPI,((ReadAddr<<8)&0xFF));
        SPI_ReadWriteByte(AT45DB_SPI,(ReadAddr & 0xFF));
        SPI_ReadWriteByte(AT45DB_SPI,0x00); //使用命令03H时，不用再输入无用的数剧，但命令为E8H（加4个）或0BH（加1个）需加无用数据以完成读操作的初始化
    
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
//函数名称：SaveDataToFlash(u32 *pBuffer, u32 cnt, u32 addr)
//输入：pBuffer-数据缓冲区首址,cnt-写入字节数,WriteAddr-保存地址
//输出：保存成功与否
//函数功能:将任意字节(小于0X800000)数据保存入FLASH任意地址
//==========================================================================================
UINT8 At45DB_Erase_Write(UINT32 WriteAddr, UINT8 *pBuffer, UINT32 sizlen)
{
	UINT32 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;
	UINT16 paddr = 0;
	UINT16 StarPage = 0;
    UINT16 Taillen = 0;
    UINT32 TailAddr = 0;
    
	if(WriteAddr + sizlen >= AT45BD_FullSize) 
	{//如果 超出地址
		return 0;
	}
    
    StarPage = WriteAddr/AT45BD_PageSize;       // 启动页
  	Addr = WriteAddr % AT45BD_PageSize;         // 页的开始地址
  	count = AT45BD_PageSize - Addr;             // 页剩余地址
    TailAddr = WriteAddr + sizlen;          // 结束地址
    if(sizlen <= count) 
    {
        Taillen = count - sizlen;               // 尾部剩余
        
        count = sizlen;
    }
//  	NumOfPage =  sizlen / AT45BD_PageSize;      // 存储多少页
//  	NumOfSingle = sizlen % AT45BD_PageSize;     // 剩余长度
	/* If WriteAddr is DF_PageSize aligned  */
    while(1) 
    {
        // 保存开头字节
        if(count)
            At45DB_Read(StarPage*AT45BD_PageSize,AT45BD_Read_HeadBuffer,Addr);
        // 保存结尾数据
        if(sizlen == count)
        {
            At45DB_Read(TailAddr,AT45BD_Read_TailBuffer,Taillen);
        }
        
        
    }
    
  	if(Addr == 0) 
  	{//地址为整页起始地址
    /* If sizlen < DF_PageSize */
    	if(NumOfPage == 0) 
    	{//不够一页
      		paddr = (u16)((WriteAddr >> 8) & 0xfffc);//查找页地址
			DF_MainMemoryPagetoBufferTransfer(paddr);
			DF_MainMemoryPageProgramThroughBuffer(WriteAddr,pBuffer,NumOfSingle);
			DF_AutoPageRewriteThroughBuffer(paddr);	
    	}
    /* If sizlen > DF_PageSize */
    	else  
    	{//超出一页
      		while(NumOfPage--)
      		{//先写整页
        		paddr = (u16)((WriteAddr >> 8) & 0xfffc);//查找页地址
				DF_MainMemoryPagetoBufferTransfer(paddr);
				DF_MainMemoryPageProgramThroughBuffer(WriteAddr,pBuffer,DF_PageSize);
				DF_AutoPageRewriteThroughBuffer(paddr);	
        		WriteAddr +=  DF_PageSize;
        		pBuffer += DF_PageSize;
      		}//再写余下部分
      		if(NumOfSingle!=0)
      		{
        		paddr = (u16)((WriteAddr >> 8) & 0xfffc);//查找页地址
				DF_MainMemoryPagetoBufferTransfer(paddr);
				DF_MainMemoryPageProgramThroughBuffer(WriteAddr,pBuffer,NumOfSingle);
				DF_AutoPageRewriteThroughBuffer(paddr);	
      		}
    	}
  	}
  	/* If WriteAddr is not DF_PageSize aligned  */
  	else 
  	{//起始地址非整页地址
    	/* If sizlen < DF_PageSize */	
    	if(NumOfPage== 0) 
    	{//不超过一页
			if(NumOfSingle > count)
			{//产生跨页
				paddr = (u16)((WriteAddr >> 8) & 0xfffc);//查找页地址
				DF_MainMemoryPagetoBufferTransfer(paddr);
				DF_MainMemoryPageProgramThroughBuffer(WriteAddr,pBuffer,count);
				DF_AutoPageRewriteThroughBuffer(paddr);	
				NumOfSingle -= count;
				WriteAddr += count;
				pBuffer += count;
			   	paddr = (u16)((WriteAddr >> 8) & 0xfffc);//查找页地址
				DF_MainMemoryPagetoBufferTransfer(paddr);
				DF_MainMemoryPageProgramThroughBuffer(WriteAddr,pBuffer,NumOfSingle);
				DF_AutoPageRewriteThroughBuffer(paddr);	
			}
			else
			{//不产生跨页
	      		paddr = (u16)((WriteAddr >> 8) & 0xfffc);//查找页地址
				DF_MainMemoryPagetoBufferTransfer(paddr);
				DF_MainMemoryPageProgramThroughBuffer(WriteAddr,pBuffer,NumOfSingle);
				DF_AutoPageRewriteThroughBuffer(paddr);	
			}
    	}
    	/* If sizlen > DF_PageSize */
    	else
    	{//超过一页
      		sizlen -= count;
      		NumOfPage =  sizlen / DF_PageSize;
      		NumOfSingle = sizlen % DF_PageSize;	
			if(count != 0)
	      	{  
	        	paddr = (u16)((WriteAddr >> 8) & 0xfffc);//查找页地址
				DF_MainMemoryPagetoBufferTransfer(paddr);
				DF_MainMemoryPageProgramThroughBuffer(WriteAddr,pBuffer,count);
				DF_AutoPageRewriteThroughBuffer(paddr);	
	        	WriteAddr += count;
	        	pBuffer += count;
	      	} 
	      	while(NumOfPage--)
	      	{
	        	paddr = (u16)((WriteAddr >> 8) & 0xfffc);//查找页地址
				DF_MainMemoryPagetoBufferTransfer(paddr);
				DF_MainMemoryPageProgramThroughBuffer(WriteAddr,pBuffer,DF_PageSize);
				DF_AutoPageRewriteThroughBuffer(paddr);	
	        	WriteAddr +=  DF_PageSize;
	        	pBuffer += DF_PageSize;  
	      	}
	      	if(NumOfSingle != 0)
	      	{
	        	paddr = (u16)((WriteAddr >> 8) & 0xfffc);//查找页地址
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
功能描述：数据先传输到缓冲器再传到主存中的指定页（带内部檫除）
参数输入：unsigned char BufferNumber选择缓冲器（1或2）
          unsigned char PageAddr主存中指定页(0~4095)
          unsigned char ByteAddr页中指定字节地址(0~527）
          unsigned char *Data欲写入的数据
          unsigned char ByteNum写入数据字节数（1~528）

返    回：PASSED:成功     FAILED：失败
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
功能描述：将数据写入到缓冲器中
参数输入：unsigned char BufferNumber选择缓冲器缓冲器（1或2）
          unsigned int ByteAddr缓冲器中写入数据的起始地址(0~527)
          unsigned char *Data需写入的数据
          unsigned int ByteNum写入的总字节数(1~528)

返    回：PASSED：读取成功 FAILED：读取失败
***********************************************/
BOOL  Buffer_Write(unsigned char BufferNumber,unsigned int ByteAddr,
                          unsigned char *Data,unsigned int ByteNum)
{
    unsigned int i;
    if(((528-ByteAddr)>=ByteNum)&&(ByteNum>0))//写入的数据量在缓冲器剩余的范围内
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
功能描述：带预檫除的缓冲器全部数据拷贝到主存中指定页(没有被檫除）
参数输入：unsigned char BufferNumber选择缓冲器（1或2）
          unsigned char PageAddr主存中页地址(0~4095)

返    回：PASSED:成功     FAILED：失败
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
功能描述:从指定的位置开始连续读出数据，直到CS信号的上升沿
参数输入：unsigned int PageAddr:页地址（0~4095）
          unsigned int ByteAddr:字节地址（0~527）
          unsigned char *DataBuffer:用于存放读出的数据
          unsigned long ByteNum:欲读出的数据字节数 
返    回：PASSED：读取成功 FAILED：读取失败
***********************************************/
BOOL Continuous_Array_Read(unsigned int PageAddr,unsigned int ByteAddr,
                           unsigned char *DataBuffer,unsigned long ByteNum)
{
    unsigned long i;
    if(ByteNum<=2162688)//欲读取的数据在片内 ,范围：4096*528 Bytes
    {
        if(!At45DB_Check_Busy_State())
            return FALSE;
        AT45DB_CS_LOW();
        SPI_ReadWriteByte(AT45DB_SPI,Continuous_Array_Read_Cmd);
        SPI_ReadWriteByte(AT45DB_SPI,(unsigned char)(PageAddr>>6));
        SPI_ReadWriteByte(AT45DB_SPI,(unsigned char)((PageAddr<<2)|(ByteAddr>>8)));
        SPI_ReadWriteByte(AT45DB_SPI,(unsigned char)ByteAddr);
        SPI_ReadWriteByte(AT45DB_SPI,0x00); //使用命令03H时，不用再输入无用的数剧，但命令为E8H（加4个）或0BH（加1个）需加无用数据以完成读操作的初始化
    
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
//函数名称：DF_SpiRxByte(void)
//输入：
//输出：
//函数功能：通过SPI接从AT45DB读一字节数据
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
//函数名称：DF_SpiTxByte(u8 dat) 
//输入：data待写入的数据
//输出：
//函数功能：通过SPI接口向AT45DB写入一字节数据
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
//函数名称：Enable_DFLASH(void)
//输入：
//输出：
//函数功能：使能FLASH(低电平使能)
//======================================================================
void Enable_DFLASH(void)
{
   	u8 i = 5;
 
  	GPIO_ResetBits(GPIOC, FCS);
  	while(i --);	
}
//======================================================================
//函数名称：Disable_DFLASH(void)
//输入：
//输出：
//函数功能：禁止FLASH(高电平禁止)
//======================================================================
void Disable_DFLASH(void)
{
  	u8 i = 5;
	
  	GPIO_SetBits(GPIOC, FCS);
  	while(i --);			//延长上一次高电平时间
}
//=========================================================================================
//*Status Register Format:                                   *
//*   -----------------------------------------------------------------------  
//* bit7 bit6 bit5 bit4 bit3 bit2 bit1 bit0 *
//* -------- -------- -------- -------- -------- -------- -------- -------- *
//* RDY/BUSY COMP   0     1     1     1     X     X   *
//*   ----------------------------------------------------------------------- * 
//* bit7 - 忙标记,0为忙1为不忙。                               * 
//*       当Status Register的位0移出之后,接下来的时钟脉冲序列将使SPI器件继续*
//*       将最新的状态字节送出。                               */ 
//* bit6 - 标记最近一次Main Memory Page和Buffer的比较结果,0相同,1不同。   *
//* bit5                                               * 
//* bit4                                               * 
//* bit3                                               *
//* bit2 - 这4位用来标记器件密度,对于AT45DB041B,这4位应该是0111,一共能标记 
//*       16种不同密度的器件。                               
//* bit1                                               
//* bit0 - 这2位暂时无效                                     
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
//函数名称：DF_Check_Busy_State(void)
//输入：
//输出：
//函数功能：读取FLASH忙标志位(最多判断255次,不行还是返回且返回0)
//======================================================================
u8 DF_Check_Busy_State(void)
{
  	u8 state = 0;
  	u8 i = 255;
	
  	while(i)
  	{
       	state = DF_ReadStatus();
	   	if(state & 0x80) break;		//读取的最高位0时器件忙
	   	--i;
		DelayNInstruct(100);
 	}
  	return i;
}
//************************************************************
//*读取FLASH的页大小
//*返回1表示每一页的大小为1024 bytes,否则为1056 bytes
//************************************************************
u8 DF_Check_Page_Size(void)
{
  	u8 Page_Size = 0;
	
  	Page_Size = DF_ReadStatus();
	
  	if(Page_Size & 0x01) return 1;
	
  	return 0;
}
//======================================================================
//函数名称：DF_Deep_Power_Down(void)
//输入：
//输出：
//函数功能：使FLASH进入DeepPowerDown模式
//======================================================================
void DF_Deep_Power_Down(void)
{
	Enable_DFLASH();   
	DF_SpiTxByte(Deep_Power_Down);
	Disable_DFLASH();
}
//======================================================================
//函数名称：DFLASH_Resume_from_Deep_Power_Down(void)
//输入：
//输出：
//函数功能：将FLASH从DeepPowerDown模式唤醒
//======================================================================
void DF_Resume_from_Deep_Power_Down(void)
{
	Enable_DFLASH();   
	DF_SpiTxByte(Resume_from_Deep_Power_Down);
	Disable_DFLASH();
}
//************************************************************
//*配置FLASH页大小为1024bytes
//************************************************************
void DF_Configer_Binary_Page_Size(void)
{
	u8 i = 0, j = 0;
	u8 Power_of_Two_Page_Size_Command[4]={0x3d,0x2a,0x80,0xa6};
	while(j < 5)
	{
		if(!DF_Check_Page_Size())
		{//如果页大小为1056 bytes，则配置为1024 bytes
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
		{//已经为1024则不用再配置
			break;
		}
		j ++;
	}
}
//************************************************************
//*使能扇区保护
//************************************************************
void DF_Enable_Sector_Protection(void)
{
  	u8 Enable_Sector_Protection_Command[4]={0x3D,0x2A,0x7F,0xA9}; //使能扇区保护操作码
  	u8 i;
 
  	DF_Check_Busy_State();
  	Enable_DFLASH();
  	for(i=0;i<4;i++)
  	{
	  	DF_SpiTxByte(Enable_Sector_Protection_Command[i]);//写使能扇区保护操作码
  	}
  	Disable_DFLASH();
}
//======================================================================
//函数名称：
//输入：
//输出：
//函数功能：禁止扇区保护
//======================================================================
void DF_Disable_Sector_Protection(void)
{
  	u8 Disable_Sector_Protection_Command[4]={0x3D,0x2A,0x7F,0x9A};//禁止扇区保护操作码
  	u8 i;
 
  	DF_Check_Busy_State();
  	Enable_DFLASH();
  	for(i=0;i<4;i++)
  	{
      	DF_SpiTxByte(Disable_Sector_Protection_Command[i]);//写禁止扇区保护操作码
  	}
  	Disable_DFLASH();
}
//======================================================================
//函数名称：
//输入：各扇区保护字0---unprotected 0xff---protected,32byte refer to 32 sector
//输出：
//函数功能：设置扇区保护  注意:会改变BUFFER1中的内容
//======================================================================
void DF_Program_Sector_Protection_Register(u8 *Sector_Protection_Register)
{
  	u8 Program_Sector_Protection_Command[4]={0x3D,0x2A,0x7F,0xFC};//设置扇区保护操作码
  	u8 i;
 
  	DF_Check_Busy_State();
  	Enable_DFLASH();
  	for(i=0;i<4;i++)
  	{
      	DF_SpiTxByte(Program_Sector_Protection_Command[i]);//写设置扇区保护操作码
  	}
  	for(i=0;i<32;i++)
  	{
      	DF_SpiTxByte(Sector_Protection_Register[i]);//写设置扇区保护数据
  	}
  	Disable_DFLASH();
}
 
//======================================================================
//函数名称：
//输入：
//输出：
//函数功能：读取扇区保护寄存器内容(返回32个字节,对应32个扇区的情况)
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
//*取消所有扇区保护
//*返回1表示成功取消扇区所以保护
//************************************************************
u8 DF_Cancel_Sector_Protection(void)
{
  	u8 Sector_Protection_Register_for_Write[32]={0,0,0,0,0,0,0,0};//写入0为去保护
  	u8 Sector_Protection_Register_for_Read[32]={1,1,1,1,1,1,1,1};//防止默认值为0
  	u16 i;
  	u8 j=1;
  //使能扇区保护
  	DF_Enable_Sector_Protection();
  //设置扇区保护
 
  	DF_Program_Sector_Protection_Register(Sector_Protection_Register_for_Write);
  //读取扇区保护寄存器内容
  	DF_Read_Sector_Protection_Register(Sector_Protection_Register_for_Read);
  //判断扇区保护寄存器内容
  	for(i=0;i<8;i++)
  	{
    	if(Sector_Protection_Register_for_Read[i] != 0) j++;
  	}
  //禁止扇区保护
  	DF_Disable_Sector_Protection();
 
  	return j;
}
//=============================================================================
//函数名称：DF_ContinusArrayRead(UIN32 readaddr,u8 *readbuff,u16 len)
//输入：readaddr指定数据在FLASH中的存储地址，指定数据的首地址,指定数据的长度 
//输出：readbuf指定数据暂存区的首地址
//函数功能：从FLASH主存readaddr地址处连续读取len字节数据
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
//函数名称：DF_BufferWrite(u8 bufferaddr,u8 *writebuff,u16 len)
//输入：BUFFER中的起始地址, 待存数据的头指针,待存数据的长度1~1024
//输出：
//函数功能：将指定数据写入从某个地址（0~1023）开始的BUFFER1中
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
//函数名称：DF_BufferToMainMemoryWithErase(u16 pageaddr,u8 buffaddr,u8 *writebuff,u16 len)
//输入：BUFFER中的起始地址, 待存数据的头指针,待存数据的长度1~1024
//输出：
//函数功能：将指定数据写入从某个地址（0~1023）开始的BUFFER1中:包含2个动作,首先将
//指定数据写入到BUFFER 1中,其中可以指定BUFFER中的起始写入地址,此写入动作不影响
//BUFFER中其它地址中的数据,然后再将BUFFER中的整个数据写入到某指定页中(带预擦除)。                                      
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
//函数名称：DF_MainMemoryPageProgramThroughBuffer(u32 writeaddr, u8 *writebuff, u16 len)
//输入:riteaddr---待写入地址,writebuff数据指针,len数据长度
//输出：
//函数功能:通过BUF1写主存，包含动作:先将1-1024字节数据写入BUF1，然后进行
//BUF1到主存的带擦除编程
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
//函数名称：DF_MainMemoryPagetoBufferTransfer(u16 pageaddr)
//输入：pageaddr
//输出：
//函数功能：
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
//函数名称：
//输入：
//输出：
//函数功能：
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
//函数名称：DF_Chip_Erase(void)
//输入：
//输出：
//函数功能:整片擦除FLASH全部内容
//==========================================================================================
void DF_Chip_Erase(void)
{
  	 u8 Chip_Erase_Command[4] = {0xC7,0x94,0x80,0x9A};//整片擦除操作码
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
//函数名称：
//输入：
//输出：
//函数功能:读取制造ID，确认是否有DATAFLASH存在,存在返回1,否则返回0
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
//函数名称：
//输入：
//输出：
//函数功能:初始化DataFlash，检查制造ID，配置为1024字节每page，禁止扇区保护
//==========================================================================================
void DF_Init(void)
{
	u8 i = 0;
	GPIO_SetBits(GPIOC, FCS);
	GPIO_ResetBits(GPIOC, FSCK);//AT45DB选择SPI方式0
	GPIO_ResetBits(GPIOC, FSDI);
 
	i = DF_Check_Busy_State();
	if(DF_ReadManufactureIDInformation())
	{
		DF_Configer_Binary_Page_Size();
		i = DF_Cancel_Sector_Protection();
	}
	else
	{//未检测到DF，则给出提示报警信息
		i = 0;
		while(i < 50)
		{
			i ++;
		}
		BeepTim = 100;
	}	
}
 
//=========================================================================================
//函数名称：SaveDataToFlash(u32 *pBuffer, u32 cnt, u32 addr)
//输入：pBuffer-数据缓冲区首址,cnt-写入字节数,WriteAddr-保存地址
//输出：保存成功与否
//函数功能:将任意字节(小于0X800000)数据保存入FLASH任意地址
//==========================================================================================
u8 SaveDataToFlash(u8 *pBuffer, u32 sizlen, u32 WriteAddr)
{
	u32 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;
	u16 paddr = 0;
	
	if(WriteAddr + sizlen >= DF_FullSize) 
	{//如果 超出地址
		return 0;
	}
  	Addr = WriteAddr % DF_PageSize;
  	count = DF_PageSize - Addr;
  	NumOfPage =  sizlen / DF_PageSize;
  	NumOfSingle = sizlen % DF_PageSize;
	/* If WriteAddr is DF_PageSize aligned  */
  	if(Addr == 0) 
  	{//地址为整页起始地址
    /* If sizlen < DF_PageSize */
    	if(NumOfPage == 0) 
    	{//不够一页
      		paddr = (u16)((WriteAddr >> 8) & 0xfffc);//查找页地址
			DF_MainMemoryPagetoBufferTransfer(paddr);
			DF_MainMemoryPageProgramThroughBuffer(WriteAddr,pBuffer,NumOfSingle);
			DF_AutoPageRewriteThroughBuffer(paddr);	
    	}
    /* If sizlen > DF_PageSize */
    	else  
    	{//超出一页
      		while(NumOfPage--)
      		{//先写整页
        		paddr = (u16)((WriteAddr >> 8) & 0xfffc);//查找页地址
				DF_MainMemoryPagetoBufferTransfer(paddr);
				DF_MainMemoryPageProgramThroughBuffer(WriteAddr,pBuffer,DF_PageSize);
				DF_AutoPageRewriteThroughBuffer(paddr);	
        		WriteAddr +=  DF_PageSize;
        		pBuffer += DF_PageSize;
      		}//再写余下部分
      		if(NumOfSingle!=0)
      		{
        		paddr = (u16)((WriteAddr >> 8) & 0xfffc);//查找页地址
				DF_MainMemoryPagetoBufferTransfer(paddr);
				DF_MainMemoryPageProgramThroughBuffer(WriteAddr,pBuffer,NumOfSingle);
				DF_AutoPageRewriteThroughBuffer(paddr);	
      		}
    	}
  	}
  	/* If WriteAddr is not DF_PageSize aligned  */
  	else 
  	{//起始地址非整页地址
    	/* If sizlen < DF_PageSize */	
    	if(NumOfPage== 0) 
    	{//不超过一页
			if(NumOfSingle > count)
			{//产生跨页
				paddr = (u16)((WriteAddr >> 8) & 0xfffc);//查找页地址
				DF_MainMemoryPagetoBufferTransfer(paddr);
				DF_MainMemoryPageProgramThroughBuffer(WriteAddr,pBuffer,count);
				DF_AutoPageRewriteThroughBuffer(paddr);	
				NumOfSingle -= count;
				WriteAddr += count;
				pBuffer += count;
			   	paddr = (u16)((WriteAddr >> 8) & 0xfffc);//查找页地址
				DF_MainMemoryPagetoBufferTransfer(paddr);
				DF_MainMemoryPageProgramThroughBuffer(WriteAddr,pBuffer,NumOfSingle);
				DF_AutoPageRewriteThroughBuffer(paddr);	
			}
			else
			{//不产生跨页
	      		paddr = (u16)((WriteAddr >> 8) & 0xfffc);//查找页地址
				DF_MainMemoryPagetoBufferTransfer(paddr);
				DF_MainMemoryPageProgramThroughBuffer(WriteAddr,pBuffer,NumOfSingle);
				DF_AutoPageRewriteThroughBuffer(paddr);	
			}
    	}
    	/* If sizlen > DF_PageSize */
    	else
    	{//超过一页
      		sizlen -= count;
      		NumOfPage =  sizlen / DF_PageSize;
      		NumOfSingle = sizlen % DF_PageSize;	
			if(count != 0)
	      	{  
	        	paddr = (u16)((WriteAddr >> 8) & 0xfffc);//查找页地址
				DF_MainMemoryPagetoBufferTransfer(paddr);
				DF_MainMemoryPageProgramThroughBuffer(WriteAddr,pBuffer,count);
				DF_AutoPageRewriteThroughBuffer(paddr);	
	        	WriteAddr += count;
	        	pBuffer += count;
	      	} 
	      	while(NumOfPage--)
	      	{
	        	paddr = (u16)((WriteAddr >> 8) & 0xfffc);//查找页地址
				DF_MainMemoryPagetoBufferTransfer(paddr);
				DF_MainMemoryPageProgramThroughBuffer(WriteAddr,pBuffer,DF_PageSize);
				DF_AutoPageRewriteThroughBuffer(paddr);	
	        	WriteAddr +=  DF_PageSize;
	        	pBuffer += DF_PageSize;  
	      	}
	      	if(NumOfSingle != 0)
	      	{
	        	paddr = (u16)((WriteAddr >> 8) & 0xfffc);//查找页地址
				DF_MainMemoryPagetoBufferTransfer(paddr);
				DF_MainMemoryPageProgramThroughBuffer(WriteAddr,pBuffer,NumOfSingle);
				DF_AutoPageRewriteThroughBuffer(paddr);	
	      	}
      	}
  	}
	return 1;
}
//=========================================================================================
//函数名称：GetFlashData(u32 readdr, u8 *pBuffer, u16 sizlen)
//输入：readdr-读取地址,pBuffer-读取数据缓存地址,sizlen-数据字节数
//输出：
//函数功能：从FLASH任意(0-0X7FFFFF)地址读取任意字节(0-65535)数据
//==========================================================================================
u8 GetFlashData(u32 readdr, u8 *pBuffer, u16 sizlen)
{
	if(readdr + sizlen >= DF_FullSize) 
	{//如果 超出地址
		return 0;
	}
	DF_ContinusArrayRead(readdr,pBuffer,sizlen);
 
	return 1;
}
//=========================================================================================
//函数名称：
//输入：
//输出：
//函数功能:测试读写DATAFLASH
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
