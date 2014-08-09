from datetime import datetime as _datetime
from calendar import timegm as _timegm
from time import strptime as _strptime

import logging

logger = logging.getLogger(__name__)

def timeFromPtime(text, pattern="%Y%m%d%H%M%S"):
    return _timegm(_strptime(text, pattern))


class Duration(object):

    # duration_seconds is positive if the period is always exactly that
    # long, allowing the start() and next() methods to use optimized
    # algorithms.  If it is negative, then the duration is not exact, and
    # the duration magnitude is the maximum length of the period.  advance
    # is a function to increment the correct field of a struct_time
    # instance.
    def __init__(self, duration_seconds, sftime):
        self._period = duration_seconds
        self._sftime = sftime

    def period(self):
        return self._period

    def ftime_start(self):
        return self._sftime

Year = Duration(-365*24*3600, "%Y")
Month = Duration(-32*24*3600, "%Y%m")
Day = Duration(24*3600, '%Y%m%d')
Hour = Duration(3600, '%Y%m%d%H')
Minute = Duration(60, '%Y%m%d%H%M')
Second = Duration(1, '%Y%m%d%H%M%S')

class DatePatternPeriod(object):

    def __init__(self, duration, wildcard, ftime):
        self._duration = duration
        self._wildcard = wildcard
        self._ftime = ftime
        self._sftime = duration.ftime_start()

    def formatTime(self, ttime, pattern=None):
        if not pattern:
            pattern = self._ftime
        return _datetime.utcfromtimestamp(ttime).strftime(pattern)

    def startPeriod(self, ttime):
        "Return the start time for the period containing the given time."
        # Use the ftime for the period start to make the comparison.
        period = self._duration.period()
        if period > 0:
            tstart = ttime - (ttime % period)
        else:
            sstart = self.formatTime(ttime, self._sftime)
            tstart = _timegm(_strptime(sstart, self._sftime))
        return tstart

    def nextPeriod(self, ttime):
        "Return the next start of this time period."
        # This is not the most efficient approach, but at least it's robust.
        period = self._duration.period()
        if period > 0:
            tend = self.startPeriod(ttime)
            tend += period
        else:
            stime = self.formatTime(ttime)
            tend = ttime
            for i_ in xrange(3):
                tend -= period / 2
                if self.formatTime(tend) != stime:
                    break
            tend = self.startPeriod(tend)
        return tend

    def wildcard(self, pattern):
        "Replace date pattern with wildcards for this time period."
        return pattern.replace(self._ftime, self._wildcard)


class GlobDatePeriods(object):
    """
    A set of DatePatternPeriod instances from year through seconds.

    This includes methods to replace periods with their wildcards.  It can
    be subclassed to use different wildcard schemes such as regular
    expressions and different strftime fields.
    """
    def __init__(self):
        self.year = DatePatternPeriod(Year, '[12][90][0-9][0-9]', '%Y')
        self.month = DatePatternPeriod(Month, '[01][0-9]', '%m')
        self.day = DatePatternPeriod(Day, '[0-3][0-9]', '%d')
        self.hour = DatePatternPeriod(Hour, '[0-2][0-9]', '%H')
        self.minute = DatePatternPeriod(Minute, '[0-5][0-9]', '%M')
        self.second = DatePatternPeriod(Second, '[0-5][0-9]', '%S')

    def getPeriods(self):
        "Return a list of the periods in order from year to second."
        return [self.year, self.month, self.day, 
                self.hour, self.minute, self.second]

    def wildcard(self, pattern, period):
        """
        Replace all lesser periods with wildcards, keeping the given period
        fixed.  So wildcard("%H%M%S", self.hour) replaces %M and %S.
        """
        periods = self.getPeriods()
        found = False
        for p in periods:
            if found:
                pattern = p.wildcard(pattern)
            if p == period:
                found = True
        return pattern


class DatePattern(object):
    """
    A general strftime date pattern which can be used to generate specific
    pattern instances to match a given time range.
    """

    def __init__(self, pattern=None, periods=None):
        self.pattern = pattern
        if not periods:
            periods = GlobDatePeriods()
        self.periods = periods

    def setPattern(self, pattern):
        self.pattern = pattern

    def formatTime(self, ttime, pattern=None):
        if not pattern:
            pattern = self.pattern
        return _datetime.utcfromtimestamp(ttime).strftime(pattern)

    def generate(self, begin, end):
        "Generate the date pattern over the given range, inclusive."
        patterns = []
        tt = int(begin)
        while tt <= int(end):
            # logger.debug("checking time %s" % 
            #              (self.formatTime(tt, "%Y-%m-%d,%H:%M:%S")))
            for p in self.periods.getPeriods():
                if p.startPeriod(tt) == tt and p.nextPeriod(tt) - 1 <= end:
                    # All the times in this period are included, so replace
                    # all the lesser periods with wildcards.
                    pattern = self.periods.wildcard(self.pattern, p)
                    pattern = self.formatTime(tt, pattern)
                    # If the pattern still comes out the same as the last
                    # one added (perhaps because the pattern does not even
                    # contain this time field), then ignore it.
                    if not patterns or patterns[-1] != pattern:
                        patterns.append(pattern)
                        logger.debug("found period %s, appending pattern %s" % 
                                     (self.formatTime(tt, "%Y-%m-%d,%H:%M:%S"),
                                      pattern))
                    tt = p.nextPeriod(tt)
                    break
                # If this time does not start on this period, then we have
                # to move to the next smaller period.
            # If all the periods were checked, then at least the seconds
            # should have matched, causing an exact time to be added.
        return patterns

    def generateRsyncRules(self, begin, end):
        patterns = self.generate(begin, end)
	options = []
        options.extend(['--include=%s**' % (p) for p in patterns])
        options.extend(['--exclude=*'])
        return options


def test_period_wildcard():
    dpp = GlobDatePeriods()
    assert dpp.day.wildcard("%Y%m%d") == "%Y%m[0-3][0-9]"
    
def test_glob_wildcard():
    dpp = GlobDatePeriods()
    assert bool(dpp.wildcard("%Y%m%d%H%M%S", dpp.hour) ==
                "%Y%m%d%H[0-5][0-9][0-5][0-9]")

def test_startPeriod():
    dpp = GlobDatePeriods()
    tt = timeFromPtime("20120102030405")
    assert dpp.year.startPeriod(tt) == timeFromPtime("20120101000000")
    assert dpp.month.startPeriod(tt) == timeFromPtime("20120101000000")
    assert dpp.day.startPeriod(tt) == timeFromPtime("20120102000000")
    assert dpp.hour.startPeriod(tt) == timeFromPtime("20120102030000")
    assert dpp.minute.startPeriod(tt) == timeFromPtime("20120102030400")
    assert dpp.second.startPeriod(tt) == timeFromPtime("20120102030405")

    tt = timeFromPtime("20120101000005")
    assert dpp.year.startPeriod(tt) != tt
    assert dpp.month.startPeriod(tt) != tt
    assert dpp.day.startPeriod(tt) != tt
    assert dpp.hour.startPeriod(tt) != tt
    assert dpp.minute.startPeriod(tt) != tt
    assert dpp.second.startPeriod(tt) == tt

    tt = timeFromPtime("20120101000000")
    assert dpp.year.startPeriod(tt) == tt
    assert dpp.month.startPeriod(tt) == tt
    assert dpp.day.startPeriod(tt) == tt
    assert dpp.hour.startPeriod(tt) == tt
    assert dpp.minute.startPeriod(tt) == tt
    assert dpp.second.startPeriod(tt) == tt

    tt = timeFromPtime("20120203000000")
    assert dpp.year.startPeriod(tt) != tt
    assert dpp.month.startPeriod(tt) != tt
    assert dpp.day.startPeriod(tt) == tt
    assert dpp.hour.startPeriod(tt) == tt
    assert dpp.minute.startPeriod(tt) == tt
    assert dpp.second.startPeriod(tt) == tt


def test_nextPeriod():
    dpp = GlobDatePeriods()
    begin = timeFromPtime("20120101000000")
    end = timeFromPtime("20130101000000")
    assert dpp.year.nextPeriod(begin) == end
    begin = timeFromPtime("20000101000000")
    end = timeFromPtime("20010101000000")
    assert dpp.year.nextPeriod(begin) == end


def test_generate_patterns():
    begin = timeFromPtime("200103152208", "%Y%m%d%H%M")
    end = timeFromPtime("200103160123", "%Y%m%d%H%M")
    dp = DatePattern("%Y-%m-%d*")
    assert dp.generate(begin, end) == ["2001-03-15*", "2001-03-16*"]
    dp.setPattern("%Y-%m-%d-%H*")
    assert dp.generate(begin, end) == [ 
        "2001-03-15-22*", "2001-03-15-23*",
        "2001-03-16-00*", "2001-03-16-01*"]

def test_generate_inclusive():
    begin = timeFromPtime("200103152208", "%Y%m%d%H%M")
    end = begin+2
    dp = DatePattern("%H%M%S")
    assert dp.generate(begin, end) == ["220800", "220801", "220802"]

def test_generate_compressed():
    begin = timeFromPtime("200103152258", "%Y%m%d%H%M")
    end = timeFromPtime("200103160103", "%Y%m%d%H%M")
    dp = DatePattern("%H%M%S")
    assert dp.generate(begin, end) == [ 
        "2258[0-5][0-9]",
        "2259[0-5][0-9]",
        "23[0-5][0-9][0-5][0-9]", 
        "00[0-5][0-9][0-5][0-9]",
        "0100[0-5][0-9]",
        "0101[0-5][0-9]",
        "0102[0-5][0-9]",
        "010300"]


def test_generate_rollover():
    begin = timeFromPtime("199912312355", "%Y%m%d%H%M")
    end = timeFromPtime("200001010005", "%Y%m%d%H%M")
    dp = DatePattern("%Y-%m-%d-%H:%M*")
    assert dp.generate(begin, end) == [ 
        "1999-12-31-23:55*",
        "1999-12-31-23:56*",
        "1999-12-31-23:57*",
        "1999-12-31-23:58*",
        "1999-12-31-23:59*",
        "2000-01-01-00:00*",
        "2000-01-01-00:01*",
        "2000-01-01-00:02*",
        "2000-01-01-00:03*",
        "2000-01-01-00:04*",
        "2000-01-01-00:05*"]


def test_generate_mixed():
    begin = timeFromPtime("199912312155", "%Y%m%d%H%M")
    end = timeFromPtime("200001010005", "%Y%m%d%H%M")
    dp = DatePattern("%Y-%m-%d-%H:%M*")
    assert dp.generate(begin, end) == [ 
        "1999-12-31-21:55*",
        "1999-12-31-21:56*",
        "1999-12-31-21:57*",
        "1999-12-31-21:58*",
        "1999-12-31-21:59*",
        "1999-12-31-22:[0-5][0-9]*",
        "1999-12-31-23:[0-5][0-9]*",
        "2000-01-01-00:00*",
        "2000-01-01-00:01*",
        "2000-01-01-00:02*",
        "2000-01-01-00:03*",
        "2000-01-01-00:04*",
        "2000-01-01-00:05*"]





