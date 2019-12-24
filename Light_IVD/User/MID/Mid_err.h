#ifndef __ERROR_H__
#define __ERROR_H__

#define ERR_NONE                    0           // �޴�
#define ERR_FRAM_DATA               1           // ���ݴ���

#define ERR_FRAM_HEAD               2           // �������ݵ�ͷ����
#define ERR_FRAM_CTRL               3           // ����λ����
#define ERR_FRAM_TAIL               4           // ��������β����

#define ERR_FRAM_LENGTH             5           // ���ݳ��ȴ���

#define ERR_STATE_MACHINE           7           // ״̬��״̬����

#define ERR_DATA_PARA               8           // �������ݲ�������

#define ERR_MOTOR_PARA              10
#define ERR_MOTOT_TIME_OVER         11          // �����ʱ
#define ERR_MOTOR_OVER_DISTANCE     12          // ������г���������ƾ���
#define ERR_MOTOR_CRASH             13          // ������ײ

#define ERR_MOTOR_MINI_DISTANCE     14          // ������г�������С���ƾ���

#define ERR_MOTOR_NOT_RESET         15          // ���û�и�λ

#define ERR_MOTOT_LIGHT             20          // ������������
#define ERR_MOTOR_RESET_TIGGER      21          // ��λ���������
#define ERR_MOTOR_ARRIVE_TIGGER     22          // ��λ���������
#define ERR_MOTOR_CODE_TIGGER       23          // ���̹��������

#define ERR_MOTOR_CHECK_RESET       25          // ��λ����������

#define ERR_PMT_HANDLE              30
#define ERR_PMT_END                 31

#define ERR_LIQUIT_CHECK            40          // Һ����У׼ʧ��
#define ERR_NOT_RESET_BEFORE        41          // ��ϴǰû��λ
#define ERR_LIQUIT_NOT_RIGHT        42          // û�м�⵽Һ��

#define ERR_MOTOR_ARR_BUF_LEN       50          // ������ز����ĳ��ȳ����趨�ĳ���

#define ERR_MOTOR_NO_RUN            51          // ���δ��������

#define ERR_TIME_NOT_ENOUGH         52          // �����ṩ��ʱ�䲻��

#define ERR_SET_PARA                53          // ������ƴ���


#define ERR_RUN_AUTO_ENTER_CUP      0x80        // ����ִ���Զ���������

/********************************************************************************/
#define ERR_WARNING_SP_DATA         240          // ���Һ����������в���







#define MTx_PWM                     0x03        // ���PWM״̬��״̬����



#endif
