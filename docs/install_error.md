# pi安装ros 出现问题及解决
## 1. 初始化rosdep失败，错误信息如下：
reading in sources list data from /etc/ros/rosdep/sources.list.d
Hit https://raw.githubusercontent.com/ros/rosdistro/master/rosdep/osx-homebrew.yaml
Hit https://raw.githubusercontent.com/ros/rosdistro/master/rosdep/base.yaml
ERROR: unable to process source [https://raw.githubusercontent.com/ros/rosdistro/master/rosdep/python.yaml]:
<urlopen error timed out> (https://raw.githubusercontent.com/ros/rosdistro/master/rosdep/python.yaml)
### 方法1：
```
$sudo apt-get update
$rosdep update
```
可能需要反复执行几次。
* 参考： https://blog.csdn.net/gddxz_zhouhao/article/details/72861266

### 方法2：
```
$sudo pip uninstall rosdep
$sudo apt-get remove --purge python-rosdep python-pip
$sudo apt-get update && sudo apt-get install python-rosdep
$which rosdep
 /usr/bin/rosdep
$rosdep --version
  0.10.11
$apt-cache policy python-rosdep
  python-rosdep:
  Installed: 0.10.11-1
  Candidate: 0.10.11-1
  Version table:
   *** 0.10.11-1 0
    500 http://packages.ros.org/ros/ubuntu/ precise/main i386 Packages
    100 /var/lib/dpkg/status
$sudo rm /etc/ros/rosdep/sources.list.d/20-default.list
$sudo rosdep init
$rosdep update
  Hit https://github.com/ros/rosdistro/raw/master/rosdep/osx-homebrew.yaml
  Hit https://github.com/ros/rosdistro/raw/master/rosdep/gentoo.yaml
  Hit https://github.com/ros/rosdistro/raw/master/rosdep/base.yaml
  Hit https://github.com/ros/rosdistro/raw/master/rosdep/python.yaml
  Hit https://github.com/ros/rosdistro/raw/master/rosdep/ruby.yaml
  Hit https://github.com/ros/rosdistro/raw/master/releases/fuerte.yaml
  Hit https://github.com/ros/rosdistro/raw/master/releases/groovy.yaml
  updated cache in /home/race/.ros/rosdep/sources.cache
```
带$号的是shell 命令，其他是命令执行结果。
* 参考：https://answers.ros.org/question/47177/rosdep-update-error/。


## 2. 编译时出错，信息如下：
/home/pi/ros_catkin_ws/src/roscpp_core/cpp_common/src/logError:82:39: error: 'logError' was not declared in this scope

## 分析：
header.cpp call external_src/console_bridge/include/console_bridge/console.h , console.h use CONSOLE_BRIDGE_logError instead of logError. indigo did not merge, kinect merge

### 解决：
在所有出错文件头部添加如下代码：
```c
#define logError(fmt,...) CONSOLE_BRIDGE_logError(fmt, ...)
```
### 参考:
* https://github.com/ros/roscpp_core/pull/69
