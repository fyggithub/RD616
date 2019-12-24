#ifndef __SAMPLE_PROBLE_C__
#define __SAMPLE_PROBLE_C__

#include <includes.h>
#include "My_includes.h"

#define SU_UL_TO_STEPS(ul)          (64*ul)

UINT8 SP_QueBuf[256] = {0};

UINT8 Liquit_Flag = 0;
static UINT8 Dwarnret = 0;

st_SpPasiState s_SpState;           // ��ʼ����״̬�ṹ��

// ��ʼ��Һ����ķ����ź�����
void Liquit_Check_PinInit(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
 	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOD,GPIO_Pin_9);
}

// ��ת����˶���ָ��λ��
UINT16 SP_Rot_Move_to(UINT8 rot_pos,UINT16 time)
{
    UINT16 ret = ERR_NONE;
    UINT16 steps = 0;
    if(rot_pos != s_SpState.rot_state)
    {
        // Ҫ��֤����ڸ�λλ��
        if(s_SpState.ud_state != ud_rst_state)
        {
            ret = Mtx_FastRst(eMtx_ID_2,time);//���������µ��
            if(ret)
            {
                s_SpState.ud_state = ud_unknow_state;
                s_SpState.rst_state &= ~0x02;
                return ret;
            }
            s_SpState.ud_state = ud_rst_state;
        }
//        Sys_Delay_ms(100);
        steps = sp_para.SP_rot_pos[rot_pos];
        ret = Mtx_Move_To(SP_ROT_MT,steps,time);
        if(ret)
        {
            s_SpState.rot_state = rot_unknow_state;
            s_SpState.rst_state &= ~0x01;
            return ret;
        }
        s_SpState.rot_state = rot_pos;
        Sys_Delay_ms(80);
    }
    return ret;
}
// ��ֱ����˶���ָ��λ��
UINT16 SP_UD_Move_to(UINT8 ud_pos,UINT16 rot_pos,UINT16 time)
{
    UINT16 ret = ERR_NONE;
    UINT16 steps = 0;
    MtxCtrl.mtxPara[SP_UD_MT].nLightCheck &= (~MT_ARRIVE_LIGHT_ENABLE);
    
    s_SpState.ud_state = ud_pos;
    switch(ud_pos)
    {
        case ud_cup_rim_state:
        {
            steps = sp_para.SP_up_pos[rot_pos-1];// �뱭��ƫ�Ʋ���((����2��Ӧ���ǲ���1)
            break;
        }
        case ud_cup_bottom_state:
        {
            steps = sp_para.SP_dowm_pos[rot_pos-1];// �뱭��ƫ�Ʋ���
            break;
        }
        case ud_liquit_state:
        {
            // Һ�������ֹͣ����
            MtxCtrl.mtxPara[SP_UD_MT].nStepsT5 = 12;
            MtxCtrl.mtxPara[SP_UD_MT].nStepsT6 = 0;
            MtxCtrl.mtxPara[SP_UD_MT].nStepsT7 = 12;
            MtxCaculateAccDec(SP_UD_MT);
            steps = sp_para.SP_dowm_pos[rot_pos-1];// �뱭��ƫ�Ʋ���
            MtxCtrl.mtxPara[eMtx_ID_2].nLightCheck |= MT_ARRIVE_LIGHT_ENABLE;       // ����Һ����
            break;
        }
        default:
            break;
    }

    ret = Mtx_Move_To(eMtx_ID_2,steps,time);
    // �ָ�Ĭ�ϼӼ���
    if(ud_pos == ud_liquit_state)
    {
        MT_STEPS_T5(SP_UD_MT, MtxCtrl.mtxPara[SP_UD_MT].nStepsT5);
        MT_STEPS_T6(SP_UD_MT, MtxCtrl.mtxPara[SP_UD_MT].nStepsT6);
        MT_STEPS_T7(SP_UD_MT, MtxCtrl.mtxPara[SP_UD_MT].nStepsT7);
        MtxCaculateAccDec(SP_UD_MT);
    }
    MtxCtrl.mtxPara[SP_UD_MT].nLightCheck &= (~MT_ARRIVE_LIGHT_ENABLE);     // �ر�Һ����
    if(ret)
    {
        s_SpState.ud_state = ud_unknow_state;
        s_SpState.rst_state &= ~0x02;
    }
    return ret;
}

// ���������˶���ָ��λ��
UINT16 SP_Move_To(UINT8 *buf)
{
    UINT16 ret = 0;
    UINT16 time = 0;
    UINT8 rot_pos = 0;
    UINT8 ud_pos = 0;
    UINT16 steps = 0;
    OS_ERR err;
    
    rot_pos = buf[0];//��ת��λ��
    // λ�ò������󣬽��б���
    if(rot_pos>8)
        return (ERR_DATA_PARA | (0x01<<0x08));
    
    ud_pos = buf[1];//s���µ�λ��
    if(ud_pos>3)
        return (ERR_DATA_PARA | (0x02<<0x08));
    
    time = (buf[2]<<8) + buf[3];//time
    
    // �ж����Ƿ��ڵ�ǰλ��
    if(rot_pos != s_SpState.rot_state)
    {
        ret = SP_Rot_Move_to(rot_pos,time);
        if(ret)
            return ret;
    }
    else
    {
        // �Ѿ��ڵ�ǰλ�ã��Ͳ����У�ֱ���˳�
        if(ud_pos == s_SpState.ud_state)
            return ERR_NONE;
    }
    
    ret = SP_UD_Move_to(ud_pos,rot_pos,time);
    
    return ret;
}

// ������˶���ָ��λ����Һ
UINT16 SP_Suction_Move_To(UINT8 *buf)
{
    UINT16 ret = 0;
    UINT16 time = 0;
    UINT8 rot_pos = 0;
    UINT8 ud_pos = 0;
    UINT8 su_mode = 0;
    UINT16 steps = 0;
    UINT16 count_steps = 0;//�洢�����Ĳ���
    UINT16 ud_pos_ctrl = 0;
    OS_ERR err;
    OS_TICK ts1,ts2;
    
    //============================================================
    // ��ȡ����
    rot_pos = buf[0];//��ת��λ��
    // λ�ò������󣬽��б���
    if(rot_pos>8)
        return (ERR_DATA_PARA | (0x01<<0x08));
    
    ud_pos = buf[1];//s���µ�λ��
    // �ڶ�������������б���
    if(ud_pos>3)
        return (ERR_DATA_PARA | (0x02<<0x08));
    
    su_mode = buf[2];//����λ��
    // ����������������б���
    if(su_mode>1)
        return (ERR_DATA_PARA | (0x03<<0x08));
    
    ud_pos_ctrl =  (buf[3]<<8) + buf[4];//
    time = (buf[11]<<8) + buf[12];//time
    
    // Ĭ��ʱ��3.5s
    if(!time)
        time = 3500;
    
    // ��ȡʱ��
    ts1 = Sys_GetTicks();
    // �ж����Ƿ��ڵ�ǰλ��
    if(rot_pos != s_SpState.rot_state)
    {
        ret = SP_Rot_Move_to(rot_pos,time);
        if(ret)
            return ret;
    }
    else
    {
        // �Ѿ��ڵ�ǰλ�ã��Ͳ����У�ֱ���˳�
        if(ud_pos == s_SpState.ud_state)
            return ERR_NONE;
    }
    // ����ʱ��
    ts2 = Sys_GetTicks() - ts1;
    if(ts2>time)
        return ERR_TIME_NOT_ENOUGH;
    ts2 = ts2-time;
    
    if(!ud_pos_ctrl)
        ret = SP_UD_Move_to(ud_pos,rot_pos,ts2);
    // ʣ��߶Ȳ���
    else
    {
        if(ud_pos_ctrl < sp_para.SP_dowm_pos[rot_pos-1]) 
        {
            steps = sp_para.SP_dowm_pos[rot_pos-1] - ud_pos_ctrl;
        }
        else
            return ERR_DATA_PARA | (0x04<<0x08);
        s_SpState.ud_state = ud_move_state;
        ret = Mtx_Move_To(SP_UD_MT,steps,ts2);
        if(ret)
        {
            s_SpState.ud_state = ud_unknow_state;
            return ret;
        }
    }
     // ����ʱ��
    ts2 = Sys_GetTicks() - ts1;
    if(ts2>time)
        return ERR_TIME_NOT_ENOUGH;
    ts2 = ts2-time;
    
    // �رշ�
    SP_Valve_Out_OFF;
    
    s_SpState.su_state = su_mode+1;
    // ����Һ��
    steps = (buf[5]<<8) + buf[6];
    steps = SU_UL_TO_STEPS(steps) + SU_UL_TO_STEPS(((buf[9]<<8) + buf[10]));
    ret = Mtx_Move(SP_SU_MT,steps,su_mode,ts2);
    if(ret)
    {
        s_SpState.su_state = su_unknow_state;
        return ret;
    }
    
    // ����ʱ��
    ts2 = Sys_GetTicks() - ts1;
    if(ts2>time)
        return ERR_TIME_NOT_ENOUGH;
    ts2 = ts2-time;
    // ��Һ����
    if(su_mode)
    {
        Sys_Delay_ms(50);
        steps = SU_UL_TO_STEPS(sp_para.SP_z_back);
        ret = Mtx_Move(SP_SU_MT,steps,0,ts2);
        if(ret)
        {
            s_SpState.su_state = su_unknow_state;
            return ret;
        }
            
    }
    Sys_Delay_ms(80);   
    // ����ʱ��
    ts2 = Sys_GetTicks() - ts1;
    if(ts2>time)
        return ERR_TIME_NOT_ENOUGH;
    ts2 = ts2-time;
//     steps = sp_para.SP_dowm_pos[rot_pos-1];
//    ret = Mtx_Move_To(SP_UD_MT,0,time);
    ret = Mtx_FastRst(SP_UD_MT,ts2);//���������µ��2019-05-09�޸�˦Һ
    if(ret)
    {
        s_SpState.ud_state = ud_rst_state;
        return ret;
    } 
    // ����ʱ��
    ts2 = Sys_GetTicks() - ts1;
    if(ts2>time)
        return ERR_TIME_NOT_ENOUGH;
    ts2 = ts2-time;
    // �ϵ�����λ�ã�������������������
    // �����ϴ��ʹ�õ�
    // �Ƿ�������
    steps = (buf[7]<<8) + buf[8];
    if(steps)
    {
        steps = SU_UL_TO_STEPS(steps);
        ret = Mtx_Move(SP_SU_MT,steps,1,ts2);    
        if(ret)
        {
            s_SpState.su_state = su_unknow_state;
            return ret;
        }
    }
    // ����ʱ��
    ts2 = Sys_GetTicks() - ts1;
    if(ts2>time)
        return ERR_TIME_NOT_ENOUGH;
    ts2 = ts2-time;
    
    // ��ת����˶���ָ��λ�ô�
    rot_pos = buf[13];
    if(rot_pos != s_SpState.rot_state)
    {
        ret = SP_Rot_Move_to(rot_pos,ts2);
        if(ret)
            return ret;
    }
    
    return ret;
}

UINT8 SP_Wash(UINT16 time)
{
    UINT8 ret = 0;
    UINT8 rot_pos = 0;
    UINT8 ud_pos = 0;
    UINT16 wash_time = 0;
    UINT16 steps = 0;
    OS_TICK ts1,ts2;
    
    OS_ERR err;
    
    // ��ϴĬ��ʱ��2s
    if(!time)
        time = 2000;
    wash_time = time-200;
    
    ts1 = Sys_GetTicks();          // ��ȡʱ��
    
    ret = SP_Rot_Move_to(SP_WS_MV_POS,time);
    
    // ����ʱ��
    ts2 = Sys_GetTicks() - ts1;
    if(ts2>time)
        return ERR_TIME_NOT_ENOUGH;
    ts2 = ts2-time;
    
    // �򿪷����½�
    SP_Valve_Out_ON;
    // �򿪷�Һ��
    Thomas_ON;    
    steps = sp_para.SP_dowm_pos[SP_WS_MV_POS-1];// �뱭��ƫ�Ʋ���
    s_SpState.ud_state = ud_cup_bottom_state;
    ret = Mtx_Move_To(SP_UD_MT,steps,ts2);
    if(ret)
    {
        s_SpState.ud_state = ud_unknow_state;
        return ret;
    }       
    
    // ������ڱ�
    SP_Pump_In_ON;
    Sys_Delay_ms(50);    
    SP_Pump_Out_ON;
    
    // ����ʱ��
    ts2 = Sys_GetTicks() - ts1;
    if(ts2>time)
        return ERR_TIME_NOT_ENOUGH;
    ts2 = ts2-time;
    
    s_SpState.su_state = su_rst_state;
    // ע�������ٸ�λ
    ret = Mtx_FastRst(SP_SU_MT,ts2);
    if(ret)
    {
        s_SpState.su_state = su_unknow_state;
        return ret;
    }
    
    Sys_Delay_ms(20);
    // ����ʱ��
    ts2 = Sys_GetTicks() - ts1;
    if(ts2>time)
        return ERR_TIME_NOT_ENOUGH;
    ts2 = ts2-time;
    
    steps = 0;
    s_SpState.su_state = su_suck_state;
    ret = Mtx_Move(SP_SU_MT,sp_para.SP_z_pos+steps,1,time);
    if(ret)
    {
        s_SpState.su_state = su_unknow_state;
        return ret;
    }
    
    // ��ϴʱ��
    ts2 = Sys_GetTicks() - ts1;
    if(ts2>time)
        return ERR_TIME_NOT_ENOUGH;
    if(ts2 < (wash_time-200))
        Sys_Delay_ms(wash_time-ts2-200);
    
    // �ر�����ڱ�
    SP_Pump_Out_OFF;
    Sys_Delay_ms(50);   
    SP_Pump_In_OFF;
    Sys_Delay_ms(250);
    
    // �رշ�
    SP_Valve_Out_OFF;
    Sys_Delay_ms(40);
    
    // ����ʱ��
    ts2 = Sys_GetTicks() - ts1;
    if(ts2>time)
        return ERR_TIME_NOT_ENOUGH;
    ts2 = ts2-time;
    // ��30ul
    s_SpState.su_state = su_row_state;
    ret = Mtx_Move(SP_SU_MT,steps,0,time);
    if(ret)
    {
        s_SpState.su_state = su_unknow_state;
        return ret;  
    }  

    // ����ʱ��
    ts2 = Sys_GetTicks() - ts1;
    if(ts2>time)
        return ERR_TIME_NOT_ENOUGH;
    ts2 = ts2-time;
    // �ص���ߵ�
    s_SpState.ud_state = ud_rst_state;
    ret = Mtx_FastRst(SP_UD_MT,time);
    if(ret)
    {
        s_SpState.ud_state = ud_unknow_state;
        return ret;
    }
     //    Sys_Delay_ms(200);
    // �رշ�Һ��
    Thomas_OFF;  
    // ��������  
    // ����ʱ��
    ts2 = Sys_GetTicks() - ts1;
    if(ts2>time)
        return ERR_TIME_NOT_ENOUGH;
    ts2 = ts2-time;
    steps = SU_UL_TO_STEPS(sp_para.SP_z_back);
    
    s_SpState.su_state = su_suck_state;
    ret = Mtx_Move(SP_SU_MT,steps,1,time);
    if(ret)
    {
        s_SpState.su_state = su_unknow_state;
        return ret; 
    }        

    return ret; 
}

// ��λ�������
UINT8 SPMtx_Rst(UINT8 MtxId,UINT16 rstfreq,UINT16 time)
{
    UINT8 ret = ERR_NONE;
    UINT16 steps = 0;
    OS_ERR err;
    UINT16 maxfreq = 0;
    //===============================================
    // ��λ�ٶ�Ϊ����ٶȵĶ���֮һ
    maxfreq = MtxCtrl.mtxPara[MtxId].nFreqMax;
    MtxCtrl.mtxPara[MtxId].nFreqMax = rstfreq;
    MtxCaculateAccDec(MtxId);
    
    ret = Mtx_Rst(MtxId,time);
    
    // ���û�ԭʼ�ٶ�
    MtxCtrl.mtxPara[MtxId].nFreqMax = maxfreq;
    MtxCaculateAccDec(MtxId);

    return ret;
}

// ��λ�����뵥Ԫ���
UINT16 SP_Rst_ALL(UINT16 time)
{
    UINT16 ret = ERR_NONE;
    UINT16 steps = 0;
    OS_ERR err;
    UINT16 maxfreq = 0;
    OS_TICK ts1 = 0,ts2 = 0;
    
    if(!time)
        time = 10000;
    
    ts1 = Sys_GetTicks();
    
    s_SpState.ud_state = ud_unknow_state;
    s_SpState.rst_state &= ~0x01;
    // ��λ��ֱ���
    ret = SPMtx_Rst(SP_UD_MT,MtxCtrl.mtxPara[SP_UD_MT].nFreqMax>>1,time);
    if(ret)
        return ret;
    s_SpState.ud_state = ud_rst_state;
    s_SpState.rst_state |= 0x01;
    Sys_Delay_ms(100);
    
    ts2 = Sys_GetTicks() - ts1; 
    if(ts2 > time)
        return ERR_TIME_NOT_ENOUGH;
    
    time = time-ts2;                // ��ȡʣ�ิλʱ��
    
    s_SpState.rot_state = rot_unknow_state;
    s_SpState.rst_state &= ~0x02;
    // ��λˮƽ���
    ret = SPMtx_Rst(SP_ROT_MT,MtxCtrl.mtxPara[SP_ROT_MT].nFreqMax>>1,time);
    if(ret)
        return ret;
    s_SpState.rot_state = rot_rst_state;
    s_SpState.rst_state |= 0x02;
    Sys_Delay_ms(100);
    
    ts2 = Sys_GetTicks() - ts1; 
    if(ts2 > time)
        return ERR_TIME_NOT_ENOUGH;
    
    time = time-ts2;                // ��ȡʣ�ิλʱ��
    
    // �˶�����ϴλ��
    steps = sp_para.SP_rot_pos[SP_WS_MV_POS];
    ret = Mtx_Move_To(SP_ROT_MT,steps,time);
    if(ret)
    {
        s_SpState.rst_state &= ~0x02;
        s_SpState.rot_state = rot_unknow_state;
        return ret;
    }
    
    s_SpState.rot_state = rot_wash_state;
    
    Sys_Delay_ms(100);
    ts2 = Sys_GetTicks() - ts1; 
    if(ts2 > time)
        return ERR_TIME_NOT_ENOUGH;
    
    time = time-ts2;                // ��ȡʣ�ิλʱ��
    
    s_SpState.su_state = su_unknow_state;
    s_SpState.rst_state &= ~0x04;
    // ��λע�������
    ret = SPMtx_Rst(SP_SU_MT,(MtxCtrl.mtxPara[SP_SU_MT].nFreqMax<<1)/3,time);
    if(ret)
    {
        return ret;
    }
    s_SpState.su_state = su_rst_state;
    s_SpState.rst_state |= 0x04;
    
    Sys_Delay_ms(100);
    
    ts2 = Sys_GetTicks() - ts1; 
    if(ts2 > time)
        return ERR_TIME_NOT_ENOUGH;
    
    time = time-ts2;                // ��ȡʣ�ิλʱ��
    // ע����ϴҺ����
    ret = Mtx_Move_To(SP_SU_MT,sp_para.SP_z_pos,time);
    if(ret)
    {
        // ����״̬��Ϊδ֪
        s_SpState.rst_state &= ~0x04;
        s_SpState.su_state = su_unknow_state;
        return ret;
    }
    
    return ret;
}

// ������λ
UINT16 SP_Rst(UINT8 SpId,UINT16 time)
{
    UINT16 ret = ERR_NONE;
    UINT16 steps = 0;
    OS_ERR err;
    UINT16 maxfreq = 0;
    
    switch(SpId)
    {
        case 0:
        {
            ret = SP_Rst_ALL(time);
            break;
        }
        case 1:
        {
            s_SpState.rst_state &= ~0x01;
            // ��ֱ������ڸ�λλ�ã������и�λ
            if(s_SpState.ud_state != rot_rst_state)
            {
                ret = ERR_MOTOR_NOT_RESET | (SP_UD_MT+1)<<0x08;
                return ret;
            }
            s_SpState.rot_state = rot_unknow_state;
            ret = SPMtx_Rst(SP_ROT_MT,MtxCtrl.mtxPara[SP_ROT_MT].nFreqMax>>1,time);
            if(ret)
                return ret;
            s_SpState.rot_state = rot_rst_state;
            s_SpState.rst_state |= 0x01;
            break;
        }
        case 2:
        {
            s_SpState.rst_state &= ~0x02;
            s_SpState.ud_state = ud_unknow_state;
            ret = SPMtx_Rst(SP_UD_MT,MtxCtrl.mtxPara[SP_UD_MT].nFreqMax>>1,time);
            if(ret)
                return ret;
            s_SpState.ud_state = ud_rst_state;
            s_SpState.rst_state |= 0x02;
            
            break;
        }
        case 3:
        {
            s_SpState.rst_state &= ~0x03;
            s_SpState.su_state = su_unknow_state;
            ret = SPMtx_Rst(SP_SU_MT,(MtxCtrl.mtxPara[SP_SU_MT].nFreqMax<<1)/3,time);
            if(ret)
                return ret;
            s_SpState.su_state = su_rst_state;
            s_SpState.rst_state |= 0x04;
            break;
        }
        default:
        {
            ret = ERR_MOTOR_PARA | (0x01<<0x08);
            break;
        }
    }
    return ret;  
}


UINT8 SP_Liquit_Check(void)
{
    UINT16 t = 0;
    SET_LIQUIT_PIN_HIGH;
    Sys_Delay_ms(10);
    while(1)
    {
        t++;
        if(MtxCtrl.mtxFunList.mtArriveRead(eMtx_ID_2))
        {
            SET_LIQUIT_PIN_LOW;
            break;
        }
        if(t == 30)
        {
            SET_LIQUIT_PIN_LOW;
            return ERR_LIQUIT_CHECK;
        }
        Sys_Delay_ms(10);
    }
    SET_LIQUIT_PIN_LOW;
    return ERR_NONE;
}

// ���������˶�
UINT16 Sp_Ctrl_Move(UINT8 *buf)
{
    UINT16 ret = ERR_NONE;
    UINT8 mtxId = 0;
    UINT8 dir = 0;
    UINT16 time = 0;
    UINT16 steps = 0;
    UINT8 *mt_state;
    // �жϲ����Ƿ����

    mtxId = buf[0];
    
    switch(mtxId)
    {
        case 1:
            mt_state = (UINT8 *)&s_SpState.rot_state;
            break;
        case 2:
            mt_state = (UINT8 *)&s_SpState.ud_state;
            break;
        case 3:
            mt_state = (UINT8 *)&s_SpState.su_state;
            break;
        default:
            ret = ERR_DATA_PARA | (0x01<<0x08);
            return ret;
    }
    *mt_state = 0xf;
    time = (sp_msg.st_dataBuf[4]<<8) + (sp_msg.st_dataBuf[5]&0xFF);
    steps = (sp_msg.st_dataBuf[2]<<8) + (sp_msg.st_dataBuf[3]&0xFF);
    ret = Mtx_Move(mtxId-1,steps,buf[1],time);
    if(ret)
    {
        *mt_state = 0xFF;
    }
    return ret;
}

UINT16 Handle_SP_Cmd(void)
{
    static UINT8 LIQUIT_CHECK_FLAG = 0;
    UINT16 ret = ERR_NONE;
    UINT16 time = 0;
    UINT16 pos = 0;
    UINT8  dir = 0;
    UINT16 maxfreq = 0;
    UINT8 needle_cmd = 0;
    OS_ERR err;
    
    // ���뱣֤��λ���Ժ�������˶�
    if(sp_msg.st_dataCmd != CMD_MT_SP_RST && s_SpState.rst_state < 0x3)
    {
        ret = ERR_MOTOR_NOT_RESET;
        return ret;
    }
    
    switch(sp_msg.st_dataCmd)
    {
        // �������λ
        case CMD_MT_SP_RST://20
        {
            if(sp_msg.st_dataLen != 3)
            {
                ret = ERR_FRAM_LENGTH;
                break;
            }
            
            ret = SP_Rst(sp_msg.st_dataBuf[0],time);
            sp_msg.st_dataLen = 0;
            break;
        }
        // ���������˶�
        case CMD_MT_SP_MOVE://21
        {
            if(sp_msg.st_dataLen != 6)
            {
                ret = ERR_FRAM_LENGTH;
                break;
            }
            ret = Sp_Ctrl_Move(sp_msg.st_dataBuf);
            
            sp_msg.st_dataLen = 0;
            break;
        }
        // ���������˶���ָ��λ��
        case CMD_MT_SP_MOVE_TO://22
        {
            if(sp_msg.st_dataLen != 4)
            {
                ret = ERR_FRAM_LENGTH;
                break;
            }

            ret = SP_Move_To(sp_msg.st_dataBuf);
            sp_msg.st_dataLen = 0;
           
            break;
        }
        // ���˶���ָ��λ������Һ
        case CMD_MT_SP_SUCTION_MOVE_TO://23
        {
            if(sp_msg.st_dataLen != 14)
            {
                ret = ERR_FRAM_LENGTH;
                break;
            }
            ret = SP_Suction_Move_To(sp_msg.st_dataBuf);
            if(ret)
            {
                ret = Mtx_FastRst(SP_UD_MT,0);//���������µ��
                if(!ret)
                    s_SpState.ud_state = ud_rst_state;
            }

            sp_msg.st_dataLen = 0;
            break;
        }
        case CMD_MT_SP_CLEAN://24
        {
            if(sp_msg.st_dataLen != 2)
            {
                ret = ERR_FRAM_LENGTH;
                break;
            }

            time = (sp_msg.st_dataBuf[0]<<8) + (sp_msg.st_dataBuf[1]&0xFF);
            ret = SP_Wash(time);
           
            if(ret)
            {
                SP_Pump_In_OFF;
                SP_Pump_Out_OFF;
                SP_Valve_Out_OFF;
                Thomas_OFF;
            }
            sp_msg.st_dataLen = 0;
//            rt_msg.st_dataBuf[0] = SYS_VERSION;
//            rt_msg.st_dataLen = 1;
            break;
        }
        case CMD_MT_SP_SUCTION_MOVE://25
        {
            if(sp_msg.st_dataLen != 5)
            {
                ret = ERR_FRAM_LENGTH;
                break;
            }
            dir = sp_msg.st_dataBuf[0];
            pos = (sp_msg.st_dataBuf[0]<<8) + (sp_msg.st_dataBuf[1]&0xFF);
            pos = SU_UL_TO_STEPS(pos);
            ret = Mtx_Move(SP_SU_MT,pos,dir,time);
            
            // ��Һ����
            if(dir)
            {
                Sys_Delay_ms(50);
                pos = SU_UL_TO_STEPS(sp_para.SP_z_back);
                ret = Mtx_Move(SP_SU_MT,pos,0,time);
                
            }
            sp_msg.st_dataLen = 0;
//            rt_msg.st_dataBuf[0] = SYS_VERSION;
//            rt_msg.st_dataLen = 1;
            break;
        }
        case CMD_MT_SP_LIQUIT_CHECK:    // 0x26
        {
            if(sp_msg.st_dataLen != 0)
            {
                ret = ERR_FRAM_LENGTH;
                break;
            }
            ret = SP_Liquit_Check();
            LIQUIT_CHECK_FLAG = 1;
            break;
        }
       
        default:
            ret = ERR_MOTOR_PARA;
            break;
    }
    
    return ret;
}


void Sample_Proble_Task(void *p_arg)
{
    OS_ERR      err;
    UINT8 Sp_buf[64] = {0};
    UINT16 ret = ERR_NONE;
    OS_TICK ts;
    
    st_QueWaitPara s_spWaitPara;
    
    (void)p_arg;
    
    // ��ʼ����������Ϣ�Ļ�ȡ����
    s_spWaitPara.data_len = 0;
    s_spWaitPara.get_len = 0;
    s_spWaitPara.len = 0;
    s_spWaitPara.RecStaue = Idel;
    
    s_SpState.run_state = FALSE;
    s_SpState.rot_state = rot_unknow_state;
    s_SpState.rot_posi  = &MtxCtrl.mtxCount[SP_ROT_MT].mStepsCur;
    s_SpState.ud_state  = ud_unknow_state;
    s_SpState.ud_posi   = &MtxCtrl.mtxCount[SP_UD_MT].mStepsCur;
    s_SpState.su_state  = su_unknow_state;
    s_SpState.su_posi   = &MtxCtrl.mtxCount[SP_SU_MT].mStepsCur;
    
    
    queue_inital(SP_QueBuf,256);
    MtxCtrl.mtxPara[eMtx_ID_2].nLightCheck &= (~MT_ARRIVE_LIGHT_ENABLE);
    while(1)
    {	
        OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,&ts,&err);
        
        while(queue_len_query(SP_QueBuf))
        {
            if(Get_DataMsg(SP_QueBuf,&sp_msg, &s_spWaitPara) == TRUE)
            {     
                
                sp_msg.st_dataLen -= 1;
                // �жϵ����ֱ��ˮƽ�Ƿ�λ������λ���������У�����
                
                s_SpState.run_state = TRUE;
                
                ret = Handle_SP_Cmd();
                s_SpState.run_state = FALSE;
                printf("timing task finish\r\n");
            }
        }
        
        sp_msg.st_dataErrNum = ret;
        Mid_ReBack_Fill(&sp_msg);
        if(Dwarnret)
        {
            sp_msg.st_dataErrNum = Dwarnret;
            Mid_ReBack_Fill(&sp_msg);
            Dwarnret = 0;
        }
        
    
    }   
}


#endif
