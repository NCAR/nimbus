#! /usr/bin/python
# -*- mode: python; python-indent-offset: 2; -*-
"""
Emulate the GNI serial interface.

This module can be used in a couple ways.  From the command line it can be
used to start up the GNI emulator on an existing serial port, meaning it
will read commands and respond with a menu.  The port can be a real serial
port or a pty device created separately.  Here's an example command-line:

  gni_emulator.py --debug /dev/ttyS0

If this module is run with the special name 'pty', then it creates a pair
of virtual serial port devices using 'socat', and it connects the emulator
to one of the ports.  The other port can be passed to the gni_server.py
script or opened with a terminal program.  The port should behave as if it
was connected to a GNI, with whatever functionality is implemented in the
emulator.  The path to the GNI port is printed in the "using virtual port"
line of the output:

  > gni_emulator.py pty
  pty device specified, using virtual serial port: /tmp/tmp15Bo4Q/gniport

Finally this module can be imported in python and used to create the
virtual port pair and the connected emulator at runtime.  The tests in
test_gni_server.py demonstrate this functionality.  Eventually
gni_server.py can use this to run in a 'demonstration mode' to test all the
GNI server code without any GNI hardware.

  from gni_emulator import GNIVirtualPorts
  emu = GNIVirtualPorts()
  userport = emu.start()
  device = gni.GNISerial(userport)

"""


import select
import serial
import time
from optparse import OptionParser
import tempfile
import subprocess as sp
import shutil
import os
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


_menu = """
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


class GNIEmulator(object):

  def __init__(self, device):
    """Open the given serial device."""
    self.device = device
    logger.debug("Opening device: %s" % (self.device))
    self.sport = serial.Serial(self.device, 9600, timeout=0)
    self.sport.nonblocking()
    self.status = "GNI,,c,9,c3,1,1"
    self.interrupted = False
    self.cdata = ""

  def writeLines(self, text):
    """Write the given text to the serial port, pausing between lines."""
    lines = text.splitlines()
    for line in lines:
      self.sport.write(line.rstrip() + "\r\n")
      time.sleep(0.01)

  def sendMenu(self):
    "Write the menu to the serial port."
    self.writeLines(_menu)

  def sendStatus(self):
    self.writeLines(self.status)

  def readData(self):
    "Read data from the serial port and echo it back."
    rdata = self.sport.read(128)
    self.sport.write(rdata)
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

  def handleLine(self, text):
    "Respond to commands and ignore the rest."
    text = text.strip()
    if text == "0":
      # The GNI actually repeats itself, so emulate that here.
      self.sendStatus()
      self.sendMenu()
      self.sendStatus()
      self.sendMenu()
    else:
      logger.debug("unrecognized command: %s" % (text))

  def loop(self):
    "Read commands from the port and respond."
    try:
      self.selectLoop()
    except KeyboardInterrupt:
      self.interrupted = True

  def selectLoop(self):
    while not self.interrupted:
      (readers, writers_, xers_) = select.select([self.sport], [], [])
      if self.sport in readers:
        self.readData()
            

class GNIVirtualPorts(object):
  """
  Setup virtual serial devices, with a GNI emulator connected to one end.

  This creates two subprocesses: the socat process which manages the pty
  devices for us, and the actual emulator process connected to one end of
  the socat "serial relay".

  The GNI emulator opens the "GNI port", or gniport, while the 
  GNI server opens the "user port", or userport.
  """
  def __init__(self):
    self.socat = None
    self.emulator = None
    self.gniport = None
    self.userport = None
    self.tmpdir = None
    self.debug = False

  def getUserPort(self):
    return self.userport

  def getGNIPort(self):
    return self.gniport

  def startPorts(self):
    "Start just the ports, leaving the emulator to be run separately."
    self.tmpdir = tempfile.mkdtemp()
    self.userport = os.path.join(self.tmpdir, "userport")
    self.gniport = os.path.join(self.tmpdir, "gniport")
    cmd = ["socat"]
    if self.debug:
      cmd.extend(["-v"] + ["-d"]*4)
    cmd.extend(["PTY,link=%s" % (self.gniport), "PTY,link=%s" % (self.userport)])
    logger.info(" ".join(cmd))
    self.socat = sp.Popen(cmd, close_fds=True, shell=False)
    started = time.time()
    found = False
    while time.time() - started < 5 and not found:
      time.sleep(1)
      found = os.path.exists(self.userport) and os.path.exists(self.gniport)
    if not found:
      raise Exception("serial port devices still do not exist after 5 seconds")
    return self.gniport

  def start(self):
    "Start the emulator processes and return the user port."
    self.startPorts()
    cmd = ["python", __file__, "--debug", self.gniport]
    logger.info(" ".join(cmd))
    self.emulator = sp.Popen(cmd, close_fds=True, shell=False)
    # Give the emulator process time to open the port and get ready to talk.
    time.sleep(2)
    return self.userport

  def stop(self):
    logger.info("Stopping...")
    if self.emulator:
      logger.debug("killing emulator...")
      self.emulator.kill()
      self.emulator.wait()
      self.emulator = None
    if self.socat:
      logger.debug("killing socat...")
      self.socat.kill()
      self.socat.wait()
      self.socat = None
    if self.tmpdir:
      logger.debug("removing %s" % (self.tmpdir))
      shutil.rmtree(self.tmpdir)
      self.tmpdir = None
      


_usage = """Usage: gni_emulator.py [--debug] [--info] <device>
Emulate the GNI serial interface on the given serial device.

If the device name is 'pty', then a virtual serial port connection will be
created.
"""

        
def main(args):
  parser = OptionParser(usage=_usage)
  parser.add_option("--debug", dest="level", action="store_const",
                    const=logging.DEBUG, default=logging.ERROR)
  parser.add_option("--info", dest="level", action="store_const",
                    const=logging.INFO, default=logging.ERROR)
  (options, args) = parser.parse_args(args)
  logging.basicConfig(level=options.level)
  if len(args) != 2:
    print("A device name must be specified.  Use -h to see usage info.")
    return 1
  gniport = args[1]
  if gniport == "pty":
    vports = GNIVirtualPorts()
    gniport = vports.startPorts()
    print("pty device specified, using virtual serial port: %s" % (gniport))
  gni = GNIEmulator(gniport)
  gni.loop()
  vports.stop()
  return 0


if __name__ == "__main__":
  import sys
  sys.exit(main(sys.argv))
  
