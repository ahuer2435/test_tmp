#include <ros/ros.h>
#include "serial_driver/serial_data.h"
#include "serial_init_interface.h"

int fd;
static int gaitcalibrationflag ;
static int motorcalibrationflag ;

RaspMsgSendTypeDef raspmsgsend;


static void serial1_callback(const serial_driver::serial_data& serial_input)
{
    std::string id;
    float data;
    int nrd;
    raspmsgsend.counter++;
    //raspmsgsend.raspstatus = evt_selfcheck_ok;//电机初始化，abc 位置检测后再发出
    raspmsgsend.s1++;
    raspmsgsend.s3++;
    id = serial_input.id;
    data = serial_input.data;

    if( (motorcalibrationflag == SIGSET) && (gaitcalibrationflag == SIGSET) ){
        raspmsgsend.raspstatus = evt_selfcheck_ok;
    }

    raspmsgsend.check = (raspmsgsend.counter)^(raspmsgsend.raspstatus)^(raspmsgsend.s1)^(raspmsgsend.s2)^(raspmsgsend.s3);

    nrd =write(fd,&raspmsgsend,sizeof(raspmsgsend));
    if( nrd == sizeof(raspmsgsend) ){
        // printf("tty send sucess!:%d\n",i++);
    }
}

void serial_msg_set()
{
    raspmsgsend.header = 0x5953;
    raspmsgsend.tail   = 0xfd;
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "serial1_send_node");
    ros::NodeHandle nh;

    fd = serial_init();
    serial_msg_set();

    ros::Subscriber sub_gps = nh.subscribe("driver/topic1", 10, serial1_callback);

    ros::spin();

    return 0;
}

//refernce: https://blog.csdn.net/CSDNhuaong/article/details/78510436
