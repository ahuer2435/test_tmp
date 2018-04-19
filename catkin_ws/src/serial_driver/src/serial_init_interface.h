#ifndef SERIAL_INIT_INTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif

#define SIGSET    1
#define SIGUNSET  0

typedef enum{
    min_state,
    IDLE,
    POWON,
    RUNNING,
    POWOFF,
    ERROR,
    max_state,
}SYS_STATE;

typedef enum{
    MOTOIDLE = max_state,
    MOTOINITIAL,//pown on
    MOTOMODESET,
    MOTOSETPARAM,
    MOTOWAITSTARTSUCCESS,
    MOTOSTART,
    MOTOSTOP,   //emg stop
    MOTORUNNING,
    MOTORELAXMODE,

    MOTOPOWOFF,
    MOTOERROR,
    motor_max_state,
}MOTOR_STATE;

typedef enum{
    GAITIDLE = motor_max_state,
    GAITINITIAL,
    GAITSTART,
    GAITSTOP,   //emg stop
    GAITRUNNING,
    GAITPOWOFF,
    gait_max_state,
}GAIT_STATE;


typedef enum{
    MCUIDLE = gait_max_state,
    MCUINITIAL,
    MCURUN,
    MCUSTOP,
    MCUPOWOFF,
    MCUERROR,
    mcu_max_state,
}MCU_STATE;


typedef enum
{
  ER_MOTORCRT = mcu_max_state, //mcu
  ER_MOTOCNCT,
  ER_MOTOINI ,
  ER_GAIT1 ,
  ER_GAIT2 ,
  er_max_state,
} ErrorMsgTypeDef;

typedef enum{
    FINDHEAD = er_max_state,
    GETMSG,
    FINDTAIL,
    CHECKDATA,
    PROCESSDATA,
    tty_max_state,
}TTY_STATE;


#define SIGSET    1
#define SIGUNSET  0

#include "mcumsg_rasp.h"
#include "thread_of_lcd.h"

int serial_init();

#ifdef __cplusplus
}
#endif

#define SERIAL_INIT_INTERFACE_H
#endif
