
import sys
import serial
import select
import time

def main(args):
    print("opening %s..." % (args[0]))
    ser = serial.Serial(args[0], 9600, timeout=0)
    ser.nonblocking()
    now = time.time()
    tprompt = now + 5
    while True:
        #print("entering select()...")
        # fds = [ser.fileno()]
        fds = [ser]
        (rlist, wlist, xlist) = select.select(fds, [], fds, 5)
        #print("returned from select().")
	now = time.time()
	if now >= tprompt:
	    ser.write("0\r\n")
	    tprompt = now + 5
        if rlist:
            data = ser.read(100)
            sys.stdout.write(data)

if __name__ == "__main__":
    main(sys.argv[1:])

    
    

