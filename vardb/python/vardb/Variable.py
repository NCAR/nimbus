"The Variable class for representing a RAF variable and its metadata."

from vardb import VDBVar
import psycopg2 as pg

import logging

logger = logging.getLogger(__name__)

class VariableList(object):
    "Interface to the variable list in the SQL database."

    def __init__(self):
        self.dbname = "real-time"
        self.user = "ads"
        self.hostname = "acserver"
        self.db = None

    def loadVariables(self):
        self.db = pg.connect(database=self.dbname, user=self.user,
                             host=self.hostname)
        cursor = self.db.cursor()
        cursor.execute("""
SELECT name, units, uncalibrated_units, long_name, missing_value 
FROM variable_list;
""")
        rows = cursor.fetchall()
        vars = {}
        for r in rows:
            var = Variable(r[0])
            var.units = r[1]
            if r[2]:
                var.uncalibrated_units = r[2]
            var.long_name = r[3]
            var.missing_value = float(r[4])
            vars[var.name] = var
        logger.info("Loaded %d variables from database." % (len(vars)))
        self.db.close()
        return vars



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

