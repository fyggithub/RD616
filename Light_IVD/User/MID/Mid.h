#ifndef __MID_H__
#define __MID_H__


#define MSG_BUF_LEN             512     // 数据长度

#define COM_RECIEVE_FLAG        0x01
#define CAN_RECIEVE_FLAG        0x02

//===============================================================
// 传输协议规定
//===============================================================
#define FRAM_HEAD_DATA          0xAB    // 帧头
#define FRAM_TAIL_DATA          0xCD    // 帧尾

//---------------------------------------------------------------
// 控制位说明
#define FRAM_CTRL_DATA_M_S      0x80    // 接收数据控制  主机返回给从机
#define FRAM_CTRL_DATA_S_S      0x40    // 从机发送给从机
#define FRAM_CTRL_DATA_S_M      0x20    // 发送数据控制  从机返回给主机
#define FRAM_CTRL_DATA_ERR      0xC0    // 返回错误帧
#define FRAM_CTRL_DATA_WAR      0xD0    // 返回警告


typedef enum
{
    Idel      = 0,          // 空闲状态
    Fram_Head = 1,          // 接收数据头
    Fram_Ctrl = 2,          // 接收控制位
    Fram_Len  = 3,          // 接收数据长度
    Fram_Data = 4,          // 接收数据
    Fram_Crc  = 5,          // 接收校验位
    Fram_Tail = 6           // 接收数据尾
}RecFramState;


typedef struct
{
    UINT16 len;             // 接收的数据长度
    UINT8  get_len;         // 每一个case获取的数据长度
    UINT8  data_len;        // 数据位的数据长度
    RecFramState RecStaue;  // 状态
}st_QueWaitPara;        // 队列接收数据的等待信息缓存

typedef struct
{
    UINT32 ID;
    UINT32  backnum;
    UINT32 Flag;
    UINT32 ByteCounts;
}APPInfoType;

extern APPInfoType gApp;

_EXT_ UINT8 set_debug;
_EXT_ UINT8 RecieveBuf[];

_EXT_ UINT8 Mid_usart_buf[];     // 串口接收数据缓存
_EXT_ UINT8 Mid_can_buf[];       // Can接收数据缓存
    
_EXT_ UINT8 ReBack_que_buf[];     // 返回数据缓存

_EXT_ UINT8 Recieve_Flag;                     // 接收数据表示

_EXT_ UINT8 Sys_Errcode;           // 系统错误码
_EXT_ UINT8 Sys_Errpos;       // 默认错误位置

UINT16 Que_WaitFram(UINT8* Que_Msgbuf,UINT8 *RecBuf,st_QueWaitPara *sWaitPara, UINT16 ts);
void Sys_Init(void);
UINT8 PL_CommandCheck(UINT8* pStr, UINT16 nLen, UINT8* pResult);
#endif

