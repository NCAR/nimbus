#!/usr/bin/python

# Creates nagios configuration files based on checks.xml.  checks.xml
# specifies the check templates, and those templates must be realized
# against a list variables and their metadata loaded from the real-time
# database and possibly also a vardb.xml file.

import logging

logger = logging.getLogger(__name__)

import sys
import os

from optparse import OptionParser
from vardb import VariableList
from Checks import ChecksXML
from NagiosConfig import NagiosConfig

# Now we have check instances generated.  The goal is to store those
# instances, with all their associated parameters, so they can be loaded and
# executed over and over without repeating all the metadata lookup.  It just
# sounds more robust to me to run the checks from one config file, without
# depending upon all the metadata sources on each run.  It will also make it
# easier to tell when the config has changed and should be regenerated.

# So the way we're going to try it is to store the checks as xml inside
# comments in the nagios config file.

# For each check, write the xml serliazation in a comment, then follow it
# with the nagios service definition.

def writeConfigFile(checks, path=None):
    HostName='RAF'
    configfile = NagiosConfig()
    configfile.open(path)
    configfile.write(configfile.makeHost(HostName))
    for check in checks:
        configfile.write("\n#CHECK: " + check.toString() + "\n")
        configfile.write(configfile.makeService(HostName, check.name()))
    configfile.close()


def main(argv):
    parser = OptionParser()

    parser.add_option("--debug", dest="loglevel", action="store_const",
                      const=logging.DEBUG, default=logging.ERROR,
                      help="Show debug log messages.")
    parser.add_option("--info", dest="loglevel", action="store_const",
                      const=logging.INFO, default=logging.ERROR,
                      help="Show info log messages.")

    parser.add_option("--db", type="string", help="""\
Specify the database connection or hostname.  The default is 'acserver'.
Use 'c130' or 'gv' to use the ground real-time database for that plane.
Use 'env' to use the settings in the PG environment variables.""")

    parser.add_option("--projdir", type="string", help="""\
Specify or override the PROJ_DIR environment variable, the directory path
which contains project configuration directories.""")

    parser.add_option("--output", type="string", help="""\
The file path to which the nagios configuration will be written.""")

    parser.add_option("--checks", type="string", help="""\
Path to the XML checks.xml file.  Defaults to the project config directory.""")

    parser.add_option("--vdb", type="string", help="""\
Override the default path to the vardb.xml file.""")

    (options, argv) = parser.parse_args(argv)

    logging.basicConfig(level=options.loglevel)

    vlist = VariableList()
    if options.db:
        vlist.setDatabaseSpecifier(options.db)
    if options.projdir:
        vlist.projdir = options.projdir
    if options.vdb:
        vlist.vdbpath = options.vdb
    vlist.loadVariables()
    vlist.loadVdbFile()

    checksxml = ChecksXML()
    checksxml.load(options.checks)
    checks = checksxml.generateChecks(vlist.getVariables())
    writeConfigFile(checks, options.output)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))



