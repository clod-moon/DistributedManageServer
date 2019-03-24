#!/bin/bash
#放在build目录，打包S2的release版本

echo "发布版本，记得修改版本号！"
echo "有新增加的模块或者文件，请记得打包进去！"

read -p "input version:" ver
dirname=OperateServer
tarname=~/${dirname}_LINUX-ARM-S2_$ver.tar.gz

now=`date +"%F %T"`
echo "time:$now, version:$ver" >> version.log

if [ -e ~/$dirname ]; then
 rm -rf ~/$dirname/*
else
 mkdir ~/$dirname
fi
#bin dir
cp ../bin/linux-s2/libIVESoftInfSDK.so  ~/$dirname/
cp ../bin/linux-s2/libVirtualNetSwitchManager.so  ~/$dirname/
cp ../bin/linux-s2/libVtAlarm.so  ~/$dirname/
cp ../bin/linux-s2/libVtConfigManager.so  ~/$dirname/
cp ../bin/linux-s2/libVtDeviceCtrl.so  ~/$dirname/
cp ../bin/linux-s2/libVtDeviceManager.so  ~/$dirname/
cp ../bin/linux-s2/libVtDeviceSrchCfg.so  ~/$dirname/
cp ../bin/linux-s2/libVtEventDistribution.so  ~/$dirname/
cp ../bin/linux-s2/libVtGPS.so  ~/$dirname/
cp ../bin/linux-s2/libVTHardwareInf.so  ~/$dirname/
cp ../bin/linux-s2/libVtLicenseManager.so  ~/$dirname/
cp ../bin/linux-s2/libVtLogManager.so  ~/$dirname/
cp ../bin/linux-s2/libVtNetFileTransfer.so  ~/$dirname/
cp ../bin/linux-s2/libVtPlanTask.so  ~/$dirname/
cp ../bin/linux-s2/libVtServiceManager.so  ~/$dirname/
cp ../bin/linux-s2/libVtStatusManager.so  ~/$dirname/
cp ../bin/linux-s2/libVtSyncTool.so  ~/$dirname/
cp ../bin/linux-s2/libVtSysCustomize.so  ~/$dirname/
cp ../bin/linux-s2/libVtTaskSchedule.so  ~/$dirname/
cp ../bin/linux-s2/libVtThriftWork.so  ~/$dirname/
cp ../bin/linux-s2/libVtUpgradeProc.so  ~/$dirname/
cp ../bin/linux-s2/libVtVideoManage.so  ~/$dirname/
cp ../bin/linux-s2/libVtNetCtrlManager.so  ~/$dirname/
cp ../bin/linux-s2/libVtWebService.so  ~/$dirname/
cp ../bin/linux-s2/libVtBusLineSync.so  ~/$dirname/
cp ../bin/linux-s2/OperateServer  ~/$dirname/
cp ../bin/linux-s2/startup.sh  ~/$dirname/
cp -r ../bin/linux-s2/tools  ~/$dirname/
#other file
cp ../code/src/VtWebService/vtns.wsdl ~/$dirname
cp ../deploy/ServerConfig_s2.xml ~/$dirname/ServerConfig.xml
cp ../deploy/ive_config_s2.xml ~/$dirname/config.xml #IVE库用的
#cp ../doc/ChangeLog.html ~/$dirname
cp ../bin/linux-s2/sys-custom.json ~/$dirname
echo "time:$now, version: $ver" |tee ~/$dirname/last_build.log

cd ~
tar -zcvf $tarname $dirname
echo "OK. package is: $tarname"

