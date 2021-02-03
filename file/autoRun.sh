# !/bin/sh
# Date:20210112
# Author: create by Glory
# Description:this script start up ftrCartCtl app when power up
# Version:1.0
# log:
# 1.0:
# first create
echo "run ftrCartCtl"
#sudo rm -rf /var/lock/LCK..ttyAMA0
#sudo rm -rf /var/lock/LCK..ttyUSB0
sudo rm -rf /var/lock/LCK..tty*
sudo chmod 777 /home/pi/ftrCartCtl/ftrCartCtl
sudo /home/pi/ftrCartCtl/ftrCartCtl
echo "exit ftrCartCtl"
