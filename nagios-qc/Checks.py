"Interface to the Checks XML file and check entries."

import os
import re
import xml.etree.ElementTree as ET

import iss.nagios as nagios


class CheckPattern(object):
    """
    A CheckPattern names a check type and the basic parameters needed
    to realize one or more Check instances.
    """

    _check_factory = {}

    def addCheckType(maker):
        "Register a factory function for a check type."
        # Create a prototype just to get the name, we don't need to keep it.
        check = maker()
        CheckPattern._check_factory[check.ctype] = maker
        
    addCheckType = staticmethod(addCheckType)

    def createCheckType(ctype):
        maker = CheckPattern._check_factory.get(ctype, None)
        if not maker:
            raise Exception("Unknown check type '%s'." % (ctype))
        return maker()

    createCheckType = staticmethod(createCheckType)


    # All the CheckPattern requires is the check type and the variable
    # name pattern.  From that it can instantiate the right Check instance
    # for a given variable and let that instance pull the rest of its 
    # parameters from the XML element.

    def __init__(self):
        self.ctype = None
        self.vname = None
        self.elm = None

    def name(self):
        return self.ctype

    def fromXML(self, elm):
        self.elm = elm
        self.vname = elm.attrib.get('variable', None)
        self.ctype = elm.attrib.get('type', None)
        
    def generate(self, variables):
        "Apply this CheckPattern to a dictionary of variables to generate Checks."
        checks = []
        rx = re.compile(self.vname)
        for vname, var in variables.items():
            if rx.match(vname):
                check = self.createCheck(var)
                if check:
                    checks.append(check)
        return checks

    def createCheck(self, var):
        "Create a prototype for this check type and instantiate it."
        check = CheckPattern.createCheckType(self.ctype)
        check.instantiate(self, var)
        return check


def _getatt(elm, name, dfault, cvt=str):
    if elm.attrib.has_key(name):
        return cvt(elm.attrib[name])
    return dfault


class Check(object):
    "An actual Check instance which corresponds with a single nagios check."

    defaultTolerance = 0.01
    defaultLookback = 25

    def __init__(self, ctype):
        self.ctype = ctype
        self.vname = None
        self.min_limit = None
        self.max_limit = None
        self.tolerance = None
        self.lookback = None
        self.missing_value = None
        self.value = None

    def toElement(self):
        elm = ET.Element('check')
        elm.set('type', self.ctype)
        elm.set('variable', self.vname)
        # If these are just the defaults, then I'm not sure they should be
        # persisted, since that will be lots of duplicated information.
        # However, if it is in the persisted xml, then it the setting will
        # be obvious and explicit.
        if self.tolerance:
            elm.set('tolerance', str(self.tolerance))
        if self.lookback:
            elm.set('lookback', str(self.lookback))
        if self.min_limit:
            elm.set('min_limit', str(self.min_limit))
        if self.max_limit:
            elm.set('max_limit', str(self.max_limit))
        if self.missing_value:
            elm.set('missing_value', str(self.missing_value))
        if self.value:
            elm.set('value', str(self.value))
        return elm

    def toString(self):
        elm = self.toElement()
        return ET.tostring(elm)

    def fromString(text):
        elm = ET.fromstring(text)
        check = CheckPattern.createCheckType(elm.attrib.get('type'))
        check.fromElement(elm)
        return check

    fromString = staticmethod(fromString)

    def fromElement(self, elm):
        self.vname = _getatt(elm, 'variable', None)
        self.tolerance = _getatt(elm, 'tolerance', Check.defaultTolerance, float)
        self.lookback = _getatt(elm, 'lookback', Check.defaultLookback, int)
        self.missing_value = _getatt(elm, 'missing_value', self.missing_value,
                                     float)
        self.min_limit = _getatt(elm, 'min_limit', self.min_limit, float)
        self.max_limit = _getatt(elm, 'max_limit', self.max_limit, float)
        self.value = _getatt(elm, 'value', self.value, float)

    def name(self):
        "Derive this check name from our Status prototype."
        return self.Status().getName()

    def instantiate(self, checkp, var):
        "Instantiate a Check from a CheckPattern prototype."
        # First fill from the variable metadata, but then override with
        # explicit settings set in the CheckPattern.
        self.missing_value = var.missing_value
        self.min_limit = var.min_limit
        self.max_limit = var.max_limit
        self.fromElement(checkp.elm)
        self.vname = var.name

    def Status(self):
        "Construct a nagios status result for this check."
        status = nagios.Status(self.ctype, catname=None, vname=self.vname)
        return status


class Flatline(Check):

    def __init__(self):
        Check.__init__(self, "flatline")

    def instantiate(self, checkp, var):
        Check.instantiate(self, checkp, var)

    def check(self, datastore):
        values = datastore.getValues(self.vname, self.lookback)
        if numpy.std(values) < self.tolerance:
            return self.Status().critical("flatlining near %f" % (values[-1]))
        return self.Status().ok("Not flatlined, value at %f." % (values[-1]))



class Bounds(Check):

    def __init__(self):
        Check.__init__(self, "bounds")

    def instantiate(self, checkp, var):
        Check.instantiate(self, checkp, var)
        if self.min_limit is None or self.max_limit is None:
            raise Exception("No limits for bounds check of '%s'." % (self.name()))

    def check(self, datastore):
        values = datastore.getValues(self.vname, 1)
        current = values[-1]
        if self.min_limit > current:
            return self.Status().critical('value %f '
                                          'is beneath minimum limit %f' %
                                          (current, self.min_limit))
        elif self.max_limit < current:
            return self.Status().critical('value %f '
                                          'is above maximum limit %f' %
                                          (current, self.max_limit))
        return self.Status().ok("value %f is within limits [%f,%f]" %
                                (current, self.min_limit, self.max_limit))


class Stable(Check):
    "The values should be close to a target value within some tolerance limit."

    def __init__(self):
        Check.__init__(self, "stable")
        self.tolerance = None
        self.lookback = None

    def instantiate(self, checkp, var):
        Check.instantiate(self, checkp, var)
        if not self.value:
            raise Exception("No target value for stable check '%s'." % 
                            (self.name()))

    def check(self, datastore):
        # Grab last 'lookback' values of our variable and analyze.
        values = datastore.getValues(self.vname, self.lookback)
        current = values[-1]
        if numpy.std(values) > self.tolerance:
            return self.Status().critical(
                "Standard deviation of %d values exceeds tolerance of %f.  "
                "Current value: %f." %
                (self.lookback, self.tolerance, values[-1]))
        if self.value - self.tolerance < current < self.value + self.tolerance:
            return self.Status().ok(
                "Current value %f within %f of %f" % 
                (current, self.tolerance, self.value))
        return self.Status().critical(
            "Current value %f is not within %f of %f." %
            (current, self.tolerance, self.value))


class NoData(Check):

    def __init__(self):
        Check.__init__(self, "nodata")

    def instantiate(self, checkp, var):
        Check.instantiate(self, checkp, var)

    def check(self, datastore):
        # Grab last 'lookback' values of our variable and analyze.
        values = datastore.getValues(self.vname, 1)
        if values[0] == self.missing_value:
            return nagios.Status(Status.CRITICAL,
                                 "%s is missing value." % (self.vname))
        return Status(Status.OK, "%s has data." % (self.vname))

class GGQUAL(Check):

    def __init__(self):
        Check.__init__(self, "ggqual")

    def instantiate(self, checkp, var):
        Check.instantiate(self, checkp, var)

    def check(self, datastore):
        values = datastore.getValues(self.vname, 1)
        if values[-1] == self.missing_value:
            return self.Status().critical("No data for GPS quality flag.")
        value = int(values[-1])
        if value == 5:
            return self.Status().ok("GPS quality flag is 5.")
        if 1 < value < 5:
            return self.Status().warning("GPS quality flag is %d" % (value))
        return self.Status().critical("Bad value for GPS quality flag: %d" %
                                      (value))


CheckPattern.addCheckType(Flatline)
CheckPattern.addCheckType(Bounds)
CheckPattern.addCheckType(Stable)
CheckPattern.addCheckType(GGQUAL)
CheckPattern.addCheckType(NoData)


class ChecksXML(object):
    "Read CheckPatterns from a checks.xml file which can then be instantiated."

    def __init__(self):
        self.path = None
        self.xtree = None
        self.checks = []

    def defaultPath(self):
        return os.path.expandvars('${PROJ_DIR}/${PROJECT}/${AIRCRAFT}/checks.xml')

    def parse(self):
        "Parse the checks file and load all the check entries."
        if not self.path:
            self.path = self.defaultPath()
        self.xtree = ET.parse(self.path)
        for elm in self.xtree.getiterator('check'):
            checkp = CheckPattern()
            checkp.fromXML(elm)
            self.checks.append(checkp)

    def getChecks(self):
        return self.checks

