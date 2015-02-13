"""Generate Check instances from ChecksXML, embed in a NagiosConfig file,
and extract them to execute the checks."""

import logging

logger = logging.getLogger(__name__)

from vardb import VariableList
from Checks import ChecksXML
from Checks import Check
from NagiosConfig import NagiosConfig
from NagiosCommands import NagiosCommands
from vardb import DataStore

# The goal is to store Check instances, with all their associated
# parameters, so they can be loaded and executed over and over without
# repeating all the metadata lookup.  It just sounds more robust to me to
# run the checks from one config file, without depending upon all the
# metadata sources on each run.  It will also make it easier to tell when
# the config has changed and should be regenerated.

# So the way we're going to try it is to store the checks as xml inside
# comments in the nagios config file.

# For each check, write the xml serliazation in a comment, then follow it
# with the nagios service definition.

class NagiosChecks(object):

    _embedded_prefix = "#CHECK: "

    def __init__(self):
        self.options = None
        self.hostname = 'RAF'

    def addOptions(self, parser):
        parser.add_option("--db", type="string", help="""\
Specify the database connection or hostname.  The default is 'acserver'.
Use 'c130' or 'gv' to use the ground real-time database for that plane.
Use 'env' to use the settings in the PG environment variables.""")

        parser.add_option("--projdir", type="string", help="""\
Specify or override the PROJ_DIR environment variable, the directory path
which contains project configuration directories.""")

        parser.add_option("--nagios", type="string", help="""\
The path to the nagios configuration file.  When generating a config,
the config will be written to this file.  When actually running the nagios
passive checks, the checks will be read from this file.  The default path
is %s.""" % (NagiosConfig.DEFAULT_PATH))

        parser.add_option("--checks", type="string", help="""\
Path to the XML checks.xml file.  Defaults to the project config directory.""")

        parser.add_option("--vdb", type="string", help="""\
Override the default path to the vardb.xml file.""")

        parser.add_option("--commands", type="string", help="""\
Path to which nagios commands will be written.  The default is %s, but
for debugging it can be a regular file or /dev/null.""" % 
                          (NagiosCommands.DefaultCommandFile), 
                          default=False)


    def setOptions(self, options):
        self.options = options

    def writeConfigFile(self, checks, path=None):
        configfile = NagiosConfig()
        configfile.open(path)
        configfile.write(configfile.makeHost(self.hostname))
        for check in checks:
            configfile.write("\n" + NagiosChecks._embedded_prefix + 
                             check.toString() + "\n")
            configfile.write(configfile.makeService(self.hostname, check.name()))
        configfile.close()

    def readChecksFromConfigFile(self, path=None):
        configfile = NagiosConfig()
        configfile.openForReading(path)
        checks = []
        for line in configfile.iterateLines(NagiosChecks._embedded_prefix):
            line = line.replace(NagiosChecks._embedded_prefix, "", 1)
            check = Check.fromString(line)
            checks.append(check)
        logger.info("extracted %d checks embedded in '%s'" %
                    (len(checks), configfile.path))
        configfile.close()
        return checks

    def setupVariableList(self):
        vlist = VariableList()
        if self.options.db:
            vlist.setDatabaseSpecifier(self.options.db)
        if self.options.projdir:
            vlist.projdir = self.options.projdir
        if self.options.vdb:
            vlist.vdbpath = self.options.vdb
        vlist.loadVariables()
        return vlist

    def writeConfig(self):
        vlist = self.setupVariableList()
        # Merge in the vardb.xml in case any checks need to inherit limits
        # from the metadata.
        vlist.loadVdbFile()

        # Load the checks.xml file and instantiate all the checks against
        # the current variables.
        checksxml = ChecksXML()
        checksxml.load(self.options.checks)
        checks = checksxml.generateChecks(vlist.getVariables())
        self.writeConfigFile(checks, self.options.nagios)
        vlist.close()


    def executeChecks(self):
        # Extract and parse the checks from the nagios config file and
        # execute them.
        checks = self.readChecksFromConfigFile(self.options.nagios)
        
        # Compile a list of variables required with the maximum lookback
        # required for each.
        vlist = self.setupVariableList()
        lookback = 1
        for c in checks:
            vlist.selectVariable(c.vname)
            lookback = max(lookback, c.lookback)

        # Now request data values for the select variables.  What we get
        # back is a map from variable name to a list of values.
        datastore = vlist.getLatestValues(DataStore(), lookback)

        # For each check, make sure values were retrieved (meaning the
        # variable is in the database) before calling the check.  If checks
        # ever depend on more than one variable, then we'll probably need a
        # syntax for that in the CheckTemplate and Check (such as CSV) so
        # we can still aggregate the variables on which the checks depend,
        # otherwise it would be up to each check to make sure it's
        # variables exist in the datastore.
        results = []
        for c in checks:
            if not datastore.getValues(c.vname):
                status = c.Status().critical(
                    "Variable '%s' is not in the database." % (c.vname))
            else:
                status = c.check(datastore)
            results.append(status)

        # Now we can pipe the results to the right place.
        #cmds is passive check output file, which is piped to nagios by shell
        commands = NagiosCommands()
        commands.open(self.options.commands)
        for status in results:
            statline = "%s;%s;%d;%s\n" % (self.hostname, status.getName(), 
                                          status.getLevel(), 
                                          status.consoleMessage())
            commands.processServiceCheckResult(statline)
        commands.close()
        vlist.close()
