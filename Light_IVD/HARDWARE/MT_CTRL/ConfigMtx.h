#ifndef __CONFIGMTX_H__
#define __CONFIGMTX_H__

#include "My_type.h"

//====================================================================
// �Ӽ��ٵĿ���ģʽ
// ֱ�Ӵ���̨�׺�Ƶ��
#define STEPS_MODE          0
// ����ʱ��ļӼ��ٵĿ���
#define TIME_MODE           0
// ���ڲ����ļӼ��ٵĿ���
#define STEPS_ACCDEC_MODE          1


//=============================�������µ��==========================
// motor 1          
//#if (MTx_NUM >= 1)
#define MT_1_MAXFREQ                5000            // ���Ƶ��ֵ
#define MT_1_MINFREQ                1000            // ��СƵ��ֵ

#define MT_1_MAXPOS                 7000           // �������������̵��������д0xFFFF

#define MT_1_STEPS_T1               20              // ��һ�ײ���
#define MT_1_STEPS_T2               20              // �ڶ��ײ���
#define MT_1_STEPS_T3               50              // �����ײ���
#define MT_1_STEPS_T5               50              // ����ײ���
#define MT_1_STEPS_T6               20              // �����ײ���
#define MT_1_STEPS_T7               20              // ���߽ײ���

#define MT_1_RSTPOS                 100             // ��λ�����еĲ���
#define MT_1_nRRSTPOS               100             // ���෴����λ���еĲ���
#define MT_1_ARRIVE                 10             // ��λ�������в�����Һ����Ҳ���൱�ڵ�λ����
#define MT_1_CODE                   0               // ���̴������в���

#define MT_1_LIGHTCHECK             0x01            // ��һλ�Ǹ�λ����ʹ�ܣ��ڶ�λ�ǵ�λ����ʹ�ܣ�
                                                    // ����λΪ���̹��λ������λ����ײ����ʹ�ܡ�
#define MT_1_ISCIRCLE               0               // �ж��Ƿ����̵��
#define MT_1_CIRCLE_STEPS           0               // �̵��һȦ�Ĳ���
#define MT_1_CIRCLE_CUPS            0               // �̵���ı���
#define MT_1_ELEVEL_POSI            (MTx_Pin_Low)   // ��������˶��ķ���

#define MT_1_RST_LIGHTCHECK         MTx_Pin_Low

//#endif

//==========================��Һ���µ��=============================
// motor 2
//#if (MTx_NUM >= 2)
#define MT_2_MAXFREQ                4000           // ���Ƶ��ֵ
#define MT_2_MINFREQ                1000            // ��СƵ��ֵ

#define MT_2_MAXPOS                 15000           // �������������̵��������д0xFFFF

#define MT_2_STEPS_T1               80              // ��һ�ײ���
#define MT_2_STEPS_T2               80              // �ڶ��ײ���
#define MT_2_STEPS_T3               80              // �����ײ���
#define MT_2_STEPS_T5               80              // ����ײ���
#define MT_2_STEPS_T6               80              // �����ײ���
#define MT_2_STEPS_T7               80              // ���߽ײ���

#define MT_2_RSTPOS                 2500            // ��λ�����еĲ���
#define MT_2_nRRSTPOS               400             // ���෴����λ���еĲ���
#define MT_2_ARRIVE                 0               // ��λ�������в�����Һ����Ҳ���൱�ڵ�λ����
#define MT_2_CODE                   0               // ���̴������в���

#define MT_2_LIGHTCHECK             0x01            // ��һλ�Ǹ�λ����ʹ�ܣ��ڶ�λ�ǵ�λ����ʹ�ܣ�
                                                    // ����λΪ���̹��λ������λ����ײ����ʹ�ܡ�
#define MT_2_ISCIRCLE               0               // �ж��Ƿ����̵��
#define MT_2_CIRCLE_STEPS           0               // �̵��һȦ�Ĳ���
#define MT_2_CIRCLE_CUPS            0               // �̵���ı���
#define MT_2_ELEVEL_POSI            (MTx_Pin_Low)   // ��������˶��ķ���
#define MT_2_RST_LIGHTCHECK         MTx_Pin_Low

//#endif

//==============================����ע����==============================
// motor 3
//#if (MTx_NUM >= 3)
#define MT_3_MAXFREQ                14000            // ���Ƶ��ֵ
#define MT_3_MINFREQ                1000            // ��СƵ��ֵ

#define MT_3_MAXPOS                 17000           // �������������̵��������д0xFFFF

#define MT_3_STEPS_T1               50              // ��һ�ײ���
#define MT_3_STEPS_T2               50              // �ڶ��ײ���
#define MT_3_STEPS_T3               50              // �����ײ���
#define MT_3_STEPS_T5               50              // ����ײ���
#define MT_3_STEPS_T6               50              // �����ײ���
#define MT_3_STEPS_T7               30              // ���߽ײ���

#define MT_3_RSTPOS                 200             // ��λ�����еĲ���
#define MT_3_nRRSTPOS               200             // ���෴����λ���еĲ���
#define MT_3_ARRIVE                 0               // ��λ�������в�����Һ����Ҳ���൱�ڵ�λ����
#define MT_3_CODE                   0               // ���̴������в���

#define MT_3_LIGHTCHECK             0x01            // ��һλ�Ǹ�λ����ʹ�ܣ��ڶ�λ�ǵ�λ����ʹ�ܣ�
                                                    // ����λΪ���̹��λ������λ����ײ����ʹ�ܡ�
#define MT_3_ISCIRCLE               0               // �ж��Ƿ����̵��
#define MT_3_CIRCLE_STEPS           0               // �̵��һȦ�Ĳ���
#define MT_3_CIRCLE_CUPS            0               // �̵���ı���
#define MT_3_ELEVEL_POSI            (MTx_Pin_Low)   // ��������˶��ķ���

#define MT_3_RST_LIGHTCHECK         MTx_Pin_Low
//#endif

//===============================����ע����2=================================
// motor 4
//#if (MTx_NUM >= 4)
#if 0
#define MT_4_MAXFREQ                14000            // ���Ƶ��ֵ
#define MT_4_MINFREQ                1000            // ��СƵ��ֵ

#define MT_4_MAXPOS                 17000           // �������������̵��������д0xFFFF

#define MT_4_STEPS_T1               50              // ��һ�ײ���
#define MT_4_STEPS_T2               50              // �ڶ��ײ���
#define MT_4_STEPS_T3               50              // �����ײ���
#define MT_4_STEPS_T5               50              // ����ײ���
#define MT_4_STEPS_T6               50              // �����ײ���
#define MT_4_STEPS_T7               50              // ���߽ײ���

#define MT_4_RSTPOS                 200             // ��λ�����еĲ���
#define MT_4_nRRSTPOS               200             // ���෴����λ���еĲ���
#define MT_4_ARRIVE                 0              // ��λ�������в�����Һ����Ҳ���൱�ڵ�λ����
#define MT_4_CODE                   0               // ���̴������в���

#define MT_4_LIGHTCHECK             0x01            // ��һλ�Ǹ�λ����ʹ�ܣ��ڶ�λ�ǵ�λ����ʹ�ܣ�
                                                    // ����λΪ���̹��λ������λ����ײ����ʹ�ܡ�
#define MT_4_ISCIRCLE               0               // �ж��Ƿ����̵��
#define MT_4_CIRCLE_STEPS           0               // �̵��һȦ�Ĳ���
#define MT_4_CIRCLE_CUPS            0               // �̵���ı���
#define MT_4_ELEVEL_POSI            (MTx_Pin_Low)   // ��������˶��ķ���

#define MT_4_RST_LIGHTCHECK         MTx_Pin_Low

#else
//=============================== ������  =================================
#define MT_4_MAXFREQ                3300             // ���Ƶ��ֵ3700
#define MT_4_MINFREQ                600            // ��СƵ��ֵ600

#define MT_4_MAXPOS                 0xFFFFFFFF           // �������������̵��������д0xFFFF

#define MT_4_STEPS_T1               100              // ��һ�ײ���50
#define MT_4_STEPS_T2               50              // �ڶ��ײ���80
#define MT_4_STEPS_T3               150              // �����ײ���50
#define MT_4_STEPS_T5               100              // ����ײ���50
#define MT_4_STEPS_T6               50              // �����ײ���80
#define MT_4_STEPS_T7               150             // ���߽ײ���50

#define MT_4_RSTPOS                 300             // ��λ�����еĲ���200
#define MT_4_nRRSTPOS               750             // ���෴����λ���еĲ���850
#define MT_4_ARRIVE                 0              // ��λ�������в�����Һ����Ҳ���൱�ڵ�λ����
#define MT_4_CODE                   450               // ���̴������в���

#define MT_4_LIGHTCHECK             0x05            // ��һλ�Ǹ�λ����ʹ�ܣ��ڶ�λ�ǵ�λ����ʹ�ܣ�
                                                    // ����λΪ���̹��λ������λ����ײ����ʹ�ܡ�
#define MT_4_ISCIRCLE               1               // �ж��Ƿ����̵��
#define MT_4_CIRCLE_STEPS           24800               // �̵��һȦ�Ĳ���
#define MT_4_CIRCLE_CUPS            62               // �̵���ı���
#define MT_4_ELEVEL_POSI            (MTx_Pin_Low)   // ��������˶��ķ���

#define MT_4_RST_LIGHTCHECK         MTx_Pin_Low
#endif
//#endif
//=========================================================================
#define MTx_MAXFREQ(x)              MT_##x##_MAXFREQ        // ���Ƶ��ֵ
#define MTx_MINFREQ(x)              MT_##x##_MINFREQ        // ��СƵ��ֵ

#define MTx_MAXPOS(x)               MT_##x##_MAXPOS         // �������������̵��������д0xFFFF

#define MTx_STEPS_T1(x)             MT_##x##_STEPS_T1       // ��һ�ײ���
#define MTx_STEPS_T2(x)             MT_##x##_STEPS_T2       // �ڶ��ײ���
#define MTx_STEPS_T3(x)             MT_##x##_STEPS_T3       // �����ײ���
#define MTx_STEPS_T5(x)             MT_##x##_STEPS_T5       // ����ײ���
#define MTx_STEPS_T6(x)             MT_##x##_STEPS_T6       // �����ײ���
#define MTx_STEPS_T7(x)             MT_##x##_STEPS_T7       // ���߽ײ���

#define MTx_RSTPOS(x)               MT_##x##_RSTPOS         // ��λ�����еĲ���
#define MTx_nRRSTPOS(x)             MT_##x##_nRRSTPOS       // ���෴����λ���еĲ���
#define MTx_ARRIVE(x)               MT_##x##_ARRIVE         // ��λ�������в�����Һ����Ҳ���൱�ڵ�λ����
#define MTx_CODE(x)                 MT_##x##_CODE           // ���̴������в���
//#define MTx_CRASH(x)                MT_##x##_CRASH          // ��ײ����

#define MTx_LIGHTCHECK(x)           MT_##x##_LIGHTCHECK     // ��һλ�Ǹ�λ����ʹ�ܣ��ڶ�λ�ǵ�λ����ʹ�ܣ�
                                                            // ����λΪ���̹��λ������λ����ײ����ʹ�ܡ�
#define MTx_ISCIRCLE(x)             MT_##x##_ISCIRCLE       // �ж��Ƿ����̵��
#define MTx_CIRCLE_STEPS(x)         MT_##x##_CIRCLE_STEPS   // �̵��һȦ�Ĳ���
#define MTx_CIRCLE_CUPS(x)          MT_##x##_CIRCLE_CUPS    // �̵���ı���
#define MTx_ELEVEL_POSI(x)          MT_##x##_ELEVEL_POSI    // ��������˶��ķ���

#define MTx_RST_LIGHTCHECK(x)      MT_##x##_RST_LIGHTCHECK    // ��������˶��ķ���

#define MT_MAXFREQ(x,y)                    do{  if(x==0) y = MTx_MAXFREQ(1);\
                                           else if(x==1) y = MTx_MAXFREQ(2);\
                                           else if(x==2) y = MTx_MAXFREQ(3);\
                                           else if(x==3) y = MTx_MAXFREQ(4);\
                                           }while(0)

#define MT_MINFREQ(x,y)                    do{  if(x==0) y = MTx_MINFREQ(1);\
                                           else if(x==1) y = MTx_MINFREQ(2);\
                                           else if(x==2) y = MTx_MINFREQ(3);\
                                           else if(x==3) y = MTx_MINFREQ(4);\
                                           }while(0)

#define MT_MAXPOS(x,y)                     do{  if(x==0) y = MTx_MAXPOS(1);\
                                           else if(x==1) y = MTx_MAXPOS(2);\
                                           else if(x==2) y = MTx_MAXPOS(3);\
                                           else if(x==3) y = MTx_MAXPOS(4);\
                                           }while(0)

#define MT_STEPS_T1(x,y)                   do{  if(x==0) y = MTx_STEPS_T1(1);\
                                           else if(x==1) y = MTx_STEPS_T1(2);\
                                           else if(x==2) y = MTx_STEPS_T1(3);\
                                           else if(x==3) y = MTx_STEPS_T1(4);\
                                           }while(0)

#define MT_STEPS_T2(x,y)                   do{  if(x==0) y = MTx_STEPS_T2(1);\
                                           else if(x==1) y = MTx_STEPS_T2(2);\
                                           else if(x==2) y = MTx_STEPS_T2(3);\
                                           else if(x==3) y = MTx_STEPS_T2(4);\
                                           }while(0)
                                           
#define MT_STEPS_T3(x,y)                   do{  if(x==0) y = MTx_STEPS_T3(1);\
                                           else if(x==1) y = MTx_STEPS_T3(2);\
                                           else if(x==2) y = MTx_STEPS_T3(3);\
                                           else if(x==3) y = MTx_STEPS_T3(4);\
                                           }while(0)
                                           
#define MT_STEPS_T5(x,y)                   do{  if(x==0) y = MTx_STEPS_T5(1);\
                                           else if(x==1) y = MTx_STEPS_T5(2);\
                                           else if(x==2) y = MTx_STEPS_T5(3);\
                                           else if(x==3) y = MTx_STEPS_T5(4);\
                                           }while(0)
                                           
#define MT_STEPS_T6(x,y)                   do{  if(x==0) y = MTx_STEPS_T6(1);\
                                           else if(x==1) y = MTx_STEPS_T6(2);\
                                           else if(x==2) y = MTx_STEPS_T6(3);\
                                           else if(x==3) y = MTx_STEPS_T6(4);\
                                           }while(0)

#define MT_STEPS_T7(x,y)                   do{  if(x==0) y = MTx_STEPS_T7(1);\
                                           else if(x==1) y = MTx_STEPS_T7(2);\
                                           else if(x==2) y = MTx_STEPS_T7(3);\
                                           else if(x==3) y = MTx_STEPS_T7(4);\
                                           }while(0)

#define MT_RSTPOS(x,y)                     do{  if(x==0) y = MTx_RSTPOS(1);\
                                           else if(x==1) y = MTx_RSTPOS(2);\
                                           else if(x==2) y = MTx_RSTPOS(3);\
                                           else if(x==3) y = MTx_RSTPOS(4);\
                                           }while(0)

#define MT_nRRSTPOS(x,y)                   do{  if(x==0) y = MTx_nRRSTPOS(1);\
                                           else if(x==1) y = MTx_nRRSTPOS(2);\
                                           else if(x==2) y = MTx_nRRSTPOS(3);\
                                           else if(x==3) y = MTx_nRRSTPOS(4);\
                                           }while(0)

#define MT_ARRIVE(x,y)                     do{  if(x==0) y = MTx_ARRIVE(1);\
                                           else if(x==1) y = MTx_ARRIVE(2);\
                                           else if(x==2) y = MTx_ARRIVE(3);\
                                           else if(x==3) y = MTx_ARRIVE(4);\
                                           }while(0)

#define MT_CODE(x,y)                       do{  if(x==0) y = MTx_CODE(1);\
                                           else if(x==1) y = MTx_CODE(2);\
                                           else if(x==2) y = MTx_CODE(3);\
                                           else if(x==3) y = MTx_CODE(4);\
                                           }while(0)

#if 0
#define MT_CRASH(x,y)                      do{  if(x==0) y = MTx_CODE(1);\
                                           else if(x==1) y = MTx_CODE(2);\
                                           else if(x==2) y = MTx_CODE(3);\
                                           else if(x==3) y = MTx_CODE(4);\
                                           }while(0)
#endif
                                           
#define MT_LIGHTCHECK(x,y)                 do{  if(x==0) y = MTx_LIGHTCHECK(1);\
                                           else if(x==1) y = MTx_LIGHTCHECK(2);\
                                           else if(x==2) y = MTx_LIGHTCHECK(3);\
                                           else if(x==3) y = MTx_LIGHTCHECK(4);\
                                           }while(0)
                                           
#define MT_ISCIRCLE(x,y)                   do{  if(x==0) y = MTx_ISCIRCLE(1);\
                                           else if(x==1) y = MTx_ISCIRCLE(2);\
                                           else if(x==2) y = MTx_ISCIRCLE(3);\
                                           else if(x==3) y = MTx_ISCIRCLE(4);\
                                           }while(0)
                                           
#define MT_CIRCLE_STEPS(x,y)               do{  if(x==0) y = MTx_CIRCLE_STEPS(1);\
                                           else if(x==1) y = MTx_CIRCLE_STEPS(2);\
                                           else if(x==2) y = MTx_CIRCLE_STEPS(3);\
                                           else if(x==3) y = MTx_CIRCLE_STEPS(4);\
                                           }while(0)
                                           
#define MT_CIRCLE_CUPS(x,y)                do{  if(x==0) y = MTx_CIRCLE_CUPS(1);\
                                           else if(x==1) y = MTx_CIRCLE_CUPS(2);\
                                           else if(x==2) y = MTx_CIRCLE_CUPS(3);\
                                           else if(x==3) y = MTx_CIRCLE_CUPS(4);\
                                           }while(0)
                                         
#define MT_ELEVEL_POSI(x,y)                do{  if(x==0) y = MTx_ELEVEL_POSI(1);\
                                           else if(x==1) y = MTx_ELEVEL_POSI(2);\
                                           else if(x==2) y = MTx_ELEVEL_POSI(3);\
                                           else if(x==3) y = MTx_ELEVEL_POSI(4);\
                                           }while(0)

#define MT_RST_LIGHTCHECK(x,y)             do{  if(x==0) y = MTx_RST_LIGHTCHECK(1);\
                                           else if(x==1) y = MTx_RST_LIGHTCHECK(2);\
                                           else if(x==2) y = MTx_RST_LIGHTCHECK(3);\
                                           else if(x==3) y = MTx_RST_LIGHTCHECK(4);\
                                           }while(0)
                                           

_EXT_ UINT16 AccTimArr_Buf[][1024];
_EXT_ UINT16 DecTimArr_Buf[][1024];
                                           
UINT8 Mtx_Init(UINT8 MtxId);
#endif

