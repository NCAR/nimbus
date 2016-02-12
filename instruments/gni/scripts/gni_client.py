#!/usr/bin/python
#
# Client program for Auto GNI.  Runs on user host collecting commands to relay
# to GNI server program, and receives back status and messages.
#

import select
import socket
import sys
import time
import logging
from optparse import OptionParser

logger = logging.getLogger(__name__)


class GNIClient(object):

    def __init__(self):
        self.udp_ip = "127.0.0.1"
        self.udp_read_port = 32100
        self.udp_send_port = 32101
        self.sock = None
        self.interrupted = False
        
    def setServerAddress(self, host):
        self.udp_ip = host

    def connect(self):
        # Connection to User Interface
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.sock.bind(("0.0.0.0", self.udp_read_port))
        logger.debug("listening on port %s" % (self.udp_read_port))

    def printMenu(self):
        print("Please issue a command:")
        print("0 = Status")
        print("1 = Home Drives")
        print("2 = Advance Carousel")
        print("3 = Advance Slide")
        print("4 = Load Slide")
        print("5 = Expose Slide")
        print("6 = Retract Slide")
        print("7 = Advance, Load, Expose & Retract")
        print("8 = Retract Slide Pickup")
        print("9 = Home Shuttle")
        print("10 = Home Slide Actuator")

    def formatTime(self, when):
        ts = time.strftime("%Y%m%dT%H%M%S", time.gmtime(when))
        return ts

    def readSocket(self):
        data = self.sock.recv(1024)
        print('STATUS:' + data)

    def readInput(self):
        data = sys.stdin.readline()
        ts = self.formatTime(time.time())
        msg = 'GNI,' + ts + ',' + data
        self.sock.sendto(msg, (self.udp_ip, self.udp_send_port))
        logger.debug("send to (%s:%s): %s" %
                     (self.udp_ip, self.udp_send_port, msg))
        # Only print the menu again after getting a command, rather than
        # every time through the event loop.  Otherwise the menu is printed
        # after every status message and in between any other data messages
        # from the GNI.
        self.printMenu()

    def select(self):
        ports = [sys.stdin, self.sock]
        readable, writable_, exceptional_ = select.select(ports, [], [])
        if self.sock in readable:
            self.readSocket()
        if sys.stdin in readable:
            self.readInput()

    def loop(self):
        try:
            while True:
                self.select()
        except KeyboardInterrupt:
            self.interrupted = True

    def close(self):
        if self.sock:
            self.sock.close()


_usage = """%prog [options]

Run the GNI client program to relay status and commands with the GNI
server, possibly through a RIC proxy."""


def main(args):
    parser = OptionParser(usage=_usage)
    parser.add_option("--debug", dest="level", action="store_const",
                      const=logging.DEBUG, default=logging.INFO)
    parser.add_option("--info", dest="level", action="store_const",
                      const=logging.INFO, default=logging.INFO)
    parser.add_option("--error", dest="level", action="store_const",
                      const=logging.ERROR, default=logging.INFO)
    parser.add_option("--server", type="string", default=None,
                      help="Host name or IP address where the GNI server "
                      """or RIC proxy is running.
Commands are sent to the <send port> on that host.  The default is
localhost.  To run on the aircraft server itself but connect to the server
on the Raspberry Pi, pass the Pi's IP address or try the host name 'gni'.
""")
    (options, args) = parser.parse_args(args)
    logging.basicConfig(level=options.level)
    client = GNIClient()
    if options.server:
        client.setServerAddress(options.server)
    client.connect()
    client.loop()
    client.close()


if __name__ == "__main__":
    main(sys.argv)

