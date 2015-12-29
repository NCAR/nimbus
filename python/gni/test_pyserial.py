

import sys
import serial
import select

def main(args):
    print("opening %s..." % (args[0]))
    ser = serial.Serial(args[0], 9600, timeout=0)
    ser.nonblocking()
    while True:
        print("entering select()...")
        # fds = [ser.fileno()]
        fds = [ser]
        (rlist, wlist, xlist) = select.select(fds, [], fds, 5)
        print("returned from select().")
        if rlist:
            data = ser.read(100)
            print("data:", data)

if __name__ == "__main__":
    main(sys.argv[1:])

    
    
