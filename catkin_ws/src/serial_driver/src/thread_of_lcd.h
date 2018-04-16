#ifndef __THREAD_OF_LCD_H
#define __THREAD_OF_LCD_H

enum pic_id_t{
	post_pic,
	start_and_confirm,
	selfcheck_and_wait,
	selfcheck_ok_and_can_continue,
	selfcheck_fail_and_restart,
	select_work_mode,
	fixed_mode_choose_max_force,
	gait_mode_choose_max_force,
	fixed_mode_wait_for_start,
	gait_mode_wait_for_start,
	fixed_mode_working_pic_1,
	gait_mode_working_pic_1,	
//	fixed_mode_working_pic_2,
//	gait_mode_working_pic_2,	
//	fixed_mode_working_pic_3,
//	gait_mode_working_pic_3,	
//	fixed_mode_working_pic_4,
//	gait_mode_working_pic_4,
	relax_mode_pic,
	urgent_stop_and_wait_restart,
	urgent_stop_and_no_restart,	
	shut_down,
	pic_max
};

enum cmd_state_t{
	cmd_state_min = pic_max,
	cmd_power_on,
	cmd_selfcheck,
	cmd_stop_work,
	cmd_start_work,
	cmd_urgent_stop,
	cmd_wait_for_shutdown,
	cmd_state_max
};

enum cmd_mode_t{
	cmd_mode_min = cmd_state_max,
	cmd_gait_mode,
	cmd_fixed_mode,
	cmd_relax_mode,
	cmd_unchoose,
	cmd_mode_max
};


enum evt_state_t{
	evt_state_min = cmd_mode_max,
	evt_power_on,
	evt_selfcheck_ok,
	evt_selfcheck_fail,
	evt_working,
	evt_paused,
	evt_urgent_stop,
	evt_lifting_urgent_stop,
	evt_shut_down,
	evt_state_max
};

#pragma pack(1)
struct msg_of_lcm_t{
	uint16_t state;
	uint16_t mode;
	uint16_t force;
};


struct evt_of_pi_t{
	uint16_t state;
	uint16_t is_fault;
	uint16_t gait;
};

struct dw_tm_t{
	uint8_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t	min;
	uint8_t sec;
};


struct lcm_para_t{
	uint16_t pic_id;
	uint16_t force;
	uint16_t is_fualt;
	struct dw_tm_t tv;
};

/*数据格式定义*/
#define WR_CTL_REG 0x80				//写控制寄存器
#define RD_CTL_REG 0x81				//读控制寄存器
#define WR_DATA_REG 0x82			//写数据寄存器
#define RD_DATA_REG 0x83			//读数据寄存器
#define FRAME_HEAD_H 0xA5
#define FRAME_HEAD_L 0x5A			//数据帧头位A55A
/*寄存器接口定义*/
#define VERSION 0x0000				//固件版本号
#define LTG_INT 0x0001 					//背光亮度控制
#define BUZ_TIME 0x0002					//触控蜂鸣器
#define PIC_ID 0x03						//写：设置需要显示的图片编号，读：读取当前显示的图片编号
#define TP_FLAG 0x0005					//0xAA：触控坐标有更新，其他：触控坐标未更新
#define TP_STATUS 0x0006				//0x01:第一次按下；0x03:一直按压中；0x02:抬起，其他：无效
#define TP_POSITION 0x000B			//触摸屏按压位置
#define TPC_ENABLE 0x000B				//0x00:触屏不启用，其他：触屏启用（0xFF）
#define RTC_TIME 0x20					//RTC时间，实时时间
/*变量地址定义*/
#define DATA_ADDR 0x0014				//数据变量存储位置



#endif
