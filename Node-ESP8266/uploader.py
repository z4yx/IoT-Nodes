#!/usr/bin/env python

import sys
import serial
import time

if len(sys.argv)<5:
    print "%s <serial-dev> <baud> <src-file> <dest-file>" % sys.argv[0]
    exit()
ser = serial.Serial(sys.argv[1], sys.argv[2])

with open(sys.argv[3]) as f:
    ser.write("file.open('%s','w')\r\n" % sys.argv[4])
    time.sleep(0.4)
    for line in f:
        ser.write("file.writeline([[%s]])\r\n" % line.rstrip('\r\n'))
        ser.flush()
        time.sleep(0.2)
    ser.write("file.close()\r\n")
