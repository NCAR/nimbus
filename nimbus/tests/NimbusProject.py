"""
Module for the :py:class:`NimbusProject` class.
"""

import os
import glob
import logging
import shutil
import tempfile
import fnmatch
import subprocess as sp

from NimbusSetup import NimbusSetup

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


class XvfbManager:

    def __init__(self):
        self._xvfb = None
        self._xvfb_count = 0

    def start(self):
        self._xvfb_count += 1
        if not self._xvfb:
            logger.debug("Starting Xvfb.")
            import eol_scons.xvfb
            self._xvfb = eol_scons.xvfb.Xvfb()
            self._xvfb.start()

    def stop(self):
        self._xvfb_count -= 1
        if self._xvfb and self._xvfb_count == 0:
            logger.debug("Stopping Xvfb.")
            self._xvfb.stop()
            self._xvfb = None

_xvfb = XvfbManager()


class NimbusProject(object):

    """
    NIMBUS processing for an entire aircraft project and related output
    comparisons.

    NIMBUS processing requires the following information at a minimum:

    project name
    aircraft name
    projects directory (PROJ_DIR)
    setup files (typically the Production directory)
    input file named in the setup file
    path to nimbus executable
    output directory (by default from the setup files)

    Together these settings make up a "project profile", described by an
    instance of NimbusProject.  If all the setup files are loaded as is
    from a standard Production directory, and if the nimbus executable is
    the one installed on the path, then that encapsulates a normal run for
    nimbus where the output will go to the paths hardcoded in the
    Production setup files.

    Two other profiles can be created.  A "local" profile takes all the
    standard settings above, but adjusts them to use a new output and a
    local nimbus executable.  The output defaults to a subdirectory named
    after the project, and the nimbus executable is taken from the local
    source tree.  Otherwise the settings are the same.

    A "base" profile is a "local" profile using the nimbus installed on the
    system path, with an output directory derived from the "local" output
    directory name.

    The various outputs from two NimbusProject instances can be compared.

    Provide methods to redirect output from production setup files and run
    the NIMBUS processing on the modified files, then compare the output
    with known good output from standard locations.  This class is the main
    implementation for the pnimbus script.

    Construct a NimbusProject instance for the project with name
    *project_name*, aircraft ID *aircraft*, and output directory
    *output_dir*.  The project name is used to derive default standard
    paths to raw data and production setup files.  See also
    setProjectName(), setAircraftName(), and setOutputDirectory().

    The projects directory is set from the PROJ_DIR environment variable by
    default.  It can be overridden by calling setProjectsPath().

    The setup files are loaded from the conventional per-project Production
    directory, based on PROJ_DIR, project, and aircraft, unless it is
    specified explicitly with setSetupPath().

    If a default NimbusProject is created, with no project or aircraft set,
    then they can be set with the methods or by calling applyOptions().

    After configuring the NimbusProject, finish the setup by calling open().
    """

    def __init__(self, project_name=None, aircraft=None, output_dir=None):
        self.project = project_name
        self.aircraft = aircraft
        self.output_dir = output_dir
        self.compare_dir = None
        self.projects = os.environ.get('PROJ_DIR')
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

    operations = ['nimbus', 'reorder', 'compare', 'diff', 'ncdiff',
                  'process2d', 'slices']

    @staticmethod
    def addOptions(parser):
        """
        Add the optparse options by which NimbusProject instances can be
        configured.
        """
        parser.add_option("--dryrun", action="store_true", default=False,
                          help="Load configuration and echo steps but "
                          "do not run any commands.")
        # parser.add_option("--setup", type='string', 
        #                   help="Path to a directory of setup files, "
        #                   "such as the Production directory of a "
        #                   "project configuration.  If the project or "
        #                   "aircraft has not been set yet, then they will be "
        #                   "set by parsing them from the setup path, "
        #                   "assuming the path is a conventional "
        #                   "Production directory tree.")
        # parser.add_option("--project", type='string', 
        #                   help="Specify the project name.  The default "
        #                   "setup path is derived from the project name and "
        #                   "projects directory using the conventional "
        #                   "Production directory path.")
        parser.add_option("--output", type='string', default=None,
                          help="Set output directory for netcdf files "
                          "and modified setup files. Defaults to project name.")
        parser.add_option("--compare", type='string', 
                          help="Look for primary netcdf files in this "
                          "directory, to be compared against the netcdf "
                          "files in the output directory.")
        parser.add_option("--nimbus", type='string', 
                          help="Alternate path to nimbus executable.")

    def applyOptions(self, options):
        """
        Apply the options created by addOptions() and parsed by
        optparse.ParseOptions().
        """
        self.dryrun = options.dryrun
        self.nimbus = options.nimbus
        # if options.project:
        #     self.project = options.project
        self.setOutputDirectory(options.output)
        self.setCompareDirectory(options.compare)
        # if options.setup:
        #     self.setSetupPath(options.setup)

    def setupLocal(self):
        """
        Setup a 'local' configuration for this instance by setting the nimbus
        executable to the path in the local source tree.  If no output
        directory has been set, derive a local for that too.
        """
        if self.output_dir is None:
            self.setOutputDirectory(self.project)
        self.nimbus = self.getLocalNimbusPath()
        logger.debug("Using local nimbus path: " + str(self.nimbus))

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

    def setupBase(self, project):
        """
        Setup a base configuration based on the given NimbusProject.  A base
        configuration derives a different output directory path, but it
        uses the nimbus executable specified in the options, since
        setupLocal() overrides it to be the one in the source tree.
        """
        outdir = self.getOutputDirectory()
        if outdir is None:
            outdir = self.project
        base = os.path.join(outdir, "BASE")
        self.setOutputDirectory(base)
        logger.debug("BASE directory: %s", base)
        project.setBaseProject(self)

    def setProjectsPath(self, projects):
        self.projects = projects

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
        Set the path from which setup files will be loaded.  Parse a project
        name, aircraft name, and projects path from the setup path, and use
        them for any settings not already set.
        """
        logger.debug("setSetupPath(%s)" % (prodpath))
        self.prodpath = prodpath
        dirs = os.path.abspath(prodpath).split("/")
        if not self.aircraft:
            self.aircraft = dirs[-2]
        if not self.project:
            self.project = dirs[-3]
        if not self.projects:
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
        # If setup files already loaded, override their output directory.
        if self.output_dir:
            for setup in self.setups.values():
                setup.setOutputDirectory(self.output_dir)
        logger.debug("Output directory: %s", self.output_dir)

    def getOutputDirectory(self):
        return self.output_dir

    def _dump(self):
        return str({'projects':self.projects, 'project':self.project,
                    'aircraft':self.aircraft, 'dryrun':self.dryrun,
                    'output':self.output_dir})

    def open(self):
        """
        Initialize and validate derived paths and load the setup files for the
        configured project.  After calling open(), getFlights() will return
        the list of flights whose setup files were loaded.
        """
        self.setupDirectories()
        self.loadSetupFiles()

    def setBaseProject(self, base):
        """
        Specify an instance of NimbusProject to be used as the base comparison
        for this instance.  This method checks that the two projects
        actually use different nimbus paths, just as a sanity check, and it
        sets the base project's output directory as the compare directory
        for this project.
        """
        if base:
            # Check that nimbus paths are different.
            bnimbus = base.getNimbusPath()
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
            self.setCompareDirectory(base.getOutputDirectory())

    def close(self):
        """
        Since there is an open() method, this is the corresponding close().
        """
        pass

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

    def makeOutputDirectory(self):
        "Make sure the output directory exists."
        if self.output_dir is None:
            self.setOutputDirectory(self.project)
        if not os.path.exists(self.output_dir):
            logger.debug("creating directory %s...", self.output_dir)
            if not self.dryrun:
                os.mkdir(self.output_dir)

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
        if self.output_dir:
            setup.setOutputDirectory(self.output_dir)
        ifile = setup.resolveInputFile()
        logger.debug("Loaded %s, input file: %s" % (setup.path, ifile))
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
        if self.projects:
            spenv['PROJ_DIR'] = self.projects
        return spenv

    def _runCommand(self, cmd, logpath=None):
        # cmd = ['echo'] + cmd
        spenv = self._getSubEnv()
        exe = which(cmd[0], spenv)
        if exe:
            cmd[0] = os.path.abspath(exe)
        msg = "env PROJ_DIR='%(PROJ_DIR)s' " % spenv
        msg += " ".join(cmd)
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
        ifile = setup.resolveInputFile()
        if not ifile:
            logger.error("Skipping %s, input file could not be resolved: %s" %
                         (setup.path, setup.ifile))
            return
        logger.debug("Running setup file %s, input: %s, output: %s" %
                     (setup.getPath(), ifile, setup.getOutputFile()))
        logger.debug("PROJ_DIR=%s; DISPLAY=%s" % 
                     (os.environ['PROJ_DIR'], os.environ.get('DISPLAY')))
        self.makeOutputDirectory()
        cmd = [self.nimbus or "nimbus", '-b', setup.getPath()]
        if False:
            cmd += ['--loglevel', 'verbose']
        if not self.dryrun:
            setup.writeSetupFile()
            try:
                os.unlink(setup.getOutputFile())
            except OSError:
                pass
        logfile = setup.getPath() + ".log"
        xcode_ = self._runCommand(cmd, logfile)
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
        # Rather than default to the original output, the lack of a
        # compare_dir indicates this project has nothing to compare
        # against.
        cfile = None
        # Redirect source file if comparison directory given.
        if self.compare_dir is not None:
            cfile = os.path.basename(setup.original_ofile)
            cfile = os.path.join(self.compare_dir, cfile)
        return cfile

    def runCompare(self, setup):
        "Run nc_compare between the setup output and the comparison file."
        cfile = self.getComparisonOutputFile(setup)
        if not cfile:
            return
        ofile = setup.getOutputFile()
        cmd = ['nc_compare', '--showindex', cfile, ofile]
        self._runCommand(cmd)

    def runNcDiff(self, setup):
        "Run diff on the ncdump output rather than nc_compare."
        diff = str("diff --side-by-side --width=200 "
                   "--ignore-matching-lines=date_created "
                   "--ignore-matching-lines=ProcessorURL "
                   "--ignore-matching-lines=ProcessorRevision "
                   "--ignore-matching-lines=NIDASrevision "
                   "--ignore-matching-lines=netcdf "
                   "--suppress-common-lines").split()
        cfile = self.getComparisonOutputFile(setup)
        if not cfile:
            return
        ofile = setup.getOutputFile()
        tdir = tempfile.mkdtemp()
        ncdump = "ncdump -p 6".split()
        base = os.path.join(tdir, 'base_'+os.path.basename(cfile)+".txt")
        actual = os.path.join(tdir, 'actual_'+os.path.basename(ofile)+".txt")
        self._runCommand(ncdump + [cfile], base)
        self._runCommand(ncdump + [ofile], actual)
        self._runCommand(diff + [base, actual])
        shutil.rmtree(tdir)

    def runDiff(self, setup):
        "Run diff between the nimbus log files."
        cfile = self.getComparisonOutputFile(setup)
        if not cfile:
            return
        cfile = os.path.dirname(cfile)
        cfile = os.path.join(cfile, os.path.basename(setup.getPath()))
        cfile += ".log"
        ofile = setup.getPath() + ".log"
        # Preprocess the files to remove the nidas log message timestamps,
        # since they interfere with the diff: 2016-04-05,14:30:10|
        base = cfile
        actual = ofile
        pp = False
        tdir = None
        if pp:
            tdir = tempfile.mkdtemp()
            base = os.path.join(tdir, 'base_'+os.path.basename(cfile))
            actual = os.path.join(tdir, 'actual_'+os.path.basename(ofile))
            self._runCommand(['sed', '-e', 's/[-0-9,:][-0-9,-:]*|/|/', cfile],
                             base)
            self._runCommand(['sed', '-e', 's/[-0-9,:][-0-9,-:]*|/|/', ofile],
                             actual)
        cmd = ['diff', '--side-by-side', '--width=300', base, actual]
        self._runCommand(cmd)
        if tdir:
            shutil.rmtree(tdir)

    def runSlices(self, setup):
        # Post-process the log file to isolate slice debugging messages.
        logfile = setup.getPath() + ".log"
        cmd = str("egrep VERBOSE < %s | "
                  "sed -e 's/[-0-9,:][-0-9,-:]*|VERBOSE|//' > %s" %
                  (logfile, logfile.replace("setup", "slices")))
        logger.info(cmd)
        sp.call(cmd, shell=True)

    def _runFlights(self, flights, opfun):
        if not flights:
            flights = self.getFlights()
        for flight in flights:
            opfun(self.setups[flight])

    def reorderFlights(self, flights):
        self._runFlights(flights, self.runReorder)

    def process2dFlights(self, flights):
        self._runFlights(flights, self.runProcess2d)

    def _startXvfb(self):
        if not self.dryrun:
            _xvfb.start()
        
    def _stopXvfb(self):
        if not self.dryrun:
            _xvfb.stop()

    def nimbusFlights(self, flights=None):
        "Run NIMBUS on all the setup files for a project."
        try:
            self._startXvfb()
            self._runFlights(flights, self.runNimbus)
        finally:
            self._stopXvfb()

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

    def sliceFlights(self, flights):
        self._runFlights(flights, self.runSlices)

    def ncdiffFlights(self, flights):
        self._runFlights(flights, self.runNcDiff)

    @staticmethod
    def splitOperations(args):
        "Separate operations from flights in an argument list."
        operations = []
        flights = []
        # Split arguments into operations and flights
        for arg in args:
            if arg in NimbusProject.operations:
                operations.append(arg)
            else:
                flights.append(arg)
        return (operations, flights)

    def selectFlights(self, pattern):
        """
        After loading all the setup files, filter them by matching the flights
        against the pattern.
        """
        flights = self.getFlights()
        flights = fnmatch.filter(flights, pattern)
        setups = {f:self.setups[f] for f in flights}
        self.setups = setups

    def _dispatch(self, operation, flight):
        if operation == "nimbus":
            self.nimbusFlights([flight])
        elif operation == "process2d":
            self.process2dFlights([flight])
        elif operation == "reorder":
            self.reorderFlights([flight])
        elif operation == "compare":
            self.compareFlights([flight])
        elif operation == "diff":
            self.diffFlights([flight])
        elif operation == "slices":
            self.sliceFlights([flight])
        elif operation == "ncdiff":
            self.ncdiffFlights([flight])

    def run(self, operations, flights=None):
        """
        For an open() project, run the given operations for each of the given
        flights.  If flights is empty, then run the operation for all of
        the flights, the list returned by getFlights().
        """
        # If we'll be running nimbus anywhere, start xvfb.
        try:
            if 'nimbus' in operations:
                self._startXvfb()

            # Run the operations for each flight, beginning with base if
            # enabled.
            if not flights:
                flights = self.getFlights()
            for flight in flights:
                for op in operations:
                    self._dispatch(op, flight)
        finally:
            if 'nimbus' in operations:
                self._stopXvfb()


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

# logging.basicConfig(level=logging.DEBUG)

def test_load_setups():
    np = NimbusProject('HIPPO-5', 'GV_N677F')
    np.setOutputDirectory('/tmp')
    np.open()
    assert(len(np.setups) == len(_setup_files))
    _setup_files.sort()
    keys = np.setups.keys()
    keys.sort()
    assert([s.path for s in [np.setups[k] for k in keys]] == 
           ['/tmp/'+s for s in _setup_files])
    np.close()

def test_prodpath_parse():
    np = NimbusProject()
    np.setSetupPath(os.environ['PROJ_DIR'] + "/HIPPO-5/GV_N677F/Production")
    assert(np.aircraft == "GV_N677F")
    assert(np.project == "HIPPO-5")
    assert(np.prodpath ==
           os.environ['PROJ_DIR'] + "/HIPPO-5/GV_N677F/Production")
    assert(np.projects == os.environ['PROJ_DIR'])
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
    assert(ac is None)
