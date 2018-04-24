# ROS与开发板入门教程-树莓派3源码安装ROS环境(jessie+indigo)

## 说明：
目前在树莓派上，仅支持源码方式安装ros。安装之前需要确认操作系统版本是jessie，以下介绍安装ros indigo版本的步骤。

## 安装准备：
设置系统源为国内的源，更新速度更快。
```
$ sudo cp /etc/apt/sources.list /etc/apt/sources.list.bak  #备份
$ sudo vim /etc/apt/sources.list
```
删除之前的源，增加新源
```
deb http://mirrors.aliyun.com/raspbian/raspbian/ jessie main non-free contrib rpi
deb-src http://mirrors.aliyun.com/raspbian/raspbian/ jessie main non-free contrib rpi
```
设置ROS源
```
$ sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu jessie main" > /etc/apt/sources.list.d/ros-latest.list'
```
添加KEY
```
$ wget https://raw.githubusercontent.com/ros/rosdistro/master/ros.key -O - | sudo apt-key add -
```
更新软件源
```
$ sudo apt-get update
$ sudo apt-get upgrade
```
安装Bootstrap依赖
```
$ sudo apt-get install python-pip python-setuptools python-yaml python-distribute python-docutils python-dateutil python-six
$ sudo pip install rosdep rosinstall_generator wstool rosinstall
```
初始化rosdep
```
$ sudo rosdep init
$ rosdep update
```
至此依赖环境ok。下面开始安装 ros 源码。

## 安装ros
建立工作空间（ros源码目录）
```
$ mkdir ~/ros_catkin_ws
$ cd ~/ros_catkin_ws
```
安装核心版

```
$ rosinstall_generator ros_comm --rosdistro indigo --deps --wet-only --exclude roslisp --tar > indigo-ros_comm-wet.rosinstall
$ wstool init src indigo-ros_comm-wet.rosinstall
```
说明：还有一种是桌面版本，是带gui的，且包含很多机器人库，由于这些我们不是必要的，且考虑资源有限且桌面版安装难度较大，所以我们安装核心版本。
上面命令下载相应的代码，需要花很长事件下载，如果中间中断，可执行：(如果太多提示is active, 中断再执行)

```
wstool update -t src
```
创建额外的编译目录
```
mkdir ~/ros_catkin_ws/external_src  
sudo apt-get install checkinstall cmake
```
安装libconsole-bridge-dev
```
$ cd ~/ros_catkin_ws/external_src
$ sudo apt-get install libboost-system-dev libboost-thread-dev
$ git clone https://github.com/ros/console_bridge.git
$ cd console_bridge
$ cmake .
$ sudo checkinstall make install
```
在最后一步checkinstall的时候，会询问多个问题，在有一个问题提供多个选项的时候，选择2修改包名称为libconsole-bridge-dev，然后在之后的两个问题都选择N，否则最后编译时会出错。

安装liblz4-dev
```
$ cd ~/ros_catkin_ws/external_src  
$ wget http://archive.raspbian.org/raspbian/pool/main/l/lz4/liblz4-1_0.0~r122-2_armhf.deb  
$ wget http://archive.raspbian.org/raspbian/pool/main/l/lz4/liblz4-dev_0.0~r122-2_armhf.deb  
$ sudo dpkg -i liblz4-1_0.0~r122-2_armhf.deb liblz4-dev_0.0~r122-2_armhf.deb

```
利用rosdep解决依赖

```
$ cd ~/ros_catkin_ws
$ rosdep install --from-paths src --ignore-src --rosdistro indigo -y -r --os=debian:jessie
```
提示：这一步时间比较久，具体是网速而定。

## 构建ros

```
$ cd ~/ros_catkin_ws
$ sudo ./src/catkin/bin/catkin_make_isolated --install -DCMAKE_BUILD_TYPE=Release --install-space /opt/ros/indigo
```
启用环境

```
$ source /opt/ros/indigo/setup.bash
```
添加到.bashrc
```
$ echo "source /opt/ros/indigo/setup.bash" >> ~/.bashrc
```
测试roscore
```
$ roscore
```
## 参考：
https://www.ncnynl.com/archives/201612/1146.html
http://wiki.ros.org/ROSberryPi/Installing%20ROS%20Indigo%20on%20Raspberry%20Pi
