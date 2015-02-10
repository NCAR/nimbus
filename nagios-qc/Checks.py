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
            emin = ET.SubElement(elm, 'min_limit')
            emin.text = str(self.min_limit)
        if self.max_limit:
            emax = ET.SubElement(elm, 'max_limit')
            emax.text = str(self.max_limit)
        if self.missing_value:
            emv = ET.SubElement(elm, 'missing_value')
            emv.text = str(self.missing_value)
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
        self.vname = elm.attrib.get('variable', None)
        self.tolerance = float(elm.attrib.get('tolerance', Check.defaultTolerance))
        self.lookback = int(elm.attrib.get('lookback', Check.defaultLookback))
        for emv in elm.iter('missing_value'):
            self.missing_value = float(emv.text)
        for emax in elm.iter('max_limit'):
            self.max_limit = float(emax.text)
        for emin in elm.iter('min_limit'):
            self.min_limit = float(emin.text)

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
        status = nagios.Status(self.name(), catname=None, vname=self.vname)
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
            raise Exception("No limits for bounds check of '%s'." % (self.vname))

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

    def __init__(self):
        Check.__init__(self, "stable")
        self.tolerance = None
        self.lookback = None

    def instantiate(self, checkp, var):
        Check.instantiate(self, checkp, var)

    def check(self, datastore):
        # Grab last 'lookback' values of our variable and analyze.
        values = datastore.getValues(self.vname, self.lookback)
        if numpy.std(values) > self.tolerance:
            return self.Status().critical("Values out of tolerance: %f" %
                                          (values[-1]))
        return self.Status().ok("Values within tolerance: %f" % (values[-1]))


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


CheckPattern.addCheckType(Flatline)
CheckPattern.addCheckType(Bounds)
CheckPattern.addCheckType(Stable)
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

