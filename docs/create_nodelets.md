# create_nodelet
以catkin_ws/src/common_tutorials/nodelet_tutorial_math为例.

## 创建包nodelet_tutorial_math:
```
catkin_create_pkg nodelet_tutorial_mat roscpp rospy std_msgs
```

## 创建节点(插件)源文件cpp:
```bash
touch plus.cpp
```
### 源文件主要架构:
* onInit()函数相当于main函数,定义句柄和话题.
* 回调函数处理逻辑.
* PLUGINLIB_EXPORT_CLASS(nodelet_tutorial_math::Plus, nodelet::Nodelet)
nodelet_tutorial_math::Plus表示类Plus,nodelet::Nodelet父类.

### CMakeLists.txt配置:
```
find_package(catkin REQUIRED COMPONENTS nodelet roscpp std_msgs)
//Setup include directories
include_directories(${catkin_INCLUDE_DIRS})

catkin_package(LIBRARIES nodelet_math CATKIN_DEPENDS nodelet roscpp std_msgs )

//Create the nodelet tutorial library
add_library(nodelet_math src/plus.cpp)
target_link_libraries(nodelet_math ${catkin_LIBRARIES})
if(catkin_EXPORTED_LIBRARIES)
  add_dependencies(nodelet_math ${catkin_EXPORTED_LIBRARIES})
endif()
```

详细了解,参考实际文件.

### 配置packagexml文件:
加入标签:
```
<export>
   <nodelet plugin="${prefix}/nodelet_math.xml"/>
</export>
```

### nodelet插件配置文件nodelet_math.xml
<library path="lib/libnodelet_math">   //nodelet_math对应CMakeLists.txt中的nodelet_math.
  <class name="nodelet_tutorial_math/Plus" type="nodelet_tutorial_math::Plus" base_class_type="nodelet::Nodelet"> //对应源文件中的PLUGINLIB_EXPORT_CLASS
    <description>
      A node to add a value and republish.
    </description>
  </class>
</library>

### 参考:
http://blog.sina.com.cn/s/blog_8a2281f70102xhzn.html
