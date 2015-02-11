#!/usr/bin/python

# Creates nagios configuration files based on checks.xml.  checks.xml
# specifies the check templates, and those templates must be realized
# against a list variables and their metadata loaded from the real-time
# database and possibly also a vardb.xml file.

import sys
import os

from optparse import OptionParser
from vardb import VariableList
from Checks import ChecksXML
from NagiosConfig import NagiosConfig

def generateChecks(vlist, xmlpath=None):
    checksxml = ChecksXML()
    if not xmlpath:
        xmlpath = os.path.join(vlist.configPath(), 'checks.xml')
    checksxml.path = xmlpath
    checksxml.parse()
    checks = []
    for checkp in checksxml.getChecks():
        checks.extend(checkp.generate(vlist.getVariables()))
    return checks

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

    parser.add_option("--db", type="string", help="""\
Database connection specifier or hostname.  The default is 'acserver'.  Use
'c130' or 'gv' to use the ground real-time database for that plane.""")

    parser.add_option("--projdir", type="string", help="""\
Specify or override the PROJ_DIR environment variable, the directory path
which contains project configuration directories.""")

    parser.add_option("--output", type="string", help="""\
The file path to which the nagios configuration will be written.""")

    parser.add_option("--checks", type="string", help="""\
Path to the XML checks.xml file.  Defaults to the project config directory.""")

    (options, argv) = parser.parse_args(argv)

    vlist = VariableList()
    if options.db:
        vlist.setDatabaseSpecifier(options.db)
    if options.projdir:
        vlist.projdir = options.projdir
    vlist.loadVariables()
    vlist.loadVdbFile()

    checks = generateChecks(vlist, options.checks)
    writeConfigFile(checks, options.output)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))



