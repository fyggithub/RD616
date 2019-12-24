#ifndef  __MY_CFG_H__
#define  __MY_CFG_H__


/***********************************************************************************************
*                                            TASK PRIO
*                                       Size of the task prio 
************************************************************************************************/
#define  START_TASK_PRIO                        4u
#define  MID_TASK_PRIO                          4u

#define  LED_TASK_PRIO                          7u

#define  ENTER_CUP_TASK_PRIO                    9u
#define  AUTO_ENTER_CUP_TASK_PRIO               10u

#define  LEDKEY_CTRL_TASK_PRIO                  11u

#define  MID_REBACK_TASK_PRIO                   30u 

/***********************************************************************************************
*                                            TASK STACK SIZES
*                             Size of the task stacks (# of OS_STK entries)
************************************************************************************************/

#define  START_TASK_STK_SIZE                        128
#define  MID_TASK_STK_SIZE                          256

#define  LED_TASK_STK_SIZE                          128  

#define  ENTER_CUP_TASK_STK_SIZE                    128
#define  AUTO_ENTER_CUP_TASK_STK_SIZE               128

#define  LEDKEY_CTRL_TASK_STK_SIZE               128


#define  MID_REBACK_TASK_STK_SIZE                   256 

/*******************************************************************************
*                                ÈÎÎñ¿ØÖÆ¿é
********************************************************************************/
_EXT_ OS_TCB MIDTaskTCB;

_EXT_ OS_TCB LEDTaskTCB;

_EXT_ OS_TCB EnterCupCtrlTaskTCB;

_EXT_ OS_TCB AutoEnterCupCtrlTaskTCB;

_EXT_ OS_TCB MIDReBackTaskTCB;

_EXT_ OS_TCB LKCTRLTaskTCB;

_EXT_ OS_FLAG_GRP MtxEventFlags;

_EXT_ void Mid_Task(void *p_arg);


_EXT_ void Led_Task(void *p_arg);
    
_EXT_ void Enter_Cup_Task(void *p_arg);
_EXT_ void Auto_Enter_Cup_Task(void *p_arg);
    
_EXT_ void LedKey_Ctrl_Task(void *p_arg);
    
_EXT_ void Mid_ReBack_Task(void *p_arg);

#endif
