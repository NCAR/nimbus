#!/usr/bin/python
#
# Server relay program for Auto GNI.  Runs on a Raspberry PI and converts/relays commands 
# from a client UI via UDP to the GNI instrument via RS-232.
#

import os
import select
import serial
import socket
import sys
import time

UDP_IP = "192.168.84.1"
UDP_PORT = 41002


sigCnt = 0
timedExposure = 0.0
statusElapsed = 0.0

# Connection to User Interface
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(("0.0.0.0", UDP_PORT))

# Connection to GNI.
ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=0)
ser.nonblocking()


def readMenu(ser):
  while True:
    line = ser.readline()
    if line.startswith('10 '):	# Last menu item starts with "10", read until then.
      break

def rqst_status():
  print "requesting status"
  ser.write("0")

def send_status():
  global sigCnt
  s = "GNI,," + str(sigCnt)
  sock.sendto(s, (UDP_IP, UDP_PORT))
  sigCnt = sigCnt + 1


while True:
  start = time.time()
  readable, writable, exceptional = select.select([ser,sock,],[],[ser,sock,], 1.0)
#  readable, writable, exceptional = select.select([sys.stdin,sock,],[],[], 1.0)
  elapsed = time.time() - start

  if timedExposure:
    exposeTime = exposeTime - elapsed
    if exposeTime <= 0:
      ser.write("6") # retract slide
      timedExposure = 0

  statusElapsed = statusElapsed + elapsed
  if statusElapsed > 2.0:
    rqst_status()
    statusElapsed = 0.0

  print "elapse = " + str(elapsed)
#  send_status()

  for s in readable:
    if s is sock:
      data = s.recv(1024)
      print 'Read ' + data
      ser.write(data)
    else:
      n = s.inWaiting()
#      readMenu(ser)
      data = s.read(n)
#      data = sys.stdin.readline()
      print 'Sending ' + str(n) + ', [' + data + ']'
      sock.sendto(data, (UDP_IP, UDP_PORT))

ser.close()
sock.close()
