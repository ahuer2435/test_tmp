# 备份磁盘镜像
```
sudo dd if=/dev/xxx of=output.img bs=256MB
```
* 说明：
/dev/xxx是磁盘挂在后的设备文件，可以通过以下命令，查看磁盘的这个文件：

```
sudo fdisk -l
```
通过分区大小可以确定设备与设备文件的对应。
