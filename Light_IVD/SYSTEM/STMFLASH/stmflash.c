#include "stm32f10x.h"
#include "STMFLASH\stmflash.h"
#include "SYS_DELAY\Sys_Delay.h"
#include "stdio.h"
#include "string.h"

//��ȡָ����ַ�İ���(16λ����)
//faddr:����ַ(�˵�ַ����Ϊ2�ı���!!)
//����ֵ:��Ӧ����.
unsigned long STMFLASH_ReadWord(unsigned long faddr)
{
	return *(vu32*)faddr; 
}
#if STM32_FLASH_WREN	//���ʹ����д   
//������д��
//WriteAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToWrite:��(32λ)��   
void STMFLASH_Write_NoCheck(unsigned long WriteAddr,unsigned long *pBuffer,unsigned short NumToWrite)   
{ 			 		 
	unsigned short i;
	for(i=0;i<NumToWrite;i++)
	{
		FLASH_ProgramWord(WriteAddr,pBuffer[i]);
	    WriteAddr+=4;//��ַ����4.
	}  
} 
//��ָ����ַ��ʼд��ָ�����ȵ�����
//WriteAddr:��ʼ��ַ(�˵�ַ����Ϊ2�ı���!!)
//pBuffer:����ָ��
//NumToWrite:����(16λ)��(����Ҫд���16λ���ݵĸ���.)
		 
unsigned long STMFLASH_BUF[STM_SECTOR_SIZE>>2];//�����2K�ֽ�
void STMFLASH_Write(unsigned long WriteAddr,unsigned long *pBuffer,unsigned short NumToWrite)	
{
	unsigned long secpos;	   //������ַ
	unsigned short secoff;	   //������ƫ�Ƶ�ַ(32λ�ּ���)
	unsigned short secremain; //������ʣ���ַ(16λ�ּ���)	   
 	unsigned short i;    
	unsigned long offaddr;   //ȥ��0X08000000��ĵ�ַ
    
    
	if(WriteAddr<STM32_FLASH_BASE||(WriteAddr>=(STM32_FLASH_BASE+1024*STM32_FLASH_SIZE)))return;//�Ƿ���ַ
	FLASH_Unlock();						//����
	offaddr=WriteAddr-STM32_FLASH_BASE;		//ʵ��ƫ�Ƶ�ַ.
	secpos=offaddr/STM_SECTOR_SIZE;			//������ַ  0~127 for STM32F103RBT6
	secoff=(offaddr%STM_SECTOR_SIZE)>>2;		//�������ڵ�ƫ��(4���ֽ�Ϊ������λ.)
	secremain=(STM_SECTOR_SIZE>>2)-secoff;		//����ʣ��ռ��С   
	if(NumToWrite<=secremain)secremain=NumToWrite;//�����ڸ�������Χ
	while(1) 
	{	
		STMFLASH_Read(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE>>2);//������������������
		for(i=0;i<secremain;i++)//У������
		{
			if(STMFLASH_BUF[secoff+i]!=0XFFFFFFFF)break;//��Ҫ����  	  
		}
		if(i<secremain)//��Ҫ����
		{
			FLASH_ErasePage(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE);//�����������
			for(i=0;i<secremain;i++)//����
			{
				STMFLASH_BUF[i+secoff]=pBuffer[i];	  
			}
			STMFLASH_Write_NoCheck(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE>>2);//д����������  
		}
        else 
            STMFLASH_Write_NoCheck(WriteAddr,pBuffer,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 				   
		if(NumToWrite==secremain)
            break;//д�������
		else//д��δ����
		{
			secpos++;				//������ַ��1
			secoff=0;				//ƫ��λ��Ϊ0 	 
		   	pBuffer+=secremain;  	//ָ��ƫ��
			WriteAddr+=secremain;	//д��ַƫ��	   
		   	NumToWrite-=secremain;	//�ֽ�(16λ)���ݼ�
			if(NumToWrite>(STM_SECTOR_SIZE>>2))
                secremain=STM_SECTOR_SIZE>>2;//��һ����������д����
			else 
                secremain=NumToWrite;//��һ����������д����
		}	 
	};	
	FLASH_Lock();//����
}
#endif

//��ָ����ַ��ʼ����ָ�����ȵ�����
//ReadAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToWrite:����(16λ)��
void STMFLASH_Read(unsigned long ReadAddr,unsigned long *pBuffer,unsigned short NumToRead)   	
{
	u16 i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadWord(ReadAddr);//��ȡ2���ֽ�.
		ReadAddr+=4;//ƫ��2���ֽ�.	
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//WriteAddr:��ʼ��ַ
//WriteData:Ҫд�������
//void Test_Write(unsigned long WriteAddr,unsigned long WriteData)   	
//{
//	STMFLASH_Write(WriteAddr,&WriteData,1);//д��һ���� 
//}

uint8_t Test_Write(void)
{
    uint16_t i = 0;
    uint8_t buf[256] = {0};
    for(i = 0;i<256;i++)
    {
        buf[i] = i;
        printf("%c",buf[i]);
    }
    printf("\r\n");
    STMFLASH_Write(0x8020000,(unsigned long *)buf,i>>2);
    Sys_Delay_ms(100);
    memset(buf,0x0,256);
    Sys_Delay_ms(100);
    STMFLASH_Read(0x8020000,(unsigned long *)buf,i>>2);
    for(i = 0;i<256;i++)
    {
        printf("%c",buf[i]);
    }
    printf("%d \r\n",i);
}














