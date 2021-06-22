#!/bin/sh

# https://github.com/kvtsang/wib-cryo
SERVICE=/usr/bin/cryo_service
[ ! -f $SERVICE ] && echo "$SERVICE not installed" && exit 1

$SERVICE wib start

#Evidentally this crashes the whole system somehow...
#echo "Starting xvcserver"
#/usr/bin/xvcserver  2>/var/log/xvcserver.err >/var/log/xvcserver.log &

sleep 3

echo "Setting 62.5 MHz clock"
timeout 60 wib_client script si5344_62p5mhz_cryo_config || exit 1

echo "Loading custom FW"
echo "0" > /sys/class/fpga_manager/fpga0/flags
echo "wib_top.bit" > /sys/class/fpga_manager/fpga0/firmware

echo "Starting xvcserver"
/home/root/xvcserver </dev/null 2>/var/log/xvcserver.err >/var/log/xvcserver.log &

$SERVICE rogue start
