#include <ros/ros.h>
#include <std_msgs/Int16.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

ros::Publisher PubPowerOff;

void host_shutdown_func()
{
    char *home = getenv("HOME");
    char buf[200] = "";
    if (home)
    {
        sprintf(buf, "%s/workspace/test_tmp/catkin_ws/src/system/shutdown_pi/srcs/shutdown.sh", home);
    }
    system(buf);
}

uint32_t power_off=0;
void PowerCallback(const std_msgs::Int16 &power_ctl)
{
    u_int16_t power_cmd = power_ctl.data;
    std_msgs::Int16 power_off_info;

    switch(power_cmd)
    {
        case 1:						//power off
            if(power_off == 0)
            {
                power_off = 1;
                power_off_info.data = 1;
                PubPowerOff.publish(power_off_info);
                //sleep(3);
                //system("echo `date` > /var/log/ros_log/power_off_cmd.txt");
                host_shutdown_func();
            }
            break;
        case 2:
            power_off_info.data = 2;
            PubPowerOff.publish(power_off_info);
            break;
        default:
            ROS_ERROR("power_cmd value:%d is invalid.", power_off_info.data);

    }
}



int main(int argc, char **argv)
{
    ros::init(argc, argv, "mcu_power_ctl");
    ros::NodeHandle n;

    PubPowerOff = n.advertise<std_msgs::Int16>("shutdown_feedback",1);
    ros::Subscriber s1 = n.subscribe("shutdown_cmd", 1, PowerCallback);
    ros::spin();
    s1.shutdown();
    return 0;
}


