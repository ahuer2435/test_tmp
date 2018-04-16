#include <ros/ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Int8.h>
#include "serial_driver/serial_data.h"
#include<fstream>

#include <stdio.h>          //puts
#include <string.h>         //strlen
#include <stdlib.h>
#include <unistd.h>         /*Unix 标准函数定义*/
#include <sys/socket.h>
#include <arpa/inet.h>      //inet_addr
#include <unistd.h>         //write
#include <pthread.h>        //for threading , link with lpthread
#include <sys/time.h>
#include <pthread.h>


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

ros::Publisher pub_msg1;
ros::Publisher pub_msg2;


static void serial1_callback(const serial_driver::serial_data& global_input)
{
    int nrd;
    raspmsgsend.counter++;
    //raspmsgsend.raspstatus = evt_selfcheck_ok;//电机初始化，abc 位置检测后再发出
    raspmsgsend.s1++;
    raspmsgsend.s3++;

    if( (motorcalibrationflag == SIGSET) && (gaitcalibrationflag == SIGSET) ){
        raspmsgsend.raspstatus = evt_selfcheck_ok;
    }

    raspmsgsend.check = (raspmsgsend.counter)^(raspmsgsend.raspstatus)^(raspmsgsend.s1)^(raspmsgsend.s2)^(raspmsgsend.s3);

    printf("head nrd = %d\n",nrd);
    nrd =write(fd,&raspmsgsend,sizeof(raspmsgsend));
    printf("tail nrd = %d\n",nrd);
    if( nrd == sizeof(raspmsgsend) ){
        // printf("tty send sucess!:%d\n",i++);
    }
}

static void serial2_callback(const serial_driver::serial_data& global_input)
{}

static int serial_init()
{
    fd = open("/dev/ttyUSB1",O_RDWR );
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



static void serial_msg_set()
{
    raspmsgsend.header = 0x5953;
    raspmsgsend.tail   = 0xfd;
}


int main(int argc, char **argv)
{
    ros::init(argc, argv, "serial1_send_node");
    ros::NodeHandle nh;

    serial_init();
    serial_msg_set();

    ros::Subscriber sub_gps = nh.subscribe("driver/topic1", 10, serial1_callback);
    ros::Subscriber sub_gps2 = nh.subscribe("driver/topic2", 10, serial2_callback);


    pub_msg1 = nh.advertise<std_msgs::Int8>("driver/send_topic1",5,true);
    pub_msg2 = nh.advertise<std_msgs::String>("driver/send_topic2",5,true);

    ros::spin();

    return 0;
}

//refernce: https://blog.csdn.net/CSDNhuaong/article/details/78510436
