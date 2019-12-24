#ifndef __QUEUE_C__
#define __QUEUE_C__

#include "CONFIG\My_define.h"
#include "CONFIG\My_Type.h"
#include "STM_SYS\Stm_Sys.h"
#include "string.h"
#include "QUEUE\queue.h"
// 延时调用 到Stm_Sys.h中修改
#if SYSTEM_SUPPORT_OS
#include "includes.h"
#endif

/***************************************************************************************
函数名称: void enter_critical(void)
函数功能: 进入临界模式，即关总中断
输入参数: 无
返 回 值: 无
***************************************************************************************/
static unsigned char g_int_nest = 0;

void enter_critical(void)
{
    #if SYSTEM_SUPPORT_OS
    CPU_SR_ALLOC();
    
    OS_CRITICAL_ENTER();	//进入临界区
    #else
    // 嵌套次数加1
    g_int_nest++;
    __set_PRIMASK(1); 
    #endif
    
}


/***************************************************************************************
函数名称: void exit_critical(void)
函数功能: 退出临界模式，即开总中断
          注意： 该函数与 enter_critical() 成对使用
输入参数: 无
返 回 值: 无
***************************************************************************************/
void exit_critical(void)
{
    #if SYSTEM_SUPPORT_OS
    CPU_SR_ALLOC();
    
    OS_CRITICAL_EXIT();	//退出临界区
    #else
    // 防止错误使用
    if (g_int_nest)
    {
        // 嵌套次数减1
        g_int_nest--;
        
        if (g_int_nest==0)
            __set_PRIMASK(0); 
    }
    #endif
}


/***************************************************************************************
函数名称: void queue_inital(void *buf, unsigned long buf_size)
函数功能: 初始化队列
输入参数: 
返 回 值: 
***************************************************************************************/
void queue_inital(void *buf, unsigned long buf_size)
{
    STRUCT_QUEUE *que;
    
    que = (STRUCT_QUEUE *)buf;
    
    que->in  = que->buf;
    que->out = que->buf;
    que->len = 0;
    
    // 计算数据缓冲最大缓存个数
    que->max = buf_size - (unsigned long)(((STRUCT_QUEUE *)0)->buf);
    
    // 计算数据缓冲结束地址
    que->end = que->buf + que->max;

}

unsigned long queue_len_query(void *buf)
{
    STRUCT_QUEUE *que;
    
    que = (STRUCT_QUEUE *)buf;
    
    if(que->in > que->out)
    {
        return (que->in-que->out);
    }
    else if(que->in == que->out)
        return 0;
    else
    {
        return (que->max - (que->out - que->in));
    }
}


/***************************************************************************************
函数名称: long queue_in_byte_fast(void *buf, unsigned char byte)
函数功能: 数据入队，仅能入一个字节
          注意：当缓冲满时，不在接受数据
输入参数: que --- 队列指针
          byte --- 入队数据
返 回 值: QUEUE_FULL_ERR 队列满
          0 成功
***************************************************************************************/
long queue_in_byte(void *buf, unsigned char byte)
{
    STRUCT_QUEUE *que;
    que = (STRUCT_QUEUE *)buf;
    
    enter_critical();
    
    que->len = queue_len_query(buf);

    //==================================================================================
    // 数据入队
    //==================================================================================
    if(que->len < que->max)
    {
        
        
        *que->in = byte;
        
        // que->in 指向下一个接受单元
        que->in++;
        // 数据缓冲到底，将指针指向
        if (que->in >= que->end)
        {
            que->in = que->buf;
        }
        
        exit_critical();
        
        return 0;
    }
    exit_critical();
    
    return 1;

}


/***************************************************************************************
函数名称: long queue_in(void *buf, unsigned char *str, unsigned long len)
函数功能: 数据入队，该函数入队 len 长度数据
          注意：当缓冲满时，不在接受数据
输入参数: que --- 队列指针
          str --- 入队数据
          len --- 入队数据长度
返 回 值: >0 --- 成功入队数据长度
          =0 --- 入队失败
***************************************************************************************/
long queue_in(void *buf, unsigned char *str, unsigned long len)
{
    STRUCT_QUEUE *que;
    unsigned long in_len, in_len1, in_len2;
    unsigned char *p;
    
    enter_critical();
    que = (STRUCT_QUEUE *)buf;

    que->len = queue_len_query(buf);

    // 如果缓冲区满
    if (que->len >= que->max)
    {
        exit_critical();
        return 0;
    }
        
        
    // 如果入队数据长度为0，直接退出
    if (len == 0)
    {
        exit_critical();
        return 0;
    }

    //==================================================================================
    // 如果缓冲区满剩余空间不够，先入队部分
    //==================================================================================
    if((que->len + len) > que->max)
        in_len = que->max - que->len;
    else
        in_len = len;
    
    //==============================================================================
    // 如果没有滚动
    //==============================================================================
//    if ((que->in + in_len) < que->end)
    if ((que->in + in_len) < que->end)
    {
        
        
        memcpy(que->in, str, in_len);
        
        que->in += in_len;
        que->len += in_len;    
        
    }
    // 如果有滚动
    else
    {
        // copy 第一批数据
        in_len1 = que->end - que->in;
        memcpy(que->in, str, in_len1);
        
        // copy 第二批数据
        in_len2 = in_len - in_len1;
        p = str + in_len1;
        que->in = que->buf;
        memcpy(que->in, p, in_len2);
        
        // que->in 指向当前位置
        que->in += in_len2;
        
    }
    exit_critical();
    return in_len;
}

/***************************************************************************************
函数名称: long queue_out(void *buf, unsigned char *pbuf, 
                            unsigned long rcvlen, unsigned long timeover)

函数功能: 数据出对列，
          数据出对列时不需要管，quet->in, 只管quet->out指针
          当缓冲中的数据小于等于rcvlen， 函数返回缓冲池实际数据长度
          当缓冲中的数据大于rcvlen， 函数返回 rcvlen
        
输入参数: *que     ---  队列指针
          pucBuffer --- 发送数据
          rcvlen    --- 接受数据长度
          timeover  --- 超时时间
                        如果设置超时时间为1s, timeover = g_ulSysClk/DIVIDE_N;
                        DIVIDE_N 为一个常数
                        0 为非阻塞接受
返 回 值: 接收数据长度
***************************************************************************************/
long queue_out(void *buf, unsigned char *pbuf, 
                unsigned long rcvlen, unsigned long timeover)
{
    STRUCT_QUEUE *que;
    unsigned long len, my_rcvlen, t;
    unsigned long que_max;
    
    
    que = (STRUCT_QUEUE *)buf;
    
    que->len = queue_len_query(buf);

    //==================================================================================
    // 输入参数检查
    //==================================================================================
    if (rcvlen > que->max)
        my_rcvlen = que->max;
    else
        my_rcvlen = rcvlen;
    
    //==================================================================================
    // timeover == 0, 一直等待，直到收满指定的数据
    //==================================================================================
    t = timeover;
    while(1)
    {
        que->len = queue_len_query(buf);
        // 如果数据长度足够，退出
        if(que->len >= my_rcvlen)
            break;
        else 
            ;
            
        //==============================================================================
        // 如果 timeover != 0, 超时退出
        //==============================================================================
        if (timeover)
        {
            if (t==0)
                break;
            else
                ;
                
            queue_delay_ms(1);
            t--;
        }
    }

    //==================================================================================
    // 从缓存中copy数据
    //==================================================================================
    if (que->len)
    {
        // 进入临界模式 
        enter_critical();
        
        // 缓存中的数据少于指定接受长度
        if(my_rcvlen > que->len)
        {
            len = que->len;
        }
        
        // 实际收到的数据大于指定接受长度
        else
        {
            len = my_rcvlen;
        }
            
        //==============================================================================
        // 数据没有滚到底
        //==============================================================================
        if ( (unsigned char *)(que->out+len) < (unsigned char *)(que->end) )
        {
            memcpy(pbuf, que->out, len);
            // que->out指向下一出缓存数据
            que->out += len;
            que->len -= len;
        }
        //==============================================================================
        // 数据滚到底后处理
        //==============================================================================
        else
        {
            // 缓存有滚动，需要分两次取数据
            unsigned long len1, len2;
            len1 = que->end - que->out;
            len2 = len - len1;
            
            // copy 第一批数据
            memcpy(pbuf, que->out, len1);
            
            // que->out 到缓存底后重新指到头
            que->out = que->buf;
                
            pbuf += len1;
                
            // copy 第二批数据
            memcpy(pbuf, que->out, len2);
            que->out += len2;
            
        }
        
        // 退出临界模式
        exit_critical();
        
        return len;
    }
    return 0;

}




/***************************************************************************************
函数名称: long queue_out_byte(void *buf, 
                                unsigned char *pbyte, unsigned long timeover)
                        
函数功能: 数据出对列,仅出一个字节
          从对列缓冲中取一个数据
          为提高效率，单独写一个取一个字节的函数，用于一些特殊场合
          
输入参数: *que      --- 对列指针
          *pbyte    --- 出队数据缓冲地址
          timeover  --- 超时时间
返 回 值: 接收数据长度
          0 --- 队列中无数据
          1 --- 成功出队一个字节
***************************************************************************************/
long queue_out_byte(void *buf, 
                    unsigned char *pbyte, unsigned long timeover)
{
    STRUCT_QUEUE *que;
    unsigned long t, my_rcvlen;
    unsigned long que_max;
    
    que = (STRUCT_QUEUE *)buf;
    que->len = queue_len_query(buf);
    // 接受一个数据
    my_rcvlen = 1;
    
    //==================================================================================
    // timeover == 0, 一直等待，直到收满指定的数据
    //==================================================================================
    t = timeover;
    while(1)
    {
        que->len = queue_len_query(buf);
        // 如果数据长度足够，退出
        if(que->len >= my_rcvlen)
            break;
        else
            ;
            
        queue_delay_ms(1);
        
        //==============================================================================
        // 如果 timeover != 0, 超时退出
        //==============================================================================
        if (timeover)
        {
            if (t==0)
                break;
            else
                ;
            queue_delay_ms(1);
            t--;
        }
    }

    
    //==================================================================================
    // copy 数据 
    //==================================================================================
    if (que->len)
    {
        enter_critical();
        *pbyte = *que->out;
        que->out++;
        
        // 数据缓存到底，将指针指向缓存块头地址
        if (que->out >= que->end)
        {
            que->out = que->buf;
        }
        
        exit_critical();

        return 1;
    }
    return 0;
}



/***************************************************************************************
函数名称: int queue_query(void *buf, unsigned char q_type)
                        
函数功能: 查询可用空间
          
输入参数: *que      --- 对列指针

返 回 值: 返回环形缓冲可用空间长度，单位 BYTE
***************************************************************************************/
int queue_query(void *buf, unsigned char q_type)
{
    STRUCT_QUEUE *que;
    
    que = (STRUCT_QUEUE *)buf;
    // 查询剩余数目
    if (q_type == Q_FREE_SIZE)
        return ( que->max - que->len );    
        
    // 查询最大允许接受数据空间
    else if (q_type == Q_MAX_SIZE)
        return ( que->max );    
        
    // 查询已入队数目
    else
        return ( que->len );    
}

#endif

