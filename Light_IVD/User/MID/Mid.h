#ifndef __MID_H__
#define __MID_H__


#define MSG_BUF_LEN             512     // ���ݳ���

#define COM_RECIEVE_FLAG        0x01
#define CAN_RECIEVE_FLAG        0x02

//===============================================================
// ����Э��涨
//===============================================================
#define FRAM_HEAD_DATA          0xAB    // ֡ͷ
#define FRAM_TAIL_DATA          0xCD    // ֡β

//---------------------------------------------------------------
// ����λ˵��
#define FRAM_CTRL_DATA_M_S      0x80    // �������ݿ���  �������ظ��ӻ�
#define FRAM_CTRL_DATA_S_S      0x40    // �ӻ����͸��ӻ�
#define FRAM_CTRL_DATA_S_M      0x20    // �������ݿ���  �ӻ����ظ�����
#define FRAM_CTRL_DATA_ERR      0xC0    // ���ش���֡
#define FRAM_CTRL_DATA_WAR      0xD0    // ���ؾ���


typedef enum
{
    Idel      = 0,          // ����״̬
    Fram_Head = 1,          // ��������ͷ
    Fram_Ctrl = 2,          // ���տ���λ
    Fram_Len  = 3,          // �������ݳ���
    Fram_Data = 4,          // ��������
    Fram_Crc  = 5,          // ����У��λ
    Fram_Tail = 6           // ��������β
}RecFramState;


typedef struct
{
    UINT16 len;             // ���յ����ݳ���
    UINT8  get_len;         // ÿһ��case��ȡ�����ݳ���
    UINT8  data_len;        // ����λ�����ݳ���
    RecFramState RecStaue;  // ״̬
}st_QueWaitPara;        // ���н������ݵĵȴ���Ϣ����

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

_EXT_ UINT8 Mid_usart_buf[];     // ���ڽ������ݻ���
_EXT_ UINT8 Mid_can_buf[];       // Can�������ݻ���
    
_EXT_ UINT8 ReBack_que_buf[];     // �������ݻ���

_EXT_ UINT8 Recieve_Flag;                     // �������ݱ�ʾ

_EXT_ UINT8 Sys_Errcode;           // ϵͳ������
_EXT_ UINT8 Sys_Errpos;       // Ĭ�ϴ���λ��

UINT16 Que_WaitFram(UINT8* Que_Msgbuf,UINT8 *RecBuf,st_QueWaitPara *sWaitPara, UINT16 ts);
void Sys_Init(void);
UINT8 PL_CommandCheck(UINT8* pStr, UINT16 nLen, UINT8* pResult);
#endif

