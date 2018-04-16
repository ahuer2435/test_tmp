
#ifndef __MCUMSG_H__
#define __MCUMSG_H__

#include "stdint.h"
#include "thread_of_lcd.h"

#define SIGDEVOK 0x01
#define SIG 0x02

typedef enum
{
	MCUMSGPOWOFF = 0U,
	MCUMSGPOWON,
	
	MCUMSGSTOP,
	MCUMSGSTART,
	
}MCU_STATEMSGDEF;//msg

#pragma pack(1)
typedef struct{

    uint16_t  counter;   
    uint16_t  powon;	    //0-powon/off
	uint16_t  stop;         //0-start/stop
    int16_t   battery;		//25%,50%,75%,100%
    int16_t   position;
    struct    msg_of_lcm_t msg_of_lcm_report_pi;
    int16_t   check;

}McuMsgSendTypeDef;


#pragma pack(1)
typedef struct{
	uint16_t  header;
    uint16_t  counter;   
    uint16_t  raspstatus;	   
	uint16_t  s1;      
    int16_t   s2;	
    int16_t   s3;
    int16_t   check;
    uint8_t   tail;
}RaspMsgSendTypeDef;



#endif

