#ifndef __QUEUE_H__
#define __QUEUE_H__
/********************************************************************************
writer : ZH Wang
time   : 2017-10-30
*********************************************************************************/
/* 
ע:������Ҫ����queue_delay_ms()����,
�ر��жϺͿ����ж�Ҳ��Ҫ����enter_critical();exit_critical();
*/

/***************************************************************************************
        �궨��
***************************************************************************************/
#define Q_USED_SIZE     0                                           // ��ѯ�������Ŀ
#define Q_FREE_SIZE     1                                           // ��ѯʣ��ռ�
#define Q_MAX_SIZE      2                                           // ��ѯ�������������ݿռ�

#define queue_delay_ms(ms)        Sys_Delay_ms(ms)

/***************************************************************************************
        ���ݽṹ����
***************************************************************************************/

typedef struct {
    unsigned char       *out;                           // ����ͷָ��
    unsigned char       *in;                            // ����βָ��
    unsigned char       *end;                           // ָ��Buf�Ľ���λ�� 

    unsigned long       len;                            // ���ݳ���
    unsigned long       max;                            // ������ݳ���

    unsigned char       buf[1];                            // ������,��������ʱ�Ļ���

}STRUCT_QUEUE;

// extern STRUCT_QUEUE my_queue;
/***************************************************************************************
        �ӿں���
***************************************************************************************/
extern void queue_inital(void *buf, unsigned long buf_size);
unsigned long queue_len_query(void *buf);
extern long queue_in(void *buf, unsigned char *str, unsigned long len);
extern long queue_in_byte(void *buf, unsigned char byte);
extern long queue_out(void *buf, unsigned char *pbuf, 
                unsigned long rcvlen, unsigned long timeover);
extern long queue_out_byte(void *buf, 
                    unsigned char *pbyte, unsigned long timeover);
extern int queue_query(void *buf, unsigned char q_type);

extern long queue_in_byte_fast(void *buf, unsigned char byte);

extern long queue_in_fast(void *buf, unsigned char *str, unsigned long len);

#endif
