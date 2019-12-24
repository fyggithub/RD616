#ifndef __MTX_H__
#define __MTX_H__


#include "MTx_Timer.h"
//--------------------------------------------------------------
// �������ģʽ
#define MODEL_RUN_MODE                  0               // �����������ģʽ
#define RST_TIGGER_RUN_MODE             1               // �����λ����ģʽ
#define ARRIVE_TIGGER_RUN_MODE          2               // ��λ��������ģʽ
#define LIQUIT_TIGGER_RUN_MODE          3               // Һ�津������ģʽ

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

// ����˶���6��״̬
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
    UINT16 nFreqMin;        // ��СƵ��ֵ
    UINT16 nFreqMax;        // ���Ƶ��ֵ
    
    UINT32 nMaxPos;         // �����
    
    UINT16 nRst;            // ��λ�����еĲ���
    UINT16 nRRst;           // ���෴����λ���еĲ���
    UINT16 nArrive;         // ��λ�������в�����Һ����Ҳ���൱�ڵ�λ����
    UINT16 nCode;           // ���̴������в���

    UINT16 nStepsT1;        // ��һ�ײ���
    UINT16 nStepsT2;        // �ڶ��ײ���
    UINT16 nStepsT3;        // �����ײ���
    UINT16 nStepsT5;        // ����ײ���
    UINT16 nStepsT6;        // �����ײ���
    UINT16 nStepsT7;        // ���߽ײ���

    UINT16 nLightCheck;     // ������
    UINT16 nIsCircle;       // �̵����־λ
    UINT16 nCircleSteps;    // �̵��һȦ�Ĳ���
    UINT16 nCircleCups;     // �̵���ı���
    UINT16 nCirclePerSteps; // �̵����һ���Ĳ���

    UINT32 nAccMax;         // �Ӽ��ٶ����ֵ
    UINT32 nAccAcc;         // �Ӽ��ټ��ٶ�
    UINT32 nAccDec;         // �Ӽ��ټ��ٶ�
    UINT32 nDecMax;         // �����ٶ����ֵ
    UINT32 nDecAcc;         // �����ټ��ٶ�
    UINT32 nDecDec;         // �����ټ��ٶ�

    UINT16 *nAccTimArr;     // ���ٶȵļ��ټ���ֵ
    UINT16 nAccSteps;       // ���ٶȲ���
    UINT16 *nDecTimArr;     // �����ٵļ��ټ���ֵ
    UINT16 nDecSteps;       // �����ٵĲ���

    UINT8  nLevelPosi;      // ��������˶��ķ���
    
    UINT8  nRst_LightCheck; // ��λ�������ƽ
    
    UINT8  nInDex;          // ������
}tMtxPara;

typedef struct
{
    funMtxOne   pinInit;                // �ܽų�ʼ��
    funMtxOne   pinDriverLock;          // �������
    funMtxOne   pinDriverUnlock;        // �������
    funMtxOne   pinDirectionPosi;       // �����������
    funMtxOne   pinDirectionNega;       // �����������
    funMtxOne   pinPulseHigh;           // ��������ø�
    funMtxOne   pinPulseLow;            // ��������õ�
//    funMtxOne   pinGetOcSta;

    funMtxOne   tmrInit;                // ��ʱ����ʼ��
    funMtxOne   tmrInterruptOn;         // ��ʱ���жϿ�
    funMtxOne   tmrInterruptOff;        // ��ʱ���жϹ�
    funMtxOne   tmrInterruptClear;      // ��ʱ������ж�
    funMtxOne   tmrCountOn;             // ��ʱ��������
    funMtxOne   tmrCountOff;            // ��ʱ��������
    funMtxTwo   tmrLoad;                // ��ʱ������

    funMtxOne   mtLightPinInit;         // ����ܽų�ʼ��
    funMtxOne   mtResetRead;            // ��ȡ��λ����
    funMtxOne   mtArriveRead;           // ��ȡ��λ����
    funMtxOne   mtCodeRead;             // ��ȡ���̹���
    funMtxOne   mtCrashRead;            // ��ȡ��ײ����
    
    funMtxFive  mtDriverCtrl;           // ������ؿ���
    
    funMtxOne   mtTimPlusCtrl;          // ����������
}tMtxFunList;


typedef struct
{
    UINT32 mStepsCur;               // ��ǰ�Ĳ���
    UINT32 mStepsRun;               // ���еĲ���
    UINT32 mStepsTotal;             // Ҫ�ߵ��ܲ���
    
    UINT32 mStepsTiger;             // ���������Ĳ���
    UINT32 mRunTigerPost;           // ���������Ҫ�ߵĲ���

    UINT16 mTimeLoadIndex;          // ���ص���̨����
    UINT32 mTimeLoad;               // ��ʱ������ֵ
    UINT32 mTimeArr;                // ��ʱ��ARRֵ
    UINT16 mStepsAcc;               // ���ٶ�ֵ
    UINT16 mStepsDec;               // ����ֵ

    eMtxPhase  mRunMode;            // �������ģʽ
    UINT8  mRunFlag;                // ������б�־

    UINT16 mSteps1;                 // ��������еĵ�һ�׼��ٵĲ���
    UINT16 mSteps2;                 // ��������еĵڶ��׼��ٵĲ���
    UINT16 mSteps3;                 // ��������еĵ����׼��ٵĲ���
    UINT16 mSteps5;                 // ��������еĵ���׼��ٵĲ���
    UINT16 mSteps6;                 // ��������еĵ����׼��ٵĲ���
    UINT16 mSteps7;                 // ��������еĵ��߽׼��ٵĲ���

    #if (STEPS_MODE == 1)
    UINT8  mStep;                   // ÿ̨�׵Ĳ���
    UINT8  mStepNum;                // �ܹ���̨����
    UINT8  mGoStep;                 // �ڵ�ǰ̨�����˶���
    #endif

    UINT8  mErrCode;                // �������

    TIM_TypeDef   *mTim;
    eMtxDir   eDirection;           // ����˶�����
    eMtxPhase ePhase;               // ��ʱ�������־
    
}tMtxCtrl;

typedef struct
{
    UINT8  mt_Clk;                  // ������������־
//    UINT8  mtRunIndex;              // �ڼ�����������������
    
    UINT8  mtRstFlag;               // ��λ����״̬��־
    UINT8  mtCodeFlag;              // ���̹���״̬��־

    UINT16 mtCodeCount;             // ���̹��������
    UINT16 mtCodeNum;               // ���̱��ؼ��
    
    UINT16 mtArriveCount;           // ��λ��������
    UINT16 mtCrashCount;            // ��ײ���������
    
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

