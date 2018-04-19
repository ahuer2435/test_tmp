#include <ros/ros.h>
#include "serial_driver/serial_data.h"
#include "boost/thread.hpp"

#include<fstream>

#include <stdio.h>          //puts
#include <string.h>         //strlen
#include <stdlib.h>
#include <unistd.h>         /*Unix 标准函数定义*/
#include <sys/time.h>

#include <assert.h>

//for tty
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "mcumsg_rasp.h"
#include "thread_of_lcd.h"

int fd;
static int gaitcalibrationflag ;
static int motorcalibrationflag ;

static RaspMsgSendTypeDef raspmsgsend;

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


ros::Publisher pub_msg1;

static int serial_init()
{
    fd = open("/dev/ttyUSB0",O_RDWR );
    if(-1 == fd){
        close(fd);
        printf("UBUNTU open /dev/ttyUSB0 error!\n");
        exit(0);
    }
    printf("Mcu_Recv open /dev/ttyUSB0 success!\n");
    sleep(1);

    struct termios opt;
    tcgetattr(fd, &opt);
    cfsetispeed(&opt, B115200);
    cfsetospeed(&opt, B115200);
    if(tcsetattr(fd, TCSANOW, &opt) != 0 )
    {
        perror("tcsetattr error");
        return -1;
    }

    opt.c_cflag &= ~CSIZE;
    opt.c_cflag |= CS8;
    opt.c_cflag &= ~CSTOPB;
    opt.c_cflag &= ~PARENB;
    opt.c_cflag &= ~INPCK;
    opt.c_cflag |= (CLOCAL | CREAD);

    opt.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    opt.c_oflag &= ~OPOST;
    opt.c_oflag &= ~(ONLCR | OCRNL);    //添加的

    opt.c_iflag &= ~(ICRNL | INLCR);
    opt.c_iflag &= ~(IXON | IXOFF | IXANY);    //添加的

    opt.c_cc[VTIME] = 1;
    opt.c_cc[VMIN] = 0;

    tcflush(fd, TCIOFLUSH);

    printf("mcu_recv ttyUSB0 configure complete\n");

    if(tcsetattr(fd, TCSANOW, &opt) != 0)
    {
        perror("serial error");
        return -1;
    }
}


void thread_of_mcu_recv(void){

    printf("Start Recving msg from mcu...\n");

    serial_driver::serial_data serial_data;
    int nrd;

    static McuMsgSendTypeDef mcumsgsend;

    TTY_STATE current_state = FINDHEAD;

    for(;;){
            printf("2.GETMSG\n");
            nrd = read(fd,&mcumsgsend,sizeof(mcumsgsend));
            if(nrd > 0){
                current_state = FINDTAIL;
                //serial_data.data = mcumsgsend

            }
            else{
                current_state = FINDHEAD;
            }
            //serial_data.id =
            pub_msg1.publish(serial_data);
    }
    return;
}

void receive_loop_msg(void)
{
    thread_of_mcu_recv();
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "serial1_receive_node");
    ros::NodeHandle nh;

    pub_msg1 = nh.advertise<serial_driver::serial_data>("driver/from_serial1",10,true);
    boost::thread reveive_loop(&receive_loop_msg);

    ros::spin();
    return 0;
}

//refernce: https://blog.csdn.net/CSDNhuaong/article/details/78510436
