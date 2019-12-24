#ifndef __CONFIGMTX_H__
#define __CONFIGMTX_H__

#include "My_type.h"

//====================================================================
// 加减速的控制模式
// 直接传递台阶和频率
#define STEPS_MODE          0
// 基于时间的加减速的控制
#define TIME_MODE           0
// 基于步数的加减速的控制
#define STEPS_ACCDEC_MODE          1


//=============================进杯上下电机==========================
// motor 1          
//#if (MTx_NUM >= 1)
#define MT_1_MAXFREQ                5000            // 最大频率值
#define MT_1_MINFREQ                1000            // 最小频率值

#define MT_1_MAXPOS                 7000           // 最大步数，如果是盘电机，就填写0xFFFF

#define MT_1_STEPS_T1               20              // 第一阶步数
#define MT_1_STEPS_T2               20              // 第二阶步数
#define MT_1_STEPS_T3               50              // 第三阶步数
#define MT_1_STEPS_T5               50              // 第五阶步数
#define MT_1_STEPS_T6               20              // 第六阶步数
#define MT_1_STEPS_T7               20              // 第七阶步数

#define MT_1_RSTPOS                 100             // 复位后运行的步数
#define MT_1_nRRSTPOS               100             // 向相反方向复位运行的步数
#define MT_1_ARRIVE                 10             // 到位触发运行步数，液面检查也是相当于到位触发
#define MT_1_CODE                   0               // 码盘触发运行步数

#define MT_1_LIGHTCHECK             0x01            // 第一位是复位光耦使能，第二位是到位光耦使能，
                                                    // 第三位为码盘光耦复位，第四位是碰撞光耦使能。
#define MT_1_ISCIRCLE               0               // 判断是否是盘电机
#define MT_1_CIRCLE_STEPS           0               // 盘电机一圈的步数
#define MT_1_CIRCLE_CUPS            0               // 盘电机的杯数
#define MT_1_ELEVEL_POSI            (MTx_Pin_Low)   // 电机正向运动的方向

#define MT_1_RST_LIGHTCHECK         MTx_Pin_Low

//#endif

//==========================吸液上下电机=============================
// motor 2
//#if (MTx_NUM >= 2)
#define MT_2_MAXFREQ                4000           // 最大频率值
#define MT_2_MINFREQ                1000            // 最小频率值

#define MT_2_MAXPOS                 15000           // 最大步数，如果是盘电机，就填写0xFFFF

#define MT_2_STEPS_T1               80              // 第一阶步数
#define MT_2_STEPS_T2               80              // 第二阶步数
#define MT_2_STEPS_T3               80              // 第三阶步数
#define MT_2_STEPS_T5               80              // 第五阶步数
#define MT_2_STEPS_T6               80              // 第六阶步数
#define MT_2_STEPS_T7               80              // 第七阶步数

#define MT_2_RSTPOS                 2500            // 复位后运行的步数
#define MT_2_nRRSTPOS               400             // 向相反方向复位运行的步数
#define MT_2_ARRIVE                 0               // 到位触发运行步数，液面检查也是相当于到位触发
#define MT_2_CODE                   0               // 码盘触发运行步数

#define MT_2_LIGHTCHECK             0x01            // 第一位是复位光耦使能，第二位是到位光耦使能，
                                                    // 第三位为码盘光耦复位，第四位是碰撞光耦使能。
#define MT_2_ISCIRCLE               0               // 判断是否是盘电机
#define MT_2_CIRCLE_STEPS           0               // 盘电机一圈的步数
#define MT_2_CIRCLE_CUPS            0               // 盘电机的杯数
#define MT_2_ELEVEL_POSI            (MTx_Pin_Low)   // 电机正向运动的方向
#define MT_2_RST_LIGHTCHECK         MTx_Pin_Low

//#endif

//==============================底物注射电机==============================
// motor 3
//#if (MTx_NUM >= 3)
#define MT_3_MAXFREQ                14000            // 最大频率值
#define MT_3_MINFREQ                1000            // 最小频率值

#define MT_3_MAXPOS                 17000           // 最大步数，如果是盘电机，就填写0xFFFF

#define MT_3_STEPS_T1               50              // 第一阶步数
#define MT_3_STEPS_T2               50              // 第二阶步数
#define MT_3_STEPS_T3               50              // 第三阶步数
#define MT_3_STEPS_T5               50              // 第五阶步数
#define MT_3_STEPS_T6               50              // 第六阶步数
#define MT_3_STEPS_T7               30              // 第七阶步数

#define MT_3_RSTPOS                 200             // 复位后运行的步数
#define MT_3_nRRSTPOS               200             // 向相反方向复位运行的步数
#define MT_3_ARRIVE                 0               // 到位触发运行步数，液面检查也是相当于到位触发
#define MT_3_CODE                   0               // 码盘触发运行步数

#define MT_3_LIGHTCHECK             0x01            // 第一位是复位光耦使能，第二位是到位光耦使能，
                                                    // 第三位为码盘光耦复位，第四位是碰撞光耦使能。
#define MT_3_ISCIRCLE               0               // 判断是否是盘电机
#define MT_3_CIRCLE_STEPS           0               // 盘电机一圈的步数
#define MT_3_CIRCLE_CUPS            0               // 盘电机的杯数
#define MT_3_ELEVEL_POSI            (MTx_Pin_Low)   // 电机正向运动的方向

#define MT_3_RST_LIGHTCHECK         MTx_Pin_Low
//#endif

//===============================底物注射电机2=================================
// motor 4
//#if (MTx_NUM >= 4)
#if 0
#define MT_4_MAXFREQ                14000            // 最大频率值
#define MT_4_MINFREQ                1000            // 最小频率值

#define MT_4_MAXPOS                 17000           // 最大步数，如果是盘电机，就填写0xFFFF

#define MT_4_STEPS_T1               50              // 第一阶步数
#define MT_4_STEPS_T2               50              // 第二阶步数
#define MT_4_STEPS_T3               50              // 第三阶步数
#define MT_4_STEPS_T5               50              // 第五阶步数
#define MT_4_STEPS_T6               50              // 第六阶步数
#define MT_4_STEPS_T7               50              // 第七阶步数

#define MT_4_RSTPOS                 200             // 复位后运行的步数
#define MT_4_nRRSTPOS               200             // 向相反方向复位运行的步数
#define MT_4_ARRIVE                 0              // 到位触发运行步数，液面检查也是相当于到位触发
#define MT_4_CODE                   0               // 码盘触发运行步数

#define MT_4_LIGHTCHECK             0x01            // 第一位是复位光耦使能，第二位是到位光耦使能，
                                                    // 第三位为码盘光耦复位，第四位是碰撞光耦使能。
#define MT_4_ISCIRCLE               0               // 判断是否是盘电机
#define MT_4_CIRCLE_STEPS           0               // 盘电机一圈的步数
#define MT_4_CIRCLE_CUPS            0               // 盘电机的杯数
#define MT_4_ELEVEL_POSI            (MTx_Pin_Low)   // 电机正向运动的方向

#define MT_4_RST_LIGHTCHECK         MTx_Pin_Low

#else
//=============================== 样本盘  =================================
#define MT_4_MAXFREQ                3300             // 最大频率值3700
#define MT_4_MINFREQ                600            // 最小频率值600

#define MT_4_MAXPOS                 0xFFFFFFFF           // 最大步数，如果是盘电机，就填写0xFFFF

#define MT_4_STEPS_T1               100              // 第一阶步数50
#define MT_4_STEPS_T2               50              // 第二阶步数80
#define MT_4_STEPS_T3               150              // 第三阶步数50
#define MT_4_STEPS_T5               100              // 第五阶步数50
#define MT_4_STEPS_T6               50              // 第六阶步数80
#define MT_4_STEPS_T7               150             // 第七阶步数50

#define MT_4_RSTPOS                 300             // 复位后运行的步数200
#define MT_4_nRRSTPOS               750             // 向相反方向复位运行的步数850
#define MT_4_ARRIVE                 0              // 到位触发运行步数，液面检查也是相当于到位触发
#define MT_4_CODE                   450               // 码盘触发运行步数

#define MT_4_LIGHTCHECK             0x05            // 第一位是复位光耦使能，第二位是到位光耦使能，
                                                    // 第三位为码盘光耦复位，第四位是碰撞光耦使能。
#define MT_4_ISCIRCLE               1               // 判断是否是盘电机
#define MT_4_CIRCLE_STEPS           24800               // 盘电机一圈的步数
#define MT_4_CIRCLE_CUPS            62               // 盘电机的杯数
#define MT_4_ELEVEL_POSI            (MTx_Pin_Low)   // 电机正向运动的方向

#define MT_4_RST_LIGHTCHECK         MTx_Pin_Low
#endif
//#endif
//=========================================================================
#define MTx_MAXFREQ(x)              MT_##x##_MAXFREQ        // 最大频率值
#define MTx_MINFREQ(x)              MT_##x##_MINFREQ        // 最小频率值

#define MTx_MAXPOS(x)               MT_##x##_MAXPOS         // 最大步数，如果是盘电机，就填写0xFFFF

#define MTx_STEPS_T1(x)             MT_##x##_STEPS_T1       // 第一阶步数
#define MTx_STEPS_T2(x)             MT_##x##_STEPS_T2       // 第二阶步数
#define MTx_STEPS_T3(x)             MT_##x##_STEPS_T3       // 第三阶步数
#define MTx_STEPS_T5(x)             MT_##x##_STEPS_T5       // 第五阶步数
#define MTx_STEPS_T6(x)             MT_##x##_STEPS_T6       // 第六阶步数
#define MTx_STEPS_T7(x)             MT_##x##_STEPS_T7       // 第七阶步数

#define MTx_RSTPOS(x)               MT_##x##_RSTPOS         // 复位后运行的步数
#define MTx_nRRSTPOS(x)             MT_##x##_nRRSTPOS       // 向相反方向复位运行的步数
#define MTx_ARRIVE(x)               MT_##x##_ARRIVE         // 到位触发运行步数，液面检查也是相当于到位触发
#define MTx_CODE(x)                 MT_##x##_CODE           // 码盘触发运行步数
//#define MTx_CRASH(x)                MT_##x##_CRASH          // 碰撞光耦

#define MTx_LIGHTCHECK(x)           MT_##x##_LIGHTCHECK     // 第一位是复位光耦使能，第二位是到位光耦使能，
                                                            // 第三位为码盘光耦复位，第四位是碰撞光耦使能。
#define MTx_ISCIRCLE(x)             MT_##x##_ISCIRCLE       // 判断是否是盘电机
#define MTx_CIRCLE_STEPS(x)         MT_##x##_CIRCLE_STEPS   // 盘电机一圈的步数
#define MTx_CIRCLE_CUPS(x)          MT_##x##_CIRCLE_CUPS    // 盘电机的杯数
#define MTx_ELEVEL_POSI(x)          MT_##x##_ELEVEL_POSI    // 电机正向运动的方向

#define MTx_RST_LIGHTCHECK(x)      MT_##x##_RST_LIGHTCHECK    // 电机正向运动的方向

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

