#include <ros/ros.h>
#include "serial_driver/serial_data.h"
#include "boost/thread.hpp"

#include "serial_init_interface.h"

#include "mcumsg_rasp.h"
#include "thread_of_lcd.h"

int fd;
static int gaitcalibrationflag ;
static int motorcalibrationflag ;

static RaspMsgSendTypeDef raspmsgsend;
ros::Publisher pub_msg1;

void thread_of_mcu_recv(void){

    printf("Start Recving msg from mcu...\n");

    serial_driver::serial_data serial_data;
    int nrd;

    static McuMsgSendTypeDef mcumsgsend;

    TTY_STATE current_state = FINDHEAD;

    for(;;){
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
    fd = serial_init();

    pub_msg1 = nh.advertise<serial_driver::serial_data>("driver/from_topic1",10,true);
    boost::thread reveive_loop(&receive_loop_msg);

    ros::spin();
    return 0;
}

//refernce: https://blog.csdn.net/CSDNhuaong/article/details/78510436
