# -*- mode:python; python-indent-offset: 2; -*-
# Run these tests like so:
#
# env PYTHONPATH=/home/local/raf/python py.test test_gni_server.py
#
# To skip the emulator tests, perhaps because they are too slow or socat is
# not available, run like this:
#
# env PYTHONPATH=.../raf/python py.test test_gni_server.py -k 'not emulator'
#

import gni
import time
import logging
import iss.time_tools as tt
from gni_emulator import GNIVirtualPorts
import re

logging.basicConfig(level=logging.DEBUG)

def test_status_parser():
  when = tt.parseTime("20160102T102030")
  gstat = gni.GNIStatus()
  gstat.parseStatusLine("GNI,,c,9,c3,1,1", when=when)
  assert(gstat.getMessage() == "GNI,20160102T102030.0,c,9,c3,1,1")
  assert(gstat.getTimestamp() == when)

  
def test_status_parser():
  "Same status close in time should not pass the changed()."
  when = tt.parseTime("20160102T102030")
  gstat = gni.GNIStatus()
  assert(gstat.changed() == True)
  gstat.parseStatusLine("GNI,,c,9,c3,1,1", when=when)
  assert(gstat.changed() == True)
  assert(gstat.getMessage() == "GNI,20160102T102030.0,c,9,c3,1,1")
  assert(gstat.getTimestamp() == when)
  gstat.parseStatusLine("GNI,,c,9,c3,1,1", when=when+0.15)
  assert(gstat.getMessage() == "GNI,20160102T102030.2,c,9,c3,1,1")
  assert(gstat.getTimestamp() == when+0.15)
  assert(gstat.changed() == False)
  gstat.parseStatusLine("GNI,,c,8,c3,2,2", when=when+0.3)
  assert(gstat.getMessage() == "GNI,20160102T102030.3,c,8,c3,2,2")
  assert(gstat.getTimestamp() == when+0.3)
  assert(gstat.changed() == True)

  
def test_ready_status():
  when = tt.parseTime("20160102T102030")
  gstat = gni.GNIStatus()
  gstat.parseStatusLine("GNI,,Controller ready", when=when)
  assert(gstat.getMessage() == "GNI,20160102T102030.0,Controller ready")
  assert(gstat.getTimestamp() == when)

  
def test_timer_event():
  when = tt.parseTime("20160102T102030")
  tv = gni.TimerEvent(10, when=when)
  assert(tv.started == when)
  assert(tv.period == 10)
  assert(tv.expires == tv.started + tv.period)
  assert(tv.repeating == False)

  
def test_repeating_timer_event():
  when = tt.parseTime("20160102T102030")
  tv = gni.TimerEvent(10, repeating=True, when=when)
  assert(tv.started == when)
  assert(tv.period == 10)
  assert(tv.expires == tv.started + tv.period)
  assert(tv.repeating == True)
  tv.reset(when+10)
  assert(tv.expires == tv.started + 2*tv.period)


def test_restart():
  when = tt.parseTime("20160102T102030")
  tv = gni.TimerEvent(10, repeating=True, when=when)
  assert(tv.started == when)
  assert(tv.period == 10)
  tv.restart(when+5)
  assert(tv.started == when+5)
  assert(tv.period == 10)
  assert(tv.expires == when+15)


def test_time_change():
  """
  Repeating events should advance past the current system clock.
  """
  when = tt.parseTime("20160102T102030")
  tv = gni.TimerEvent(10, repeating=True, when=when)
  now = float(int(time.time()/10)*10)
  tv.reset(now)
  assert(tv.expires == now + 10)
  tv.reset(now - 5)
  assert(tv.expires == now)
  tv.reset(now + 59.9)
  assert(tv.expires == now + 60)
  tv.reset(now + 60.1)
  assert(tv.expires == now + 70)
  # Check that system clock used if no reference time is passed.
  now = time.time()
  tv.reset()
  assert(now < tv.expires <= now + 10)
  assert(tv.expires == (now - (now % 10)) + 10)


def test_timer_queue():
  tq = gni.TimerQueue()
  assert(tq.timeToNextEvent(99) == 99)
  when = tt.parseTime("20160102T102030")
  tv = gni.TimerEvent(10, when=when)
  assert(tq.expired(when) == [])
  tq.append(tv)
  assert(tq.nextEvent() == tv)
  assert(tq.timeToNextEvent(99, when=when) == 10)
  assert(tq.timeToNextEvent(99, when=when+5) == 5)
  assert(tq.timeToNextEvent(99, when=when+15) == 0)
  

def test_expired():
  tq = gni.TimerQueue()
  when = tt.parseTime("20160102T102030")
  tv = gni.TimerEvent(10, when=when)
  assert(tq.expired(when) == [])
  tq.append(tv)
  assert(tq.expired(when-20) == [])
  assert(tq.expired(when+20) == [tv])
  assert(tq.timeToNextEvent(99, when+20) == 0)
  assert(tv.ncalls == 0)
  tq.handleExpired(when+20)
  assert(tv.ncalls == 1)
  assert(len(tq) == 0)

  
def test_emulator():
  "Start an emulator and connect a server to it, and see if it works!"
  emu = GNIVirtualPorts()
  userport = emu.start()
  device = gni.GNISerial(userport)
  # Verify null status at first.
  when = tt.parseTime("20160102T102030")
  assert(device.getStatus().getMessage(when) == "GNI,20160102T102030.0,")
  assert(device.gotmenu == False)
  status = device.getStatus()
  message = status.getMessage()
  status.snapshot()
  device.requestStatus()
  # Rather than get too fancy, just brute force a read loop long enough
  # that the emulator should have responded.
  i = 0
  while bool(i < 10 and not status.changed() and not device.gotmenu):
    time.sleep(0.5)
    device.readData()
    i += 1
  assert(status.getMessage() != message)
  assert(i < 10)
  assert(device.gotmenu == True)
  
  
def test_emulator_ready():
  "Test parsing of the initial ready status using the emulator."
  when = tt.parseTime("20160102T102030")
  emu = GNIVirtualPorts()
  gniport = emu.startPorts()
  # Connect to the user port before starting up the emulator on the other
  # port.
  userport = emu.getUserPort()
  device = gni.GNISerial(userport)
  assert(device.gotmenu == False)
  assert(device.getStatus().getMessage(when) == "GNI,20160102T102030.0,")

  # Now start the emulator and the ready status should appear.
  emu.startEmulator()

  # Rather than get too fancy, just brute force a read loop long enough
  # that the emulator should have responded.
  i = 0
  status = device.getStatus()
  status.snapshot()
  while i < 10 and not status.changed():
    time.sleep(0.5)
    device.readData()
    i += 1
  assert(status.changed())
  assert(re.match("^GNI,\d{8}T\d{6}\.\d,Controller ready", status.getMessage()))
  

def test_status_repeater():
  "The status repeater should kick in if the emulator stops responding."
  emu = GNIVirtualPorts()
  userport = emu.start()

  # Create a server on the port but do not connect the socket.
  device = gni.GNISerial(userport)
  server = gni.GNIServer()
  server.setSerialGNI(device)

  # Get a status and snapshot it.  We want to make sure the repeater sends
  # a status even if changed() is False.
  status = device.getStatus()
  status.getMessage()
  status.snapshot()

  # Tell the emulator to stop.
  device.writeCommand("STOP")

  assert(server.nsent == 0)
  assert(server.repeater.ncalls == 0)
  server.repeater.restart()

  # Handle events over 12 seconds, after which we should see 2 repeater
  # calls, at 5 and 10 seconds.
  start = time.time()
  while time.time() < start + 12:
    server.handleEvents(1)

  assert(server.repeater.ncalls == 2)
  assert(server.nsent == 2)
  server.close()
  device.close()
  
  
