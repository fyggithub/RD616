#ifndef __ENTERCUP_H__
#define __ENTERCUP_H__

#define EnterCupMt          eMtx_ID_1

_EXT_ UINT8 EC_QueBuf[];

// �������޷���ʽ����
#define ENTERCUP_UP_CHECK_CLK       RCC_APB2Periph_GPIOD
#define ENTERCUP_UP_CHECK_PORT      GPIOD
#define ENTERCUP_UP_CHECK_PIN       GPIO_Pin_2

// �������޷���ʽ����
#define ENTERCUP_DOWN_CHECK_CLK     RCC_APB2Periph_GPIOD
#define ENTERCUP_DOWN_CHECK_PORT    GPIOD
#define ENTERCUP_DOWN_CHECK_PIN     GPIO_Pin_1

// ����λ�ļ�⿪��
#define ENTERCUP_MOVE_CHECK_CLK     RCC_APB2Periph_GPIOD
#define ENTERCUP_MOVE_CHECK_PORT    GPIOD
#define ENTERCUP_MOVE_CHECK_PIN     GPIO_Pin_0

// �����������Դ���ؿ����ź�
#define ENTERCUP_SPRING_POWER_CLK   RCC_APB2Periph_GPIOE
#define ENTERCUP_SPRING_POWER_PORT  GPIOE
#define ENTERCUP_SPRING_POWER_PIN   GPIO_Pin_13

void Enter_Cup_Check_Light(void);
void Spring_Pin_Init(void);
UINT8 Get_UpLight_State(void);
UINT8 Get_DowmLight_State(void);
UINT8 Get_MoveLight_State(void);

#endif
