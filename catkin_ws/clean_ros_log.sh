#! /bin/bash
find /home/hank/.ros/log -mmin +60  -exec rm -rf {} \;
