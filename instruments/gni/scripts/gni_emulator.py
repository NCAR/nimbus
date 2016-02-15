#! /usr/bin/python
# -*- mode: python; python-indent-offset: 4; -*-
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
emulator.  The path to the GNI port is printed as the "user port":

  > gni_emulator.py pty
  Emulator connecting to virtual serial port: /tmp/tmpJD610z/gniport
  User clients connect to virtual serial port: /tmp/tmpJD610z/userport

  Connect to the emulator with the server or a terminal program:

  > gni_server.py --device=/tmp/tmpJD610z/userport

  > python -m serial.tools.miniterm /tmp/tmpJD610z/userport

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
        self.hang = None
        logger.debug("Opening device: %s" % (self.device))
        self.sport = serial.Serial(self.device, 9600, timeout=0)
        self.sport.nonblocking()
        self.status = "GNI,,c,9,c3,1,1"
        self.started = None
        self.interrupted = False
        self.cdata = ""
        self.writeLines("GNI,,Controller ready")

    def setHangup(self, hang):
        self.hang = hang

    def isStopped(self):
        return self.hang is not None and self.started + self.hang <= time.time()

    def writeLines(self, text):
        """Write the given text to the serial port, pausing between lines."""
        # When hung, do not write any data back.
        if self.isStopped():
            return
        lines = text.splitlines()
        for line in lines:
            buf = line.rstrip() + "\r\n"
            logger.debug("writing to serial port:'%s'" % (repr(buf)))
            self.sport.write(buf)
            self.sport.flush()
            time.sleep(0.01)

    def sendMenu(self):
        "Write the menu to the serial port."
        self.writeLines(_menu)

    def sendStatus(self):
        self.writeLines(self.status)

    def readData(self):
        "Read data from the serial port and echo it back."
        rdata = self.sport.read(128)
        logger.debug("readData() got: '%s'" % (repr(rdata)))
        if not self.isStopped():
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
        logger.debug("handling line: '%s'" % (text))
        text = text.strip()
        if text == "0":
            # The GNI actually repeats itself, so emulate that here.
            self.sendStatus()
            self.sendMenu()
            self.sendStatus()
            self.sendMenu()
        elif text == "STOP":
            # Special command recognized by the emulator to enter "hung" state.
            self.hang = 0
        elif text == "RESUME":
            self.hang = None
        else:
            logger.debug("unrecognized command: '%s'" % (text))

    def loop(self):
        "Read commands from the port and respond."
        try:
            self.selectLoop()
        except KeyboardInterrupt:
            self.interrupted = True

    def selectLoop(self):
        logger.debug("entering selectLoop()...")
        self.started = time.time()
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
        self.server = None
        self.gniport = None
        self.userport = None
        self.tmpdir = None
        self.loglevel = logging.ERROR

    def setLogLevel(self, level):
        self.loglevel = level

    def logOption(self, level=None):
        if level is None:
            level = self.loglevel
        if level == logging.INFO:
            return "--info"
        if level == logging.ERROR:
            return "--error"
        if level == logging.DEBUG:
            return "--debug"

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
        if self.loglevel == logging.DEBUG:
            cmd.extend(["-v"])
            # cmd.extend(["-d"]*4)
        cmd.extend(["PTY,echo=0,link=%s" % (self.gniport),
                    "PTY,echo=0,link=%s" % (self.userport)])
        logger.info(" ".join(cmd))
        self.socat = sp.Popen(cmd, close_fds=True, shell=False)
        started = time.time()
        found = False
        while time.time() - started < 5 and not found:
            time.sleep(1)
            found = bool(os.path.exists(self.userport) and
                         os.path.exists(self.gniport))
        if not found:
            raise Exception("serial port devices still do not exist "
                            "after 5 seconds")
        return self.gniport

    def start(self):
        "Start the emulator processes and return the user port."
        self.startPorts()
        return self.startEmulator()

    def startEmulator(self):
        cmd = ["python", __file__]
        cmd.append(self.logOption())
        cmd.append(self.gniport)
        logger.info(" ".join(cmd))
        self.emulator = sp.Popen(cmd, close_fds=True, shell=False)
        # Give the emulator process time to open the port and get ready to talk.
        time.sleep(2)
        logger.debug("Emulator started on %s, returning user port: %s" %
                     (self.gniport, self.userport))
        return self.userport

    def startServer(self):
        path = os.path.join(os.path.dirname(__file__), "gni_server.py")
        cmd = ["python", path]
        cmd.append(self.logOption())
        cmd.extend(["--device", self.userport, "--client", "127.0.0.1"])
        logger.info(" ".join(cmd))
        self.server = sp.Popen(cmd, close_fds=True, shell=False)
        # Give the server process time to open the port and get ready to talk.
        time.sleep(2)
        logger.debug("Server started on %s..." % (self.userport))

    def emulatorMain(self, devicemode, options):
        """
        Main method for running just the emulator, possibly over virtual ports.
        """
        gniport = devicemode
        if devicemode in ["pty", "server"]:
            self.setLogLevel(options.level)
            gniport = self.startPorts()
            print("Emulator connecting to virtual serial port: %s" % (gniport))
            print("User clients connect to virtual serial port: %s" %
                  (self.getUserPort()))
        if devicemode == "server":
            self.startServer()
            print("Server started on port: %s" % (self.getUserPort()))
        gni = GNIEmulator(gniport)
        gni.setHangup(options.hang)
        gni.loop()
        self.stop()

    def startChain(self, options):
        """
        Start the whole chain: an emulator connected to a server over virtual
        serial ports.
        """
        self.setLogLevel(options.level)
        self.startPorts()
        self.startServer()
        logger.info("Server started on port: %s" % (self.getUserPort()))
        self.startEmulator()
        logger.info("Emulator started.")

    def stop(self):
        logger.info("Stopping...")
        if self.emulator:
            logger.debug("killing emulator...")
            self.emulator.kill()
            self.emulator.wait()
            self.emulator = None
        if self.server:
            logger.debug("killing server...")
            self.server.kill()
            self.server.wait()
            self.server = None
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

If the device name is 'pty', then this creates a virtual serial port
connection using 'socat' and connects the emulator to the gni port.  If the
device name is 'server', then this also starts a server process connected
to the 'user' end of the virtual serial connection."""

        
def main(args):
    parser = OptionParser(usage=_usage)
    parser.add_option("--debug", dest="level", action="store_const",
                      const=logging.DEBUG, default=logging.ERROR)
    parser.add_option("--info", dest="level", action="store_const",
                      const=logging.INFO, default=logging.ERROR)
    parser.add_option("--error", dest="level", action="store_const",
                      const=logging.ERROR, default=logging.ERROR)
    parser.add_option("--hang", type="int", default=None,
                      help=
                      "Stop responding (hang) after the "
                      "given number of seconds")
    (options, args) = parser.parse_args(args)
    logging.basicConfig(level=options.level)
    if len(args) != 2:
        print("A device name must be specified.  Use -h to see usage info.")
        return 1
    vports = GNIVirtualPorts()
    vports.emulatorMain(args[1], options)
    return 0


if __name__ == "__main__":
    import sys
    sys.exit(main(sys.argv))
  
