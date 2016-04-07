#! /usr/bin/python

"""
This is the command-line script for running all the operations in the
NimbusProject class.
"""

import os
import sys
import fnmatch
import logging

# Hardcode some python paths so that SCons and eol_scons are found
# somewhere.
sys.path.append("/usr/lib/scons")
sys.path.append(os.path.expanduser("~/.scons/site_scons"))
_sspath = os.path.abspath(os.path.join(os.path.dirname(__file__), 
                                       "..", "..", "caledit", "site_scons"))
sys.path.append(_sspath)
sys.path.append("/usr/share/scons/site_scons")

from optparse import OptionParser 
from NimbusProject import NimbusProject
from NimbusProject import NimbusProjectException

_usage = """Usage: pnimbus.py [options] {operation ...} [flight-spec ...]

The named operations will be run for all the flights named by setup files
in the <setup> directory, or else only the flights listed after the
operations.  These are the available operations:

  nimbus:

    Run NIMBUS on the setup files in the given production directory but
    with the output redirected to the specified output directory.

  reorder:

    Run just ncReorder on the output files.  The 'nimbus' operation runs
    ncReorder automatically when the nimbus program succeeds.

  process2d:

    Run process2d for the project flights, looking for the 2d input file
    corresponding to the flight from the setup file.

  compare: 

    Compare the netcdf files in the output directory with the output
    specified in the original setup files.

  diff:

    Run diff on the nimbus output logs.  This is not necessarily possible
    except when comparing against a base project (see --base), since
    otherwise there are no log files.  The log files are preprocessed to
    remove timestamps from the log messages.

  ncdiff:

    Run diff on the ncdump output of the netcdf output files.  The diff
    command adds options to ignore the typical differences, and the ncdump
    prints floating point numbers with 6 significant digits.

  flights:

    List the flights for all of the setup files loaded for this project.

Modified setup files and netcdf output files will be written to the output
directory named by the --output option or else to a directory named
after the project.

The flight specifiers have the form <project>/<flight>, where either
<project> or <flight> can be glob patterns.  If there is no /, then the
specifier is just a project name and selects all the flights in that
project.  An empty string on left side of the slash selects all the
projects, and an empty string after the slash selects all the flights.  So
a specifier like */rf01 selects the first research flight of all the known
projects."""

_default_projects = """
HIPPO-1
HIPPO-2
HIPPO-3
HIPPO-4
HIPPO-5
PREDICT
TORRERO
DC3
MPEX
DEEPWAVE
CSET
CONTRAST
""".split()

def main(args):
    dlevel = logging.INFO
    parser = OptionParser(usage=_usage)
    parser.add_option("--debug", dest="level", action="store_const",
                      const=logging.DEBUG, default=dlevel)
    parser.add_option("--info", dest="level", action="store_const",
                      const=logging.INFO, default=dlevel)
    parser.add_option("--error", dest="level", action="store_const",
                      const=logging.ERROR, default=dlevel)
    NimbusProject.addOptions(parser)
    (options, args) = parser.parse_args(args)
    logging.basicConfig(level=options.level)
    operations, flights = NimbusProject.splitOperations(args[1:])
    if not operations:
        print("Specify at least one operation.  Use --help to see usage info.")
        sys.exit(1)

    try:
        nprojects = []
        for fspec in flights:
            pproject = None
            pflights = ''
            if '/' not in fspec:
                pproject = fspec
            else:
                (pproject, slash, pflights) = fspec.partition('/')
            if not pproject:
                pproject = '*'
            if not pflights:
                pflights = '*'
            
            projects = fnmatch.filter(_default_projects, pproject)
            if not projects:
                # Nothing matched, accept it as a fixed project name.
                projects = [pproject]
            for project in projects:
                np = NimbusProject(project)
                np.applyOptions(options)
                np.open()
                np.selectFlights(pflights)
                nprojects.append(np)

            for np in nprojects:
                np.run(operations)
                np.close()
    except NimbusProjectException, ex:
        print(str(ex))
        sys.exit(1)

if __name__ == "__main__":
    main(sys.argv)

