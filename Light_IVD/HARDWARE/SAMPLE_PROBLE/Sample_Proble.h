#ifndef __SAMPLE_PROBLE_H__
#define __SAMPLE_PROBLE_H__

#include "My_Type.h"

#define SP_ROT_MT       eMtx_ID_1       // 旋转电机
#define SP_UD_MT        eMtx_ID_2       // 上下电机
#define SP_SU_MT        eMtx_ID_3       // 吸液电机

#define SP_R1_MV_POS       1            // 试剂R1偏移量
#define SP_R2_MV_POS       2
#define SP_R3_MV_POS       3
#define SP_R4_MV_POS       4
#define SP_ST_MV_POS       5
#define SP_WS_MV_POS       6
#define SP_RC_MV_POS       7

#define SET_LIQUIT_PIN_LOW      GPIO_ResetBits(GPIOD,GPIO_Pin_9)
#define SET_LIQUIT_PIN_HIGH     GPIO_SetBits(GPIOD,GPIO_Pin_9)

_EXT_ UINT8 Liquit_Flag;

typedef enum
{
    rot_rst_state           = 0,            // 旋转电机复位状态
    rot_reagent1_state      = 1,            // 旋转电机在试剂1位置
    rot_reagent2_state      = 2,            // 旋转电机在试剂2位置
    rot_reagent3_state      = 3,            // 旋转电机在试剂3状态
    rot_reagent4_state      = 4,            // 旋转电机在试剂4状态
    rot_sample_state        = 5,            // 旋转电机在样本位置状态
    rot_wash_state          = 6,            // 旋转电机在清洗位置状态
    rot_reaction_out_state  = 7,            // 旋转电机在孵育盘位置状态
    rot_reaction_in_state   = 8,            // 旋转电机在孵育盘位置状态
    rot_move_state          = 0xf,          // 旋转电机在运动的指定步数状态
    rot_unknow_state        = 0xff,         // 旋转电机状态未知
}e_Rot_State;

typedef enum
{
    ud_rst_state        = 0,            // 垂直电机复位状态
    ud_cup_rim_state    = 1,            // 垂直电机在杯口位置
    ud_cup_bottom_state = 2,            // 垂直电机在杯底位置
    ud_liquit_state     = 3,            // 垂直电机在液面状态
    ud_move_state       = 0xf,          // 垂直电机在运动的指定步数状态
    ud_unknow_state     = 0xff,         // 垂直电机状态未知
}e_Ud_State;

typedef enum
{
    su_rst_state        = 0,            // 注射器在复位状态
    su_row_state        = 1,            // 注射器在排液的状态
    su_suck_state       = 2,            // 注射器在吸液的状态
    su_move_state       = 0xf,          // 垂直电机在运动的指定步数状态
    su_unknow_state     = 0xff,         // 垂直电机状态未知
}e_Su_State;

typedef struct
{
    UINT8       run_state;        // 组件状态
    UINT8       rst_state;        // 复位状态
    e_Rot_State rot_state;        // 旋转电机在的位置状态
    UINT32      *rot_posi;        // 旋转电机在的步数
    e_Ud_State  ud_state;         // 上下电机的位置状态
    UINT32      *ud_posi;         // 旋转电机的步数
    e_Su_State  su_state;         // 注射器电机状态
    UINT32      *su_posi;         // 注射器电机的步数
}st_SpPasiState;





_EXT_ st_SpPasiState s_SpState; 

UINT16 Handle_SP_Cmd(void);
void Liquit_Check_PinInit(void);

#endif
