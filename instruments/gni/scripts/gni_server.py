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

import re
import select
import serial
import socket
import sys
import time
import logging

logger = logging.getLogger(__name__)

from optparse import OptionParser
import collections
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
        self._status_handler = None

    def getSerial(self):
        return self.sport

    def requestStatus(self):
        logger.debug("requesting status")
        self.writeCommand("0")
        self.gotmenu = False

    def retractSlide(self):
        self.writeCommand("6") # retract slide
        self.gotmenu = False

    def writeCommand(self, text):
        self.sport.write(text + "\r\n")
        self.sport.flush()

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

    def _emitStatus(self):
        "Call a handler function on status changes."
        if self._status_handler:
            logger.debug("calling status handler")
            self._status_handler(self)

    def handleLine(self, text):
        logger.debug("handling line: " + text.rstrip())
        if text.startswith("GNI,"):
            self.status.parseStatusLine(text)
            logger.info("GNI status line: %s" % (self.status.getMessage()))
            self._emitStatus()
        elif text.startswith("10 = Home Slide Actuator"):
            self.gotmenu = True
        elif re.search(r"^Set In Program Mode", text):
            # Ignore this line
            pass
        elif re.match(r"^\d\d? = ", text):
            # Menu entry
            pass
        elif re.match(r"^\d+\s*$", text):
            # Command echo
            pass
        elif not text:
            # Empty line
            pass
        else:
            # Everything not recognized as a status or menu line is just a
            # "data line".  The line is logged as an info message so it
            # shows up in the output with the default logging level, since
            # that's what users seem to like, but probably something
            # smarter can be done with it.  For example, consider relaying
            # these lines back through the server so they can be seen by
            # the client, as long as they are not too large.  Maybe there
            # should be two different modes, either high or low bandwitdh,
            # where everything can be sent back to the client in high
            # bandwidth mode, even the menu.
            logger.info("GNI data line: %s" % (text))

    def getStatus(self):
        return self.status

    def close(self):
        self.sport.close()


StatusRecord = collections.namedtuple('StatusRecord', ['timestamp', 'status'])


class GNIStatus(object):
    "Keep track of the current state of the GNI."

    def __init__(self):
        # Keep the status and timestamp in a tuple, and keep the most recent
        # two of them.  The GNI repeats the status line after requests, so this
        # provides a way to detect redundant status updates, meaning the status
        # string is the same and the timestamps are close.
        self.latest = None
        self.previous = None
        self.time_tolerance = 0.2
        self.srx = re.compile(r"^GNI,(?P<gnitime>[^,]*),(?P<status>.*)$")

    def formatTimestamp(self, when=None):
        if when is None:
            when = self.latest.timestamp
        if when is None:
            when = time.time()
        stime = time.strftime("%Y%m%dT%H%M%S", time.gmtime(when))
        stime = stime + '.%1d' % (round((when - int(when)) * 10))
        return stime

    def formatRecord(self, record):
        msg = "GNI,%s," % (self.formatTimestamp(record.timestamp))
        if record.status:
            msg = msg + record.status
        return msg

    def getMessage(self, when=None):
        "Return the latest status message, or else build one."
        if self.latest is not None:
            return self.formatRecord(self.latest)
        # Create a null status as of when or now.
        if not when:
            when = time.time()
        self.latest = StatusRecord(when, "")
        return self.formatRecord(self.latest)

    def getTimestamp(self, when=None):
        # If there is no status yet, make one, then return the timestamp.
        if self.latest is None:
            self.getMessage(when)
        return self.latest.timestamp

    def snapshot(self):
        """
        Copy the latest status so that changed() returns False until a
        new status is received.
        """
        self.previous = self.latest

    def changed(self):
        """
        Return true when the latest status differs from the previous either in
        the status string or the timestamp.
        """
        prev = self.previous
        last = self.latest
        return bool(not prev or not last or
                    abs(last.timestamp - prev.timestamp) > self.time_tolerance
                    or prev.status != last.status)

    def parseStatusLine(self, line, when=None):
        """
        Parse lines like GNI,,c,9,c3,1,1 and GNI,,Controller ready

        Insert a timestamp into the second field if it's empty.
        """
        if when is None:
            when = time.time()
        line = line.strip()

        rx = self.srx.match(line)
        if not rx:
            # If we do not get something we can least parse as
            # "GNI,[time],[status]", then there is no point passing it on
            # because it is probably not a valid status string and not
            # something any downstream status consumers will be able to
            # handle.
            logger.error("Unexpected status line: %s" % (line))
            return

        # If GNI were ever to give us a valid timestamp field, then this
        # would be where we parse it into a time.  But for now that doesn't
        # happen, so just grab the status string.
        gnitime = rx.group('gnitime')
        if gnitime:
            logger.debug("timestamp from gni: %s" % (gnitime))
        self.previous = self.latest
        self.latest = StatusRecord(when, rx.group('status'))


class GNIServer(object):

    DEFAULT_IP = "192.168.84.255"
    LOCALHOST_IP = "127.0.0.1"

    UDP_SEND_PORT = 32100
    UDP_READ_PORT = 32101
    UDP_NIDAS_PORT = 32102

    def __init__(self):
        self.udp_ip = GNIServer.DEFAULT_IP
        # Corresponds to udp_read_port in GNIClient
        self.udp_send_port = GNIServer.UDP_SEND_PORT
        # Corresponds to udp_send_port in GNIClient
        self.udp_read_port = GNIServer.UDP_READ_PORT
        self.udp_nidas_port = GNIServer.UDP_NIDAS_PORT
        self.timerq = gni.TimerQueue()
        self.timed_exposure = 0.0
        self.expose_time = 0.0
        # Repeat the last status if no new status has been received in the last
        # 5 seconds.
        self.repeater = gni.TimerEvent(5, repeating=True, name="repeater")
        self.repeater.setHandler(lambda tv: self.sendStatus(True))
        self.timerq.append(self.repeater)
        # Every 2 seconds request a new status.  When the GNI responds, the
        # updated status will be immediately sent out from inside the event
        # loop, and the repeater status timer will be restarted.
        tv = gni.TimerEvent(2, repeating=True, name="requester")
        tv.setHandler(lambda tv: self.requestStatus())
        self.timerq.append(tv)
        # Connection to the GNI serial interface
        self.gni = None
        self.sock = None
        self.interrupted = False
        self.nsent = 0

    def setClientAddress(self, client):
        if client:
            self.udp_ip = client

    def connect(self):
        # Connection to User Interface
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
        self.sock.bind(("0.0.0.0", self.udp_read_port))

    def sendClient(self, message):
        self.nsent += 1
        if self.sock:
            logger.debug("sending: %s" % (message))
            self.sock.sendto(message, (self.udp_ip, self.udp_send_port))
            self.sock.sendto(message, (self.udp_ip, self.udp_nidas_port))
        else:
            logger.error("socket not connected yet, not sending: %s" %
                         (message))

    def setSerialGNI(self, gserial):
        self.gni = gserial
        # Whenever the serial port receives a new status, we want to send it
        # immediately.
        self.gni.setStatusHandler(lambda sp: self.sendStatus())

    def requestStatus(self):
        if self.gni:
            self.gni.requestStatus()

    def sendStatus(self, force=False):
        # Send the status and restart the timer which repeats the current
        # status in case no new status is received.  This call may have been
        # triggered by that timer, in which case the timer is effectively
        # rescheduling itself.
        self.repeater.restart()
        # Since the GNI can send duplicate status lines in rapid succession,
        # make sure this one is different before sending it.
        status = self.gni.getStatus()
        if force or status.changed():
            self.sendClient(status.getMessage())

    def loop(self):
        while not self.interrupted:
            try:
                self.handleEvents()
            except KeyboardInterrupt:
                self.interrupted = True

    def handleEvents(self, timeout=None):
        ser = self.gni.getSerial()
        ports = [ser]
        if self.sock:
            ports.append(self.sock)
        start = time.time()
        if timeout is None:
            timeout = 99
        remaining = self.timerq.timeToNextEvent(timeout)
        # We do not need to check for any "exceptional conditions" on the
        # fds.  According to the select_tut(2) man page, we are not
        # interested in any of them.
        readable, wfds_, xfds_ = select.select(ports, [], [], remaining)
        elapsed = time.time() - start
        logger.debug("elapse = " + str(elapsed))

        self.timerq.handleExpired()

        if self.timed_exposure:
            self.expose_time -= elapsed
            if self.expose_time <= 0:
                self.gni.retractSlide()
                self.timed_exposure = 0

        if self.sock and self.sock in readable:
            data = self.sock.recv(1024)
            print 'Read ' + data
            fields = data.split(',')
            ser.write(fields[2])
            self.sock.sendto(data, (self.udp_ip, self.udp_nidas_port))
        if ser in readable:
            self.gni.readData()

    def close(self):
        if self.sock:
            self.sock.close()


_usage = "%prog " + """[log-level-option] [--device <device>] [--client <host>]

Run the server for a GNI connected to the given serial device.

The server sends status updates to <client>:%s, where by default the
<client> host or broadcast address is %s.  The server listens for commands
from clients on port %s.  Set the client address with the --client option.
All UDP status updates are also copied to a second port, %s, for recording
by NIDAS, assuming NIDAS is listening on the <client> host or broadcast
address.

The default INFO logging level prints all status and data lines received
from the GNI, skipping all the menu lines.  Specifying the ERROR logging
level will suppress all output except for errors.  The DEBUG logging level
is much more verbose, showing most messages back and forth as well as some
timing information.""" % (GNIServer.UDP_SEND_PORT,
                          GNIServer.DEFAULT_IP,
                          GNIServer.UDP_READ_PORT,
                          GNIServer.UDP_NIDAS_PORT)



def main(args):
    parser = OptionParser(usage=_usage)
    parser.add_option("--debug", dest="level", action="store_const",
                      const=logging.DEBUG, default=logging.INFO)
    parser.add_option("--info", dest="level", action="store_const",
                      const=logging.INFO, default=logging.INFO)
    parser.add_option("--error", dest="level", action="store_const",
                      const=logging.ERROR, default=logging.INFO)
    parser.add_option("--device", type="string", default='/dev/ttyUSB0')
    parser.add_option("--client", type="string", default=None,
                      help="The address of the host which will receive "
                      "UDP status packets from the GNI.  By default this "
                      "is the aircraft server at %s, where the RIC proxy "
                      "runs.  Set it to 'localhost' to connect to a client "
                      "running on the same host.")
    (options, args) = parser.parse_args(args)
    logging.basicConfig(level=options.level)
    server = GNIServer()
    if options.client:
        server.setClientAddress(client)
    server.connect()
    device = GNISerial(options.device)
    server.setSerialGNI(device)
    server.loop()
    server.close()
    device.close()


if __name__ == "__main__":
    main(sys.argv)


