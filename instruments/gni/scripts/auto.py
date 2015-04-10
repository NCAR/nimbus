#!/usr/bin/python
#
# Simple script to loop exposing slide command every few seconds.
# Written for lab/shop testing purposes.
#

import serial
import time


def readMenu(ser):
  while True:
    line = ser.readline()
    if line.startswith('10 '):	# Last menu item starts with "10", read until then.
      break



ser = serial.Serial('/dev/ttyS0', 9600)

i = 0
while True:
  readMenu(ser)
  ser.write('7\n')
  print i
  i = i + 1
  time.sleep(3)

ser.close()
