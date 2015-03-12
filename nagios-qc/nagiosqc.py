#!/usr/bin/python

# Creates nagios configuration files based on checks.xml.  checks.xml
# specifies the check templates, and those templates must be realized
# against a list variables and their metadata loaded from the real-time
# database and possibly also a vardb.xml file.

import logging

logger = logging.getLogger(__name__)

import sys
import os

# Add some explicit paths to python packages until they get installed in
# the system path.  This avoids requiring that the PYTHONPATH environment
# variable be set when calling this script from nagios to run checks.
# 
thisdir = os.path.dirname(__file__)

sys.path.insert(0, os.path.abspath(os.path.join(thisdir, "../python")))
sys.path.insert(0, os.path.abspath(os.path.join(thisdir, "../vardb/python")))

from optparse import OptionParser
from NagiosQC import NagiosQC

_usage = """
nagiosqc.py {config|check} [options]

The 'config' operation generates the nagios config file.

The 'check' operation reads the checks from the nagios config file and
executes them, submitting the results as passive service checks to nagios."""


def main(argv):
    parser = OptionParser(usage=_usage)

    parser.add_option("--debug", dest="loglevel", action="store_const",
                      const=logging.DEBUG, default=logging.ERROR,
                      help="Show debug log messages.")
    parser.add_option("--info", dest="loglevel", action="store_const",
                      const=logging.INFO, default=logging.ERROR,
                      help="Show info log messages.")

    nqc = NagiosQC()
    options = nqc.parseOptions(argv, parser)

    logging.basicConfig(level=options.loglevel)
    logger.debug("operation: %s" % (",".join(argv)))

    return nqc.run()


if __name__ == "__main__":
    sys.exit(main(sys.argv))


