#include <ros/ros.h>
#include <message_filters/subscriber.h>
#include <message_filters/synchronizer.h>
#include <message_filters/sync_policies/exact_time.h>
#include <boost/bind.hpp>
#include "serial_driver/serial_data_stamp.h"

#include "serial_init_interface.h"

int fd;
static int gaitcalibrationflag ;
static int motorcalibrationflag ;

static RaspMsgSendTypeDef raspmsgsend;


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

    nrd =write(fd,&raspmsgsend,sizeof(raspmsgsend));
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
    ros::init(argc, argv, "serial1_sync_send_node");
    ros::NodeHandle nh;

    fd = serial_init();
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
