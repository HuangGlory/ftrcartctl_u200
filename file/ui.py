# This is a sample Python script.

# Press Shift+F10 to execute it or replace it with your code.
# Press Double Shift to search everywhere for classes, files, tool windows, actions, and settings.
# Version:0.0.4.0
# log:
# Version:0.0.4.0
# 1.增加更新AppServer function
# 2.增加更新版本文件，使手机可以知道当前版本
# Ver:0.0.3.0
# 1.增加build map;
# 2.增加上传路径信息
# 3.整合licence与cpuid项
# Ver:0.0.2.0
# 1.修改version显示格式
# 2.增加upload
#
# Ver:0.0.1.0
# 1. first create
import subprocess

import threading
from graphviz import Digraph as digraph
import streamlit as st
from flask import Flask
import numpy as np
import pandas as pd
import time
import json
import os, sys, stat
from PIL import Image
import pywifi
import numpy as np

import requests
import hashlib

VersionInfo = 'UI:0.0.4.0'

PI = 3.1415926
BETA_JSON_FILE = "Versionsbate.json"
RELEASE_JSON_FILE = "Versions.json"
EBOX_VERSION_KEY = "ElVsersion"
FTRCARTCTL_VERSION_KEY = "FtrCarControlVsersion"
VISION_VERSION_KEY = "ViSionVsertion"
APP_SERVER_VERSION_KEY = "AppSerVerVertion"
EBOX_FIRMWARE_DIR = "/tmp/EBox/"
FTRCARTCTL_FIRMWARE_DIR = "/tmp/ftrCartCtl/"
VISION_FIRMWARE_DIR = "/tmp/vision/"
APPSERVER_FIRMWARE_DIR = "/tmp/AppServer/"
VERSION_JSON_FILE_DIR = "/home/pi/Versions.json"

EBOX_UPDATEURL_DIR = "/home/pi/ftrCartCtl/download/EBox/"
FTRCARTCTL_UPDATEURL_DIR = "/home/pi/ftrCartCtl/download/"
VISION_UPDATEURL_DIR = "/home/pi/Downloads/"

EBOX_APP_DIR = "/home/pi/ftrCartCtl/download/EBox/"
FTRCARTCTL_APP_DIR = "/home/pi/ftrCartCtl/"
VISION_APP_DIR = "/home/pi/vision/"
APPSERVER_APP_DIR = "/home/pi/Lz/"

PREFIX_EBOX_ADDR = "http://www.passionmobility.cn/update/Ukart/U200/EBox/"
PREFIX_FTRCARTCTL_ADDR = "http://www.passionmobility.cn/update/Ukart/U200/ftrCartCtl/"
PREFIX_VISION_ADDR = "http://www.passionmobility.cn/update/Ukart/U200/vision/"
PREFIX_APPSERVER_ADDR = "http://www.passionmobility.cn/update/Ukart/U200/AppSerer/"
PREFIX_VERSION_JSON_ADDR = "http://www.passionmobility.cn/update/Ukart/U200/"

JSON_FILE_NAME = '/home/pi/ftrCartCtl/settings.json'
JSON_D_FILE_NAME = '/home/pi/ftrCartCtl/setting_d.json'
FTRCARTCTL_EBOX_VERSION_FILE_NAME = '/home/pi/ftrCartCtl/version'
VISION_VERSION_FILE_NAME = '/home/pi/vision/VERSION.md'
APPSERVRER_VERSION_FILE_NAME = '/home/pi/Lz/VERSION.md'
FTRCARTCTL_INPUT_PIPE_FILE_NAME = '/tmp/ftrCartCtl_pipe_input'
FTRCARTCTL_OUTPUT_PIPE_FILE_NAME = '/tmp/ftrCartCtl_pipe_output'
LICENCE_FILE_NAME = '/home/pi/ftrCartCtl/license'
SSID_LIST_FILE_NAME = '/tmp/ssid.npy'
MAP_JSON_FILE_NAME = '/tmp/map.json'
UPDATE_FILE_NAME = '/tmp/update.tgz'
UPLOAD_ROUTE_FILE_NAME = '/tmp/route.json'

progressCnt = 0
# install scripy
InstallScript = \
    '''#!/bin/sh
#Date:20201010
#Author: create by Glory
#Description:this script install firmware
#version:1.0
#log:
#1.0:
#first create

# curPath=$(pwd)
curPath=$1
echo $curPath

#copy vision firmware to /home/pi/
DEST_DIR="/home/pi/Downloads/"
SORC_DIR="vision"
#sudo cp -f $SORC_DIR/* $DEST_DIR
#echo $SORC_DIR To $DEST_DIR
#sync;sync;sync;sync;
#sleep 1
#cd $DEST_DIR

#cd $curPath
VISION_DIR="$curPath$SORC_DIR"
echo ${VISION_DIR}
GETNAMESTR=$VISION_DIR/*.md5
FILENAME=$(ls $GETNAMESTR) 
updateURL="updateURL"
sudo rm -rf updateURL
sleep 1
echo file://"${FILENAME%.*}"
echo file://"${FILENAME%.*}" > $DEST_DIR$updateURL
sync;sync;sync;sync;
sleep 2
echo "created updateURL"

cd $DEST_DIR
SWUPDATE="swupdate2.sh"
echo "start run $DEST_DIR$SWUPDATE"
sudo $DEST_DIR$SWUPDATE
cd $curPath
echo 
echo
sleep 2



#copy ftrCartCtl firmware to /home/pi/ftrCartCtl/download
echo "ftrCartCtl Start!"
DEST_DIR="/home/pi/ftrCartCtl/download/"
SORC_DIR="./ftrCartCtl/"
sudo cp -f $SORC_DIR/* $DEST_DIR
echo $SORC_DIR To $DEST_DIR
sync;sync;sync;sync;
sleep 1
cd $DEST_DIR
TGZSIZE=$(ls -l *.tgz | awk '{print $5}')
echo $TGZSIZE
RES=$(md5sum -c *.md5 | awk '{print $2}')
echo "Copy Result $RES"

if [ "${RES}" = "OK" ] && [ $TGZSIZE -ne 0 ];then
    echo "start to deploy..."
    sudo tar zxvf *.tgz -C ../

    FILE1="../ftrCartCtl"
    FILE2="../settings.json"
    FILE3="../autoRun.sh"
    SIZE1=$(ls -l "${FILE1}" | awk '{print $5}')
    SIZE2=$(ls -l "${FILE2}" | awk '{print $5}')
    SIZE3=$(ls -l "${FILE3}" | awk '{print $5}')
    if [ $SIZE1 -eq 0 ] || [ $SIZE2 -eq 0 ] || [ $SIZE3 -eq 0 ]; then
        echo
        sleep 2
        echo "Tar Err! re-tar"
        sudo tar zxvf *.tgz -C ../
    fi
    SIZE1=$(ls -l "${FILE1}" | awk '{print $5}')
    SIZE2=$(ls -l "${FILE2}" | awk '{print $5}')
    SIZE3=$(ls -l "${FILE3}" | awk '{print $5}')
    if [ $SIZE1 -eq 0 ] || [ $SIZE2 -eq 0 ] || [ $SIZE3 -eq 0 ]; then
        echo "ftrCartCtl Update Fail!!!"
    else
        echo "ftrCartCtl finished!!"
    fi
else
    echo "ftrCartCtl Copy Fail!!!"
fi
#sudo $DEST_DIR/update_u.sh
cd $curPath
echo
echo

#copy E-Box firmware to /home/pi/ftrCartCtl/download/EBox
echo "EBox Start!"
DEST_DIR="/home/pi/ftrCartCtl/download/EBox"
SORC_DIR="./EBox/"
sudo cp -f $SORC_DIR/* $DEST_DIR
echo $SORC_DIR To $DEST_DIR
sync;sync;sync;sync;
sleep 1
cd $DEST_DIR

TGZSIZE=$(ls -l *.tgz | awk '{print $5}')
echo $TGZSIZE
RES=$(md5sum -c *.md5 | awk '{print $2}')
echo "Copy Result $RES"

EBOX_UPDATE_STATE=0
if [ "${RES}" = "OK" ] && [ $TGZSIZE -ne 0 ];then
    echo "start to deploy..."
    sudo tar zxvf *.tgz -C ./

    FILE1="./ftrEBoxUpdate"
    FILE2="./updateConfig.json"
    SIZE1=$(ls -l "${FILE1}" | awk '{print $5}')
    SIZE2=$(ls -l "${FILE2}" | awk '{print $5}')
    if [ $SIZE1 -eq 0 ] || [ $SIZE2 -eq 0 ]; then
        echo
        sleep 2
        echo "Tar Err! re-tar"
        sudo tar zxvf *.tgz -C ./
    fi
    SIZE1=$(ls -l "${FILE1}" | awk '{print $5}')
    SIZE2=$(ls -l "${FILE2}" | awk '{print $5}')
    if [ $SIZE1 -eq 0 ] || [ $SIZE2 -eq 0 ]; then
        echo "EBox Update Fail!!!"
    else
        PID=$(pidof ftrCartCtl)
        if [ "${PID}" = "" ]; then
            echo "ftrCartCtl No Running"
        else
            sudo kill ${PID}
            echo "Killed ftrCartCtl PID=${PID}"
        fi
        EBOX_UPDATE_STATE=1
        echo "EBox finished!!"
    fi
else
    echo "EBOx Copy Fail!!!"
fi
sync;sync;sync;sync;
echo
echo
sleep 2

if [ $EBOX_UPDATE_STATE -eq 1 ]; then
    echo "Start to Run EBoxUpdate"
    sudo  $DEST_DIR/ftrEBoxUpdate
fi

#sudo $DEST_DIR/update_u.sh
cd $curPath
echo
echo'''


# JSON_FILE_NAME = './settings.json'
# JSON_D_FILE_NAME ='./setting_d.json'
# FTRCARTCTL_EBOX_VERSION_FILE_NAME = './version'
# VISION_VERSION_FILE_NAME = './VERSION.md'


class softwareUpdate:
    def __init__(self, whichVersion):
        if not os.path.exists(EBOX_FIRMWARE_DIR):
            os.mkdir(EBOX_FIRMWARE_DIR)

        if not os.path.exists(FTRCARTCTL_FIRMWARE_DIR):
            os.mkdir(FTRCARTCTL_FIRMWARE_DIR)

        if not os.path.exists(VISION_FIRMWARE_DIR):
            os.mkdir(VISION_FIRMWARE_DIR)

        if not os.path.exists(APPSERVER_FIRMWARE_DIR):
            os.mkdir(APPSERVER_FIRMWARE_DIR)

        self.prefixEbox = PREFIX_EBOX_ADDR
        self.prefixFtrCartCtl = PREFIX_FTRCARTCTL_ADDR
        self.prefixVision = PREFIX_VISION_ADDR
        self.prefixAppServer = PREFIX_APPSERVER_ADDR
        self.prefixVersionJson = PREFIX_VERSION_JSON_ADDR

        if whichVersion == 'Beta':
            self.beta = True
        else:
            self.beta = False

    def onStartButtonSlot(self, myBar):
        for file in os.listdir(EBOX_FIRMWARE_DIR):
            os.remove(EBOX_FIRMWARE_DIR + file)
            # loglist.append("del "+EBOX_FIRMWARE_DIR + file + " \n")

        for file in os.listdir(FTRCARTCTL_FIRMWARE_DIR):
            os.remove(FTRCARTCTL_FIRMWARE_DIR + file)
            # loglist.append("del"+FTRCARTCTL_FIRMWARE_DIR + file + " \n")

        for file in os.listdir(VISION_FIRMWARE_DIR):
            os.remove(VISION_FIRMWARE_DIR + file)
            # loglist.append("del"+VISION_FIRMWARE_DIR + file + " \n")

        for file in os.listdir(APPSERVER_FIRMWARE_DIR):
            os.remove(APPSERVER_FIRMWARE_DIR + file)
            # loglist.append("del"+APPSERVER_FIRMWARE_DIR + file + " \n")

        st.write("\n")
        st.write("\n")
        st.write("\n")
        st.write("\n")
        st.write("Clear to Start:\n")
        myBar.progress(10)

        if self.beta:
            self.prefixVersionJson = self.prefixVersionJson + BETA_JSON_FILE
        else:
            self.prefixVersionJson = self.prefixVersionJson + RELEASE_JSON_FILE

        st.write("Download Version File:" + self.prefixVersionJson + "\n")

        r = requests.get(self.prefixVersionJson)
        json_data = json.loads(r.content)

        for item in json_data.keys():
            if EBOX_VERSION_KEY == item:
                self.prefixEbox = self.prefixEbox + json_data[item]
                self.urlEboxMD5 = self.prefixEbox + ".md5"
                self.urlEboxTGZ = self.prefixEbox + ".tgz"
                # print(self.prefixEbox)
            elif FTRCARTCTL_VERSION_KEY == item:
                self.prefixFtrCartCtl = self.prefixFtrCartCtl + json_data[item]
                self.urlFtrCartCtlMD5 = self.prefixFtrCartCtl + ".md5"
                self.urlFtrCartCtlTGZ = self.prefixFtrCartCtl + ".tgz"
                # print(self.prefixFtrCartCtl)
            elif VISION_VERSION_KEY == item:
                self.prefixVision = self.prefixVision + json_data[item]
                self.urlVisionMD5 = self.prefixVision + ".md5"
                self.urlVisionTGZ = self.prefixVision + ".tgz"
                # print(self.prefixVision)
            elif APP_SERVER_VERSION_KEY == item:
                self.prefixAppServer = self.prefixAppServer + json_data[item]
                self.urlAppServerMD5 = self.prefixAppServer + ".md5"
                self.urlAppServerTGZ = self.prefixAppServer + ".tgz"
                # print(self.prefixAppServer)

        # st.write(self.urlEboxTGZ,self.urlFtrCartCtlTGZ,self.urlVisionTGZ)

        st.write("DownLoad EBox Firmware From : " + self.prefixEbox + "......")
        downloadFileSlot(EBOX_FIRMWARE_DIR, self.urlEboxMD5)
        downloadFileSlot(EBOX_FIRMWARE_DIR, self.urlEboxTGZ)
        md5Calc = md5sum(EBOX_FIRMWARE_DIR + self.urlEboxTGZ.split('/')[-1])

        self.EBoxFirmwareInLocal = EBOX_FIRMWARE_DIR + self.urlEboxTGZ.split('/')[-1]

        md5Got = None;
        downloadEBoxResult = False
        downloadFtrCartCtlResult = False
        downloadVisionResult = False
        downloadAppServerResult = False

        with open(EBOX_FIRMWARE_DIR + self.urlEboxMD5.split('/')[-1], 'r') as fp:
            fcont = fp.read()
            md5Got = fcont.split(' ')[0].replace('\n', '');

        if md5Got == md5Calc and os.path.getsize(self.EBoxFirmwareInLocal) != 0:
            st.write("size:", os.path.getsize(self.EBoxFirmwareInLocal), "Successful!!\n\n")
            downloadEBoxResult = True
            myBar.progress(20)
        else:
            st.write("Fail!!\n\n")

        st.write("DownLoad CartCtl Firmware From : " + self.prefixFtrCartCtl + "......")
        downloadFileSlot(FTRCARTCTL_FIRMWARE_DIR, self.urlFtrCartCtlMD5)
        downloadFileSlot(FTRCARTCTL_FIRMWARE_DIR, self.urlFtrCartCtlTGZ)
        md5Calc = md5sum(FTRCARTCTL_FIRMWARE_DIR + self.urlFtrCartCtlTGZ.split('/')[-1])

        self.ftrCartCtlFirmwareInLocal = FTRCARTCTL_FIRMWARE_DIR + self.urlFtrCartCtlTGZ.split('/')[-1]

        md5Got = None;

        with open(FTRCARTCTL_FIRMWARE_DIR + self.urlFtrCartCtlMD5.split('/')[-1], 'r') as fp:
            fcont = fp.read()
            md5Got = fcont.split(' ')[0].replace('\n', '')

        if md5Got == md5Calc and os.path.getsize(self.ftrCartCtlFirmwareInLocal) != 0:
            st.write("size:", os.path.getsize(self.ftrCartCtlFirmwareInLocal), "Successful!!\n\n")
            downloadFtrCartCtlResult = True
            myBar.progress(40)
        else:
            st.write("Fail!!\n\n")

        st.write("Download Appserver Firmware From: " + self.prefixAppServer + "......")
        downloadFileSlot(APPSERVER_FIRMWARE_DIR, self.urlAppServerMD5)
        downloadFileSlot(APPSERVER_FIRMWARE_DIR, self.urlAppServerTGZ)
        md5Calc = md5sum(APPSERVER_FIRMWARE_DIR + self.urlAppServerTGZ.split('/')[-1])

        self.AppServerFirmwareInLocal = APPSERVER_FIRMWARE_DIR + self.urlAppServerTGZ.split('/')[-1]

        md5Got = None

        with open(APPSERVER_FIRMWARE_DIR + self.urlAppServerMD5.split('/')[-1], 'r') as fp:
            fcont = fp.read()
            md5Got = fcont.split(' ')[0].replace('\n', '')

        if md5Got == md5Calc and os.path.getsize(self.AppServerFirmwareInLocal) != 0:
            st.write("size:", os.path.getsize(self.AppServerFirmwareInLocal), "Successful!!\n\n")
            downloadAppServerResult = True
            myBar.progress(45)
        else:
            st.write("Fail!!\n\n")

        st.write("DownLoad Vision Firmware From : " + self.prefixVision + "......")
        downloadFileSlot(VISION_FIRMWARE_DIR, self.urlVisionMD5)
        downloadFileSlot(VISION_FIRMWARE_DIR, self.urlVisionTGZ)
        md5Calc = md5sum(VISION_FIRMWARE_DIR + self.urlVisionTGZ.split('/')[-1])

        self.visionFirmwareInLocal = VISION_FIRMWARE_DIR + self.urlVisionTGZ.split('/')[-1]

        md5Got = None;
        myBar.progress(50)

        with open(VISION_FIRMWARE_DIR + self.urlVisionMD5.split('/')[-1], 'r') as fp:
            fcont = fp.read()
            myBar.progress(70)
            md5Got = fcont.split(' ')[0].replace('\n', '')

        if md5Got == md5Calc and os.path.getsize(self.visionFirmwareInLocal) != 0:
            st.write("size:", os.path.getsize(self.visionFirmwareInLocal), "Successful!!\n\n")
            downloadVisionResult = True
            myBar.progress(80)
        else:
            st.write("Fail!!\n\n")

        if downloadEBoxResult and downloadFtrCartCtlResult and downloadVisionResult and downloadAppServerResult:
            myBar.progress(100)
            st.success("---Download All Successful---\n")
            return True
        else:
            st.subheader("---FAIL----\n")
            return False


def downloadFileSlot(toDir, url):
    r = requests.get(url);
    fileName = url.split('/')[-1];
    with open(toDir + fileName, 'wb') as fp:
        fp.write(r.content)
    os.system('sync;sync;sync')


def md5hex(word):
    """ MD5加密算法，返回32位小写16进制符号
    """
    if isinstance(word, unicode):
        word = word.encode("utf-8")
    elif not isinstance(word, str):
        word = str(word)
    m = hashlib.md5()
    m.update(word)
    return m.hexdigest()


def md5sum(filename):
    """ 计算文件的MD5值
    """
    fd = open(filename, "rb")
    fcont = fd.read()
    fd.close()
    fmd5 = hashlib.md5(fcont)
    return fmd5.hexdigest()


def runScript(cmd, placeholder):
    p = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE)
    log = ""
    for i in iter(p.stdout.readline, 'b'):
        info = i.decode()
        log = log + '\n' + info
        placeholder.text(log)
        if len(i) == 0:
            break;

    st.write(log)
    os.system('sync;sync;sync;sync')


def runScriptEBox(cmd, placeholder, myBar):
    p = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE)
    log = ""
    for i in iter(p.stdout.readline, 'b'):
        info = i.decode()
        if not info.__contains__('Transferring...'):
            log = log + '\n' + info
            myBar.progress(50)

        placeholder.text(log + info)

        if info.__contains__("All Finished 100%!"):
            st.write(log)
            return True
        if info.__contains__('Timeout'):
            return False

        if len(i) == 0:
            break;
    st.write(log)
    return False


def runScriptVision(cmd, placeholder, myBar, value):
    p = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE)
    myBar.progress(value)
    log = ""
    for i in iter(p.stdout.readline, 'b'):
        info = i.decode()
        if info.__contains__('good') or info.__contains__('[1] update vision necessary') or info.__contains__(
                '[2] install necessary packages'):
            value = value + 10;
            myBar.progress(value)

        if not info.__contains__('removed') and not info.__contains__('vision/') and not info.__contains__('setup/'):
            # st.write(info)
            log = log + '\n' + info
            placeholder.text(log)
        if info.__contains__('finished'):
            st.write(log)
            return True
        if len(i) == 0:
            break
    st.write(log)
    return False


def WIFISetting(somearg):
    st.info("Note:only support 2.4GHz")

    a = np.load(SSID_LIST_FILE_NAME)
    ssidlist = a.tolist()

    ssid = st.selectbox("SSID:", ssidlist)
    psw = st.text_input("PSW:", "", None, "password")
    st.selectbox("security", ['WPA-PSK/WPA2-PSK'], 0)

    buttonState = st.button('save')

    if (buttonState):
        if (len(ssid) == 0) or (len(psw) < 8):
            st.write("ssid or psw invalid")
        else:
            st.write("SSID:", ssid)
            st.write("PSW:", psw)
            outputStr = 'echo WIFISetting:' + ssid + ',' + psw + ' > /tmp/ftrCartCtl_pipe_input'
            os.system(outputStr)


def Update(somearg):
    version = st.radio("Which Version", ('Beta', 'Release'))
    my_bar = st.progress(0)
    start = st.button('start')
    placeholder = st.empty()

    if start:
        update = softwareUpdate(version)
        result = update.onStartButtonSlot(my_bar)

        if result:
            time.sleep(2)
            st.write("Start To Deploy")
            st.write("Create updateURL")
            my_bar.progress(5)

            EBOXFirmwareTGZInLoacl = update.urlEboxTGZ
            EBOXFirmwareMD5InLoacl = update.urlEboxMD5

            with open('/tmp/updateURL', 'w') as fp:
                fp.seek(0)
                fp.truncate();
                fp.writelines(EBOXFirmwareTGZInLoacl)
                fp.write("\n")
                fp.writelines(EBOXFirmwareMD5InLoacl)
                fp.flush()
                fp.close()
            cmd = "sudo cp -f /tmp/updateURL " + EBOX_UPDATEURL_DIR
            os.system(cmd)
            my_bar.progress(10)

            cmd = "sudo tar zxvf " + update.EBoxFirmwareInLocal + " -C " + EBOX_APP_DIR
            st.write(cmd)
            runScript(cmd, placeholder)
            my_bar.progress(15)

            ftrCartCtlFirmwareTGZInLoacl = update.urlFtrCartCtlTGZ
            ftrCartCtlFirmwareMD5InLoacl = update.urlFtrCartCtlMD5
            with open('/tmp/updateURL', 'w') as fp:
                fp.seek(0)
                fp.truncate();
                fp.write(ftrCartCtlFirmwareTGZInLoacl)
                fp.write("\n")
                fp.write(ftrCartCtlFirmwareMD5InLoacl)
                fp.flush()
                fp.close()
            cmd = "sudo cp -f /tmp/updateURL " + FTRCARTCTL_UPDATEURL_DIR
            os.system(cmd)
            my_bar.progress(20)

            cmd = "sudo tar zxvf " + update.ftrCartCtlFirmwareInLocal + " -C " + FTRCARTCTL_APP_DIR
            st.write(cmd)
            runScript(cmd, placeholder)
            my_bar.progress(25)

            AppServerFirmwareTGZInLocal = update.urlAppServerTGZ
            AppServerFirmwareMD5InLocal = update.urlAppServerMD5
            with open('/tmp/updateURL', 'w') as fp:
                fp.seek(0)
                fp.truncate();
                fp.write(AppServerFirmwareTGZInLocal)
                fp.write("\n")
                fp.write(AppServerFirmwareMD5InLocal)
                fp.flush()
                fp.close()
            cmd = "sudo tar zxvf " + update.AppServerFirmwareInLocal + " -C " + APPSERVER_APP_DIR
            st.write(cmd)
            runScript(cmd,placeholder)
            my_bar.progress(28)
            st.success('---Decompress Finished---')


            visionFirmwareInLocal = 'file://' + VISION_FIRMWARE_DIR + update.urlVisionTGZ.split('/')[-1].replace('.tgz',
                                                                                                                 '')
            with open('/tmp/updateURL', 'w') as fp:
                fp.seek(0)
                fp.truncate();
                fp.write(visionFirmwareInLocal)
                fp.flush()
                fp.close()
            cmd = "sudo cp -f /tmp/updateURL " + VISION_UPDATEURL_DIR
            os.system(cmd)
            my_bar.progress(40)
            progressCnt = 40
            cmd = "sudo /home/pi/Downloads/swupdate2.sh"
            st.write(cmd)
            result = runScriptVision(cmd, placeholder, my_bar, progressCnt)
            if result:
                st.success('---vision deploy successful---')
                my_bar.progress(100)
                placeholder.title('---vision deploy successful---')

                my_bar.progress(0)
                st.write("---Start Update EBox Firmware---")
                cmd = "sudo kill $(pidof ftrCartCtl)"
                os.system(cmd)
                my_bar.progress(10)

                cmd = "sudo " + EBOX_APP_DIR + "ftrEBoxUpdate"
                placeholder.text(cmd)
                result = runScriptEBox(cmd, placeholder, my_bar)
                my_bar.progress(80)

                if result:
                    st.success('---EBox deploy successful---')

                    st.write("Update Version File:" + update.prefixVersionJson + "\n")
                    url = update.prefixVersionJson
                    r = requests.get(url);
                    with open(VERSION_JSON_FILE_DIR, 'wb') as fp:
                        fp.seek(0)
                        fp.truncate()
                        fp.write(r.content)
                        fp.flush()
                        fp.close()
                    os.system('sync;sync;sync')
                    st.write(r.content)

                    my_bar.progress(100)
                    placeholder.title('---All Finished and SUCCESSFUL!!!---')
                else:
                    my_bar.progress(80)
                    st.warning('---EBox deploy FAIL---')
                    placeholder.title('---EBox deploy FAIL---')

            else:
                my_bar.progress(80)
                st.warning('---vision deploy FAIL---')
                placeholder.title('---vision deploy FAIL---')


        else:
            st.warning('---FAIL---')


def ConfigOA(somearg):
    # os.chmod(JSON_FILE_NAME,stat.S_IRUSR | stat.S_IWUSR | stat.S_IRGRP | stat.S_IWGRP | stat.S_IROTH | stat.S_IWOTH)
    os.system('sudo chmod 666 /home/pi/ftrCartCtl/settings.json')
    with open(JSON_FILE_NAME, 'r+', encoding='utf8') as fp:
        json_data = json.load(fp)
        oaItemsList = list();
        for key in list(json_data.keys()):
            if (key.find('oa') != -1):
                oaItemsList.append(key)

        selectItem = st.selectbox('Json Items:', oaItemsList)

        if (type(json_data[selectItem]) == bool):
            tup = ('False', 'True')
            stateModify = st.radio(selectItem, tup, json_data[selectItem])
            json_data[selectItem] = bool(tup.index(stateModify))

        buttonState = st.button('save')
        st.write(json_data)

        if (buttonState):
            fp.seek(0)
            fp.truncate()
            json.dump(json_data, fp, indent=4, ensure_ascii=False)
            fp.flush()
            fp.close()


def EditSettingJson(somearg):
    # os.chmod(JSON_FILE_NAME, stat.S_IRUSR | stat.S_IWUSR | stat.S_IRGRP | stat.S_IWGRP | stat.S_IROTH | stat.S_IWOTH)
    os.system('sudo chmod 666 /home/pi/ftrCartCtl/settings.json')
    with open(JSON_FILE_NAME, 'r+', encoding='utf8') as fp:
        json_data = json.load(fp)
        selectItem = st.selectbox('Json Items:', list(json_data.keys()))

        if (type(json_data[selectItem]) == bool):
            tup = ('False', 'True')
            stateModify = st.radio(selectItem, tup, json_data[selectItem])
            json_data[selectItem] = bool(tup.index(stateModify))

        elif (type(json_data[selectItem]) == int):
            numModify = st.number_input(selectItem, None, None, json_data[selectItem], 1)
            json_data[selectItem] = numModify

        elif (type(json_data[selectItem]) == float):
            numModify = st.number_input(selectItem, None, None, json_data[selectItem], 0.0001, '%.4f')
            json_data[selectItem] = numModify
        else:
            print(type(json_data[selectItem]))

        buttonState = st.button('save')
        st.write(json_data)

        if (buttonState):
            fp.seek(0)
            fp.truncate()
            json.dump(json_data, fp, indent=4, ensure_ascii=False)
            fp.flush()
            fp.close()


def EditSettingDJson(somearg):
    # os.chmod(JSON_D_FILE_NAME, stat.S_IRUSR | stat.S_IWUSR | stat.S_IRGRP | stat.S_IWGRP | stat.S_IROTH | stat.S_IWOTH)
    os.system('sudo chmod 666 /home/pi/ftrCartCtl/setting_d.json')
    with open(JSON_D_FILE_NAME, 'r+', encoding='utf8') as fp:
        json_data = json.load(fp)
        selectItem = st.selectbox('Json Items:', list(json_data.keys()))

        if (type(json_data[selectItem]) == bool):
            tup = ('False', 'True')
            stateModify = st.radio(selectItem, tup, json_data[selectItem])
            json_data[selectItem] = bool(tup.index(stateModify))
        elif (type(json_data[selectItem]) == int):
            numModify = st.number_input(selectItem, None, None, json_data[selectItem], 1)
            json_data[selectItem] = numModify

        elif (type(json_data[selectItem]) == float):
            numModify = st.number_input(selectItem, None, None, json_data[selectItem], 0.0001, '%.4f')
            json_data[selectItem] = numModify

        buttonState = st.button('save')
        st.write(json_data)

        if (buttonState):
            fp.seek(0)
            fp.truncate()
            json.dump(json_data, fp, indent=4, ensure_ascii=False)
            fp.flush()
            fp.close()


def Version(somearg):
    CTLplaceholder = st.empty()
    VISplaceholder = st.empty()
    EBOXplaceholder = st.empty()
    UIplaceholder = st.empty()
    SERplaceholder = st.empty()
    versionStr = ""
    with open(FTRCARTCTL_EBOX_VERSION_FILE_NAME, 'r') as fp:
        versionStr = '\r\n'.join(fp.readlines())
        fp.close()

    VisionVersionStr = ''
    with open(VISION_VERSION_FILE_NAME, 'r') as fp:
        VisionVersionStr = '\r\n'.join(fp.readlines())
        fp.close()
    st.info("vision:" + VisionVersionStr + '\n' + versionStr)

    EBoxVer = ""
    ftrCartCtlVer = ""
    with open(FTRCARTCTL_EBOX_VERSION_FILE_NAME, 'r') as fp:
        for line in fp:
            if line.__contains__('--- UK'):
                EBoxVer = '\nEBO:' + line.replace('--- UK-I', '').replace('---', '')
            if line.__contains__('ftrCartCtl'):
                ftrCartCtlVer = line.replace('ftrCartCtl Ver', 'CTL')
        fp.close()

    VisionVer = ''
    with open(VISION_VERSION_FILE_NAME, 'r') as fp:
        VisionVer = fp.readline()
        fp.close()

    AppServerVer = ''
    with open(APPSERVRER_VERSION_FILE_NAME, 'r') as fp:
        AppServerVer = fp.readline().split(':')[-1]
        fp.close()

    CTLplaceholder.subheader(ftrCartCtlVer)
    VISplaceholder.subheader('VIS:' + VisionVer)
    EBOXplaceholder.subheader(EBoxVer)
    UIplaceholder.subheader(VersionInfo)
    SERplaceholder.subheader('SER:' + AppServerVer)


def WheelCali(somearg):
    numInput = st.number_input("Calibration Dist:", None, None, 10000, 1)

    startButtonState = st.button('start')
    stopButtonState = st.button('save')
    cancleButtonState = st.button('cancle')
    if (startButtonState):
        outputStr = 'echo WheelCali:1,' + str(numInput) + ' > /tmp/ftrCartCtl_pipe_input';
        # fp.write(outputStr)
        os.system(outputStr)

    if (stopButtonState):
        outputStr = 'echo WheelCali:0,1 > /tmp/ftrCartCtl_pipe_input'
        os.system(outputStr)

    if (cancleButtonState):
        outputStr = 'echo WheelCali:0,0 > /tmp/ftrCartCtl_pipe_input'
        os.system(outputStr)


def CameraCali(somearg):
    startButtonState = st.button('start')
    if (startButtonState):
        image = Image.open('./sunrise.png')
        st.image(image, caption='Sunrise by the mountains', use_column_width=True)


def AccGyroCali(somearg):
    outputStr = 'echo AccGyroCali: > /tmp/ftrCartCtl_pipe_input'
    os.system(outputStr)


def GetCPUSerial():
    # Extract serial from cpuinfo file
    cpuserial = "0000000000000000"
    try:
        f = open('/proc/cpuinfo', 'r')
        for line in f:
            if (line.find('Serial') != -1):
                line = line.replace('Serial', '').lstrip().replace(':', '').lstrip()
                cpuserial = line
        f.close()
    except:
        cpuserial = "ERROR000000000"

    return cpuserial


def GetMAC_CPUID(somearg):
    # 获取mac地址
    with open('/sys/class/net/eth0/address', 'r') as fp:
        st.text_input("MAC:", fp.readline().upper())
        fp.close()
    cpuserial = GetCPUSerial().upper()
    st.text_input("CPUID:", cpuserial)

    os.system('sudo chmod 666 /home/pi/ftrCartCtl/license')
    with open(LICENCE_FILE_NAME, 'r') as fp:
        st.text_input("Licence:", fp.readline())
        fp.close()


def Licence(somearg):
    os.system('sudo chmod 666 /home/pi/ftrCartCtl/license')
    with open(LICENCE_FILE_NAME, 'r') as fp:
        st.code(fp.readline())
        fp.close()

    # confirmButtonState = st.button('Confirm')
    # if(confirmButtonState):
    #     if((len(licenceNum) == 32)):
    #         licenceNum
    #         os.system('sudo chmod 666 /home/pi/ftrCartCtl/license')
    #         with open(LICENCE_FILE_NAME,'w') as fp:
    #             fp.truncate()
    #             fp.seek(0)
    #             fp.write(licenceNum)
    #
    #             fp.close()
    #     else:
    #         st.write('licence NO. Error')


def Upload_Update(somearg):
    uploaded_file = st.file_uploader("file:", type=['md5', 'tgz'])
    if uploaded_file is not None:
        content = uploaded_file.read()
        with open(UPDATE_FILE_NAME, 'wb') as fp:
            fp.truncate()
            fp.write(content)
            fp.flush()
            fp.close()
        st.success('---upload finished---')
        my_bar = st.progress(0)
        placeholder = st.empty()
        placeholder.info('start to deploy')

        os.system('sync;sync;')
        cmd = 'sudo tar -zxvf ' + UPDATE_FILE_NAME + ' -C /tmp/';
        os.system(cmd)
        st.write('---------------------------------------')
        st.write('---------------------------------------')
        st.write(cmd)
        time.sleep(1)

        md5fileName = ''
        tgzfileName = ''
        for file in os.listdir(EBOX_FIRMWARE_DIR):
            if file.__contains__('.md5'):
                md5fileName = EBOX_FIRMWARE_DIR + file
            if file.__contains__('.tgz'):
                tgzfileName = EBOX_FIRMWARE_DIR + file

        st.write(md5fileName)
        md5Calc = md5sum(tgzfileName)

        md5Got = ''
        with open(md5fileName, 'r') as fp:
            fcont = fp.read()
            md5Got = fcont.split(' ')[0].replace('\n', '');

        if md5Got == md5Calc and os.path.getsize(tgzfileName) != 0:
            downloadEBoxResult = True
            my_bar.progress(10)

            cmd = 'sudo tar -zxvf ' + tgzfileName + ' -C ' + EBOX_APP_DIR
            st.write(cmd)
            runScript(cmd, placeholder)
            my_bar.progress(20)
        else:
            st.write("Fail!!\n\n")

        md5fileName = ''
        tgzfileName = ''
        for file in os.listdir(FTRCARTCTL_FIRMWARE_DIR):
            if file.__contains__('.md5'):
                md5fileName = FTRCARTCTL_FIRMWARE_DIR + file
            if file.__contains__('.tgz'):
                tgzfileName = FTRCARTCTL_FIRMWARE_DIR + file

        st.write(md5fileName)
        md5Calc = md5sum(tgzfileName)

        md5Got = ''
        with open(md5fileName, 'r') as fp:
            fcont = fp.read()
            md5Got = fcont.split(' ')[0].replace('\n', '');

        if md5Got == md5Calc and os.path.getsize(tgzfileName) != 0:
            downloadftrCartCtlResult = True
            my_bar.progress(30)

            cmd = 'sudo tar -zxvf ' + tgzfileName + ' -C ' + FTRCARTCTL_APP_DIR
            st.write(cmd)
            runScript(cmd, placeholder)
            my_bar.progress(40)
        else:
            st.write("Fail!!\n\n")

        md5fileName = ''
        tgzfileName = ''
        for file in os.listdir(VISION_FIRMWARE_DIR):
            if file.__contains__('.md5'):
                md5fileName = VISION_FIRMWARE_DIR + file
            if file.__contains__('.tgz'):
                tgzfileName = VISION_FIRMWARE_DIR + file
        visionFirmwareInLocal = 'file://' + VISION_FIRMWARE_DIR + tgzfileName.split('/')[-1].replace('.tgz', '')
        with open('/tmp/updateURL', 'w') as fp:
            fp.seek(0)
            fp.truncate();
            fp.write(visionFirmwareInLocal)
            fp.flush()
            fp.close()
        st.write(visionFirmwareInLocal)
        cmd = "sudo cp -f /tmp/updateURL " + VISION_UPDATEURL_DIR
        os.system(cmd)
        my_bar.progress(40)
        progressCnt = 40
        cmd = "sudo /home/pi/Downloads/swupdate2.sh"
        result = runScriptVision(cmd, placeholder, my_bar, progressCnt)
        if result:
            st.success('---vision deploy successful---')
            my_bar.progress(100)
            placeholder.title('---vision deploy successful---')

            my_bar.progress(0)
            st.write("---Start Update EBox Firmware---")
            cmd = "sudo kill $(pidof ftrCartCtl)"
            os.system(cmd)
            my_bar.progress(10)

            cmd = "sudo " + EBOX_APP_DIR + "ftrEBoxUpdate"
            placeholder.text(cmd)
            result = runScriptEBox(cmd, placeholder, my_bar)
            my_bar.progress(80)

            if result:
                st.success('---EBox deploy successful---')
                my_bar.progress(100)
                placeholder.title('---All Finished and SUCCESSFUL!!!---')
            else:
                my_bar.progress(80)
                st.warning('---EBox deploy FAIL---')
                placeholder.title('---EBox deploy FAIL---')

        else:
            my_bar.progress(80)
            st.warning('---vision deploy FAIL---')
            placeholder.title('---vision deploy FAIL---')


def Monitor(somearg):
    fileName = st.text_input("file:")
    if len(fileName) != 0:
        if os.path.exists(fileName):
            # p = subprocess.Popen(['cat', fileName], stdout=subprocess.PIPE)
            # st.write(p.stdout.read().decode('ascii'))
            with open(fileName, 'r') as fp:
                info = '\r\n'.join(fp.readlines())
                fp.close()
                info
        else:
            st.write(fileName, "no exist")


def ModeChange(somearg):
    SBButtonState = st.button('SB')
    VTPButtonState = st.button('Tape')
    VTKButtonState = st.button('Follow')
    MRButtonState = st.button('Motor Release')
    if SBButtonState:
        outputStr = 'echo ModeChange:SB > /tmp/ftrCartCtl_pipe_input';
        os.system(outputStr)

    if VTPButtonState:
        outputStr = 'echo ModeChange:VTP > /tmp/ftrCartCtl_pipe_input';
        os.system(outputStr)

    if VTKButtonState:
        outputStr = 'echo ModeChange:VTK > /tmp/ftrCartCtl_pipe_input';
        os.system(outputStr)

    if MRButtonState:
        outputStr = 'echo ModeChange:MR > /tmp/ftrCartCtl_pipe_input';
        os.system(outputStr)


exitFlag = True


def getStreamlitThreadState():
    cmd = 'cat /tmp/StreamlitThreadState'
    p = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE)
    value = int(p.stdout.readline().decode())
    if value:
        return True
    else:
        return False


class ReadPipeThread(threading.Thread):
    def __init__(self, ID):
        threading.Thread.__init__(self)
        self.m_ID = ID
        self.mapDict = {}
        print("Create")

    def run(self):
        # 0:run,1:stop
        cmd = 'echo 0 > /tmp/StreamlitThreadState'
        os.system(cmd)
        value = False
        os.system('sudo chmod 666 /tmp/map.json')
        stationsNum = 0
        # UpdateMap(self.placeholder)
        while not value:
            time.sleep(1)
            value = getStreamlitThreadState()
            with open(MAP_JSON_FILE_NAME, 'r+', encoding='utf8') as fp:
                json_data = json.load(fp)
                # if stationsNum != json_data['stations_num']:
                if stationsNum != json_data.items():
                    self.mapDict.clear()
                    stationsNum = json_data.items()  # json_data['stations_num']
                    for k, v in json_data.items():
                        if k != 'stations_num':
                            self.mapDict.update({k: v})
                            print({k: v})
                            # st.write({k: v})
                    with open('/tmp/mapDict', 'w') as fp:
                        fp.write(str(self.mapDict))
                        fp.flush()
                        fp.close()

    def readMapDict(self):
        with open('/tmp/mapDict', 'r') as fp:
            self.mapDict = eval(fp.read())
            fp.close()
        return self.mapDict

    def stop(self):
        cmd = 'echo 1 > /tmp/StreamlitThreadState'
        os.system(cmd)
        print('stop')


def UpdateMap(mapDict, placeholder):
    # edge arrowhead = vee
    dot = '''
        digraph{
            label=""
            labelloc = b
            labeljust = m
            rankdir = LR

            graph [compound=false]
            subgraph cluster_001{
                label="cluster_001"
                node [shape=box,color = green]
            }
            graph [bgcolor = white]
            node [style = bold,shape = circle,color=green,fontcolor=black,peripheries = 1,sides=5,width=0.5,height=0.3]
            edge [dir=forward,color=navy,weight=10,dir = both]
            {BODY}
        }
        '''
    body = ''
    for k, v in mapDict.items():
        if k != '0->0':
            content = k + '[label = "{label}"]\n'.format(label=v)
            body = body + content

    dot = dot.replace('{BODY}', body)
    placeholder.graphviz_chart(dot)


# def fn(i):
#     # UpdateMap(placeholder)
#     # st.write('h')
#     print("h")
#     threading.Timer(2,fn,(i,)).start()


def BuildMap(somearg):
    placeholder = st.empty()
    # chart_data = pd.DataFrame(np.random.randn(10, 1) * 50, columns=['a'])
    # placeholder.line_chart(chart_data)
    # placeholder.area_chart(chart_data)
    # placeholder.bar_chart(chart_data)
    # UpdateMap(placeholder)
    # fn(1)

    st.write('----------------------------------------------')

    startButton = st.button('Start')
    updateButton = st.button('Update')
    st.write('-----------------------------------------------')
    CancleButton = st.button('Cancle')
    ConfirmButton = st.button('Submit')
    CheckMapThread = ReadPipeThread(1)

    if startButton:
        outputStr = 'echo ModeChange:BuildMap > /tmp/ftrCartCtl_pipe_input';
        os.system(outputStr)
        CheckMapThread.start()
        st.write('start')

    if updateButton:
        mapDict = CheckMapThread.readMapDict()
        UpdateMap(mapDict, placeholder)

    if CancleButton:
        outputStr = 'echo ModeChange:SB > /tmp/ftrCartCtl_pipe_input';
        os.system(outputStr)
        CheckMapThread.stop()
        st.write('cancle')

    if ConfirmButton:
        outputStr = 'echo MapSave: > /tmp/ftrCartCtl_pipe_input';
        os.system(outputStr)
        CheckMapThread.stop()
        st.write("save map")


def EditMap(someagr):
    placeholder = st.empty()
    with open(MAP_JSON_FILE_NAME, 'r+', encoding='utf8') as fp:
        json_data = json.load(fp)
        keyList = list(json_data.keys())
        del keyList[-1]
        selectItem = st.selectbox('Map Stations:', keyList)

        if type(json_data[selectItem]) == str:
            json_data[selectItem] = st.text_input('Dist,Speed:', json_data[selectItem])

        buttonState = st.button('save')
        st.write(json_data)
        fp.seek(0)
        fp.truncate()
        json.dump(json_data, fp, indent=4, ensure_ascii=False)
        fp.flush()
        fp.close()

        with open(MAP_JSON_FILE_NAME, 'r+', encoding='utf8') as f:
            json_data = json.load(f)
            f.close()
            mapDict = {}
            for k, v in json_data.items():
                if k != 'stations_num':
                    mapDict.update({k: v})
                    # print({k: v})
            with open('/tmp/mapDict', 'w') as fpp:
                fpp.write(str(mapDict))
                fpp.flush()
                fpp.close()

            UpdateMap(mapDict, placeholder)

        if buttonState:
            outputStr = 'echo MapSave: > /tmp/ftrCartCtl_pipe_input';
            os.system(outputStr)
            st.write("save map")


def UploadRoute(somearg):
    uploaded_file = st.file_uploader("route file:", type=['json'])
    if uploaded_file is not None:
        content = uploaded_file.read()
        with open(UPLOAD_ROUTE_FILE_NAME, 'w') as fp:
            fp.truncate()
            fp.write(content)
            fp.flush()
            fp.close()
        cmd = "sudo cp -f " + UPLOAD_ROUTE_FILE_NAME + " /home/pi/ftrCartCtl/route.json"
        os.system(cmd)
        st.success('---upload finished---')

    faceOption = st.selectbox("face:", ('Cisoid', 'Reverse'))
    currentStation = st.number_input("C-Station", 0, None, 0, 1, "%d")
    submitButton = st.button("submit")
    if submitButton:
        face = '1'
        if faceOption == 'Reverse':
            face = '0'

        cmd = "echo VTPInit:" + face + "," + str(currentStation) + " > /tmp/ftrCartCtl_pipe_input"
        os.system(cmd)


def do_default(somearg):
    print("default:", somearg)


switch = {
    0: Version,
    1: ConfigOA,
    2: EditSettingJson,
    3: EditSettingDJson,
    4: WheelCali,
    5: CameraCali,
    6: AccGyroCali,
    7: GetMAC_CPUID,
    8: Upload_Update,
    9: WIFISetting,
    10: Update,
    11: Monitor,
    12: ModeChange,
    13: BuildMap,
    14: EditMap,
    15: UploadRoute,
}
# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    # map_data = pd.DataFrame(
    #     np.random.randn(1000, 2) / [50, 50] + [37.76, -122.4],
    #     columns=['lat', 'lon'])
    #
    # st.map(map_data)

    # option = st.selectbox(
    #     'Which number do you like best?',
    #     df['first column'])
    #
    # 'You selected: ', option

    funChoiseList = ['Version', 'OA Config', 'Show&Edit Setting.json', 'Show&Edit Setting_d.json', 'WheelCali',
                     'CameraCali', 'AccGyroCali', 'GetMAC_CPUID', 'Upload&Update', 'WIFISetting',
                     'Update',
                     'Monitor', 'Mode Change', 'Build Map', 'Edit Map', 'Upload Route'];
    option = st.sidebar.selectbox(
        'Function Choise:',
        funChoiseList)
    if not os.path.exists(SSID_LIST_FILE_NAME):
        wifi = pywifi.PyWiFi()
        iface = wifi.interfaces()[0]
        iface.scan()
        time.sleep(2)
        ssidlist = []
        result = iface.scan_results()
        for i in range(len(result)):
            ssidlist.append(result[i].ssid)

        m = np.array(ssidlist)
        np.save(SSID_LIST_FILE_NAME, m)
    # 'You selected:', funChoiseList.index(option),option
    st.title(option)
    # switch[funChoiseList.index(option)](option)
    switch.get(funChoiseList.index(option), do_default)(option)

    # x=st.sidebar.slider('')
    # x

    # 'Starting a long computation...'

    # Add a placeholder
    # latest_iteration = st.empty()
    # bar = st.progress(0)
    #
    # for i in range(100):
    #     # Update the progress bar with each iteration.
    #     latest_iteration.text(f'Iteration {i + 1}')
    #     bar.progress(i + 1)
    #     time.sleep(0.1)
    #
    # '...and now we\'re done!'
# See PyCharm help at https://www.jetbrains.com/help/pycharm/
