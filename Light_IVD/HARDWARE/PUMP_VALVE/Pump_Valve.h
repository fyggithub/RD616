#ifndef __PUMP_VALVE_H__
#define __PUMP_VALVE_H__	

#define PVx_NUM             5    //PV的个数 

//===============================================================
//针内壁泵控制
#define PV_1_PIN        GPIO_Pin_8
#define PV_1_PORT       GPIOC
#define PV_1_CLK        RCC_APB2Periph_GPIOC
//针外壁泵控制
#define PV_2_PIN        GPIO_Pin_9
#define PV_2_PORT       GPIOC
#define PV_2_CLK        RCC_APB2Periph_GPIOC
//针内壁阀控制
#define PV_3_PIN        GPIO_Pin_10
#define PV_3_PORT       GPIOC
#define PV_3_CLK        RCC_APB2Periph_GPIOC

//托马斯废液泵控制
#define PV_4_PIN        GPIO_Pin_12
#define PV_4_PORT       GPIOC
#define PV_4_CLK        RCC_APB2Periph_GPIOC

//12V LED灯控制
#define PV_5_PIN        GPIO_Pin_11
#define PV_5_PORT       GPIOC
#define PV_5_CLK        RCC_APB2Periph_GPIOC


/*=============================================================*/
#define PVx_PIN(x)			PV_##x##_PIN
#define PVx_PORT(x)         PV_##x##_PORT
#define PVx_CLK(x)			PV_##x##_CLK

//===============================================================
#define PV_PIN(x,y)		    do{  if(x==0) y = PVx_PIN(1);\
                            else if(x==1) y = PVx_PIN(2);\
                            else if(x==2) y = PVx_PIN(3);\
                            else if(x==3) y = PVx_PIN(4);\
                            else if(x==4) y = PVx_PIN(5);\
                              }while(0)
                                        
#define PV_PORT(x,y)        do{  if(x==0) y = PVx_PORT(1);\
                            else if(x==1) y = PVx_PORT(2);\
                            else if(x==2) y = PVx_PORT(3);\
                            else if(x==3) y = PVx_PORT(4);\
                            else if(x==4) y = PVx_PORT(5);\
                              }while(0)
                                        
#define PV_CLK(x,y)		    do{  if(x==0) y = PVx_CLK(1);\
                            else if(x==1) y = PVx_CLK(2);\
                            else if(x==2) y = PVx_CLK(3);\
                            else if(x==3) y = PVx_CLK(4);\
                            else if(x==4) y = PVx_CLK(5);\
                               }while(0)
 //=============================================================== 
typedef struct
{
    GPIO_TypeDef*  PV_Port;
    UINT32         PV_Clk;
    UINT16         PV_Pin;
}tPV_Config;

typedef enum
{
    E_PV_1 = 0,
    E_PV_2 = 1,
    E_PV_3 = 2,
    E_PV_4 = 3,
    E_PV_5 = 4,
}ePV_Type;




typedef enum
{
    ON = 0,
    OFF = 1
}ePV_Sta;



_EXT_ tPV_Config PV_Config[PVx_NUM]; 
//针内壁泵控制
#define SP_Pump_In_ON       Fun_PV_Open(E_PV_1)
#define SP_Pump_In_OFF      Fun_PV_Close(E_PV_1)
//针外壁泵控制
#define SP_Pump_Out_ON      Fun_PV_Open(E_PV_2)
#define SP_Pump_Out_OFF     Fun_PV_Close(E_PV_2)
//针内壁阀控制
#define SP_Valve_Out_ON     Fun_PV_Open(E_PV_3)
#define SP_Valve_Out_OFF    Fun_PV_Close(E_PV_3)

//托马斯废液泵控制
#define Thomas_ON           Fun_PV_Open(E_PV_4)
#define Thomas_OFF          Fun_PV_Close(E_PV_4)

//12V LED灯控制
#define PV_1_ON             Fun_PV_Open(E_PV_5)
#define PV_1_OFF            Fun_PV_Close(E_PV_5)

 //=============================================================== 
void Fun_Config_PV_Struct(ePV_Type ePV);
UINT8 FUN_PV_INIT(ePV_Type ePV);

UINT8 Fun_PV_Close(ePV_Type ePV);
UINT8 Fun_PV_Open(ePV_Type ePV);
UINT8 Fun_PVx_GetState(ePV_Type PV_Type);
UINT8 PV_AllInit(void);

 //=============================================================== 
#endif
