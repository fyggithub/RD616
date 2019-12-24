#ifndef __ENTERCUP_C__
#define __ENTERCUP_C__

#include "My_includes.h"

UINT8 EC_QueBuf[128] = {0};

UINT8 Auto_Enter_Flag = 1;

UINT8 Enter_Cup_Rst_Flag = 0;

UINT8 Run_State = 0;

void Enter_Cup_Check_Light(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(ENTERCUP_UP_CHECK_CLK | ENTERCUP_DOWN_CHECK_CLK | ENTERCUP_MOVE_CHECK_CLK,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin     = ENTERCUP_UP_CHECK_PIN;

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   
    
	GPIO_Init(ENTERCUP_UP_CHECK_PORT,&GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin     = ENTERCUP_DOWN_CHECK_PIN;
    GPIO_Init(ENTERCUP_DOWN_CHECK_PORT,&GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin     = ENTERCUP_MOVE_CHECK_PIN;
    GPIO_Init(ENTERCUP_MOVE_CHECK_PORT,&GPIO_InitStructure);
}

// 推力电磁铁电源开关控制信号
void Spring_Pin_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(ENTERCUP_SPRING_POWER_CLK,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin     = ENTERCUP_SPRING_POWER_PIN;
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

    
	GPIO_Init(ENTERCUP_SPRING_POWER_PORT,&GPIO_InitStructure);
	
	GPIO_ResetBits(ENTERCUP_SPRING_POWER_PORT,ENTERCUP_SPRING_POWER_PIN); //GPIOF 高电平
}

void Spring_Set(void)
{
    ENTERCUP_SPRING_POWER_PORT->BRR = ENTERCUP_SPRING_POWER_PIN;
}

void Spring_Reset(void)
{
    ENTERCUP_SPRING_POWER_PORT->BSRR = ENTERCUP_SPRING_POWER_PIN;
}


UINT8 Get_UpLight_State(void)
{
    if(ENTERCUP_UP_CHECK_PORT->IDR & ENTERCUP_UP_CHECK_PIN)
        return Bit_SET;
    else
        return Bit_RESET;
}

UINT8 Get_DowmLight_State(void)
{
    if(ENTERCUP_DOWN_CHECK_PORT->IDR & ENTERCUP_DOWN_CHECK_PIN)
        return Bit_SET;
    else
        return Bit_RESET;
}

UINT8 Get_MoveLight_State(void)
{
    if(ENTERCUP_MOVE_CHECK_PORT->IDR & ENTERCUP_MOVE_CHECK_PIN)
        return Bit_SET;
    else
        return Bit_RESET;
}

UINT8 Judge_Light_State(void)
{
    UINT8 light_flag = 0;
    light_flag = (Get_UpLight_State()<<1) | Get_DowmLight_State();
    return light_flag;
}

UINT8 Handle_EC_Cmd(void)
{
    static UINT8 LIQUIT_CHECK_FLAG = 0;
    UINT16 ret = ERR_NONE;
    UINT16 time = 0;
    UINT16 pos = 0;
    UINT8  dir = 0;
    UINT16 maxfreq = 0;
    UINT8 needle_cmd = 0;
    OS_ERR err;
    
    
    switch(ec_msg.st_dataCmd)
    {
        // 进杯器上下复位
        case CMD_MT_EC_RST://40
        {
            if(ec_msg.st_dataLen != 2)
            {
                ret = ERR_FRAM_LENGTH;
                break;
            }
            if(!Run_State)
            {
                time = FunUnionTwoByte(ec_msg.st_dataBuf[0],ec_msg.st_dataBuf[1]);
                ret = Mtx_Rst(EnterCupMt,time);
                if(!ret)
                    Enter_Cup_Rst_Flag = 1;
            }
            

            ec_msg.st_dataLen = 0;
            break;
        }
        // 进杯器电机运动
        case CMD_MT_EC_MOVE://41
        {
            if(ec_msg.st_dataLen != 5)
            {
                ret = ERR_FRAM_LENGTH;
                break;
            }
            if(!Run_State)
            {
                dir = ec_msg.st_dataBuf[0];
                pos = FunUnionTwoByte(ec_msg.st_dataBuf[1],ec_msg.st_dataBuf[2]);
                time = FunUnionTwoByte(ec_msg.st_dataBuf[3],ec_msg.st_dataBuf[4]);
                ret = Mtx_Move(EnterCupMt,pos, dir,time);
                if(ret)
                {
                    Enter_Cup_Rst_Flag = 0;
                }
            }
            


            ec_msg.st_dataLen = 0;
            break;
        }
        // 进杯器电机运动到指定位置
        case CMD_MT_EC_MOVE_TO://42
        {
            if(ec_msg.st_dataLen != 3)
            {
                ret = ERR_FRAM_LENGTH;
                break;
            }
            if(!Run_State)
            {
                time = FunUnionTwoByte(ec_msg.st_dataBuf[1],ec_msg.st_dataBuf[2]);
                if(ec_msg.st_dataBuf[0] == 1)
                {
                    ret = Mtx_Move_To(EnterCupMt,s_ecpara.ec_ud_pos[1],time);
                }
                else if(ec_msg.st_dataBuf[0] == 0)
                    ret = Mtx_FastRst(EnterCupMt,time);
                if(ret)
                    Enter_Cup_Rst_Flag = 0;
            }
            


            ec_msg.st_dataLen = 0;
           
            break;
        }
        // 进杯器自动进杯控制
        case CMD_MT_AUTO_ENTER://48
        {
            if(ec_msg.st_dataLen != 1)
            {
                ret = ERR_FRAM_LENGTH;
                break;
            }
            if(ec_msg.st_dataBuf[0] && (Auto_Enter_Flag == 0))
            {
                
                if(!Enter_Cup_Rst_Flag)
                {
                    ret = Mtx_Rst(EnterCupMt,0);
                    if(!ret)
                        Enter_Cup_Rst_Flag = 1;
                }
                Auto_Enter_Flag = 1;
                OSTaskSemPost(&AutoEnterCupCtrlTaskTCB,OS_OPT_POST_NONE,&err);
            }
            else if(!ec_msg.st_dataBuf[0])
                Auto_Enter_Flag = 0;
            ec_msg.st_dataLen = 0;
            break;
        }
        // 进杯器推杯运动
        case CMD_SPRING_CTRL://49
        {
            if(ec_msg.st_dataLen != 1)
            {
                ret = ERR_FRAM_LENGTH;
                break;
            }
            if(ec_msg.st_dataBuf[0])
            {
                Spring_Reset();     // 开启
                Sys_Delay_ms(3000);     // 延时3s后关闭
                Spring_Set();
            }
            else
                Spring_Set();
            ec_msg.st_dataLen = 0;  // 关闭
            break;
        }
        
        default:
            ret = ERR_MOTOR_PARA;
            break;
    }
    
    return ret;
}

void Enter_Cup_Task(void *p_arg)
{
    OS_ERR      err;
    UINT8 ret = ERR_NONE;
    UINT8 flag = 0;
    OS_TICK ts;
    
    st_QueWaitPara s_ecmtWaitPara;
    
    (void)p_arg;
    
    s_ecmtWaitPara.data_len = 0;
    s_ecmtWaitPara.get_len = 0;
    s_ecmtWaitPara.len = 0;
    s_ecmtWaitPara.RecStaue = Idel;
    
    queue_inital(EC_QueBuf,128);
    
    while(1)
    {
        OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,&ts,&err);
        while(queue_len_query(EC_QueBuf))
        {
            if(Get_DataMsg(EC_QueBuf,&ec_msg, &s_ecmtWaitPara) == TRUE)
            {     
                
                ec_msg.st_dataLen -= 1;
                ret = Handle_EC_Cmd();
                ec_msg.st_dataErrNum = ret;
                Mid_ReBack_Fill(&ec_msg,FRAM_CTRL_DATA_S_M);
                printf("timing task finish\r\n");
            }
        }
    }
}


void Auto_Enter_Cup_Task(void *p_arg)
{
    OS_ERR      err;
    UINT8 ret = ERR_NONE;
    UINT8 flag = 0;
    UINT8 ts1 = 0,ts2=0,ts3=0,ts4=0;

    OS_TICK ts;
    
    (void)p_arg;
     
    while(1)
    {
        
        if(Auto_Enter_Flag)
        {

            flag = Judge_Light_State();
            ts3++;
            if(!flag)
            {
                ts1++;
                if(ts1>=20)
                {
                    ts1 = 0;
                    Run_State = 1;
                    ts3 = 0;
                }
            }
            if(flag>=2)
            {
                ts2++;
                if(ts2>=20)
                {
                    ts2 = 0;
                    Run_State = 0;
                    ts3 = 0;
                }
            }
            
            // 下限有检测到杯子，7次后停止检测
            if((flag == 1) && Run_State)
            {
                ts4++;
                if(ts4>=20)
                {
                    ts4 = 0;
                    Run_State ++;
                    if(Run_State >=7)
                        Run_State = 0;
                }
            }
            Sys_Delay_ms(100);
            if(!(ts3%20))
            {
                if(Run_State)
                {
                    ret = Mtx_Rst(EnterCupMt,0);
                    Sys_Delay_ms(400); 
                    ret = Mtx_Move(EnterCupMt,/*1000*/s_ecpara.ec_ud_pos[ec_Shake_Pos]>>1,1,0);//??
                    Sys_Delay_ms(100); 
                    ret = Mtx_Move(EnterCupMt,/*1000*/s_ecpara.ec_ud_pos[ec_Shake_Pos]>>1,1,0);//??
                    Sys_Delay_ms(100); 
                    ret = Mtx_Move(EnterCupMt,/*2000*/s_ecpara.ec_ud_pos[ec_Shake_Pos],0,0);//??
                    Sys_Delay_ms(200); 
                    ret = Mtx_Move_To(EnterCupMt,/*5360*/s_ecpara.ec_ud_pos[ec_High_Pos],0);//平齐
                    Sys_Delay_ms(250);
                    ret = Mtx_Move_To(EnterCupMt,/*4860*/(s_ecpara.ec_ud_pos[ec_High_Pos]-s_ecpara.ec_ud_pos[ec_Low_Pos]),0);//往下500
                    Sys_Delay_ms(200);
                    ret = Mtx_Move_To(EnterCupMt,/*5360*/s_ecpara.ec_ud_pos[ec_High_Pos],0);//平齐
                    Sys_Delay_ms(500);
                }
            }

        }
          
        else
        {
            ret = Mtx_Rst(EnterCupMt,0);
            Sys_Delay_ms(100); 
            Auto_Enter_Flag = 0;
            OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,&ts,&err);
        }
      
    
    }
}


#endif
