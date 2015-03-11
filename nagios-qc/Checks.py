"Interface to the Checks XML file and check entries."

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
        
    def matchesName(self, vname, ndims=1):
        """
        If this is an exact match, then select it.  If not, then only select
        scalar variables for which the regular expression pattern matches
        the entire name.  'LON' should not match 'LONC', but 'LON.*' does.
        """
        if vname == self.vname:
            return True
        if ndims != 1:
            return False
        # Force regex to match the whole name, not just the beginning.
        m = re.match(self.vname, vname)
        return bool(m and m.end() == len(vname))

    def generate(self, variables):
        """
        Apply this CheckTemplate to a dictionary of variables to generate
        Checks.
        """
        checks = []
        for vname, var in variables.items():
            if self.matchesName(vname, var.ndims):
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
        self._name = None
        self.min_limit = None
        self.max_limit = None
        self.missing_value = None
        self.value = None
        self._tolerance = None
        self._lookback = None
        if not parameters:
            parameters = []
        self.parameters = [Check.MISSING_VALUE] + parameters

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
        check = CheckTemplate.createCheckType(elm.attrib['type'])
        check.vname = elm.attrib['variable']
        check._fromElement(elm)
        return check

    fromString = staticmethod(fromString)


    def _getatt(self, elm, name, dfault, cvt=str, emessage=None):
        value = None
        if name in self.parameters:
            if elm.attrib.has_key(name):
                value = cvt(elm.attrib[name])
            else:
                value = dfault
            if emessage and value is None:
                raise Exception(emessage)
        return value

    def _fromElement(self, elm, var=None):
        """
        This does not restore the variable name from the element tree, since
        this method may be either instantiating a CheckTemplate (in which
        case the variable attribute may actually be a pattern) or restoring
        a Check from a saved string.  Therefore the variable is set first
        of all in the corresponding caller, either instantiate() or
        fromString().
        """
        # Get defaults from self unless a source variable is provided.
        if not var:
            var = self
        self._tolerance = self._getatt(elm, Check.TOLERANCE, 
                                       self._tolerance, float)
        if self._tolerance is not None:
            self.tolerance = self._tolerance
        self._lookback = self._getatt(elm, Check.LOOKBACK,
                                      self._lookback, int)
        if self._lookback is not None:
            self.lookback = self._lookback
        self.missing_value = self._getatt(
            elm, Check.MISSING_VALUE, 
            var.missing_value, float,
            emessage="Need missing_value set for check '%s'." % 
            (self.name()))
        self.min_limit = self._getatt(
            elm, Check.MIN_LIMIT, var.min_limit, float,
            emessage="No min_limit for bounds check '%s'." % (self.name()))
        self.max_limit = self._getatt(
            elm, Check.MAX_LIMIT, var.max_limit, float,
            emessage="No max_limit for bounds check '%s'." % (self.name()))
        self.value = self._getatt(
            elm, Check.VALUE, self.value, float,
            emessage="Check '%s' needs value parameter." % (self.name()))

    def name(self):
        "Derive this check name from our Status prototype."
        if not self._name:
            self._name = self.newStatus().getName()
        return self._name

    def instantiate(self, checkt, var):
        "Instantiate a Check from a CheckTemplate prototype."
        # First fill from the variable metadata, but then override with
        # explicit settings set in the CheckTemplate.
        self.vname = var.name
        self._fromElement(checkt.elm, var)
        logger.debug("instantiated check %s from "
                     "template(ctype=%s,variable=%s)" %
                     (self.name(), checkt.name(), checkt.vname))

    def is_superceded(self, _checks):
        """
        Checks can indicate they are superceded by another check in the list by
        returning True.  The base class implementation always returns
        False.
        """
        return False

    def newStatus(self):
        "Construct a nagios status result for this check."
        status = nagios.Status(self.ctype, catname=None, vname=self.vname)
        return status

    def check(self, datastore):
        """
        Public entry point to running this check instance against data.  It
        first checks whether any variables are missing values, then calls
        the subclass implementation.
        """
        status = self.check_missing(datastore)
        if not status.is_ok():
            return status
        return self.check_self(datastore)

    def check_self(self, _datastore):
        raise Exception("A Check instance is missing an implementation of the "
                        "check_self() method.")

    def check_missing(self, datastore):
        "Check for a missing value, useful before any other checks."
        values = datastore.getValues(self.vname, 1)
        if values[0] == self.missing_value:
            return self.newStatus().critical("%s is a missing value." % 
                                             (self.vname))
        return self.newStatus().ok("%s has value %g" % (self.vname, values[0]))

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

    def check_self(self, datastore):
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

    def check_self(self, datastore):
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

    def check_self(self, datastore):
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

    def is_superceded(self, checks):
        """
        All other checks first check for missing values, so if there any other
        checks in the list for the same variable, this one is redundant.
        """
        for c in checks:
            if c != self and c.vname == self.vname:
                logger.debug("%s is superceded by %s" % (self.name(), c.name()))
                return True
        return False

    def check(self, datastore):
        # Override the public method since we don't need to check twice for
        # a missing value.
        return self.check_self(datastore)

    def check_self(self, datastore):
        return self.check_missing(datastore)


class GGQUAL(Check):

    def __init__(self):
        Check.__init__(self, "ggqual")

    def check_self(self, datastore):
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
    "Read CheckTemplates from an XML file which can then be instantiated."

    def __init__(self, xmlpath=None):
        """Create empty ChecksXML with no templates."""
        self.path = xmlpath
        self.xtree = None
        self.templates = []

    def setPath(self, xmlpath):
        "Set the path to the XML file."
        self.path = xmlpath

    def load(self):
        """Parse the checks file and load all the check templates.

        The @p xmlpath parameter is a convenient shortcut for specifying
        the path to open.  The templates are appended to any existing
        templates, so it is possible to read multiple files into one
        combined list of templates.

        """
        self.xtree = ET.parse(self.path)
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

        # Now cull the checks, asking each check if it is redundant and
        # should be removed.  Put the checks to remove in a dictionary
        # for faster lookup, hopefully.
        remove = {}
        for check in checks:
            if check.is_superceded(checks):
                remove[check.name()] = check
        checks = [check for check in checks 
                  if not remove.has_key(check.name())]

        logger.info("generated %d checks from %d templates, "
                    "culled %d redundant checks" % 
                    (len(checks), len(self.getCheckTemplates()),
                     len(remove)))
        return checks


