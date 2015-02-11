"The Variable class for representing a RAF variable and its metadata."

from vardb import VDBVar
from vardb import VDBFile
import psycopg2 as pg
import os

import logging

logger = logging.getLogger(__name__)

# The database variable_list table contains the currently active
# real-time variables and some of their metadata, so in real-time it
# should serve as the most consistent source for variables and
# metadata.  Normally it will be consistent with what is in the nidas
# XML file and the XML variable database (VDBFile), although it will
# not contain all the metadata from those files.  (For example, the
# variable_list table does not contain the min/max limits which might
# be needed for real-time qc checks.)  Therefore this class provides a
# consolidated view of the real-time variable metadata by augmenting
# the database variable_list with the metadata from the VDB file.  In
# the future, such consolidation should also include the nidas XML
# file.  Either all of the requisite metadata can be put in the
# variable_list table where other applications can get it, or there
# should be an API in both C++ and Python which provides a single,
# consistent, consolidated view of variables and metadata.

class VariableList(object):
    """
    Provide a consolidated, consistent list of real-time
    variables and metadata from the SQL database and VDB file.
    """

    def __init__(self):
        self.dbname = "real-time"
        self.user = "ads"
        self.hostname = "acserver"
        self.db = None
        self.vdbpath = None
        self.project = None
        self.aircraft = None
        self.platform = None
        self.projdir = None
        self.configpath = None
        self.variables = {}

    def getVariables(self):
        return self.variables

    def setVariables(self, variables):
        "Explicitly replace the current variable dictionary."
        self.variables = variables

    def setDatabaseSpecifier(self, hostspec):
        if hostspec == 'acserver':
            self.dbname = "real-time"
            self.hostname = "acserver"
        elif hostspec == 'c130':
            self.dbname = "real-time-C130"
            self.hostname = "eol-rt-data.fl-ext.ucar.edu"
        elif hostspec == 'gv':
            self.dbname = "real-time-GV"
            self.hostname = "eol-rt-data.fl-ext.ucar.edu"
        else:
            self.hostname = hostspec

    def configPath(self):
        if not self.configpath:
            select = (lambda a, b: [a,b][int(not a)])
            parms = {}
            parms['project'] = select(self.project, '${PROJECT}')
            parms['aircraft'] = select(self.aircraft, '${AIRCRAFT}')
            parms['projdir'] = select(self.projdir, '${PROJ_DIR}')
            path = '%(projdir)s/%(project)s/%(aircraft)s' % parms
            self.configpath = os.path.expandvars(path)
        return self.configpath

    def vdbPath(self):
        return os.path.join(self.configPath(), 'vardb.xml')

    def aircraftFromPlatform(self, platform):
        if platform == "N130AR":
            return "C130_N130AR"
        elif platform == "N677F":
            return "GV_N677F"
        return platform

    def loadVariables(self):
        self.db = pg.connect(database=self.dbname, user=self.user,
                             host=self.hostname)
        cursor = self.db.cursor()
        # We want to initialize global metadata also in case it's needed
        # to locate the vdb file or other files.
        cursor.execute("""
SELECT key, value
FROM global_attributes;
""")
        rows = cursor.fetchall()
        gatts = {}
        for r in rows:
            (key, value) = r
            gatts[key] = value
        if not self.project:
            self.project = gatts['ProjectName']
        if not self.platform:
            self.platform = gatts['Platform']
        if not self.aircraft:
            self.aircraft = self.aircraftFromPlatform(self.platform)
        cursor.execute("""
SELECT name, units, uncalibrated_units, long_name, missing_value 
FROM variable_list;
""")
        rows = cursor.fetchall()
        for r in rows:
            var = Variable(r[0])
            var.units = r[1]
            if r[2]:
                var.uncalibrated_units = r[2]
            var.long_name = r[3]
            var.missing_value = float(r[4])
            self.variables[var.name] = var
        logger.info("Loaded %d variables from database." % (len(self.variables)))
        self.db.close()
        return self.variables

    def loadVdbFile(self):
        """
        Load variable metadata from a vdb file and return it.  It can be merged
        separately with mergeVdbVariables().
        """
        # Any existing variable metadata from the database takes
        # precedence.
        if not self.vdbpath:
            self.vdbpath = self.vdbPath()
        vdb = VDBFile(self.vdbpath)
        if not vdb.is_valid():
            raise Exception("failed to open %s" % (self.vdbpath))
        variables = {}
        for i in xrange(vdb.num_vars()):
            vdbvar = vdb.get_var_at(i)
            var = Variable(vdbvar.name)
            var.fromVDBVar(vdbvar)
            variables[var.name] = var
        return variables

    def mergeVdbVariables(self, variables):
        for var in variables.itervalues():
            dbvar = self.variables.get(var.name, None)
            if dbvar:
                var.units = dbvar.units
                var.uncalibrated_units = dbvar.uncalibrated_units
                var.long_name = dbvar.long_name
                var.missing_value = dbvar.missing_value
                self.variables[var.name] = var
        return self.variables


class Variable(object):

    _attributes = {
        VDBVar.UNITS : str,
        VDBVar.LONG_NAME : str,
        VDBVar.IS_ANALOG : (lambda txt: txt == "true"),
        VDBVar.VOLTAGE_RANGE : str,
        VDBVar.DEFAULT_SAMPLE_RATE : int,
        VDBVar.MIN_LIMIT : float,
        VDBVar.MAX_LIMIT : float,
        VDBVar.CATEGORY : str,
        VDBVar.MODULUS_RANGE : str,
        VDBVar.DERIVE : str,
        VDBVar.DEPENDENCIES : str,
        VDBVar.STANDARD_NAME : str,
        VDBVar.REFERENCE : (lambda txt: txt == "true")
    }

    def __init__(self, name):
        self.name = name
        self.units = None
        self.uncalibrated_units = None
        self.long_name = None
        self.is_analog = None
        self.voltage_range = None
        self.default_sample_rate = None
        self.min_limit = None
        self.max_limit = None
        self.category = None
        self.modulus_range = None
        self.derive = None
        self.dependencies = None
        self.standard_name = None
        self.reference = None
        self.missing_value = None

    def _get_value(self, vdbvar, att, dfault, cvt):
        attvalue = vdbvar.get_attribute(att)
        if attvalue:
            return cvt(attvalue)
        return dfault

    def fromVDBVar(self, vdbvar):
        "Fill this Variable from the given variable database entry."
        self.name = vdbvar.name()
        for attname, attcvt in Variable._attributes.items():
            value = self._get_value(vdbvar, attname, 
                                    self.__getattribute__(attname), attcvt)
            self.__setattr__(attname, value)

