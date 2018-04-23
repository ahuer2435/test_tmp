#include <ros/ros.h>
#include "serial_driver/serial_data.h"

// global raspmsgsend

ros::Publisher pub_msg1;

static void callback1(const serial_driver::serial_data& serial_input)
{
    //setting raspmsgsend by serial_input
}

static void callback2(const serial_driver::serial_data& serial_input)
{
    //setting raspmsgsend by serial_input
}


static void callback3(const serial_driver::serial_data& serial_input)
{
    //setting raspmsgsend by serial_input
}

void timer_callback(const ros::TimerEvent&)
{
    serial_driver::serial_data pub_serial_data;
    //setting pub_serial_data by raspmsgsend
    pub_serial_data.id = "pass";
    pub_serial_data.data = 0.1;
    pub_msg1.publish(pub_serial_data);
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "ros_timer_mode_node");
    ros::NodeHandle nh;

    const double_t pub_freq = 10.0;

    ros::Subscriber sub_msg1 = nh.subscribe("driver/from_topic1",10,callback1);
    ros::Subscriber sub_msg2 = nh.subscribe("driver/from_topic2",10,callback2);
    ros::Subscriber sub_msg3 = nh.subscribe("driver/from_topic3",10,callback3);
    pub_msg1 = nh.advertise<serial_driver::serial_data>("driver/from_topic4",10,true);
    ros::Timer timer1 = nh.createTimer(ros::Duration(1.0/pub_freq), timer_callback);

    ros::spin();
    return 0;
}
