#!/bin/bash
#放在build目录，打包TK1的debug版本
v=$1
dirname=OperateServer
tarname=${dirname}_LINUX-X86-64_$v.tar.gz

if [ "$v" == "" ];then
	echo "  Usage: $0 version"
	exit 1
fi

now=`date +"%F %T"`
echo "time:$now, version:$v" >> version.log

if [ -e ~/$dirname ]; then
 rm -rf ~/$dirname/*
else
 mkdir ~/$dirname
fi

chmod +x ../bin_d/linux-x86_64/*.sh

cp -r ../bin_d/linux-x86_64/* ~/$dirname/
cp ../code/src/VtWebService/vtns.wsdl ~/$dirname
cp ../deploy/ServerConfig-linux-x86_64.xml ~/$dirname/ServerConfig.xml
cp ../deploy/config.xml ~/$dirname #IVE库用的
#cp ../doc/ChangeLog.html ~/$dirname
cp ../bin_d/linux-x86_64/sys-custom.json ~/$dirname
echo "time:$now, version:$v" |tee ~/$dirname/last_build.log

cd ~
find $dirname -name ".svn" -exec rm -rf {} \;
tar zcvf ~/$tarname $dirname
echo "package is: ~/$tarname"
