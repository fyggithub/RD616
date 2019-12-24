#ifndef __MOTOR_H__
#define __MOTOR_H__
#include "My_Type.h"
#include "Mtx.h"

UINT16 MtxCountFreqAccDec(UINT8 MtxId, UINT16 *AccSteps, UINT16 *DecSteps);
UINT8 Mtx_Waiting(eMtxID MtxId, UINT32 timeov);
UINT8 Mtx_Move(eMtxID MtxId, UINT32 nSteps, eMtxDir eDir, UINT32 timeov);
UINT8 Mtx_Move_To(eMtxID MtxId, UINT32 nSteps, UINT32 timeov);
UINT8 Mtx_UntilTegger(eMtxID MtxId, UINT32 tiggerpost, eMtxDir eDir, UINT32 timeov);
UINT8 Mtx_Rst(eMtxID MtxId,UINT32 timeov);
UINT8 Mtx_FastRst(eMtxID MtxId,UINT32 timeov);
UINT8 MT_Cup_Move(eMtxID MtxId,UINT8 cup,UINT16 offset_pos,UINT8 mode,UINT16 time);
UINT8 MT_Cup_Move_One_Dir(eMtxID MtxId,UINT8 cup,UINT16 offset_pos,UINT8 mode,UINT16 time);
UINT8 Mtx_Cup_Move_To(eMtxID MtxId, UINT32 nSteps, UINT32 timeov);
UINT8 Mtx_Move_NoWait(eMtxID MtxId, UINT32 nSteps, eMtxDir eDir);
#endif


