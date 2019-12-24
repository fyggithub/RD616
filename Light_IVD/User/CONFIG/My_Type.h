#ifndef __MY_TYPE_H__
#define __MY_TYPE_H__
/********************************************************************************
writer : ZH Wang
time   : 2017-10-30
*********************************************************************************/
//===============================================================================
// 定义个人的一些定义
//===============================================================================
// __packet

#ifndef NULL
#define NULL        ((void*)0)
#endif

#define     O_     volatile                  /*!< defines 'write only' permissions     */
#define     IO_    volatile const            /*!< defines 'read / write' permissions   */
#define     I_     const                     /*!< defines 'read only' permissions      */

#define    _STA_    static
#define    _EXT_    extern

typedef   signed           char INT8;
typedef   signed short     int  INT16;
typedef   signed           int  INT32;

typedef unsigned           char UINT8;
typedef unsigned short     int  UINT16;
typedef unsigned           int  UINT32;
typedef unsigned long           UINT64;

typedef const INT32				INT32_C;  /*!< Read Only */
typedef const INT16 			INT16_C;  /*!< Read Only */
typedef const INT8 				INT8_C;   /*!< Read Only */

typedef IO_ INT32  			    INT32_V;
typedef IO_ INT16  			    INT16_V;
typedef IO_ INT8   			    INT8_v;

typedef I_ INT32 				INT32_VC;  /*!< Read Only */
typedef I_ INT16 				INT16_VC;  /*!< Read Only */
typedef I_ INT8 				INT8_VC;   /*!< Read Only */



typedef const UINT8 			UINT8_C;  /*!< Read Only */
typedef const UINT16            UINT16_C;  /*!< Read Only */
typedef const UINT32            UINT32_C;   /*!< Read Only */

typedef IO_ UINT32              UINT32_V;
typedef IO_ UINT16              UINT16_V;
typedef IO_ UINT8               UINT8_V;

typedef I_ UINT32               UINT32_VC;  /*!< Read Only */
typedef I_ UINT16               UINT16_VC;  /*!< Read Only */
typedef I_ UINT8                UINT8_VC;   /*!< Read Only */

typedef enum{ FALSE = 1, TRUE = !FALSE }BOOL;

typedef enum{ e_true = 0, e_false = 1, e_error = 2 }e_Return;

#define SET_BIT_T(value, offset)        ((value) = (value) | (0x01<<(offset)))          // 相应位置位
#define CLR_BIT_T(value, offset)        ((value) = (value) & (~(0x01<<(offset))))       // 相应位清零

#define FunUnionTwoByte(ByteH,ByteL)                    ((UINT16)((((UINT8)ByteH)<<8) | (UINT8)ByteL))
#define FunUnionFourByte(Byte1,Byte2,Byte3,Byte4)       ((UINT32)((((UINT8)Byte1)<<24) | (((UINT8)Byte2)<<16) |\
                                                                  (((UINT8)Byte3)<<8)  | ((UINT8)Byte4)))

#define GetPinSource(Gpio_pin,PinSource)          { PinSource = 0;\
                                                    while(Gpio_pin != (0x01<<PinSource)){\
                                                    PinSource ++;\
                                                    };}


#endif

