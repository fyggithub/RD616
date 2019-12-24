#ifndef __SAMPLE_PROBLE_H__
#define __SAMPLE_PROBLE_H__

#include "My_Type.h"

#define SP_ROT_MT       eMtx_ID_1       // ��ת���
#define SP_UD_MT        eMtx_ID_2       // ���µ��
#define SP_SU_MT        eMtx_ID_3       // ��Һ���

#define SP_R1_MV_POS       1            // �Լ�R1ƫ����
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
    rot_rst_state           = 0,            // ��ת�����λ״̬
    rot_reagent1_state      = 1,            // ��ת������Լ�1λ��
    rot_reagent2_state      = 2,            // ��ת������Լ�2λ��
    rot_reagent3_state      = 3,            // ��ת������Լ�3״̬
    rot_reagent4_state      = 4,            // ��ת������Լ�4״̬
    rot_sample_state        = 5,            // ��ת���������λ��״̬
    rot_wash_state          = 6,            // ��ת�������ϴλ��״̬
    rot_reaction_out_state  = 7,            // ��ת����ڷ�����λ��״̬
    rot_reaction_in_state   = 8,            // ��ת����ڷ�����λ��״̬
    rot_move_state          = 0xf,          // ��ת������˶���ָ������״̬
    rot_unknow_state        = 0xff,         // ��ת���״̬δ֪
}e_Rot_State;

typedef enum
{
    ud_rst_state        = 0,            // ��ֱ�����λ״̬
    ud_cup_rim_state    = 1,            // ��ֱ����ڱ���λ��
    ud_cup_bottom_state = 2,            // ��ֱ����ڱ���λ��
    ud_liquit_state     = 3,            // ��ֱ�����Һ��״̬
    ud_move_state       = 0xf,          // ��ֱ������˶���ָ������״̬
    ud_unknow_state     = 0xff,         // ��ֱ���״̬δ֪
}e_Ud_State;

typedef enum
{
    su_rst_state        = 0,            // ע�����ڸ�λ״̬
    su_row_state        = 1,            // ע��������Һ��״̬
    su_suck_state       = 2,            // ע��������Һ��״̬
    su_move_state       = 0xf,          // ��ֱ������˶���ָ������״̬
    su_unknow_state     = 0xff,         // ��ֱ���״̬δ֪
}e_Su_State;

typedef struct
{
    UINT8       run_state;        // ���״̬
    UINT8       rst_state;        // ��λ״̬
    e_Rot_State rot_state;        // ��ת����ڵ�λ��״̬
    UINT32      *rot_posi;        // ��ת����ڵĲ���
    e_Ud_State  ud_state;         // ���µ����λ��״̬
    UINT32      *ud_posi;         // ��ת����Ĳ���
    e_Su_State  su_state;         // ע�������״̬
    UINT32      *su_posi;         // ע��������Ĳ���
}st_SpPasiState;





_EXT_ st_SpPasiState s_SpState; 

UINT16 Handle_SP_Cmd(void);
void Liquit_Check_PinInit(void);

#endif
