# 开机自启动ros
## /etc/rc.local文件：
### 添加如下语句：
```
su - pi -c "/home/hank/workspaces/test_tmp/catkin_ws/startup.bash"
```
### 说明：
* hank: 表示当前用户，需要根据实际系统制定，pi上默认是pi。
* 引号部分： 是实际启动ros的脚本，需要根据实际系统修改路径。
* startup.bash是启动ros的脚本，roscore 指令需要在清理ros log脚本文件前面clean_ros_log.sh。
