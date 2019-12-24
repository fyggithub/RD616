#ifndef __MTX_H__
#define __MTX_H__


#include "MTx_Timer.h"
//--------------------------------------------------------------
// 电机运行模式
#define MODEL_RUN_MODE                  0               // 正常电机运行模式
#define RST_TIGGER_RUN_MODE             1               // 电机复位运行模式
#define ARRIVE_TIGGER_RUN_MODE          2               // 到位触发运行模式
#define LIQUIT_TIGGER_RUN_MODE          3               // 液面触发运行模式

#define MT_TIGGER_CHECK_TIME            5


typedef UINT8  (*funMtxOne)   (UINT8);
typedef UINT8  (*funMtxTwo)   (UINT8,UINT16);
typedef UINT32 (*funMtxThree) (UINT8);
typedef UINT8  (*funMtxFour)  (UINT32);
typedef UINT8  (*funMtxFive)  (UINT8, UINT8);


enum eMtxOut
{
    eMtx_CLK = 0,
    eMtx_EN  = 1,
    eMtx_DIR = 2
};

enum eMtxLight
{
    eMtx_Reset_Light  = 0,
    eMtx_Arrive_Light = 1,
    eMtx_Code_Light   = 2,
    eMtx_Crash_Light  = 3
};

// 电机运动的6种状态
typedef enum 
{
    eMtx_Phase_IDEL   = 0,
    eMtx_Phase_LOAD   = 1,
    eMtx_Phase_ACC    = 2,
    eMtx_Phase_EQU    = 3,
    eMtx_Phase_DEC    = 4,
    eMtx_Phase_FIN    = 5
}eMtxPhase;

typedef enum
{
    MTx_Pin_Low  = 0,
    MTx_Pin_High = 1
}eMtxPinLevel;

typedef enum
{
    eMtx_Dir_POSI = 0,
    eMtx_Dir_NEGA = 1
}eMtxDir;

typedef enum
{
    eMtx_ID_1 = 0,
    eMtx_ID_2 = 1,
    eMtx_ID_3 = 2,
    eMtx_ID_4 = 3
}eMtxID;

typedef struct
{
    UINT16 nFreqMin;        // 最小频率值
    UINT16 nFreqMax;        // 最大频率值
    
    UINT32 nMaxPos;         // 最大步数
    
    UINT16 nRst;            // 复位后运行的步数
    UINT16 nRRst;           // 向相反方向复位运行的步数
    UINT16 nArrive;         // 到位触发运行步数，液面检查也是相当于到位触发
    UINT16 nCode;           // 码盘触发运行步数

    UINT16 nStepsT1;        // 第一阶步数
    UINT16 nStepsT2;        // 第二阶步数
    UINT16 nStepsT3;        // 第三阶步数
    UINT16 nStepsT5;        // 第五阶步数
    UINT16 nStepsT6;        // 第六阶步数
    UINT16 nStepsT7;        // 第七阶步数

    UINT16 nLightCheck;     // 光耦检查
    UINT16 nIsCircle;       // 盘电机标志位
    UINT16 nCircleSteps;    // 盘电机一圈的步数
    UINT16 nCircleCups;     // 盘电机的杯数
    UINT16 nCirclePerSteps; // 盘电机的一杯的步数

    UINT32 nAccMax;         // 加加速度最大值
    UINT32 nAccAcc;         // 加加速加速度
    UINT32 nAccDec;         // 加减速加速度
    UINT32 nDecMax;         // 减加速度最大值
    UINT32 nDecAcc;         // 减加速加速度
    UINT32 nDecDec;         // 减减速加速度

    UINT16 *nAccTimArr;     // 加速度的加速计数值
    UINT16 nAccSteps;       // 加速度步数
    UINT16 *nDecTimArr;     // 减加速的减速计数值
    UINT16 nDecSteps;       // 减加速的步数

    UINT8  nLevelPosi;      // 电机正向运动的方向
    
    UINT8  nRst_LightCheck; // 复位光耦检查电平
    
    UINT8  nInDex;          // 电机编号
}tMtxPara;

typedef struct
{
    funMtxOne   pinInit;                // 管脚初始化
    funMtxOne   pinDriverLock;          // 电机锁紧
    funMtxOne   pinDriverUnlock;        // 电机解锁
    funMtxOne   pinDirectionPosi;       // 电机正向运行
    funMtxOne   pinDirectionNega;       // 电机反向运行
    funMtxOne   pinPulseHigh;           // 电机脉冲置高
    funMtxOne   pinPulseLow;            // 电机脉冲置低
//    funMtxOne   pinGetOcSta;

    funMtxOne   tmrInit;                // 定时器初始化
    funMtxOne   tmrInterruptOn;         // 定时器中断开
    funMtxOne   tmrInterruptOff;        // 定时器中断关
    funMtxOne   tmrInterruptClear;      // 定时器清楚中断
    funMtxOne   tmrCountOn;             // 定时器计数开
    funMtxOne   tmrCountOff;            // 定时器计数关
    funMtxTwo   tmrLoad;                // 定时器加载

    funMtxOne   mtLightPinInit;         // 光耦管脚初始化
    funMtxOne   mtResetRead;            // 读取复位光耦
    funMtxOne   mtArriveRead;           // 读取到位光耦
    funMtxOne   mtCodeRead;             // 读取码盘光耦
    funMtxOne   mtCrashRead;            // 读取碰撞光耦
    
    funMtxFive  mtDriverCtrl;           // 电机开关控制
    
    funMtxOne   mtTimPlusCtrl;          // 电机脉冲控制
}tMtxFunList;


typedef struct
{
    UINT32 mStepsCur;               // 当前的步数
    UINT32 mStepsRun;               // 运行的步数
    UINT32 mStepsTotal;             // 要走的总步数
    
    UINT32 mStepsTiger;             // 触发光耦后的步数
    UINT32 mRunTigerPost;           // 触发光耦后要走的步数

    UINT16 mTimeLoadIndex;          // 加载到的台阶数
    UINT32 mTimeLoad;               // 定时器加载值
    UINT32 mTimeArr;                // 定时器ARR值
    UINT16 mStepsAcc;               // 加速度值
    UINT16 mStepsDec;               // 减速值

    eMtxPhase  mRunMode;            // 电机运行模式
    UINT8  mRunFlag;                // 电机运行标志

    UINT16 mSteps1;                 // 计算后运行的第一阶加速的步数
    UINT16 mSteps2;                 // 计算后运行的第二阶加速的步数
    UINT16 mSteps3;                 // 计算后运行的第三阶加速的步数
    UINT16 mSteps5;                 // 计算后运行的第五阶加速的步数
    UINT16 mSteps6;                 // 计算后运行的第六阶加速的步数
    UINT16 mSteps7;                 // 计算后运行的第七阶加速的步数

    #if (STEPS_MODE == 1)
    UINT8  mStep;                   // 每台阶的步数
    UINT8  mStepNum;                // 总共的台阶数
    UINT8  mGoStep;                 // 在当前台阶走了多少
    #endif

    UINT8  mErrCode;                // 错误代码

    TIM_TypeDef   *mTim;
    eMtxDir   eDirection;           // 电机运动方向
    eMtxPhase ePhase;               // 定时器任务标志
    
}tMtxCtrl;

typedef struct
{
    UINT8  mt_Clk;                  // 步进电机脉冲标志
//    UINT8  mtRunIndex;              // 第几个电机步进电机运行
    
    UINT8  mtRstFlag;               // 复位光耦状态标志
    UINT8  mtCodeFlag;              // 码盘光耦状态标志

    UINT16 mtCodeCount;             // 码盘光耦触发计数
    UINT16 mtCodeNum;               // 码盘边沿检测
    
    UINT16 mtArriveCount;           // 到位触发计数
    UINT16 mtCrashCount;            // 碰撞光耦触发计数
    
}tMtxFlag;

_EXT_ tMtxPara    MtxPara[MTx_NUM];
_EXT_ tMtxFunList MtxFunList[MTx_NUM];
_EXT_ tMtxCtrl    MtxCtrl[MTx_NUM];
_EXT_ tMtxFlag    MtxFlag[MTx_NUM];
_EXT_ UINT8 mtRunIndex;

UINT8 MtxCaculateAccDec(UINT8 MtxId);
UINT8 Mtx_DriverCtrl(UINT8 MtxId, UINT8 state);
UINT16 MtxCountFreqAccDec(UINT8 MtxId, UINT16 *AccSteps, UINT16 *DecSteps);
BOOL Mtx_Phase_Idel(UINT8 MtxId);
BOOL Mtx_Phase_Load(UINT8 MtxId);
BOOL Mtx_Phase_Acc(UINT8 MtxId);
BOOL Mtx_Phase_Equ(UINT8 MtxId);
BOOL Mtx_Phase_Dec(UINT8 MtxId);
BOOL Mtx_Phase_Fin(UINT8 MtxId);
BOOL MtPhasePwm(UINT8 MtxId);
void Mtx_Pwm_ISR(UINT8 MtxId);
#endif

