#!/usr/bin/python
#
# Server relay program for Auto GNI.  Runs on a Raspberry PI and converts/relays commands 
# from a client UI via UDP to the GNI instrument via RS-232.
#

import select
import socket
import sys
import time

UDP_IP = "127.0.0.1"
UDP_READ_PORT = 32100
UDP_SEND_PORT = 32101

#UDP_IP = "192.168.84.204"
#UDP_PORT = 41002

def printMenu():
  print "Please issue a command:"
  print "0 = Status"
  print "1 = Home Drives"
  print "2 = Advance Carousel"
  print "3 = Advance Slide"
  print "4 = Load Slide"
  print "5 = Expose Slide"
  print "6 = Retract Slide"
  print "7 = Advance, Load, Expose & Retract"
  print "8 = Retract Slide Pickup"
  print "9 = Home Shuttle"
  print "10 = Home Slide Actuator"



# Connection to User Interface
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(("0.0.0.0", UDP_READ_PORT))


while True:
  printMenu()

  readable, writable, exceptional = select.select([sys.stdin,sock,],[],[])

  for s in readable:
    if s is sock:
      data = s.recv(1024)
      print 'STATUS :' + data
    else:
      data = sys.stdin.readline()
      t = time.time()
      ts = time.strftime("%Y%m%dT%H%M%S", time.gmtime(t))
      msg = 'GNI,' + ts + ',' + data
      sock.sendto(msg, (UDP_IP, UDP_SEND_PORT))

sock.close()
