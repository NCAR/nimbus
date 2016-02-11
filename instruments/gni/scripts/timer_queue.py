"Classes to build a queue of event timers."

import time
import logging

logger = logging.getLogger(__name__)

class TimerEvent(object):
  """
  Timed events trigger callbacks from the event loop at specified times
  or intervals.
  """
  def __init__(self, period, repeating=False, when=None):
    self.period = period
    self.repeating = repeating
    self.started = None
    self.elapsed = None
    self.expires = None
    self.start(when)
    self.handler = None
    self.ncalls = 0

  def start(self, when=None):
    if when is None:
      when = time.time()
    self.started = when
    self.expires = self.started + self.period

  def restart(self, when=None):
    """
    Reinitialize this TimerEvent with a new start time and expiration time.
    """
    self.start(when)

  def reset(self, when=None):
    """
    Advance repeating timers and nullify expired ones.

    Repeating timers are always advanced relative to the current system
    clock, in case the system time changes.
    """
    if self.repeating and self.period and self.expires:
      if not when:
        when = time.time()
      when = when - (when % self.period)
      self.expires = when + self.period
    else:
      self.expires = None

  def remaining(self, when=None):
    """
    Return time remaining from now (or when) until timer expires.  If the
    expiration time has passed, meaning the remaining time would be
    negative, then return zero.
    """
    if when is None:
      when = time.time()
    return max(0, self.expires - when)

  def setHandler(self, handler):
    self.handler = handler
    
  def handle(self):
    logger.debug("timer handle: %s" % (str(self.handler)))
    self.ncalls += 1
    if self.handler is not None:
      self.handler(self)
    self.reset()



class TimerQueue(list):
  """
  Keep a queue of TimerEvent instances and compute how much time until
  the next event.
  """
  def __init__(self):
    list.__init__(self)

  def nextEvent(self):
    "Return seconds until next event or else max."
    for xt in [xt for xt in self if xt.expires is None]:
      del self[self.index(xt)]
    self.sort(key=lambda te: te.expires)
    if self:
      return self[0]
    return None

  def timeToNextEvent(self, max, when=None):
    tv = self.nextEvent()
    if tv:
      return tv.remaining(when)
    return max

  def expired(self, when=None):
    "Return the timer events which have expired as of when."
    if not when:
      when = time.time()
    return [t for t in self if t.expires <= when]

  def handleExpired(self, when=None):
    expired = self.expired(when)
    for xt in expired:
      xt.handle()
    # Resort the events and filter the expired ones.
    nxt = self.nextEvent() 



