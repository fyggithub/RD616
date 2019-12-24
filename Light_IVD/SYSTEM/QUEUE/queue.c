#ifndef __QUEUE_C__
#define __QUEUE_C__

#include "CONFIG\My_define.h"
#include "CONFIG\My_Type.h"
#include "STM_SYS\Stm_Sys.h"
#include "string.h"
#include "QUEUE\queue.h"
// ��ʱ���� ��Stm_Sys.h���޸�
#if SYSTEM_SUPPORT_OS
#include "includes.h"
#endif

/***************************************************************************************
��������: void enter_critical(void)
��������: �����ٽ�ģʽ���������ж�
�������: ��
�� �� ֵ: ��
***************************************************************************************/
static unsigned char g_int_nest = 0;

void enter_critical(void)
{
    #if SYSTEM_SUPPORT_OS
    CPU_SR_ALLOC();
    
    OS_CRITICAL_ENTER();	//�����ٽ���
    #else
    // Ƕ�״�����1
    g_int_nest++;
    __set_PRIMASK(1); 
    #endif
    
}


/***************************************************************************************
��������: void exit_critical(void)
��������: �˳��ٽ�ģʽ���������ж�
          ע�⣺ �ú����� enter_critical() �ɶ�ʹ��
�������: ��
�� �� ֵ: ��
***************************************************************************************/
void exit_critical(void)
{
    #if SYSTEM_SUPPORT_OS
    CPU_SR_ALLOC();
    
    OS_CRITICAL_EXIT();	//�˳��ٽ���
    #else
    // ��ֹ����ʹ��
    if (g_int_nest)
    {
        // Ƕ�״�����1
        g_int_nest--;
        
        if (g_int_nest==0)
            __set_PRIMASK(0); 
    }
    #endif
}


/***************************************************************************************
��������: void queue_inital(void *buf, unsigned long buf_size)
��������: ��ʼ������
�������: 
�� �� ֵ: 
***************************************************************************************/
void queue_inital(void *buf, unsigned long buf_size)
{
    STRUCT_QUEUE *que;
    
    que = (STRUCT_QUEUE *)buf;
    
    que->in  = que->buf;
    que->out = que->buf;
    que->len = 0;
    
    // �������ݻ�����󻺴����
    que->max = buf_size - (unsigned long)(((STRUCT_QUEUE *)0)->buf);
    
    // �������ݻ��������ַ
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
��������: long queue_in_byte_fast(void *buf, unsigned char byte)
��������: ������ӣ�������һ���ֽ�
          ע�⣺��������ʱ�����ڽ�������
�������: que --- ����ָ��
          byte --- �������
�� �� ֵ: QUEUE_FULL_ERR ������
          0 �ɹ�
***************************************************************************************/
long queue_in_byte(void *buf, unsigned char byte)
{
    STRUCT_QUEUE *que;
    que = (STRUCT_QUEUE *)buf;
    
    enter_critical();
    
    que->len = queue_len_query(buf);

    //==================================================================================
    // �������
    //==================================================================================
    if(que->len < que->max)
    {
        
        
        *que->in = byte;
        
        // que->in ָ����һ�����ܵ�Ԫ
        que->in++;
        // ���ݻ��嵽�ף���ָ��ָ��
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
��������: long queue_in(void *buf, unsigned char *str, unsigned long len)
��������: ������ӣ��ú������ len ��������
          ע�⣺��������ʱ�����ڽ�������
�������: que --- ����ָ��
          str --- �������
          len --- ������ݳ���
�� �� ֵ: >0 --- �ɹ�������ݳ���
          =0 --- ���ʧ��
***************************************************************************************/
long queue_in(void *buf, unsigned char *str, unsigned long len)
{
    STRUCT_QUEUE *que;
    unsigned long in_len, in_len1, in_len2;
    unsigned char *p;
    
    enter_critical();
    que = (STRUCT_QUEUE *)buf;

    que->len = queue_len_query(buf);

    // �����������
    if (que->len >= que->max)
    {
        exit_critical();
        return 0;
    }
        
        
    // ���������ݳ���Ϊ0��ֱ���˳�
    if (len == 0)
    {
        exit_critical();
        return 0;
    }

    //==================================================================================
    // �����������ʣ��ռ䲻��������Ӳ���
    //==================================================================================
    if((que->len + len) > que->max)
        in_len = que->max - que->len;
    else
        in_len = len;
    
    //==============================================================================
    // ���û�й���
    //==============================================================================
//    if ((que->in + in_len) < que->end)
    if ((que->in + in_len) < que->end)
    {
        
        
        memcpy(que->in, str, in_len);
        
        que->in += in_len;
        que->len += in_len;    
        
    }
    // ����й���
    else
    {
        // copy ��һ������
        in_len1 = que->end - que->in;
        memcpy(que->in, str, in_len1);
        
        // copy �ڶ�������
        in_len2 = in_len - in_len1;
        p = str + in_len1;
        que->in = que->buf;
        memcpy(que->in, p, in_len2);
        
        // que->in ָ��ǰλ��
        que->in += in_len2;
        
    }
    exit_critical();
    return in_len;
}

/***************************************************************************************
��������: long queue_out(void *buf, unsigned char *pbuf, 
                            unsigned long rcvlen, unsigned long timeover)

��������: ���ݳ����У�
          ���ݳ�����ʱ����Ҫ�ܣ�quet->in, ֻ��quet->outָ��
          �������е�����С�ڵ���rcvlen�� �������ػ����ʵ�����ݳ���
          �������е����ݴ���rcvlen�� �������� rcvlen
        
�������: *que     ---  ����ָ��
          pucBuffer --- ��������
          rcvlen    --- �������ݳ���
          timeover  --- ��ʱʱ��
                        ������ó�ʱʱ��Ϊ1s, timeover = g_ulSysClk/DIVIDE_N;
                        DIVIDE_N Ϊһ������
                        0 Ϊ����������
�� �� ֵ: �������ݳ���
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
    // ����������
    //==================================================================================
    if (rcvlen > que->max)
        my_rcvlen = que->max;
    else
        my_rcvlen = rcvlen;
    
    //==================================================================================
    // timeover == 0, һֱ�ȴ���ֱ������ָ��������
    //==================================================================================
    t = timeover;
    while(1)
    {
        que->len = queue_len_query(buf);
        // ������ݳ����㹻���˳�
        if(que->len >= my_rcvlen)
            break;
        else 
            ;
            
        //==============================================================================
        // ��� timeover != 0, ��ʱ�˳�
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
    // �ӻ�����copy����
    //==================================================================================
    if (que->len)
    {
        // �����ٽ�ģʽ 
        enter_critical();
        
        // �����е���������ָ�����ܳ���
        if(my_rcvlen > que->len)
        {
            len = que->len;
        }
        
        // ʵ���յ������ݴ���ָ�����ܳ���
        else
        {
            len = my_rcvlen;
        }
            
        //==============================================================================
        // ����û�й�����
        //==============================================================================
        if ( (unsigned char *)(que->out+len) < (unsigned char *)(que->end) )
        {
            memcpy(pbuf, que->out, len);
            // que->outָ����һ����������
            que->out += len;
            que->len -= len;
        }
        //==============================================================================
        // ���ݹ����׺���
        //==============================================================================
        else
        {
            // �����й�������Ҫ������ȡ����
            unsigned long len1, len2;
            len1 = que->end - que->out;
            len2 = len - len1;
            
            // copy ��һ������
            memcpy(pbuf, que->out, len1);
            
            // que->out ������׺�����ָ��ͷ
            que->out = que->buf;
                
            pbuf += len1;
                
            // copy �ڶ�������
            memcpy(pbuf, que->out, len2);
            que->out += len2;
            
        }
        
        // �˳��ٽ�ģʽ
        exit_critical();
        
        return len;
    }
    return 0;

}




/***************************************************************************************
��������: long queue_out_byte(void *buf, 
                                unsigned char *pbyte, unsigned long timeover)
                        
��������: ���ݳ�����,����һ���ֽ�
          �Ӷ��л�����ȡһ������
          Ϊ���Ч�ʣ�����дһ��ȡһ���ֽڵĺ���������һЩ���ⳡ��
          
�������: *que      --- ����ָ��
          *pbyte    --- �������ݻ����ַ
          timeover  --- ��ʱʱ��
�� �� ֵ: �������ݳ���
          0 --- ������������
          1 --- �ɹ�����һ���ֽ�
***************************************************************************************/
long queue_out_byte(void *buf, 
                    unsigned char *pbyte, unsigned long timeover)
{
    STRUCT_QUEUE *que;
    unsigned long t, my_rcvlen;
    unsigned long que_max;
    
    que = (STRUCT_QUEUE *)buf;
    que->len = queue_len_query(buf);
    // ����һ������
    my_rcvlen = 1;
    
    //==================================================================================
    // timeover == 0, һֱ�ȴ���ֱ������ָ��������
    //==================================================================================
    t = timeover;
    while(1)
    {
        que->len = queue_len_query(buf);
        // ������ݳ����㹻���˳�
        if(que->len >= my_rcvlen)
            break;
        else
            ;
            
        queue_delay_ms(1);
        
        //==============================================================================
        // ��� timeover != 0, ��ʱ�˳�
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
    // copy ���� 
    //==================================================================================
    if (que->len)
    {
        enter_critical();
        *pbyte = *que->out;
        que->out++;
        
        // ���ݻ��浽�ף���ָ��ָ�򻺴��ͷ��ַ
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
��������: int queue_query(void *buf, unsigned char q_type)
                        
��������: ��ѯ���ÿռ�
          
�������: *que      --- ����ָ��

�� �� ֵ: ���ػ��λ�����ÿռ䳤�ȣ���λ BYTE
***************************************************************************************/
int queue_query(void *buf, unsigned char q_type)
{
    STRUCT_QUEUE *que;
    
    que = (STRUCT_QUEUE *)buf;
    // ��ѯʣ����Ŀ
    if (q_type == Q_FREE_SIZE)
        return ( que->max - que->len );    
        
    // ��ѯ�������������ݿռ�
    else if (q_type == Q_MAX_SIZE)
        return ( que->max );    
        
    // ��ѯ�������Ŀ
    else
        return ( que->len );    
}

#endif

