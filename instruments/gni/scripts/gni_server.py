#!/usr/bin/python
#
# Server relay program for Auto GNI.  Runs on a Raspberry PI and
# converts/relays commands from a client UI via UDP to the GNI instrument
# via RS-232.
#
# Requires the pyserial package, preferably version 3.0:
#
#   Github: https://github.com/pyserial/pyserial
#   PyPI downloads: https://pypi.python.org/pypi/pyserial
#
# On the Raspberry PI, download the tar.gz link with wget, then extract the
# tar file:
#
#   tar zxf pyserial-3.0.tar.gz
#   cd pyserial-3.0
#   python setup.py install --user
#
# That installs pyserial into the user's site-packages directory.  pyserial
# 3.0 includes a miniterm tool that can be used to test the GNI serial
# connection directly, like so:
#
#   python -m serial.tools.miniterm /dev/ttyUSB0
#

import os
import re
import select
import serial
import socket
import sys
import time
import logging

logger = logging.getLogger(__name__)

# When the GNI is prompted with a string of "0\r\n", it echoes back
# the 0 then prints the status line followed by the menu, but it does
# it twice:

_example_menu = """
0
GNI,,c,9,c3,1,1

Set In Program Mode

0 = Status
1 = Home Drives
2 = Advance Carousel
3 = Advance Slide
4 = Load Slide
5 = Expose Slide
6 = Retract Slide
7 = Advance, Load, Expose & Retract
8 = Retract Slide Pickup
9 = Home Shuttle
10 = Home Slide Actuator

GNI,,c,9,c3,1,1

Set In Program Mode

0 = Status
1 = Home Drives
2 = Advance Carousel
3 = Advance Slide
4 = Load Slide
5 = Expose Slide
6 = Retract Slide
7 = Advance, Load, Expose & Retract
8 = Retract Slide Pickup
9 = Home Shuttle
10 = Home Slide Actuator
"""

class GNISerial:

  def __init__(self):
    "Serial connection to GNI."
    self.sport = serial.Serial('/dev/ttyUSB0', 9600, timeout=0)
    self.sport.nonblocking()
    # This is a buffer of characters read from the serial port.  It
    # should be replaced with a real buffer at some point.
    self.cdata = ""
    self.status = GNIStatus()
    # Keep a record of when the full menu has been received from the
    # GNI, as a sanity check and to make it possible to keep the
    # message exchange more orderly.
    self.gotmenu = False
    self.gotstatus = False

  def getSerial(self):
    return self.sport

  def requestStatus(self):
    logger.debug("requesting status")
    self.sport.write("0\r\n")
    self.gotmenu = False
    self.gotstatus = False

  def retractSlide(self):
    self.sport.write("6") # retract slide
    self.gotmenu = False

  def readData(self):
    "Read data from the serial port and parse it for status updates."
    self.cdata = self.cdata + self.sport.read(128)
    self.handleData()

  def handleData(self):
    # Only handle lines whose newline has been received.
    lines = self.cdata.splitlines()
    remainder = ""
    if lines and not self.cdata.endswith("\n"):
      remainder = lines[-1]
      lines = lines[:-1]
    self.cdata = remainder
    for l in lines:
      self.handleLine(l)

  def handleLine(self, text):
    if text.startswith("GNI"):
      self.status.parseStatusLine(text)
      self.gotstatus = True
    elif text.startswith("10 = Home Slide Actuator"):
      self.gotmenu = True
    elif re.match("^\d\d? = ", text):
      # Menu entry
      pass
    elif not text:
      # Empty line
      pass
    else:
      logger.error("Unrecognized data line from GNI: %s" % (text))

  def getStatus(self):
    return self.status.message

  def close(self):
    self.sport.close()


class GNIStatus:
  "Keep track of the current state of the GNI."

  def __init__(self):
    self.error = False
    self.fields = None
    self.message = None

  def parseStatusLine(self, line):
    "Parse lines of the form: GNI,,c,9,c3,1,1"
    line = line.strip()
    self.message = line
    fields = line.split(',')
    if len(fields) != 7 or fields[0] != "GNI":
      raise Exception("Could not parse status line: %s" % (line))
    if len(fields[1]) == 0:
      # Add local timestamp if none from GNI.
      t = time.time()
      fields[1] = time.strftime("%Y%m%dT%H%M%S", time.gmtime(t))
      fields[1] = fields[1] + '.' + str(int((t - int(t)) * 10))
      self.message = 'GNI,' + fields[1] + line[4:]
    self.fields = fields



class GNIServer:

  def __init__(self):
    self.UDP_IP = "192.168.84.1"
    self.UDP_SEND_PORT = 32100
    self.UDP_READ_PORT = 32101
#    self.UDP_PORT = 41002
    self.sigCnt = 0
    self.timedExposure = 0.0
    self.statusElapsed = 0.0
    self.exposeTime = 0.0

    # Connection to the GNI serial interface
    self.gni = None

    # Connection to User Interface
    self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    self.sock.bind(("0.0.0.0", self.UDP_READ_PORT))

  def sendClient(self, message):
    logger.debug("sending: %s" % (message))
    self.sock.sendto(message, (self.UDP_IP, self.UDP_SEND_PORT))

  def setSerialGNI(self, gni):
    self.gni = gni

  def sendStatus(self):
    s = "GNI,," + str(self.sigCnt)
    self.sendClient(s)
    self.sigCnt += 1

  def loop(self):
    while True:
      start = time.time()
      ser = self.gni.getSerial()
      ports = [ser, self.sock]
      readable, writable, exceptional = select.select(ports, [], ports, 1.0)
      elapsed = time.time() - start

      if self.timedExposure:
        self.exposeTime -= elapsed
        if self.exposeTime <= 0:
          self.gni.retractSlide()
          self.timedExposure = 0

      self.statusElapsed += elapsed
      if self.statusElapsed > 2.0:
        self.gni.requestStatus()
        self.statusElapsed = 0.0

      logger.debug("elapse = " + str(elapsed))

      if self.sock in readable:
        data = self.sock.recv(1024)
        print 'Read ' + data
        fields = data.split(',')
        ser.write(fields[2])
      if ser in readable:
        self.gni.readData()
        if self.gni.gotstatus:
          self.sendClient(self.gni.getStatus())
          self.gni.gotstatus = False

  def close(self):
    self.sock.close()


logging.basicConfig(level=logging.DEBUG)

server = GNIServer()
gni = GNISerial()
server.setSerialGNI(gni)
server.loop()
server.close()
gni.close()
