#ifndef __CTRLPMT_H__
#define __CTRLPMT_H__
#include "My_Type.h"

#define PMT_COM                     0

UINT8 PMT_End(void);
UINT8 PMT_Handle(void);
UINT8 PMT_Check_Version(UINT8 *rec_buf);
UINT16 PMT_Time(UINT16 set_time);
UINT16 PMT_Resolution(UINT16 set_resol);
UINT32 PMT_Star(void);
UINT32 PMT_Recieve(void);
UINT32 PMT_RecEnd(void);
UINT8 Handle_PMT_Cmd(void);
#endif

