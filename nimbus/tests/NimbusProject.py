"""
Module for the :py:class:`NimbusProject` class.
"""

import os
import glob
import logging
from NimbusSetup import NimbusSetup
import shutil
import tempfile
import subprocess as sp

logger = logging.getLogger(__name__)

# http://stackoverflow.com/questions/377017/
#         test-if-executable-exists-in-python/377028

class NimbusProjectException(Exception):
    pass


def which(program, env=None):
    if env is None:
        env = os.environ
    def is_exe(fpath):
        return os.path.isfile(fpath) and os.access(fpath, os.X_OK)

    fpath, fname_ = os.path.split(program)
    if fpath:
        if is_exe(program):
            return program
    else:
        for path in env['PATH'].split(os.pathsep):
            path = path.strip('"')
            exe_file = os.path.join(path, program)
            if is_exe(exe_file):
                return exe_file
    return None


class NimbusProject(object):

    """
    NIMBUS processing and comparison for an entire aircraft project.

    Provide methods to redirect output from production setup files and run
    the NIMBUS processing on the modified files, then compare the output
    with known good output from standard locations.  This class is the main
    implementation for the pnimbus script.

    Construct a NimbusProject instance for the project with name
    *project_name*, aircraft ID *aircraft*, and output directory
    *output_dir*.  The project name is used to derive default standard
    paths to raw data and production setup files.  See also
    setProjectName(), setAircraftName(), and setOutputDirectory().

    After configuring the NimbusProject, finish the setup by calling open().
    """

    def __init__(self, project_name=None, aircraft=None, output_dir=None):
        self.project = project_name
        self.aircraft = aircraft
        self.output_dir = output_dir
        self.compare_dir = None
        self.projects = os.environ['PROJ_DIR']
        self.prodpath = None
        # Custom executable path includes the locally built nc_compare.
        nccmp = os.path.join(os.path.dirname(__file__),
                             '../../nc_utils/nc_compare')
        ncreo = os.path.join(os.path.dirname(__file__),
                             '../../nc_utils/ncReorder')
        self.xpath = [nccmp, ncreo]
        # A dictionary of setup files for this project keyed by flight.
        self.setups = {}
        self.dryrun = False
        # NIMBUS executable
        self.nimbus = None
        # If this project run should be compared against the same project
        # run but with a different nimbus executable, then this member
        # holds the base project instance.
        self.base = None

    operations = ['nimbus', 'reorder', 'compare', 'diff',
                  'flights', 'process2d']

    @staticmethod
    def addOptions(parser):
        """
        Add the optparse options by which NimbusProject instances can be
        configured.
        """
        parser.add_option("--dryrun", action="store_true", default=False,
                          help="Load configuration and echo steps but "
                          "do not run any commands.")
        parser.add_option("--setup", type='string', 
                          help="Path to directory of setup files, "
                          "typically the Production directory of a "
                          "project configuration.")
        parser.add_option("--project", type='string', 
                          help="Specify project name to derive a setup "
                          "path using the Production directory of a "
                          "project configuration.")
        parser.add_option("--output", type='string', default='.',
                          help="Set output directory for netcdf files "
                          "and modified setup files.")
        parser.add_option("--compare", type='string', 
                          help="Look for primary netcdf files in this "
                          "directory, to be compared against the netcdf "
                          "files in the output directory.")
        parser.add_option("--nimbus", type='string', 
                          help="Alternate path to nimbus executable.")
# pylint: disable=C0330
        parser.add_option("--base", action="store_true", default=False,
                          help=
"""Generate a 'base' project from this project which uses the standard
nimbus on the path, and compare this project against the output of that
base project.  In other words, rather than compare against existing
'production' netcdf output files, create the production files directly with
this script using the installed nimbus (the one on the PATH), then compare
those files with the output of this same script but run with the alternate
nimbus path."""
        )
# pylint: enable=C0330

    def applyOptions(self, options):
        """
        Apply the options created by addOptions() and parsed by
        optparse.ParseOptions().
        """
        self.dryrun = options.dryrun
        self.nimbus = options.nimbus
        if options.project:
            self.project = options.project
        self.setOutputDirectory(options.output)
        self.setCompareDirectory(options.compare)
        if options.setup:
            self.setSetupPath(options.setup)
        if options.base:
            options.base = False
            self.createBase(options)
            # If using a base but nimbus not set, then use the local source
            # path as the default.
            if self.nimbus is None:
                self.nimbus = self.getLocalNimbusPath()
                logger.info("Using local nimbus path by default "
                            "since base is enabled: " + str(self.nimbus))

    def getLocalNimbusPath(self):
        """
        Return the path to the nimbus executable built in the local source
        tree, even if it does not exist.
        """
        nimbus = os.path.join(os.path.dirname(__file__), "../src/filter/nimbus")
        nimbus = os.path.normpath(nimbus)
        return nimbus

    def getNimbusPath(self):
        """
        Try to determine the fully qualified absolute path to nimbus that will
        be used by this configuration.
        """
        nimbus = which(self.nimbus or "nimbus", self._getSubEnv())
        if nimbus:
            nimbus = os.path.abspath(nimbus)
        return nimbus

    def createBase(self, options):
        """
        Configure a base project instance against which the output from this
        NimbusProject instance can be compared.
        """
        basedir = self.createBaseDirectory()
        base = NimbusProject()
        base.applyOptions(options)
        # Reset the nimbus path if it was set.  If it was not set, then
        # that is an error that will be caught on open().
        if options.nimbus:
            base.nimbus = None
        base.setOutputDirectory(basedir)
        self.setCompareDirectory(base.getOutputDirectory())
        self.base = base

    def setProjectName(self, project):
        """
        Set the project name, same as if done when this instance was created,
        as long as open() has not been called yet.
        """
        self.project = project

    def setAircraftName(self, aircraft):
        """
        Set the aircraft name, same as if done when this instance was created,
        as long as open() has not been called yet.
        """
        self.aircraft = aircraft

    def setSetupPath(self, prodpath):
        """
        Set the production path and parse the project name and aircraft name
        from it.
        """
        logger.debug("setSetupPath(%s)" % (prodpath))
        self.prodpath = prodpath
        dirs = os.path.abspath(prodpath).split("/")
        self.aircraft = dirs[-2]
        self.project = dirs[-3]
        self.projects = "/".join(dirs[:-3])
        logger.debug("setSetupPath: %s" % (self._dump()))

    def setCompareDirectory(self, compdir):
        """
        Set the directory in which to find output files to compare against.
        This directory overrides the full output file path that may have
        been specified in the original setup file.
        """
        self.compare_dir = compdir

    def setOutputDirectory(self, outdir):
        """
        Set the output directory, same as if done when this instance was
        created, as long as open() has not been called yet.  When setup
        files are loaded, they are reconfigured to point their output to
        this directory.
        """
        self.output_dir = outdir

    def getOutputDirectory(self):
        return self.output_dir

    def createBaseDirectory(self):
        "Create and return a BASE directory under the output directory."
        base = os.path.join(self.getOutputDirectory(), "BASE")
        if not os.path.exists(base):
            logger.debug("creating directory %s", base)
            os.mkdir(base)
        logger.info("BASE directory: %s", base)
        return base

    def _dump(self):
        return str({'projects':self.projects, 'project':self.project,
                    'aircraft':self.aircraft, 'dryrun':self.dryrun,
                    'output':self.output_dir})

    def open(self):
        """
        Initialize and validate derived paths and load the setup files for the
        configured project.  After calling open(), getFlights() will return
        the list of flights whose setup files were loaded.  If a base
        project has been configured, than that will also be opened.
        """
        if self.base:
            # Check that nimbus paths are different.
            bnimbus = self.base.getNimbusPath()
            dnimbus = self.getNimbusPath()
            if bnimbus is None:
                raise NimbusProjectException(
                    "nimbus path for baseline project not found.")
            if bnimbus == dnimbus:
                raise NimbusProjectException(
                    "nimbus paths for baseline project and "
                    "parent project are the same! (%s) "
                    "Use --nimbus to set an alternate path for the "
                    "parent project configuration." % (dnimbus))
            self.base.open()
        self.setupDirectories()
        self.loadSetupFiles()

    def close(self):
        """
        Since there is an open() method, this is the corresponding close().
        """
        if self.base:
            self.base.close()

    def aircraftFromProject(self, pdir):
        """
        By convention, the configuration directory for a project has a
        subdirectory for the particular aircraft on the project.  This
        method tries to infer the aircraft name for a project from the
        subdirectories of the project directory.  For example,
        /net/jlocal/projects/WINTER contains the subdirectory C130_N130AR,
        so that is returned as the aircraft name.
        """
        acdir = glob.glob(os.path.join(pdir, "GV_*"))
        acdir.extend(glob.glob(os.path.join(pdir, "C130_*")))
        if len(acdir) == 1:
            return os.path.basename(acdir[0])
        if len(acdir) > 1:
            logger.error("Multiple aircraft directories under %s", pdir)
        elif len(acdir) < 1:
            logger.error("No aircraft directories recognized under %s", pdir)
        return None

    def setupDirectories(self):
        """
        Derive and validate important paths like the setup directory.

        The setup directory defaults to the Production directory as long as
        the project is specified and the aircraft can be inferred from the
        project directory with aircraftFromProject().
        """
        if self.output_dir is None:
            self.output_dir = '.'
        logger.debug("setupDirectories: " + self._dump())
        if self.project and not self.prodpath:
            pdir = os.path.join(self.projects, self.project)
            self.aircraft = self.aircraftFromProject(pdir)
            if self.aircraft:
                pdir = os.path.join(pdir, self.aircraft, 'Production')
            self.prodpath = pdir
        if not self.prodpath:
            raise NimbusProjectException(
                "Setup directory path not specified, and the "
                "Production path cannot be derived because "
                "no project is set or aircraft is indeterminable.")
        if not os.path.exists(self.prodpath):
            raise NimbusProjectException("Setup directory does not exist: %s" % 
                                         (self.prodpath))

    def loadSetupFiles(self):
        "Load and reconfigure all the setup files for this project."
        files = glob.glob(os.path.join(self.prodpath, "setup_?f??"))
        for f in files:
            self.loadSetup(f)

    def loadSetup(self, path):
        """
        Create a NimbusSetup for the setup file at the given path, associated
        with the flight ID parsed from the setup file name.

        The input parameters in the setup file are not changed, on the
        assumption that this is running on the same system for which the
        setup file was created and on which any comparison output files
        exist.  The output is redirected to the current output directory,
        so this raises an exception if no output directory has been set.
        """
        setup = NimbusSetup(path, self.project)
        if not setup.flight:
            logger.error("Skipping %s: could not determine flight number." %
                         (path))
            return
        if self.output_dir is None:
            raise NimbusProjectException("No output directory is set.")
        setup.setOutputDirectory(self.output_dir)
        ifile = setup.resolveInputFile()
        if not ifile:
            logger.error("Skipping %s: input file could not be found." %
                         (path))
            return
        logger.debug("Resolved input file: %s" % (ifile))
        self.setups[setup.flight] = setup

    def getFlights(self):
        """
        Return the list of sorted flight IDs for all the setup files loaded for
        this NimbusProject.
        """
        flights = self.setups.keys()
        flights.sort()
        return flights

    def _getSubEnv(self):
        spenv = {}
        spenv.update(os.environ)
        xpath = spenv.get('PATH', '').split(":")
        xpath = self.xpath + xpath
        spenv['PATH'] = os.pathsep.join(xpath)
        return spenv

    def _runCommand(self, cmd, logpath=None):
        # cmd = ['echo'] + cmd
        spenv = self._getSubEnv()
        exe = which(cmd[0], spenv)
        if exe:
            cmd[0] = os.path.abspath(exe)
        msg = " ".join(cmd)
        if logpath:
            msg += " >& " + logpath
        logger.info(msg)
        xcode = 0
        if self.dryrun:
            return xcode
        lf = None
        try:
            if logpath:
                lf = open(logpath, "w")
            proc = sp.Popen(cmd, stdout=lf, stderr=sp.STDOUT, 
                            shell=False, env=spenv)
            xcode = proc.wait()
        finally:
            if lf:
                lf.close()
        if xcode != 0:
            logger.error("%s exited with status %d" % (cmd[0], xcode))
        return xcode

    def runProcess2d(self, setup):
        """
        Run process2d for the given setup file.

        Given the raw input data from the setup file, find the
        corresponding 2d input file, run process2d on it and write the
        changes to the output file.
        """
        ifile = setup.getInputFile()
        p2ddir = os.path.join(os.path.dirname(ifile), "PMS2D")
        file2d = glob.glob(os.path.join(p2ddir, "*_%s.2d" % (setup.flight)))
        if not file2d:
            logger.error("No 2D file found for %s." % (ifile))
            return
        elif len(file2d) > 1:
            logger.error("Too many matches for 2D file for %s." % (ifile))
            return
        file2d = file2d[0]
        cmd = ['process2d', file2d, '-o', setup.getOutputFile()]
        self._runCommand(cmd)

    def runReorder(self, setup):
        """
        Run the ncReorder step on the netcdf output file for the given
        NimbusSetup instance.  The ncReorder output is created in a
        temporary directory and then moved to replace the original output.
        """
        tdir = tempfile.mkdtemp()
        ofile = setup.getOutputFile()
        tfile = os.path.join(tdir, os.path.basename(ofile))
        cmd = ["ncReorder", ofile, tfile]
        xcode = self._runCommand(cmd)
        if xcode == 0:
            # move file
            logger.info("mv %s %s" % (tfile, ofile))
            if not self.dryrun:
                shutil.move(tfile, ofile)
        shutil.rmtree(tdir)

    def runNimbus(self, setup):
        """
        Run NIMBUS on the given setup file.

        The setup file should have already been redirected to the output
        directory, such as happens for setup files created by loadSetup()
        and returned by getFlights().  This writes the setup file to disk
        for NIMBUS to read with the -b option.  If NIMBUS succeeds, then
        the ncReorder step is run automatically on the output.
        """
        os.environ['PROJ_DIR'] = self.projects
        logger.debug("Running setup file %s with output: %s" %
                     (setup.getPath(), setup.getOutputFile()))
        logger.debug("PROJ_DIR=%s; DISPLAY=%s" % 
                     (os.environ['PROJ_DIR'], os.environ.get('DISPLAY')))
        cmd = [self.nimbus or "nimbus", '-b', setup.getPath()]
        if not self.dryrun:
            setup.writeSetupFile()
        logfile = setup.getPath() + ".log"
        xcode = self._runCommand(cmd, logfile)
        # Run ncReorder on the output file no matter what.  If the output
        # was not created, then it fails.  If it was created but nimbus
        # exited with an error, then it will still be reordered.
        self.runReorder(setup)

    def getComparisonOutputFile(self, setup):
        """
        Return the path to the output file which should serve as the base
        comparison file.  The original output file is taken from the
        NimbusSetup *setup*, the output file path as it was before being
        redirected by loadSetup().  If a comparison directory has been set
        with setCompareDirectory(), then that overrides the original
        directory of the output file.  For example, if a base project is
        associated with this project, then the output directory of the base
        project is used as the compare directory for this project.  Or, if
        the production output files do not exist on this system at the
        original path in the setup files, then their correct location can
        be specified with setCompareDirectory() and this method will use
        that to return the correct path to the output file.
        """
        cfile = setup.original_ofile
        # Redirect source file if comparison directory given.
        if self.compare_dir is not None:
            cfile = os.path.basename(cfile)
            cfile = os.path.join(self.compare_dir, cfile)
        return cfile

    def runCompare(self, setup):
        "Run nc_compare between the setup output and the comparison file."
        cfile = self.getComparisonOutputFile(setup)
        ofile = setup.getOutputFile()
        cmd = ['nc_compare', cfile, ofile]
        self._runCommand(cmd)

    def runDiff(self, setup):
        "Run diff between the nimbus log files."
        cfile = self.getComparisonOutputFile(setup)
        cfile = os.path.dirname(cfile)
        cfile = os.path.join(cfile, os.path.basename(setup.getPath()))
        cfile += ".log"
        ofile = setup.getPath() + ".log"
        cmd = ['diff', '--side-by-side', '--width=200', cfile, ofile]
        self._runCommand(cmd)

    def _runFlights(self, flights, opfun):
        if not flights:
            flights = self.getFlights()
        for flight in flights:
            opfun(self.setups[flight])

    def reorderFlights(self, flights):
        self._runFlights(flights, self.runReorder)

    def process2dFlights(self, flights):
        self._runFlights(flights, self.runProcess2d)

    def nimbusFlights(self, flights=None):
        "Run NIMBUS on all the setup files for a project."
        import eol_scons.xvfb
        xvfb = None
        if not self.dryrun:
            xvfb = eol_scons.xvfb.Xvfb()
            xvfb.start()
        self._runFlights(flights, self.runNimbus)
        if xvfb:
            xvfb.stop()

    def compareFlights(self, flights):
        """
        Compare the output files generated for this project.

        The expected outputs are taken from all the setup files loaded for
        this project, then compared against the corresponding filenames in
        the comparison directory.
        """
        self._runFlights(flights, self.runCompare)

    def diffFlights(self, flights):
        self._runFlights(flights, self.runDiff)

    @staticmethod
    def splitOperations(args):
        "Separate operations from flights in an argument list."
        operations = []
        flights = []
        # Split arguments into operations and flights
        for arg in args:
            if arg in NimbusProject.operations:
                operations.append(arg)
            elif not operations:
                raise NimbusProjectException(
                    "An operation must be specified first.")
            else:
                flights.append(arg)
        return (operations, flights)

    def run(self, operations, flights):
        """
        For an open() project, run the given operations for each of the given
        flights.  If flights is empty, then run the operation for all of
        the flights, the list returned by getFlights().
        """
        # When running a base project, the compare and diff operations are
        # not relevant.
        if self.base:
            self.base.run([op for op in operations 
                           if op not in ['compare', 'diff']], flights)

        for op in operations:
            if op == "nimbus":
                self.nimbusFlights(flights)
            if op == "process2d":
                self.process2dFlights(flights)
            elif op == "flights":
                print("Flights: %s" % (" ".join(self.getFlights())))
            elif op == "reorder":
                self.reorderFlights(flights)
            elif op == "compare":
                self.compareFlights(flights)
            elif op == "diff":
                self.diffFlights(flights)


_setup_files = """
setup_ff01
setup_rf01
setup_rf02
setup_rf03
setup_rf04
setup_rf05
setup_rf06
setup_rf07
setup_rf08
setup_rf09
setup_rf10
setup_rf11
setup_rf12
setup_rf13
""".split()

# To run the tests:
#
# env PYTHONPATH=$HOME/.scons/site_scons:/usr/lib/scons py.test NimbusProject.py
#

def test_load_setups():
    np = NimbusProject('HIPPO-5', 'GV_N677F')
    np.open()
    assert(len(np.setups) == len(_setup_files))
    _setup_files.sort()
    keys = np.setups.keys()
    keys.sort()
    assert([s.path for s in [np.setups[k] for k in keys]] == 
           ['./'+s for s in _setup_files])
    np.close()

def test_prodpath_parse():
    np = NimbusProject()
    np.setSetupPath("/net/jlocal/projects/HIPPO-5/GV_N677F/Production")
    assert(np.aircraft == "GV_N677F")
    assert(np.project == "HIPPO-5")
    assert(np.prodpath == "/net/jlocal/projects/HIPPO-5/GV_N677F/Production")
    assert(np.projects == "/net/jlocal/projects")
    np = NimbusProject()
    np.setSetupPath("./projects/WINTER/C130_N130AR/Production")
    assert(np.aircraft == "C130_N130AR")
    assert(np.project == "WINTER")

def test_aircraftFromProject():
    logging.basicConfig(level=logging.DEBUG)
    np = NimbusProject()
    pdir = os.path.join(os.path.dirname(__file__), "projects")
    ac = np.aircraftFromProject(os.path.join(pdir, "CONTRAST"))
    assert(ac == "GV_N677F")
    ac = np.aircraftFromProject(os.path.join(pdir, "WINTER"))
    assert(ac == "C130_N130AR")
    ac = np.aircraftFromProject(os.path.join(pdir, "IDEAS-4"))
    assert(ac == None)
