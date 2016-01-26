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
  assert(gstat.message == "GNI,20160102T102030.0,c,9,c3,1,1")
  assert(gstat.timestamp == when)

  
def test_ready_status():
  when = tt.parseTime("20160102T102030")
  gstat = gni.GNIStatus()
  gstat.parseStatusLine("GNI,,Controller ready", when=when)
  assert(gstat.message == "GNI,20160102T102030.0,Controller ready")
  assert(gstat.timestamp == when)

  
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
  tv.reset()
  assert(tv.expires == tv.started + 2*tv.period)


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
  assert(device.getStatus(when) == "GNI,20160102T102030.0,")
  assert(device.gotmenu == False)
  status = device.getStatus()
  device.requestStatus()
  # Rather than get too fancy, just brute force a read loop long enough
  # that the emulator should have responded.
  i = 0
  while i < 10 and device.getStatus() == status and not device.gotmenu:
    time.sleep(0.5)
    device.readData()
    i += 1
  assert(device.getStatus() != status)
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
  assert(device.getStatus(when) == "GNI,20160102T102030.0,")

  # Now start the emulator and the ready status should appear.
  emu.startEmulator()

  # Rather than get too fancy, just brute force a read loop long enough
  # that the emulator should have responded.
  i = 0
  status = device.getStatus()
  while i < 10 and status == device.getStatus():
    time.sleep(0.5)
    device.readData()
    i += 1
  assert(device.getStatus() != status)
  assert(re.match("^GNI,\d{8}T\d{6}\.\d,Controller ready", device.getStatus()))
  
