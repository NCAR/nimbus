"Classes to build a queue of event timers."

import time
import logging

logger = logging.getLogger(__name__)

class TimerEvent(object):
    """
    Timed events trigger callbacks from the event loop at specified times
    or intervals.
    """
    def __init__(self, period, repeating=False, when=None, name=None):
        self.period = period
        self.repeating = repeating
        self.started = None
        self.elapsed = None
        self.expires = None
        self.start(when)
        self.handler = None
        self.name = name
        self.ncalls = 0

    def getName(self):
        name = self.name
        if not name:
            name = str(self.handler)
        return name

    def start(self, when=None):
        if when is None:
            when = time.time()
        self.started = when
        self.expires = self.started + self.period

    def restart(self, when=None):
        """
        Reinitialize this TimerEvent with a new start time and expiration time.
        """
        logger.debug("restarting timer @ %s: %s" %
                     (str(time.time()), self.getName()))
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
        expired = self.expires
        logger.debug("handling timer@%s, expired@%s: %s" %
                     (str(time.time()), str(self.expires), self.getName()))
        self.ncalls += 1
        if self.handler is not None:
            self.handler(self)
        # Reschedule this timer unless it was restarted in the handler.
        if self.expires == expired:
            self.reset()



class TimerQueue(list):
    """
    Keep a queue of TimerEvent instances and compute how much time until
    the next event.
    """
    def __init__(self):
        list.__init__(self)

    def _normalize(self):
        for xt in [xt for xt in self if xt.expires is None]:
            del self[self.index(xt)]
        self.sort(key=lambda te: te.expires)

    def nextEvent(self):
        "Return seconds until next event or else max."
        self._normalize()
        if self:
            return self[0]
        return None

    def timeToNextEvent(self, delay, when=None):
        tv = self.nextEvent()
        if tv:
            delay = min(delay, tv.remaining(when))
        return delay

    def expired(self, when=None):
        "Return the timer events which have expired as of when."
        self._normalize()
        if not when:
            when = time.time()
        return [t for t in self if t.expires <= when]

    def handleExpired(self, when=None):
        expired = self.expired(when)
        logger.debug("handleExpired(): %d timers expired: %s" %
                     (len(expired), ",".join([te.getName() for te in expired])))
        for xt in expired:
            xt.handle()
        # This is kind of redundant, since the queue will be normalized
        # again on the next method call.  However, since this inherits
        # list, it makes sense that it be normalized after being modified
        # too, rather than letting expired timers remain in the queue.
        self._normalize()
