#!/bin/sh

echo "Starting wib_server"
/bin/wib_server CRYO 2>/var/log/wib_server.err >/var/log/wib_server.log &

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

echo "Starting rogue server"
python3 /home/root/sw_cryo/python/script/cryo_wib_server.py --type=wib-hw \
        </dev/null 2>/var/log/cryo_wib_server.err >/var/log/cryo_wib_server.log &
