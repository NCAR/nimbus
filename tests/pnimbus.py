#! /usr/bin/python

"""
This is the command-line script for running all the operations in the
NimbusProject class.
"""

import os
import sys
import fnmatch
import logging
import collections

logger = logging.getLogger('pnimbus')

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

The named operations will be run for all the flights with setup files which
match the flight specifiers listed after the operations.

Modified setup files, netcdf output files, and log files will be written to
the output directory named by the --output option.  The default output
directory is a subdirectory named after the project.

The flight specifiers have the form <project>/<profile>/<flight>, where any
of the fields can be glob patterns.  If there is no /, then the specifier
is just a project name and selects all the flights and profiles in that
project.  The project must be found as a subdirectory of the PROJ_DIR
variable.  An empty string on left side of the slash selects all the
projects, and an empty string after the slash selects all the flights.  The
profile field can be matched also, using the names mentioned below.  So a
specifier like //rf01 selects the first research flight of all the known
projects and profiles.

Two profiles are created automatically for each project: base and local.
The local profile runs the nimbus program within the local source tree.
The base profile runs the installed nimbus program and writes output to a
subdirectory of the local profile called BASE.  For the operations which
compare the outputs of a project, the local profiles will be compared
against the output of the corresponding base profiles.  In effect, the
comparison operations only run on the 'local' profiles and not on the
'base' profiles.  The command below only compares the local run with the
base run.

  pnimbus.py compare CONTRAST//rf01

Even though CONTRAST//rf01 matches both local and base profiles, the base
profile does not have anything to compare against.

These are the available operations:

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

    Compare the netcdf file in the output directory with the output
    specified in the original setup files, using nc_compare.

  diff:

    Run diff on the nimbus output logs.  This is not necessarily possible
    except when comparing against a base project (see --base), since
    otherwise there are no log files.  The log files are preprocessed to
    remove timestamps from the log messages.

  ncdiff:

    Run diff on the ncdump output of the netcdf output files.  The diff
    command adds options to ignore the typical differences, and the ncdump
    prints floating point numbers with 6 significant digits.
"""

_default_projects = """
HIPPO-1
HIPPO-2
HIPPO-3
HIPPO-4
HIPPO-5
PREDICT
TORERO
DC3
MPEX
DEEPWAVE
CSET
CONTRAST
""".split()

_default_profiles = ["local", "base"]

def pnimbus(options, operations, flights):
    """
    Accumulate a list of flights as specified in the command line
    specifiers.  Allow duplicates for now.  It shouldn't hurt anything to
    have duplicate NimbusProject instances.

    Specifiers are parsed from general to specific, so project is followed
    by flight is followed by profile: project/flight/profile.  To get all
    flights for one profile, use //profile or */*/profile.  Any unspecified
    or empty fields match all possibilities.
    """
    nprojects = collections.OrderedDict()
    if not flights:
        flights = _default_projects
    for fspec in flights:
        pproject = None
        pflights = ''
        (pproject, slash_, pflights) = fspec.partition('/')
        (pprofiles, slash_, pflights) = pflights.partition('/')
        if not pproject:
            pproject = '*'
        if not pflights:
            pflights = '*'
        if not pprofiles:
            pprofiles = '*'

        projects = fnmatch.filter(_default_projects, pproject)
        profiles = fnmatch.filter(_default_profiles, pprofiles)
        if not projects:
            # Nothing matched, accept it as a fixed project name.
            projects = [pproject]
        if not profiles:
            raise NimbusProjectException("no profiles matching: '%s'" %
                                         pprofiles)
        for project in projects:
            local = NimbusProject(project)
            local.applyOptions(options)
            local.setupLocal()
            local.open()
            local.selectFlights(pflights)

            base = NimbusProject(project)
            base.applyOptions(options)
            base.setupBase(local)
            base.open()
            base.selectFlights(pflights)
            if 'base' in profiles:
                nprojects["%s/base" % (project)] = base
            if 'local' in profiles:
                nprojects["%s/local" % (project)] = local

    if options.flights:
        print(" ".join([" ".join(["%s/%s" % (key, f)
                                  for f in np.getFlights()])
                        for key, np in nprojects.items()
                        if np.getFlights()]))
    else:
        for key, np in nprojects.items():
            np.run(operations)
            np.close()


def main(args):
    dlevel = logging.INFO
    parser = OptionParser(usage=_usage)
    parser.add_option("--debug", dest="level", action="store_const",
                      const=logging.DEBUG, default=dlevel)
    parser.add_option("--info", dest="level", action="store_const",
                      const=logging.INFO, default=dlevel)
    parser.add_option("--error", dest="level", action="store_const",
                      const=logging.ERROR, default=dlevel)
    parser.add_option("--flights", action="store_const",
                      const=True, default=False,
                      help="""\
Ignore the operations and just list the flights selected by the specifiers.""")
    NimbusProject.addOptions(parser)
    (options, args) = parser.parse_args(args)
    logging.basicConfig(level=options.level)
    operations, flights = NimbusProject.splitOperations(args[1:])
    if not options.flights and not operations:
        print("Specify at least one operation.  Use --help to see usage info.")
        sys.exit(1)

    try:
        pnimbus(options, operations, flights)

    except NimbusProjectException, ex:
        print(str(ex))
        sys.exit(1)

if __name__ == "__main__":
    main(sys.argv)

