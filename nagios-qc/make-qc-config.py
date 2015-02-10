#!/usr/bin/python

# Creates nagios configuration files based on checks.xml.  checks.xml
# specifies the check templates, and those templates must be realized
# against a list variables and their metadata loaded from the real-time
# database and possibly also a vardb.xml file.

from vardb import VariableList
from Checks import ChecksXML

vlist = VariableList()

vlist.loadVariables()
vlist.loadVdbFile()

configfile = NagiosConfig.NagiosConfig()

HostName='RAF'

checksxml = ChecksXML()
checksxml.path = os.path.join(vlist.configPath(), 'checks.xml')
checksxml.parse()

checks = []
for checkp in checksxml.getChecks():
   checks.extend(checkp.generate(vlist.getVariables()))

# Now we have check instances generated.  The goal is to store those
# instances, with all their associated parameters, so they can be loaded and
# executed over and over without repeating all the metadata lookup.  It just
# sounds more robust to me to run the checks from one config file, without
# depending upon all the metadata sources on each run.  It will also make it
# easier to tell when the config has changed and should be regenerated.

# So the way we're going to try it is to store the checks as xml inside
# comments in the nagios config file.




configfile.write(configfile.makeHost(HostName))


configFile.close()
