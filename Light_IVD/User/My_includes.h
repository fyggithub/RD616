#ifndef __MY_INCLUDES_H__
#define __MY_INCLUDES_H__

/************************************************************************
Company:KISSH
Editor : ZH wang
Time   : 2019/04/16
************************************************************************/

#include "CONFIG\My_define.h"       // 定义自己的宏
#include "CONFIG\MY_Type.h"         // 定义自己的数据类型

#include "string.h"
#include "stm32f10x.h"
#include "STM_SYS\Stm_Sys.h"
#include "SYS_DELAY\Sys_Delay.h"
#include "stdio.h"

// 判断是否支持OS系统
#if SYSTEM_SUPPORT_OS
#include "includes.h"
#include  <os.h>
#include "CONFIG\My_cfg.h"
#endif

// 中间控制层
#include "MID\Mid.h"
#include "MID\Mid_op.h"
#include "MID\Mid_err.h"
#include "MID\Mid_ReBack.h"


#include "LED\led.h"
#include "CTRLDMA\CtrlDma.h"
#include "CTRLCOM\CtrlCom.h"
#include "CTRLCAN\CtrlCan.h"
#include "CTRLSPI\CtrlSpi.h"
#include "QUEUE\queue.h"
#include "STMFLASH\stmflash.h"

// HardWare
#include "OUT_FLASH\FM25CL64.h"
#include "LOAD_PARAMETER\Load_Parameter.h"
#include "MT_CTRL\Mtx_Timer.h"
#include "MT_CTRL\Mtx.h"
#include "MT_CTRL\Mtx_CtrlPin.h"
#include "MT_CTRL\Mtx_LightPin.h"
#include "MT_CTRL\ConfigMtx.h"
#include "MT_CTRL\Motor.h"
#include "PUMP_VALVE\Pump_Valve.h"
#include "ENTER_CUP\EnterCupMt.h"
#include "LED_KEY_CTRL\Led_key_ctrl.h"
//#include "PARA\Parameter.h"


#endif

