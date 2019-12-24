#ifndef __MID_OP_H__
#define __MID_OP_H__

#include <includes.h>


#define REBACK_BUF_LEN              256

#define SYSTEM_VERSION                  20              // 系统版本号
#define VERSION_NAME                    1               // 版本正式版为V-0，临时版为T-1

#define TASK_QUE_BUF_LEN            256                 // 任务命令缓存大小

//----------------------------------------------------------------------------------------
// 命令模块     （把命令分模块处理）
#define SYS_CMD                     0
#define IO_CMD                      1
#define SP_CMD                      2
#define PARA_CMD                    3
#define EC_CMD                      4

#define CMD_SYS_HANDLE              0x01            // 握手命令

#define CMD_SYS_VERSION             0x02            // 系统版本号

#define CMD_ENTER_BOOTLOAD          0x04            // 系统进入bootload模式

#define CMD_SYS_DEBUG               0x07            // 调试控制开关

#define CMD_LIGHT_STATE_CHECK       0x13            // 光耦状态查询个
#define CMD_LED_STATE_CTRL          0x14            // 灯状态控制


#define CMD_PARA_EC_SET             0x35            // 进杯器参数设置
#define CMD_PARA_EC_LOAD            0x36            // 进杯器参数读取

#define CMD_MT_EC_RST               0x40            // 进杯器电机复位
#define CMD_MT_EC_MOVE              0x41            // 进杯器电机运动
#define CMD_MT_EC_MOVE_TO           0x42            // 进杯器电机运动到指定位置

#define CMD_MT_AUTO_ENTER           0x48            // 进杯器开启关闭自动进杯

#define CMD_SPRING_CTRL             0x49            // 进杯器推力电磁铁电源开关

#define CMD_LIGHT_STATE_BACK        0xF0            // 光耦状态自动返回指令
#define CMD_KEY_STATE_BACK          0xF1            // 按键状态自动返回指令

#define  APP_IS_VALID                   0xFFFFFFFF      // 程序生效
#define  APP_IS_BACKUP                  0x3456789A      // 程序存在，但未生效
#define  APP_IS_INVALID                 0x12345678      // 程序不存在 或更新状态
#define  APP_IS_ID_ERR                  0x89ABCDEF      // ID编号错误

// 光耦状态
typedef enum
{
    e_EC_udRest      = 1,
    e_EC_upCheck     = 2,
    e_EC_downCheck   = 3,
    e_EC_moveCheck   = 4,
    e_WC_1_haveCheck = 5,
    e_WC_2_haveCheck = 6,
    e_WC_1_fullCheck = 7,
    e_WC_2_fullCheck = 8,
}e_lightType;


typedef struct
{
    UINT8   st_dataRecFlag;
    UINT8   st_dataOp;
    UINT8   st_dataSendId;
    UINT8   st_dataCmd;
    UINT8 * st_dataBuf;
    UINT16  st_dataLen;
    UINT8   st_dataErrModule;
    UINT8   st_dataErrNum;
    UINT8   st_dataErrPos;
}st_DataMsg;

typedef struct
{
    OS_TCB* st_taskTcb;
    UINT8 * st_taskQueBuf;
    UINT16  st_taskQueLen;
    
    st_DataMsg s_DataMsg;
}st_TaskCtrlType;

_EXT_ st_DataMsg sys_msg;           // 系统命令参数信息
_EXT_ st_DataMsg io_msg;            // 泵阀命令参数信息
_EXT_ st_DataMsg ec_msg;            // 试剂盘命令参数信息
_EXT_ st_DataMsg para_msg;          // 存储数据命令参数信息


void SoftReset(void);
void Msg_Init(void);
BOOL Get_DataMsg(UINT8 *que_buf,st_DataMsg * s_dataM, st_QueWaitPara *sWaitPara);
UINT8 Handle_Msg(UINT8 *Recieve_buf, UINT16 msg_len);
#endif

