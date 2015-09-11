#!/usr/bin/python
#
# Server relay program for Auto GNI.  Runs on a Raspberry PI and converts/relays commands 
# from a client UI via UDP to the GNI instrument via RS-232.
#

import select
import serial
import socket
import sys
import time

UDP_IP = "192.168.84.1"
UDP_PORT = 41002

def readMenu(ser):
  while True:
    line = ser.readline()
    if line.startswith('10 '):	# Last menu item starts with "10", read until then.
      break



# Connection to GNI.
ser = serial.Serial('/dev/ttyS0', 9600)

# Connection to User Interface
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
#sock.bind((UDP_IP, UDP_PORT))


while True:
  readable, writable, exceptional = select.select([ser,sock,],[],[ser,sock,])

  for s in readable:
    if s is sock:
      data = s.recv(1024)
      print data
      ser.write(data)
    else:
      data = s.readline()
      print data
      sock.sendTo(data, (UDP_IP, UDP_PORT))

ser.close()
sock.close()
