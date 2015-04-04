#!/bin/bash
BROKER=192.168.1.2

mosquitto_pub -h $BROKER -t /bootstrap/script -f bootstrap.lua -r
mosquitto_pub -h $BROKER -t /bootstrap/list -m "`ls *.lua`" -r
for i in *.lua;do mosquitto_pub -h $BROKER -t /bootstrap/file/$i -f $i -r;done