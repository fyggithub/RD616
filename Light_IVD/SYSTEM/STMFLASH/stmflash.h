#ifndef __STMFLASH_H__
#define __STMFLASH_H__

//////////////////////////////////////////////////////////////////////////////////////////////////////
//用户根据自己的需要设置
#define STM32_FLASH_SIZE 512 	 		//所选STM32的FLASH容量大小(单位为K)
#define STM32_FLASH_WREN 1              //使能FLASH写入(0，不是能;1，使能)
//////////////////////////////////////////////////////////////////////////////////////////////////////

//FLASH起始地址
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH的起始地址
//FLASH解锁键值

#if STM32_FLASH_SIZE<256
#define STM_SECTOR_SIZE 1024 //字节
#else 
#define STM_SECTOR_SIZE	2048
#endif

#define  FLASH_APP_FLAG_ADDR        (STM32_FLASH_BASE + ((STM32_FLASH_SIZE-1) * 1024))

unsigned long STMFLASH_ReadWord(unsigned long faddr);		  //读出字  
void STMFLASH_WriteLenByte(unsigned long WriteAddr,unsigned long DataToWrite,unsigned short Len);	//指定地址开始写入指定长度的数据
unsigned long STMFLASH_ReadLenByte(unsigned long ReadAddr,unsigned short Len);						//指定地址开始读取指定长度数据
void STMFLASH_Write(unsigned long WriteAddr,unsigned long *pBuffer,unsigned short NumToWrite);		//从指定地址开始写入指定长度的数据
void STMFLASH_Read(unsigned long ReadAddr,unsigned long *pBuffer,unsigned short NumToRead);   		//从指定地址开始读出指定长度的数据

//测试写入
uint8_t Test_Write(void);								   
#endif

















