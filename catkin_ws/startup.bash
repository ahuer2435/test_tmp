#! /bin/bash
source /opt/ros/indigo/setup.bash
source /home/hank/workspaces/test_tmp/catkin_ws/devel/setup.bash
roscore&
sleep 2s
/home/hank/workspaces/test_tmp/catkin_ws/clean_ros_log.sh
rosrun serial_driver serial_receive_node&
rosrun serial_driver serial_receive_multi_topic_node&
rosrun serial_driver serial_send_node&
rosrun serial_driver serial_sync_send_node&
rosrun serial_driver serial_async_send_node&
rosrun serial_driver ros_timer_mode&

while [ true ]; do
	sleep 30s
done


