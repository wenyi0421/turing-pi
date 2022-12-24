#! /bin/sh

check_ip() {
    IP=$1
    VALID_CHECK=$(echo $IP|awk -F. '$1<=255&&$2<=255&&$3<=255&&$4<=255{print "yes"}')
    if echo $IP|grep -E "^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$">/dev/null; then
        if [ ${VALID_CHECK:-no} == "yes" ]; then
            echo "IP $IP available."
            ifconfig eth0 $IP up
        else
            echo "IP $IP not available!"
        fi
    else
        echo "IP format error!"
    fi
}

echo " _____ _   _ ____  ___ _   _  ____ "
echo "|_   _| | | |  _ \|_ _| \ | |/ ___|"
echo "  | | | | | | |_) || ||  \| | |  _ "
echo "  | | | |_| |  _ < | || |\  | |_| |"
echo "  |_|  \___/|_| \_\___|_| \_|\____|"
bmctest &
mount /dev/mmcblk0p1 /mnt/sdcard/
sleep 1
if [ -f /mnt/sdcard/ip.txt ];then
        ip=`cat /mnt/sdcard/ip.txt`
        check_ip $ip
fi
echo 3 4 1 7 > /proc/sys/kernel/printk
/etc/test_ping.sh &
