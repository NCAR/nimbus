"Interface to the Checks XML file and check entries."

import os
import re
import xml.etree.ElementTree as ET
import numpy as np

import iss.nagios as nagios

import logging

logger = logging.getLogger(__name__)


class CheckTemplate(object):
    """
    A CheckTemplate names a check type and the basic parameters needed
    to realize one or more Check instances.
    """

    _check_factory = {}

    def addCheckType(maker):
        "Register a factory function for a check type."
        # Create a prototype just to get the name, we don't need to keep it.
        check = maker()
        CheckTemplate._check_factory[check.ctype] = maker
        
    addCheckType = staticmethod(addCheckType)

    def createCheckType(ctype):
        maker = CheckTemplate._check_factory.get(ctype, None)
        if not maker:
            raise Exception("Unknown check type '%s'." % (ctype))
        return maker()

    createCheckType = staticmethod(createCheckType)


    # All the CheckTemplate requires is the check type and the variable
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
        """Apply this CheckTemplate to a dictionary of variables to generate
        Checks.

        """
        checks = []
        rx = re.compile(self.vname)
        for vname, var in variables.items():
            # If this is an exact match, then select it.  If not, then
            # only select matches for scalar variables.
            if vname == self.vname or (var.ndims == 1 and rx.match(vname)):
                check = self.createCheck(var)
                if check:
                    checks.append(check)
        if not checks:
            logger.warning("No matching variables for template "
                           "ctype='%s' variable='%s'" % 
                           (self.ctype, self.vname))
        return checks

    def createCheck(self, var):
        "Create a prototype for this check type and instantiate it."
        check = CheckTemplate.createCheckType(self.ctype)
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

    # Different checks use different subsets of check parameters, so each
    # Check can specify its particular parameters, and only those
    # parameteres will be persisted in configurations.

    MISSING_VALUE = "missing_value"
    TOLERANCE = "tolerance"
    MIN_LIMIT = "min_limit"
    MAX_LIMIT = "max_limit"
    LOOKBACK = "lookback"
    VALUE = "value"

    def __init__(self, ctype, parameters=None):
        self.ctype = ctype
        self.vname = None
        self.min_limit = None
        self.max_limit = None
        self.missing_value = None
        self.value = None
        self._tolerance = None
        self._lookback = None
        if not parameters:
            parameters = []
        self.parameters = parameters

        # These are the "public" attributes which check implementations
        # should use.  They have defaults so they always have values, but
        # they do not get persisted unless explicitly assigned in a
        # configuration.
        self.tolerance = Check.defaultTolerance
        self.lookback = Check.defaultLookback

    def toElement(self):
        elm = ET.Element('check')
        elm.set('type', self.ctype)
        elm.set('variable', self.vname)
        # We only need to check the parameter list when instantiating a
        # check config.  If a parameter is not in the list then it will
        # still be None and thus won't be persisted.
        if self._tolerance is not None:
            elm.set('tolerance', str(self._tolerance))
        if self._lookback is not None:
            elm.set('lookback', str(self._lookback))
        if self.min_limit is not None:
            elm.set('min_limit', str(self.min_limit))
        if self.max_limit is not None:
            elm.set('max_limit', str(self.max_limit))
        if self.missing_value is not None:
            elm.set('missing_value', str(self.missing_value))
        if self.value is not None:
            elm.set('value', str(self.value))
        return elm

    def toString(self):
        elm = self.toElement()
        return ET.tostring(elm)

    def fromString(text):
        elm = ET.fromstring(text)
        check = CheckTemplate.createCheckType(elm.attrib.get('type'))
        check.fromElement(elm)
        return check

    fromString = staticmethod(fromString)

    def fromElement(self, elm, var=None):
        self.vname = _getatt(elm, 'variable', None)
        if Check.TOLERANCE in self.parameters:
            self._tolerance = _getatt(elm, 'tolerance', self._tolerance, float)
            if self._tolerance is not None:
                self.tolerance = self._tolerance
        if Check.LOOKBACK in self.parameters:
            self._lookback = _getatt(elm, 'lookback', self._lookback, int)
            if self._lookback is not None:
                self.lookback = self._lookback
        if Check.MISSING_VALUE in self.parameters:
            if var:
                self.missing_value = var.missing_value
            self.missing_value = _getatt(elm, 'missing_value', 
                                         self.missing_value, float)
            if self.missing_value is None:
                raise Exception("Need missing_value set for check '%s'." % 
                                (self.name()))
        if Check.MIN_LIMIT in self.parameters:
            if var:
                self.min_limit = var.min_limit
            self.min_limit = _getatt(elm, 'min_limit', self.min_limit, float)
            if self.min_limit is None:
                raise Exception("No min_limit for bounds check '%s'." % 
                                (self.name()))
        if Check.MAX_LIMIT in self.parameters:
            if var:
                self.max_limit = var.max_limit
            self.max_limit = _getatt(elm, 'max_limit', self.max_limit, float)
            if self.max_limit is None:
                raise Exception("No max_limit for bounds check '%s'." % 
                                (self.name()))
        if Check.VALUE in self.parameters:
            self.value = _getatt(elm, 'value', self.value, float)
            if self.value is None:
                raise Exception("Check '%s' needs value parameter." %
                                (self.name()))

    def name(self):
        "Derive this check name from our Status prototype."
        return self.newStatus().getName()

    def instantiate(self, checkt, var):
        "Instantiate a Check from a CheckTemplate prototype."
        # First fill from the variable metadata, but then override with
        # explicit settings set in the CheckTemplate.
        self.fromElement(checkt.elm, var)
        self.vname = var.name
        logger.debug("instantiated check %s from "
                     "template(ctype=%s,variable=%s)" %
                     (self.name(), checkt.name(), checkt.vname))

    def newStatus(self):
        "Construct a nagios status result for this check."
        status = nagios.Status(self.ctype, catname=None, vname=self.vname)
        return status

    def check(self, _datastore):
        raise Exception("A Check instance is missing an implementation of the "
                        "check() method.")

    def stddev(self, values):
        """Return a standard deviation for the array of values.

        Accounts for missing values, and returns nan if there are no good
        values.

        """
        va = np.array(values)
        missing = va == self.missing_value
        if not values or missing.all():
            return float("nan")
        return np.std(va[~missing])


class Flatline(Check):

    def __init__(self):
        Check.__init__(self, "flatline", 
                       [Check.TOLERANCE, Check.LOOKBACK, Check.MISSING_VALUE])

    def instantiate(self, checkt, var):
        Check.instantiate(self, checkt, var)

    def check(self, datastore):
        values = datastore.getValues(self.vname, self.lookback)
        sdev = self.stddev(values)
        if sdev < self.tolerance:
            return self.newStatus().critical(
                "flatlining near %g, %i values with stddev=%g < tolerance=%g" %
                (values[0], len(values), sdev, self.tolerance))
        return self.newStatus().ok("Not flatlined, value at %g." % (values[0]))



class Bounds(Check):

    def __init__(self):
        Check.__init__(self, "bounds", [Check.MIN_LIMIT, Check.MAX_LIMIT])

    def instantiate(self, checkt, var):
        Check.instantiate(self, checkt, var)

    def check(self, datastore):
        values = datastore.getValues(self.vname, 1)
        current = values[0]
        if self.min_limit > current:
            return self.newStatus().critical('value %g '
                                             'is beneath minimum limit %f' %
                                             (current, self.min_limit))
        elif self.max_limit < current:
            return self.newStatus().critical('value %g '
                                             'is above maximum limit %f' %
                                             (current, self.max_limit))
        return self.newStatus().ok("value %g is within limits [%g,%g]" %
                                   (current, self.min_limit, self.max_limit))


class Stable(Check):
    "The values should be close to a target value within some tolerance limit."

    def __init__(self):
        Check.__init__(self, "stable",
                       [Check.MISSING_VALUE, Check.TOLERANCE, Check.LOOKBACK,
                        Check.VALUE])

    def instantiate(self, checkt, var):
        Check.instantiate(self, checkt, var)
        if self.value is None:
            raise Exception("No target value for stable check '%s'." % 
                            (self.name()))

    def check(self, datastore):
        # Grab last 'lookback' values of our variable and analyze.
        values = datastore.getValues(self.vname, self.lookback)
        current = values[0]
        sdev = self.stddev(values)
        if sdev > self.tolerance:
            return self.newStatus().critical(
                "Standard deviation %g of %d values exceeds tolerance of %g.  "
                "Current value: %g." %
                (sdev, len(values), self.tolerance, values[0]))
        if self.value - self.tolerance < current < self.value + self.tolerance:
            return self.newStatus().ok(
                "Current value %g within %g of %g" % 
                (current, self.tolerance, self.value))
        return self.newStatus().critical(
            "Current value %g is not within %g of %g." %
            (current, self.tolerance, self.value))


class NoData(Check):

    def __init__(self):
        Check.__init__(self, "nodata", [Check.MISSING_VALUE])

    def instantiate(self, checkt, var):
        Check.instantiate(self, checkt, var)

    def check(self, datastore):
        # Grab last 'lookback' values of our variable and analyze.
        values = datastore.getValues(self.vname, 1)
        if values[0] == self.missing_value:
            return self.newStatus().critical("%s is a missing value." % 
                                             (self.vname))
        return self.newStatus().ok("%s has value %g" % 
                                   (self.vname, values[0]))


class GGQUAL(Check):

    def __init__(self):
        Check.__init__(self, "ggqual")

    def instantiate(self, checkt, var):
        Check.instantiate(self, checkt, var)

    def check(self, datastore):
        values = datastore.getValues(self.vname, 1)
        if values[0] == self.missing_value:
            return self.newStatus().critical("No data for GPS quality flag.")
        value = int(values[0])
        if value == 5:
            return self.newStatus().ok("GPS quality flag is 5.")
        if 1 < value < 5:
            return self.newStatus().warning("GPS quality flag is %d" % (value))
        return self.newStatus().critical("Bad value for GPS quality flag: %d" %
                                         (value))


CheckTemplate.addCheckType(Flatline)
CheckTemplate.addCheckType(Bounds)
CheckTemplate.addCheckType(Stable)
CheckTemplate.addCheckType(GGQUAL)
CheckTemplate.addCheckType(NoData)


class ChecksXML(object):
    "Read CheckTemplates from a checks.xml file which can then be instantiated."

    def __init__(self):
        """Create empty ChecksXML with no file path and no templates."""
        self.path = None
        self.xtree = None
        self.templates = []

    def setupPath(self, path=None, vlist=None):
        """Set the path to a XML file with check templates.

        Use @p path if not None, else generate a path from @p vlist if not
        None, and otherwise generate a path using the RAF environment
        variables.

        """
        if path:
            self.path = path
        if self.path:
            pass
        elif vlist:
            self.path = os.path.join(vlist.configPath(), 'checks.xml')
        else:
            self.path = os.path.expandvars(
                '${PROJ_DIR}/${PROJECT}/${AIRCRAFT}/checks.xml')
        return self.path

    def load(self, xmlpath=None):
        """Parse the checks file and load all the check templates.

        The @p xmlpath parameter is a convenient shortcut for specifying
        the path to open.  The templates are appended to any existing
        templates, so it is possible to read multiple files into one
        combined list of templates.

        """
        self.xtree = ET.parse(self.setupPath(xmlpath))
        ntemplates = len(self.templates)
        for elm in self.xtree.getiterator('check'):
            checkt = CheckTemplate()
            checkt.fromXML(elm)
            self.templates.append(checkt)
        logger.info("loaded %d check templates from file %s" %
                    (len(self.templates) - ntemplates, self.path))

    def getCheckTemplates(self):
        "Return the templates loaded into this ChecksXML so far."
        return self.templates

    def generateChecks(self, vdict):
        "Instantiate all the templates against the variables in @p vdict."
        checks = []
        for checkt in self.getCheckTemplates():
            checks.extend(checkt.generate(vdict))
        logger.info("generated %d checks from %d templates" % 
                    (len(checks), len(self.getCheckTemplates())))
        return checks


