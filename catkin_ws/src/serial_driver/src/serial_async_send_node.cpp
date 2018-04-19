#include <ros/ros.h>
#include "serial_driver/serial_data.h"
#include<fstream>

#include "serial_init_interface.h"

#include "mcumsg_rasp.h"
#include "thread_of_lcd.h"

int fd;
static int gaitcalibrationflag ;
static int motorcalibrationflag ;

static RaspMsgSendTypeDef raspmsgsend;


static void serial3_callback(const serial_driver::serial_data& global_input)
{
    //setting raspmsgsend by global_input
}

static void serial2_callback(const serial_driver::serial_data& global_input)
{
    //setting raspmsgsend by global_input
}


static void serial1_callback(const serial_driver::serial_data& global_input)
{
    std::string id;
    float data;
    int nrd;
    raspmsgsend.counter++;
    //raspmsgsend.raspstatus = evt_selfcheck_ok;//电机初始化，abc 位置检测后再发出
    raspmsgsend.s1++;
    raspmsgsend.s3++;
    id = global_input.id;
    data = global_input.data;

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

static void serial_msg_set()
{
    raspmsgsend.header = 0x5953;
    raspmsgsend.tail   = 0xfd;
}


int main(int argc, char **argv)
{
    ros::init(argc, argv, "serial1_async_send_node");
    ros::NodeHandle nh;

    fd = serial_init();
    serial_msg_set();

    ros::Subscriber sub_msg1 = nh.subscribe("driver/topic1", 10, serial1_callback);
    ros::Subscriber sub_msg2 = nh.subscribe("driver/topic2", 10, serial2_callback);
    ros::Subscriber sub_msg3 = nh.subscribe("driver/topic3", 10, serial3_callback);

    ros::spin();

    return 0;
}
