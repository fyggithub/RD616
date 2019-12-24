#ifndef __QUEUE_H__
#define __QUEUE_H__
/********************************************************************************
writer : ZH Wang
time   : 2017-10-30
*********************************************************************************/
/* 
注:调用需要更改queue_delay_ms()函数,
关闭中断和开启中断也需要更改enter_critical();exit_critical();
*/

/***************************************************************************************
        宏定义
***************************************************************************************/
#define Q_USED_SIZE     0                                           // 查询已入队数目
#define Q_FREE_SIZE     1                                           // 查询剩余空间
#define Q_MAX_SIZE      2                                           // 查询最大允许接受数据空间

#define queue_delay_ms(ms)        Sys_Delay_ms(ms)

/***************************************************************************************
        数据结构定义
***************************************************************************************/

typedef struct {
    unsigned char       *out;                           // 数据头指针
    unsigned char       *in;                            // 数据尾指针
    unsigned char       *end;                           // 指向Buf的结束位置 

    unsigned long       len;                            // 数据长度
    unsigned long       max;                            // 最大数据长度

    unsigned char       buf[1];                            // 数据域,接收数据时的缓存

}STRUCT_QUEUE;

// extern STRUCT_QUEUE my_queue;
/***************************************************************************************
        接口函数
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
