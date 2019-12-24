#ifndef __CTRLCAN_H__
#define __CTRLCAN_H__
// 支持UCOSIII
#if SYSTEM_SUPPORT_OS
#define MAIN_CAN_REC_TASKTCB       MIDTaskTCB

#endif
/********************************************************************************
writer : ZH Wang
time   : 2017-10-30
*********************************************************************************/
// STM32F103的CAN个数只有1个，所以这里只使用一个
#define CAN_BUSY_DEBUG              0
#define CAN_EN_SET                  0

#define CAN_ID                      8               // CAN 的ID号

#define MAIN_CAN_ID                 (CAN_ID<<6|0x20)            // 主CAN的ID

#define CTRLCAN1				    CAN1    
#define CAN1_CLK					RCC_APB1Periph_CAN1

#if defined (STM32F40_41xxx)
#define CAN1_TX_PINSOURCE           GPIO_PinSource12
#define CAN1_RX_PINSOURCE           GPIO_PinSource11
#else
#define CAN1_PINREMAP_EN            0
#endif

#define CAN1_TX_GPIO_CLK			RCC_APB2Periph_GPIOA
#define CAN1_TX_GPIO_PORT			GPIOA
#define CAN1_TX_GPIO_PIN			GPIO_Pin_12

#define CAN1_RX_GPIO_CLK			RCC_APB2Periph_GPIOA
#define CAN1_RX_GPIO_PORT			GPIOA
#define CAN1_RX_GPIO_PIN			GPIO_Pin_11

#if CAN_EN_SET
#define CAN1_EN_GPIO_CLK			RCC_APB2Periph_GPIOD
#define CAN1_EN_GPIO_PORT			GPIOD
#define CAN1_EN_GPIO_PIN			GPIO_Pin_2
#endif

// APB1 4分频
//设置波特率   42000000/[(CAN_BS1+CAN_BS2+1)*CAN_Prescaler]   500KHz
#define CAN1_SJW                    CAN_SJW_2tq
#define CAN1_BS1                    CAN_BS1_9tq//CAN_BS1_7tq
#define CAN1_BS2                    CAN_BS2_8tq//CAN_BS2_4tq
#define CAN1_BPR                    4//7



typedef struct
{
  UINT32 Id;  /*!< Specifies the standard identifier.
                        This parameter can be a value between 0 to 0x7FF. */

  UINT8 IDE;     /*!< Specifies the type of identifier for the message that 
                        will be transmitted. This parameter can be a value 
                        of @ref CAN_identifier_type */

  UINT8 RTR;     /*!< Specifies the type of frame for the message that will 
                        be transmitted. This parameter can be a value of 
                        @ref CAN_remote_transmission_request */

  UINT8 DLC;     /*!< Specifies the length of the frame that will be 
                        transmitted. This parameter can be a value between 
                        0 to 8 */

  UINT8 Data[8]; /*!< Contains the data to be transmitted. It ranges from 0 
                        to 0xFF. */
} tCANMsgTx;

typedef struct
{
  UINT32 Id;  /*!< Specifies the standard identifier.
                        This parameter can be a value between 0 to 0x7FF. */

  UINT8 IDE;     /*!< Specifies the type of identifier for the message that 
                        will be received. This parameter can be a value of 
                        @ref CAN_identifier_type */

  UINT8 RTR;     /*!< Specifies the type of frame for the received message.
                        This parameter can be a value of 
                        @ref CAN_remote_transmission_request */

  UINT8 DLC;     /*!< Specifies the length of the frame that will be received.
                        This parameter can be a value between 0 to 8 */

  UINT8 Data[8]; /*!< Contains the data to be received. It ranges from 0 to 
                        0xFF. */

  UINT8 FMI;     /*!< Specifies the index of the filter the message stored in 
                        the mailbox passes through. This parameter can be a 
                        value between 0 to 0xFF */
} tCANMsgRx;


typedef UINT8 (*funCANOne)  (void);
typedef UINT8 (*funCANTwo)  (tCANMsgTx*);
typedef UINT8 (*funCANTree)  (tCANMsgRx*);
typedef UINT8 (*funCANFour)  (UINT8,UINT32);
typedef UINT8 (*funCANFive)  (UINT8*,UINT16);

typedef struct
{
    funCANOne   canInit;
    funCANOne   canCheckFlagRI;
    funCANOne   canCheckFlagTI;
    funCANOne   canCheckFlagTXE;
    funCANOne   canClearFlagRI;
    funCANOne   canClearFlagTI;
    funCANOne   canIntRxDisable;
    funCANOne   canIntRxEnable;
    funCANOne   canIntTxDisable;
    funCANOne   canIntTxEnable;
    funCANOne   canMonitorRXP;
    funCANOne   canMonitorSCE;
    funCANTree  canMsgRx;
    funCANTwo   canMsgTx;
    funCANFive  canSendBuf;
}st_CanFunListType;

// Can配置参数
typedef struct
{
    UINT32      canBpr;                 // Can的周期频率
    UINT32      canBusyNum;             // Can超时循环等待的时间
    UINT8*      canRecieveBuf;
    UINT16      canRecieveLen;
    UINT8       can_Receive_Flag;       // 接收完成标志
    UINT16      canID;
}st_CanConfigParaType;

typedef struct
{
    st_CanFunListType    sCanFunList;
    st_CanConfigParaType sCanConfig;
    
}st_CanCtrlModule;


_EXT_ st_CanCtrlModule  sCanCtrl;

BOOL Can_Initialization(UINT8 *can_buf,UINT16 can_len);
UINT8 FUN_CAN_SendBuf(UINT8* CanSendBuf, UINT16 len);
UINT8 CAN_SetInit(UINT8* Can_buf,UINT16 Can_len);
#endif
