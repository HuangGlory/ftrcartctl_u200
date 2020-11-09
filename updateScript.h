#ifndef UPDATESCRIPT_H
#define UPDATESCRIPT_H

//update.sh
#define EBOX_UPDATE_SH_FILE_NAME tr("/home/pi/ftrCartCtl/download/EBox/update.sh")
#define FTRCARTCTL_UPDATE_SH_FILE_NAME tr("/home/pi/ftrCartCtl/download/update.sh")

#define EBOX_UPDATE_SH_VERSION tr("version:3.0")
#define FTRCARTCTL_UPDATE_SH_VERSION tr("version:4.0")
#define FTRCARTCTL_UPDATE_SH tr(\
"#!/bin/sh\n\
#Date:20200924\n\
#Author: create by Glory\n\
#Description:this script pack a tar file to be update\n\
#version:4.0\n\
#log:\n\
#V4.0:\n\
#1.A breakpoint after transfer\n\
#\n\
#V3.0:\n\
#1.added check autoRun.sh file empty\n\
#\n\
#V2.0:\n\
#1.check file empty\n\
#\n\
#V1.0:\n\
#first create\n\
#echo \"!!!!!!!!!!!!!start update ftrCartCtl!!!!!!!!!!!!!!!!!!!!\"\n\
\n\
sudo rm -rf *.tgz *.md5\n\
echo \"deleted tgz and md5 file!\"\n\
sudo wget -c -i updateURL\n\
sync;sync;sync;sync;\n\
TGZSIZE=$(ls -l *.tgz | awk '{print $5}')\n\
sleep 2\n\
echo $TGZSIZE\n\
echo\n\
\n\
RES=$(md5sum -c *.md5 | awk '{print $2}')\n\
echo \"DownLoad Result $RES\"\n\
\n\
#echo\n\
#echo\n\
\n\
if [ \"${RES}\" = \"OK\" ] && [ $TGZSIZE -ne 0 ];then\n\
    echo \"start to deploy...\"\n\
    sudo tar zxvf *.tgz -C ../\n\
    \n\
    FILE1=\"../ftrCartCtl\"\n\
    FILE2=\"../settings.json\"\n\
    FILE3=\"../autoRun.sh\"\n\
    SIZE1=$(ls -l \"${FILE1}\" | awk '{print $5}')\n\
    SIZE2=$(ls -l \"${FILE2}\" | awk '{print $5}')\n\
    SIZE3=$(ls -l \"${FILE3}\" | awk '{print $5}')\n\
    if [ $SIZE1 -eq 0 ] || [ $SIZE2 -eq 0 ] || [ $SIZE3 -eq 0 ]; then\n\
        echo\n\
        sleep 2\n\
        echo \"Tar Err! re-tar\"\n\
        sudo tar zxvf *.tgz -C ../\n\
    fi\n\
    SIZE1=$(ls -l \"${FILE1}\" | awk '{print $5}')\n\
    SIZE2=$(ls -l \"${FILE2}\" | awk '{print $5}')\n\
    SIZE3=$(ls -l \"${FILE3}\" | awk '{print $5}')\n\
    if [ $SIZE1 -eq 0 ] || [ $SIZE2 -eq 0 ] || [ $SIZE3 -eq 0 ]; then\n\
        echo \"Download Fail!!!\"\n\
    else\n\
        echo \"finished!!\"\n\
    fi\n\
else\n\
    echo \"Download Fail!!!\"\n\
fi\n")

#define EBOX_UPDATE_SH tr(\
"#!/bin/sh\n\
#Date:20200924\n\
#Author: create by Glory\n\
#Description:this script pack a tar file to be update\n\
#version:3.0\n\
#log:\n\
#V3.0:\n\
#1.A breakpoint after transfer\n\
#\n\
#V2.0:\n\
#1.check file empty\n\
#\n\
#V1.0:\n\
#first create\n\
#echo \"!!!!!!!!!!!!!start update ftrEBoxUpdate!!!!!!!!!!!!!!!!!!!!\"\n\
\n\
sudo rm -rf *.tgz *.md5 ftrEBoxUpdate log.rm updateConfig.json release.sh\n\
echo \"deleted tgz and md5 file!\"\n\
sudo wget -c -i updateURL\n\
sync;sync;sync;sync;\n\
TGZSIZE=$(ls -l *.tgz | awk '{print $5}')\n\
sleep 2\n\
echo $TGZSIZE\n\
echo\n\
\n\
RES=$(md5sum -c *.md5 | awk '{print $2}')\n\
echo \"DownLoad Result $RES\"\n\
\n\
#echo\n\
#echo\n\
\n\
if [ \"${RES}\" = \"OK\" ] && [ $TGZSIZE -ne 0 ];then\n\
    echo \"start to deploy...\"\n\
    sudo tar zxvf *.tgz -C ./\n\
    \n\
    FILE1=\"./ftrEBoxUpdate\"\n\
    FILE2=\"./updateConfig.json\"\n\
    SIZE1=$(ls -l \"${FILE1}\" | awk '{print $5}')\n\
    SIZE2=$(ls -l \"${FILE2}\" | awk '{print $5}')\n\
    if [ $SIZE1 -eq 0 ] || [ $SIZE2 -eq 0 ]; then\n\
        echo\n\
        sleep 2\n\
        echo \"Tar Err! re-tar\"\n\
        sudo tar zxvf *.tgz -C ./\n\
    fi\n\
    SIZE1=$(ls -l \"${FILE1}\" | awk '{print $5}')\n\
    SIZE2=$(ls -l \"${FILE2}\" | awk '{print $5}')\n\
    if [ $SIZE1 -eq 0 ] || [ $SIZE2 -eq 0 ]; then\n\
        echo \"Download Fail!!!\"\n\
    else\n\
        sudo kill $(pidof ftrCartCtl)\n\
        echo \"finished!!\"\n\
    fi\n\
else\n\
    echo \"Download Fail!!!\"\n\
fi\n")
#endif // UPDATESCRIPT_H
