#include <ros/ros.h>
#include <message_filters/subscriber.h>
#include <message_filters/synchronizer.h>
#include <message_filters/sync_policies/exact_time.h>
#include <boost/bind.hpp>
#include "serial_driver/serial_data_stamp.h"

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

typedef boost::shared_ptr< ::serial_driver::serial_data_stamp const> SerialDataStampedConstPtr;

void callback(const SerialDataStampedConstPtr &global_input1, const SerialDataStampedConstPtr &global_input2,const SerialDataStampedConstPtr &global_input3)
{
    std::string id;
    float data;
    int nrd;
    raspmsgsend.counter++;
    //raspmsgsend.raspstatus = evt_selfcheck_ok;//电机初始化，abc 位置检测后再发出
    raspmsgsend.s1++;
    raspmsgsend.s3++;
    id = global_input1->id;
    data = global_input2->data;

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



static void serial_msg_set()
{
    raspmsgsend.header = 0x5953;
    raspmsgsend.tail   = 0xfd;
}


int main(int argc, char **argv)
{
    ros::init(argc, argv, "serial1_sync_send_node");
    ros::NodeHandle nh;

    serial_init();
    serial_msg_set();

    message_filters::Subscriber<serial_driver::serial_data_stamp> sub_msg1(nh, "driver/topic1", 10);
    message_filters::Subscriber<serial_driver::serial_data_stamp> sub_msg2(nh, "driver/topic2", 10);
    message_filters::Subscriber<serial_driver::serial_data_stamp> sub_msg3(nh, "driver/topic3", 10);

    typedef message_filters::sync_policies::ExactTime<serial_driver::serial_data_stamp, serial_driver::serial_data_stamp, serial_driver::serial_data_stamp> MySyncPolicy;
    message_filters::Synchronizer<MySyncPolicy> sync(MySyncPolicy(10), sub_msg1, sub_msg2, sub_msg3);
    sync.registerCallback(boost::bind(&callback, _1, _2, _3));


    ros::spin();

    return 0;
}

//refernce: https://blog.csdn.net/CSDNhuaong/article/details/78510436
