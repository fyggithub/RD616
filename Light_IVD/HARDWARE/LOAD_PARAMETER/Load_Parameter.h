#ifndef __LOAD_PARAMETER_H__
#define __LOAD_PARAMETER_H__

#include "My_Type.h"

#define FRAME_ADDR_PAGE_LEN      256

#define SP_FRAME_PAGE            1

// 针参数
__packed typedef struct
{
    UINT16 ec_ud_pos[4];       // 自动进杯上下位置参数
    UINT8 updata_flag;
}st_lEC_Para;

typedef enum
{
    ec_Rst_Pos   = 0,
    ec_High_Pos  = 1,
    ec_Low_Pos   = 2,
    ec_Shake_Pos = 3
}em_LEC_Type;

_EXT_ st_lEC_Para s_ecpara;

UINT8 Set_Paramenter(void);
UINT8 Load_Paramenter(void);
UINT8 Handle_Para_Cmd(void);
UINT8 Load_SysPara(void);

#endif

