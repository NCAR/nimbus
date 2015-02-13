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
from NagiosChecks import NagiosChecks

_usage = """
nagios-qc [options] {config|check}

The 'config' operation generates the nagios config file.

The 'check' operation reads the checks from the nagios config file and
executes them, submitting the results as passive service checks to nagios.

"""


def main(argv):
    parser = OptionParser(usage=_usage)

    parser.add_option("--debug", dest="loglevel", action="store_const",
                      const=logging.DEBUG, default=logging.ERROR,
                      help="Show debug log messages.")
    parser.add_option("--info", dest="loglevel", action="store_const",
                      const=logging.INFO, default=logging.ERROR,
                      help="Show info log messages.")

    nc = NagiosChecks()
    nc.addOptions(parser)

    (options, argv) = parser.parse_args(argv)
    # Delete the program name.
    del argv[0]

    logging.basicConfig(level=options.loglevel)
    logger.debug("operation: %s" % (",".join(argv)))

    if not argv:
        print("Missing operation: 'config' or 'check'.\n" + _usage)
        sys.exit(1)

    operation = argv[0]
    nc.setOptions(options)

    if operation == "config":
        nc.writeConfig()
    elif operation == "check":
        nc.executeChecks()
    else:
        print("Unknown operation: %s" % (operation))
        sys.exit(1)

    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))


