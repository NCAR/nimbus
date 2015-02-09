"Interface to the Checks XML file and check entries."

import os
from lxml import etree


class CheckPattern(object):
    """
    A CheckPattern names a check type and the basic parameters needed
    to realize one or more Check instances.
    """

    # All the CheckPattern requires is the check type and the variable
    # name pattern.  From that it can instantiate the right Check instance
    # for a given variable and let that instance pull the rest of its 
    # parameters from the XML element.

    def __init__(self):
        self.ctype = None
        self.vname = None
        self.elm = None

    def fromXML(self, elm):
        self.elm = elm
        self.vname = elm.attrib.get('variable', None)
        self.ctype = elm.attrib.get('type', None)
        
    def instantiate(self, vars):
        "Instantiate this CheckPattern against the dictionary of variables."
        checks = []
        rx = re.compile(self.vname)
        for vname, var in vars.items():
            if rx.match(vname):
                check = createCheck(self.ctype, var, self.elm)
                if check:
                    checks.append(check)
        return checks


_check_factory = {}

def createCheck(ctype, var, elm):
    "Look up the prototype for this check type and create it."
    maker = _check_factory.get(ctype, None)
    if not maker:
        raise Exception("Unknown check type '%s'." % (ctype))
    check = maker()
    check.fromXML(var, elm)
    return check


class Check(object):
    "An actual Check instance which corresponds with a single nagios check."

    defaultTolerance = 0.01
    defaultCheckRecords = 25

    def __init__(self, ctype):
        "Instantiate a Check from a CheckPattern prototype."
        self.ctype = ctype
        self.vname = None
        self.min_limit = None
        self.max_limit = None
        self.tolerance = None
        self.checkRecords = None

    def fromXML(self, var, elm):
        self.vname = var.name
        min_limit = elm.attrib.get('min_limit', None)
        max_limit = elm.attrib.get('max_limit', None)
        if not min_limit:
            min_limit = var.min_limit
        if not max_limit:
            max_limit = var.max_limit
        if min_limit:
            self.min_limit = float(min_limit)
        if max_limit:
            self.max_limit = float(max_limit)
        self.tolerance = float(elm.attrib.get('tolerance', Check.defaultTolerance))
        self.checkRecords = float(elm.attrib.get('lookback', 
                                                 Check.defaultCheckRecords))


class Flatline(Check):

    def __init__(self):
        Check.__init__("flatline")

    def fromXML(self, var, elm):
        Check.fromXML(self, var, elm)


class Bounds(Check):

    def __init__(self):
        Check.__init__("bounds")

    def fromXML(self, var, elm):
        Check.fromXML(self, var, elm)
        if self.min_limit is None or self.max_limit is None:
            raise Exception("No limits for bounds check of '%s'." % (self.vname))


class Stable(Check):

    def __init__(self):
        Check.__init__("stable")
        self.tolerance = None
        self.lookback = None

    def fromXML(self, var, elm):
        Check.fromXML(self, var, elm)

    def check(self, datastore):
        # Grab last 'lookback' values of our variable and analyze.
        values = datastore.getValues(self.vname, self.lookback)
        if numpy.std(values) > self.tolerance:
            return flaps
        return ok+metSep+'constant'


_check_factory['flatline'] = Flatline
_check_factory['bounds'] = Bounds
_check_factory['stable'] = Stable


class Checks(object):
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
        self.xtree = etree.parse(self.path)
        for elm in self.xtree.getiterator('check'):
            checkp = CheckPattern()
            checkp.fromXML(elm)
            self.checks.append(checkp)

    def getChecks(self):
        return self.checks

