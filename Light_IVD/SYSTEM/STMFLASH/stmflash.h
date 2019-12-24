#ifndef __STMFLASH_H__
#define __STMFLASH_H__

//////////////////////////////////////////////////////////////////////////////////////////////////////
//�û������Լ�����Ҫ����
#define STM32_FLASH_SIZE 512 	 		//��ѡSTM32��FLASH������С(��λΪK)
#define STM32_FLASH_WREN 1              //ʹ��FLASHд��(0��������;1��ʹ��)
//////////////////////////////////////////////////////////////////////////////////////////////////////

//FLASH��ʼ��ַ
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH����ʼ��ַ
//FLASH������ֵ

#if STM32_FLASH_SIZE<256
#define STM_SECTOR_SIZE 1024 //�ֽ�
#else 
#define STM_SECTOR_SIZE	2048
#endif

#define  FLASH_APP_FLAG_ADDR        (STM32_FLASH_BASE + ((STM32_FLASH_SIZE-1) * 1024))

unsigned long STMFLASH_ReadWord(unsigned long faddr);		  //������  
void STMFLASH_WriteLenByte(unsigned long WriteAddr,unsigned long DataToWrite,unsigned short Len);	//ָ����ַ��ʼд��ָ�����ȵ�����
unsigned long STMFLASH_ReadLenByte(unsigned long ReadAddr,unsigned short Len);						//ָ����ַ��ʼ��ȡָ����������
void STMFLASH_Write(unsigned long WriteAddr,unsigned long *pBuffer,unsigned short NumToWrite);		//��ָ����ַ��ʼд��ָ�����ȵ�����
void STMFLASH_Read(unsigned long ReadAddr,unsigned long *pBuffer,unsigned short NumToRead);   		//��ָ����ַ��ʼ����ָ�����ȵ�����

//����д��
uint8_t Test_Write(void);								   
#endif

















