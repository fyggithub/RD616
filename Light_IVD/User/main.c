#include "My_includes.h"


/*
********************************************************************************
*                                ������ƿ�
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
*                                  �����ջ
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


//������
int main(void)
{
    OS_ERR  err;
	CPU_SR_ALLOC();
	
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�жϷ�������
	Sys_delay_init();
    Led_Initialization();
    OSInit(&err);		//��ʼ��UCOSIII
	OS_CRITICAL_ENTER();//�����ٽ���			 
	//������ʼ����
	OSTaskCreate((OS_TCB 	* )&STARTTaskTCB,		//������ƿ�
				 (CPU_CHAR	* )"start task", 		//��������
                 (OS_TASK_PTR )Start_Task, 			//������
                 (void		* )0,					//���ݸ��������Ĳ���
                 (OS_PRIO	  )START_TASK_PRIO,     //�������ȼ�
                 (CPU_STK   * )&START_TASK_STK[0],	//�����ջ����ַ
                 (CPU_STK_SIZE)START_TASK_STK_SIZE/10,	//�����ջ�����λ
                 (CPU_STK_SIZE)START_TASK_STK_SIZE,		//�����ջ��С
                 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	  )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	* )0,					//�û�����Ĵ洢��
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //����ѡ��
                 (OS_ERR 	* )&err);				//��Ÿú�������ʱ�ķ���ֵ
	OS_CRITICAL_EXIT();	//�˳��ٽ���	 
	OSStart(&err);      //����UCOSIII 
}


//��ʼ������
void Start_Task(void *p_arg)
{
    OS_ERR err;
//    
    CPU_SR_ALLOC();
    (void)p_arg;	
	
    CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//ͳ������                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//���ʹ���˲����жϹر�ʱ��
    CPU_IntDisMeasMaxCurReset();	
#endif
	
#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //��ʹ��ʱ��Ƭ��ת��ʱ��
	 //ʹ��ʱ��Ƭ��ת���ȹ���,ʱ��Ƭ����Ϊ1��ϵͳʱ�ӽ��ģ���1*5=5ms
    OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif		
    OS_CRITICAL_ENTER();	//�����ٽ���
    
    // ϵͳ��ʼ��
    Sys_Init();
//    Led_Initialization();
    AppTaskCreate();                                            /* Create Application Tasks                             */

    OS_CRITICAL_EXIT();	//�˳��ٽ���
    OSTaskDel(&STARTTaskTCB,&err);	//ɾ��start_task��������}
}


/**
  * ��������: ����Ӧ������
  * �������: p_arg ���ڴ���������ʱ���ݵ��β�
  * �� �� ֵ: ��
  * ˵    ������
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
