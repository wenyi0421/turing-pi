#!/bin/sh

date=$(date +%F)

version=$1

if [ 1 != $# ]; then
	echo please input version
fi
echo "${1}" |grep "^[0-9]\{1,3\}\.[0-9]\{1,3\}\.[0-9]\{1,3\}$" > /dev/null
if [ $? = 1 ];then
	echo version 0~999 and must 3 version eg:1.1.1
	echo ./mkfw.sh "<version>"
	exit 1
fi

echo  "/* This file is auto generated. do not modify */ 
#define BMCVERSION \"${version}\"
#define BUILDTIME \"${date}\" " > app/bmc/version.h

if [ ! -d "build/${date}" ];then
    echo "mkdir build/${date}"
    mkdir -p "build/${date}"
fi

echo "----- make fw -----" 
echo "Version: ${version}" 
echo "Date: ${date}"

echo "build fw"
make -C buildroot bmc-rebuild V=1
make -C buildroot V=1
cp -vrf buildroot/output/images/buildroot_linux_nand_uart3.img "./build/${date}/turingpi-${version}.img"

cd buildroot/output/images/ || exit 1
./genSWU.sh
cd - || exit 1

cp -vrf ./buildroot/output/images/turingpi_.swu ."/build/${date}/turingpi-${version}.swu"

echo "build turing pi firmware over"
if [ ! -f "build/tpi/linux/tpi" ];then
	mkdir -p build/tpi/linux
	echo "build tpi cli"
	gcc app/tpi/tpi.c -o build/tpi/linux/tpi
	echo "build tpi cli over"
fi

