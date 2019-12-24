#include "My_includes.h"


/*
********************************************************************************
*                                任务控制块
********************************************************************************
*/
OS_TCB      STARTTaskTCB;
OS_TCB      MIDTaskTCB;

OS_TCB      LEDTaskTCB;

OS_TCB      SPTaskTCB;

OS_TCB      EnterCupCtrlTaskTCB;
OS_TCB      AutoEnterCupCtrlTaskTCB;

OS_TCB      LKCTRLTaskTCB;

OS_TCB      MIDReBackTaskTCB;
OS_FLAG_GRP MtxEventFlags;

/*
********************************************************************************
*                                  任务堆栈
********************************************************************************
*/
static  CPU_STK  START_TASK_STK[START_TASK_STK_SIZE];
static  CPU_STK  MID_TASK_STK[MID_TASK_STK_SIZE];


static  CPU_STK  LED_TAST_STK[LED_TASK_STK_SIZE];

static  CPU_STK  ENTER_CUP_TASK_STK[ENTER_CUP_TASK_STK_SIZE];
static  CPU_STK  AUTO_ENTER_CUP_TASK_STK[AUTO_ENTER_CUP_TASK_STK_SIZE];

static  CPU_STK  LEDKEY_CTRL_TASK_STK[LEDKEY_CTRL_TASK_STK_SIZE];

static  CPU_STK  MID_REBACK_TASK_STK[MID_REBACK_TASK_STK_SIZE];


static  void  AppTaskCreate (void);
static void  Start_Task(void *p_arg);


//主函数
int main(void)
{
    OS_ERR  err;
	CPU_SR_ALLOC();
	
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//中断分组配置
	Sys_delay_init();
    Led_Initialization();
    OSInit(&err);		//初始化UCOSIII
	OS_CRITICAL_ENTER();//进入临界区			 
	//创建开始任务
	OSTaskCreate((OS_TCB 	* )&STARTTaskTCB,		//任务控制块
				 (CPU_CHAR	* )"start task", 		//任务名字
                 (OS_TASK_PTR )Start_Task, 			//任务函数
                 (void		* )0,					//传递给任务函数的参数
                 (OS_PRIO	  )START_TASK_PRIO,     //任务优先级
                 (CPU_STK   * )&START_TASK_STK[0],	//任务堆栈基地址
                 (CPU_STK_SIZE)START_TASK_STK_SIZE/10,	//任务堆栈深度限位
                 (CPU_STK_SIZE)START_TASK_STK_SIZE,		//任务堆栈大小
                 (OS_MSG_QTY  )0,					//任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
                 (OS_TICK	  )0,					//当使能时间片轮转时的时间片长度，为0时为默认长度，
                 (void   	* )0,					//用户补充的存储区
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //任务选项
                 (OS_ERR 	* )&err);				//存放该函数错误时的返回值
	OS_CRITICAL_EXIT();	//退出临界区	 
	OSStart(&err);      //开启UCOSIII 
}


//开始任务函数
void Start_Task(void *p_arg)
{
    OS_ERR err;
//    
    CPU_SR_ALLOC();
    (void)p_arg;	
	
    CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//统计任务                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//如果使能了测量中断关闭时间
    CPU_IntDisMeasMaxCurReset();	
#endif
	
#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //当使用时间片轮转的时候
	 //使能时间片轮转调度功能,时间片长度为1个系统时钟节拍，既1*5=5ms
    OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif		
    OS_CRITICAL_ENTER();	//进入临界区
    
    // 系统初始化
    Sys_Init();
//    Led_Initialization();
    AppTaskCreate();                                            /* Create Application Tasks                             */

    OS_CRITICAL_EXIT();	//退出临界区
    OSTaskDel(&STARTTaskTCB,&err);	//删除start_task任务自身}
}


/**
  * 函数功能: 创建应用任务
  * 输入参数: p_arg 是在创建该任务时传递的形参
  * 返 回 值: 无
  * 说    明：无
  */
static  void  AppTaskCreate (void)
{
    OS_ERR      err;
    

    /***********************************/
	OSTaskCreate((OS_TCB       *)&MIDTaskTCB,             
                 (CPU_CHAR     *)"Mid Task",
                 (OS_TASK_PTR   )Mid_Task, 
                 (void         *)0,
                 (OS_PRIO       )MID_TASK_PRIO,
                 (CPU_STK      *)&MID_TASK_STK[0],
                 (CPU_STK_SIZE  )MID_TASK_STK_SIZE / 10,
                 (CPU_STK_SIZE  )MID_TASK_STK_SIZE,
                 (OS_MSG_QTY    )5,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);

	/***********************************/
	OSTaskCreate((OS_TCB       *)&LEDTaskTCB,            
                 (CPU_CHAR     *)"Led Task",
                 (OS_TASK_PTR   )Led_Task, 
                 (void         *)0,
                 (OS_PRIO       )LED_TASK_PRIO,
                 (CPU_STK      *)&LED_TAST_STK[0],
                 (CPU_STK_SIZE  )LED_TASK_STK_SIZE / 10,
                 (CPU_STK_SIZE  )LED_TASK_STK_SIZE,
                 (OS_MSG_QTY    )5,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);
                 
    OSFlagCreate(&MtxEventFlags,"Mt event flags",0,&err);
                 
                 
    /***********************************/
	OSTaskCreate((OS_TCB       *)&EnterCupCtrlTaskTCB,            
                 (CPU_CHAR     *)"Enter Cup Task",
                 (OS_TASK_PTR   )Enter_Cup_Task, 
                 (void         *)0,
                 (OS_PRIO       )ENTER_CUP_TASK_PRIO,
                 (CPU_STK      *)&ENTER_CUP_TASK_STK[0],
                 (CPU_STK_SIZE  )ENTER_CUP_TASK_STK_SIZE / 10,
                 (CPU_STK_SIZE  )ENTER_CUP_TASK_STK_SIZE,
                 (OS_MSG_QTY    )5,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);

#if 1
    /***********************************/
	OSTaskCreate((OS_TCB       *)&LKCTRLTaskTCB,            
                 (CPU_CHAR     *)"Led_Key Task",
                 (OS_TASK_PTR   )LedKey_Ctrl_Task, 
                 (void         *)0,
                 (OS_PRIO       )LEDKEY_CTRL_TASK_PRIO,
                 (CPU_STK      *)&LEDKEY_CTRL_TASK_STK[0],
                 (CPU_STK_SIZE  )LEDKEY_CTRL_TASK_STK_SIZE / 10,
                 (CPU_STK_SIZE  )LEDKEY_CTRL_TASK_STK_SIZE,
                 (OS_MSG_QTY    )5,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);
#endif
    OSTaskCreate((OS_TCB       *)&AutoEnterCupCtrlTaskTCB,            
                 (CPU_CHAR     *)"Auto Enter Cup Task",
                 (OS_TASK_PTR   )Auto_Enter_Cup_Task, 
                 (void         *)0,
                 (OS_PRIO       )AUTO_ENTER_CUP_TASK_PRIO,
                 (CPU_STK      *)&AUTO_ENTER_CUP_TASK_STK[0],
                 (CPU_STK_SIZE  )AUTO_ENTER_CUP_TASK_STK_SIZE / 10,
                 (CPU_STK_SIZE  )AUTO_ENTER_CUP_TASK_STK_SIZE,
                 (OS_MSG_QTY    )5,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);
                 
    /***********************************/
	OSTaskCreate((OS_TCB       *)&MIDReBackTaskTCB,             
                 (CPU_CHAR     *)"Mid ReBack Task",
                 (OS_TASK_PTR   )Mid_ReBack_Task, 
                 (void         *)0,
                 (OS_PRIO       )MID_REBACK_TASK_PRIO,
                 (CPU_STK      *)&MID_REBACK_TASK_STK[0],
                 (CPU_STK_SIZE  )MID_REBACK_TASK_STK_SIZE / 10,
                 (CPU_STK_SIZE  )MID_REBACK_TASK_STK_SIZE,
                 (OS_MSG_QTY    )5,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);
}
