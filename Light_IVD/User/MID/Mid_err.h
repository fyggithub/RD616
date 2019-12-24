#ifndef __ERROR_H__
#define __ERROR_H__

#define ERR_NONE                    0           // 无错
#define ERR_FRAM_DATA               1           // 数据错误

#define ERR_FRAM_HEAD               2           // 接收数据的头出错
#define ERR_FRAM_CTRL               3           // 控制位出错
#define ERR_FRAM_TAIL               4           // 接收数据尾出错

#define ERR_FRAM_LENGTH             5           // 数据长度错误

#define ERR_STATE_MACHINE           7           // 状态机状态出错

#define ERR_DATA_PARA               8           // 传递数据参数错误

#define ERR_MOTOR_PARA              10
#define ERR_MOTOT_TIME_OVER         11          // 电机超时
#define ERR_MOTOR_OVER_DISTANCE     12          // 电机运行超出最大限制距离
#define ERR_MOTOR_CRASH             13          // 发生碰撞

#define ERR_MOTOR_MINI_DISTANCE     14          // 电机运行超出了最小限制距离

#define ERR_MOTOR_NOT_RESET         15          // 电机没有复位

#define ERR_MOTOT_LIGHT             20          // 电机光耦检测出错
#define ERR_MOTOR_RESET_TIGGER      21          // 复位光耦触发错误
#define ERR_MOTOR_ARRIVE_TIGGER     22          // 到位光耦触发错误
#define ERR_MOTOR_CODE_TIGGER       23          // 码盘光耦触发错误

#define ERR_MOTOR_CHECK_RESET       25          // 复位光耦检测问题

#define ERR_PMT_HANDLE              30
#define ERR_PMT_END                 31

#define ERR_LIQUIT_CHECK            40          // 液面检测校准失败
#define ERR_NOT_RESET_BEFORE        41          // 清洗前没复位
#define ERR_LIQUIT_NOT_RIGHT        42          // 没有检测到液面

#define ERR_MOTOR_ARR_BUF_LEN       50          // 电机加载步数的长度超出设定的长度

#define ERR_MOTOR_NO_RUN            51          // 电机未开启运行

#define ERR_TIME_NOT_ENOUGH         52          // 命令提供的时间不足

#define ERR_SET_PARA                53          // 参数设计错误


#define ERR_RUN_AUTO_ENTER_CUP      0x80        // 正在执行自动进杯命令

/********************************************************************************/
#define ERR_WARNING_SP_DATA         240          // 检测液面检测后电机运行步数







#define MTx_PWM                     0x03        // 电机PWM状态机状态错误



#endif
