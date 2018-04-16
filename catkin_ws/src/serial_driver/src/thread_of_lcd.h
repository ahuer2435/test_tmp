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

/*���ݸ�ʽ����*/
#define WR_CTL_REG 0x80				//д���ƼĴ���
#define RD_CTL_REG 0x81				//�����ƼĴ���
#define WR_DATA_REG 0x82			//д���ݼĴ���
#define RD_DATA_REG 0x83			//�����ݼĴ���
#define FRAME_HEAD_H 0xA5
#define FRAME_HEAD_L 0x5A			//����֡ͷλA55A
/*�Ĵ����ӿڶ���*/
#define VERSION 0x0000				//�̼��汾��
#define LTG_INT 0x0001 					//�������ȿ���
#define BUZ_TIME 0x0002					//���ط�����
#define PIC_ID 0x03						//д��������Ҫ��ʾ��ͼƬ��ţ�������ȡ��ǰ��ʾ��ͼƬ���
#define TP_FLAG 0x0005					//0xAA�����������и��£���������������δ����
#define TP_STATUS 0x0006				//0x01:��һ�ΰ��£�0x03:һֱ��ѹ�У�0x02:̧����������Ч
#define TP_POSITION 0x000B			//��������ѹλ��
#define TPC_ENABLE 0x000B				//0x00:���������ã��������������ã�0xFF��
#define RTC_TIME 0x20					//RTCʱ�䣬ʵʱʱ��
/*������ַ����*/
#define DATA_ADDR 0x0014				//���ݱ����洢λ��



#endif
