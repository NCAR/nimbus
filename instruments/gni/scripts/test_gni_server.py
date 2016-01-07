#
# Run these tests like so:
#
# env PYTHONPATH=/home/local/raf/python py.test test_gni_server.py
#

import gni

def test_status_parser():
  import iss.time_tools as tt
  when = tt.parseTime("20160102T102030")
  gstat = gni.GNIStatus()
  gstat.parseStatusLine("GNI,,c,9,c3,1,1", when=when)
  assert(gstat.message == "GNI,20160102T102030.0,c,9,c3,1,1")
  assert(gstat.timestamp == when)

  
def test_timer_event():
  import iss.time_tools as tt
  when = tt.parseTime("20160102T102030")
  tv = gni.TimerEvent(10, when=when)
  assert(tv.started == when)
  assert(tv.period == 10)
  assert(tv.expires == tv.started + tv.period)
  assert(tv.repeating == False)

  
def test_repeating_timer_event():
  import iss.time_tools as tt
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
  import iss.time_tools as tt
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
  import iss.time_tools as tt
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

  
