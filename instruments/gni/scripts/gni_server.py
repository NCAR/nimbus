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

from optparse import OptionParser
import gni

class GNISerial(object):

  def __init__(self, device=None):
    "Serial connection to GNI."
    if not device:
      device = '/dev/ttyUSB0'
    self.device = device
    self.sport = serial.Serial(self.device, 9600, timeout=0)
    self.sport.nonblocking()
    # This is a buffer of characters read from the serial port.  It
    # should be replaced with a real buffer at some point.
    self.cdata = ""
    self.status = GNIStatus()
    # Keep a record of when the full menu has been received from the
    # GNI, as a sanity check and to make it possible to keep the
    # message exchange more orderly.
    self.gotmenu = False

  def getSerial(self):
    return self.sport

  def requestStatus(self):
    logger.debug("requesting status")
    self.sport.write("0\r\n")
    self.gotmenu = False

  def retractSlide(self):
    self.sport.write("6") # retract slide
    self.gotmenu = False

  def readData(self):
    "Read data from the serial port and parse it for status updates."
    rdata = self.sport.read(512)
    logger.debug("read data: " + repr(rdata))
    self.cdata = self.cdata + rdata
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

  def setStatusHandler(self, handler):
    self._status_handler = handler

  def handleStatus(self):
    "Call a handler function on status changes."
    if self._status_handler:
      logger.debug("calling status handler")
      self._status_handler(self)

  def handleLine(self, text):
    logger.debug("handling line: " + text.rstrip())
    if text.startswith("GNI,"):
      self.status.parseStatusLine(text)
      logger.info("GNI status line: %s" % (self.status.getMessage()))
      self.handleStatus()
    elif text.startswith("10 = Home Slide Actuator"):
      self.gotmenu = True
    elif re.search("^Set In Program Mode", text):
      # Ignore this line
      pass
    elif re.match("^\d\d? = ", text):
      # Menu entry
      pass
    elif re.match("^\d+\s*$", text):
      # Command echo
      pass
    elif not text:
      # Empty line
      pass
    else:
      # Everything not recognized as a status or menu line is just a "data
      # line".  The line is logged as an info message so it shows up in the
      # output with the default logging level, since that's what users seem
      # to like, but probably something smarter can be done with it.  For
      # example, consider relaying these lines back through the server so
      # they can be seen by the client, as long as they are not too large.
      # Maybe there should be two different modes, either high or low
      # bandwitdh, where everything can be sent back to the client in high
      # bandwidth mode, even the menu.
      logger.info("GNI data line: %s" % (text))

  def getStatus(self, when=None):
    return self.status.getMessage(when)

  def close(self):
    self.sport.close()


class GNIStatus(object):
  "Keep track of the current state of the GNI."

  def __init__(self):
    self.error = False
    self.message = None
    self.timestamp = None

  def formatTimestamp(self, when=None):
    if when is None:
      when = self.timestamp
    if when is None:
      when = time.time()
    stime = time.strftime("%Y%m%dT%H%M%S", time.gmtime(when))
    stime = stime + '.' + str(int((when - int(when)) * 10))
    return stime

  def getMessage(self, when=None):
    "Return the latest status message, or else build one."
    if self.message is not None:
      return self.message
    # Create a null status as of when or now.
    if not when:
      when = time.time()
    self.timestamp = when
    return "GNI,%s," % (self.formatTimestamp())
    
  def parseStatusLine(self, line, when=None):
    """
    Parse lines like GNI,,c,9,c3,1,1 and GNI,,Controller ready

    Insert a timestamp into the second field if it's empty.
    """
    line = line.strip()
    self.message = line
    if when is None:
      when = time.time()
    self.timestamp = when
    fields = line.split(',')
    if len(fields) < 3 or fields[0] != "GNI":
      logger.error("Unexpected status line: %s" % (line))
    if len(fields) > 1 and fields[0] == "GNI" and len(fields[1]) == 0:
      # Add local timestamp if none from GNI.
      fields[1] = self.formatTimestamp()
      self.message = 'GNI,' + fields[1] + line[4:]


class GNIServer(object):

  def __init__(self):
    self.UDP_IP = "192.168.84.255"
    self.UDP_IP = "127.0.0.1"
    self.UDP_SEND_PORT = 32100
    self.UDP_READ_PORT = 32101
    self.UDP_NIDAS_PORT = 32102
#    self.UDP_PORT = 41002
    self.timerq = gni.TimerQueue()
    self.timedExposure = 0.0
    self.statusElapsed = 0.0
    self.exposeTime = 0.0

    # Connection to the GNI serial interface
    self.gni = None

    # Connection to User Interface
    self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
    self.sock.bind(("0.0.0.0", self.UDP_READ_PORT))

  def sendClient(self, message):
    logger.debug("sending: %s" % (message))
    self.sock.sendto(message, (self.UDP_IP, self.UDP_SEND_PORT))
    self.sock.sendto(message, (self.UDP_IP, self.UDP_NIDAS_PORT))

  def setSerialGNI(self, serial):
    self.gni = serial

  def sendStatus(self):
    self.sendClient(self.gni.getStatus())

  def loop(self):
    # Every 5 seconds send the latest status.  This is in addition to
    # sending the status whenever a new status is received.
    tv = gni.TimerEvent(5, repeating=True)
    tv.setHandler(lambda tv: self.sendStatus())
    self.timerq.append(tv)

    # Every 2 seconds request a new status.  When the GNI responds, the
    # updated status will be immediately sent out from inside the event
    # loop.
    tv = gni.TimerEvent(2, repeating=True)
    tv.setHandler(lambda tv: self.gni.requestStatus())
    self.timerq.append(tv)

    # Whenever the serial port receives a new status, we want to send it
    # immediately.
    self.gni.setStatusHandler(lambda sp: self.sendStatus())

    while True:
      ser = self.gni.getSerial()
      ports = [ser, self.sock]
      start = time.time()
      remaining = self.timerq.timeToNextEvent(99)
      # We do not need to check for any "exceptional conditions" on the
      # fds.  According to the select_tut(2) man page, we are not
      # interested in any of them.
      readable, writable, exceptional = select.select(ports, [], [], remaining)
      elapsed = time.time() - start
      logger.debug("elapse = " + str(elapsed))

      self.timerq.handleExpired()

      if self.timedExposure:
        self.exposeTime -= elapsed
        if self.exposeTime <= 0:
          self.gni.retractSlide()
          self.timedExposure = 0

      if self.sock in readable:
        data = self.sock.recv(1024)
        print 'Read ' + data
        fields = data.split(',')
        ser.write(fields[2])
        self.sock.sendto(data, (self.UDP_IP, self.UDP_NIDAS_PORT))
      if ser in readable:
        self.gni.readData()

  def close(self):
    self.sock.close()


_usage = """%prog [log-level-option] [--device <device>]

Run the server for a GNI connected to the given serial device.

The default INFO logging level prints all status and data lines received
from the GNI, skipping all the menu lines.  Specifying the ERROR logging
level will suppress all output except for errors.  The DEBUG logging level
is much more verbose, showing most messages back and forth as well as some
timing information."""



def main(args):
  parser = OptionParser(usage=_usage)
  parser.add_option("--debug", dest="level", action="store_const",
                    const=logging.DEBUG, default=logging.INFO)
  parser.add_option("--info", dest="level", action="store_const",
                    const=logging.INFO, default=logging.INFO)
  parser.add_option("--error", dest="level", action="store_const",
                    const=logging.ERROR, default=logging.INFO)
  parser.add_option("--device", type="string", default='/dev/ttyUSB0')
  (options, args) = parser.parse_args(args)
  logging.basicConfig(level=options.level)
  server = GNIServer()
  device = GNISerial(options.device)
  server.setSerialGNI(device)
  server.loop()
  server.close()
  device.close()


if __name__ == "__main__":
  main(sys.argv)


