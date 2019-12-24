#ifndef __FLASH_AT45DB641_H__
#define __FLASH_AT45DB641_H__

#define AT45DB_SPI       1

/******************************
*opcode-操作码
******************************/
//读取状态的操作码
#define Status_Register_Opcode  			0xD7
//读取ID的操作码
#define Device_ID_Opcode        			0x9F
//Write to buffer
#define Buffer1_Write_Cmd 0x84
#define Buffer2_Write_Cmd 0x87
//Write buffer to page
#define Buffer1ToMMPage_WithErase_Cmd       0x83
#define Buffer2ToMMPage_WithErase_Cmd       0x86

#define Buffer1ToMMPage_WithoutErase_Cmd    0x88
#define Buffer2ToMMPage_WithoutErase_Cmd    0x89

//Continuous Array Read
#define Continuous_Array_Read_Cmd           0x0B  //读取连续主存
#define Continuous_Array_Read_Command   	0xe8//0x03//0x0b//0xe8
#define Deep_Power_Down						0xb9
#define Resume_from_Deep_Power_Down			0xab
#define Main_MemoryPage_to_Buffer1_Transfer	0x53
#define Auto_Page_Rewrite_through_Buffer1	0x58
#define Main_Memory_Page_Pro_Through_Buff1	0x82
#define Main_Memory_Page_Pro_Through_Buff2	0x85
 
#define AT45BD_FullSize							0x7fffff//8M byte
#define AT45BD_PageSize							0x100//页大小


#define AT45DB_CS_GPIO_CLK			RCC_AHB1Periph_GPIOA
#define AT45DB_CS_GPIO_PORT			GPIOA
#define AT45DB_CS_GPIO_PIN			GPIO_Pin_4

#define AT45DB_RESET_GPIO_CLK       RCC_AHB1Periph_GPIOC
#define AT45DB_RESET_GPIO_PORT	    GPIOC
#define AT45DB_RESET_GPIO_PIN		GPIO_Pin_9

#define AT45DB_CS_HIGH()    GPIO_SetBits(AT45DB_CS_GPIO_PORT,AT45DB_CS_GPIO_PIN)
#define AT45DB_CS_LOW()     GPIO_ResetBits(AT45DB_CS_GPIO_PORT,AT45DB_CS_GPIO_PIN)

#define AT45DB_RESET_HIGH() GPIO_SetBits(AT45DB_RESET_GPIO_PORT,AT45DB_RESET_GPIO_PIN)
#define AT45DB_RESET_LOW()  GPIO_ResetBits(AT45DB_RESET_GPIO_PORT,AT45DB_RESET_GPIO_PIN)


void At45DB641_INIT(void);
UINT8 At45DB_ReadIDInformation(void);
void At45DB_Configer_Binary_Page_Size(void);

BOOL  At45DB_WritePage_WithErase(UINT8 BufferNumber,UINT32 WriteAddr,UINT8 *Data,UINT32 ByteNum);
BOOL At45DB_Read(UINT32 ReadAddr,UINT8 *DataBuffer,UINT32 ByteNum);


BOOL Continuous_Array_Read(unsigned int PageAddr,unsigned int ByteAddr,unsigned char *DataBuffer,unsigned long ByteNum);
unsigned char DeviceInformation_Read(void);

//写数据功能函数

BOOL  Buffer_Write(unsigned char BufferNumber,unsigned int ByteAddr,unsigned char *Data,unsigned int ByteNum);
BOOL  BufferToMMPage_WithErase(unsigned char BufferNumber,unsigned int PageAddr);
//人工组合功能连续写
BOOL  DataToPage_ViaBuffer_WithErase_OneWay(unsigned char BufferNumber,unsigned int PageAddr,unsigned int ByteAddr,unsigned char *Data,unsigned int ByteNum);
//指令方式连续写
BOOL  DataToPage_ViaBuffer_WithErase_SecWay(unsigned char BufferNumber,unsigned int PageAddr,unsigned int ByteAddr,unsigned char *Data,unsigned int ByteNum);


#endif
