# -*- mode: python;  -*-

"""
Module for the :py:class:`NimbusSetup` class.
"""

import os
import copy
import StringIO
import re

from calendar import timegm as _timegm
from time import strptime as _strptime

def find_file(idirs, filepath):
    for d in idirs:
        dpath = os.path.expandvars(d)
        for root, dirs_, files_ in os.walk(dpath):
            print("checking for %s under %s" % (filepath, root))
            path = os.path.join(root, filepath)
            if os.path.exists(path):
                print("found %s" % (path))
                return path
    return None


class NimbusSetup(object):
    """
    Encapsulate the NIMBUS setup parameters.

    A NimbusSetup instance contains the parameters which can be specified
    in a NIMBUS setup file.  It has methods to import existing setup files,
    inspect and manipulate parameters, and also export the settings to a
    new setup file.

    NimbusSetup initialization parameters:

    If *path* is not None, then the settings will first be loaded from
    the setup file at that location, and this setup instance will be
    associated with that path.  The other keywords can be set to
    override the settings from the setup file, if any:

    *project*: The ID of the field project, like WINTER.

    *flight*: The ID of the flight, like rf13 or ff01.

    *caseid*: An ID for this setup file, to further distinguish it,
              such as to identify a 'production' run or the 'test' run.

    *ifile*: The path to the ADS raw input file.

    *outputdir*: The output directory path.  When the output directory
                 is set, then the setup output filename is preserved
                 but the directory is automatically redirected to the
                 new output directory.  Likewise a modified setup file
                 can be written out to the output directory rather than
                 replacing the original setup file.
    """

    def __init__(self, path=None, project=None, flight=None, caseid=None,
                 ifile=None, outputdir=None):
        self.ifile = None
        self.ofile = None
        self.outputdir = None
        self.ti = None
        self.project = None
        self.flight = None
        self.caseid = None
        self.path = None
        self.original_ofile = None
        if path:
            self.load(path)
        # After loading a setup file, let the other keywords override it.
        if project:
            self.setProject(project)
        if flight:
            self.flight = flight
        if caseid:
            self.setCaseId(caseid)
        if ifile:
            self.setInputFile(ifile)
        if outputdir:
            self.outputdir = outputdir


    def generateSetupFilename(self):
        """
        Generate a conventional filename for the setup file which includes the
        project, flight, and case ID qualifiers, eg WINTER_rf13_base_setup.
        """
        return self.qualifier() + "_setup"

    def generateOutputFilename(self):
        """
        Generate
        """
        if self.ofile:
            ofile = os.path.basename(self.ofile)
        else:
            ofile = os.path.basename(self.ifile)
            if ofile[-4:] == '.ads':
                ofile = ofile[:-4]
            ofile = ofile + '.nc'
        return self.qualifier() + '_' + ofile

    def qualifier(self):
        """
        Create a qualifier string using the current project, flight, and case
        ID, suitable for a prefix or suffix to make filenames unique.  The
        qualifier will not contain any leading or trailing underscore
        separators; those must be added by the caller if needed.  If none
        of project, flight, or caseid have been set, then the returned
        qualifier string will be empty.
        """
        qname = "_"
        if self.project:
            qname = self.project + '_'
        if self.flight:
            qname = qname + self.flight + '_'
        if self.caseid:
            qname = qname + self.caseid + '_'
        return qname.strip('_')

    def createCase(self, caseid, outputdir=None):
        """
        Create a copy of this setup with a new case ID and redirecting the
        output written to the given output directory.  If *outputdir* is
        None, then the output directory will be whatever is returned by
        getOutputDirectory().
        """
        config = self.clone()
        return config.retargetCase(caseid, outputdir)

    def getOutputDirectory(self):
        """
        Return the output directory for the current setup.

        If the output directory was set explicitly on initialization or by
        calling setOutputDirectory(), then that directory is returned.
        Otherwise the output directory is taken to be the directory path of
        the current output file (*ofile*), and if no *ofile* is sent then
        the directory path of the current input file (*ifile*).
        """
        outputdir = self.outputdir
        if not outputdir:
            if self.ofile:
                outputdir = os.path.dirname(self.ofile)
            else:
                outputdir = os.path.dirname(self.ifile)
        return outputdir

    def retargetCase(self, caseid, outputdir=None):
        self.caseid = caseid
        if outputdir:
            self.outputdir = outputdir
        # Both the setup filename and the output filename will contain the
        # project, flight, and caseid.
        outputdir = self.getOutputDirectory()
        self.ofile = os.path.join(outputdir, self.generateOutputFilename())
        self.path = os.path.join(outputdir, self.generateSetupFilename())
        return self

    def locate(self, project, flight):
        """
        Find the setup file and load it for the given project and flight.

        This method looks for setup files in the standard directories which
        use the conventional setup file names.  The file name convention is
        setup_<flight> under the project's configuration directory, eg
        $PROJ_DIR/WINTER/.
        """
        searchdirs = ['$PROJ_DIR/%(project)s',
                      '/net/jlocal/projects/%(project)s']
        searchdirs = [d % {'project':project} for d in searchdirs]
        filename = "setup_%s" % (flight)
        found = find_file(searchdirs, filename)
        if not found:
            raise Exception("setup file not found for "
                            "project %s and flight %s" %
                            (project, flight))
        self.load(found)
        self.flight = flight
        self.project = project

    def load(self, setupfile):
        "Read and parse a setup file."
        with open(setupfile, "r") as sfile:
            lines = sfile.readlines()
        self.path = setupfile
        self.loadLines(lines)
        self.flight = self.parseFlight(setupfile)
        # Cache the output file whenever a setup file is loaded, so it can be
        # recovered later if the output is redirected.
        self.original_ofile = self.ofile

    def parseFlight(self, setupfile):
        # See if flight can be parsed from the file name.
        filename = os.path.basename(setupfile)
        match = re.search(r'_(rf|tf|ff)(\d\d)$', filename)
        if not match:
            match = re.search(r'_(rf|tf|ff)(\d\d)_', filename)
        flight = None
        if match:
            flight = match.group(1) + match.group(2)
        return flight


    def loadLines(self, lines):
        config = {}
        for line in lines:
            line = line.strip()
            (key, equal_sign, value) = line.partition("=")
            if equal_sign:
                config[key] = value
        self.ifile = config.get('if')
        self.ofile = config.get('of')
        self.ti = config.get('ti')
        return self

    def setProject(self, project):
        self.project = project
        return self

    def setCaseId(self, caseid):
        self.caseid = caseid
        return self

    def resolveInputFile(self):
        """
        Search for and return a new input file path if it does not exist.
        """
        if not self.ifile:
            return None
        if os.path.exists(self.ifile):
            return self.ifile
        rawdatadir = os.environ.get("RAW_DATA_DIR")
        if not rawdatadir:
            return None
        # Break up the path and start looking for it relative to
        # RAW_DATA_DIR.
        ifile = None
        (parent, path) = os.path.split(self.ifile)
        found = False
        while parent and path:
            ifile = os.path.join(rawdatadir, path)
            # print("parent=%s, path=%s, ifile=%s" % (parent, path, ifile))
            found = os.path.exists(ifile)
            if found or parent == "/":
                break
            (parent, ppath) = os.path.split(parent)
            path = os.path.join(ppath, path)
        if found:
            self.ifile = ifile
        else:
            ifile = None
        return ifile

    def setOutputDirectory(self, outdir):
        "Retarget output parameter to write to @p outdir."
        if outdir:
            self.outputdir = outdir
        if self.ofile:
            self.ofile = os.path.basename(self.ofile)
            if outdir:
                self.ofile = os.path.join(outdir, self.ofile)
        if self.path:
            self.path = os.path.basename(self.path)
            if outdir:
                self.path = os.path.join(outdir, self.path)
        return self

    def getOutputFile(self):
        return self.ofile

    def getInputFile(self):
        return self.ifile

    def getTimeWindow(self):
        return self.ti

    def timeFromPtime(self, text, pattern):
        return _timegm(_strptime(text, pattern))

    def intervalToHMS(self, itime):
        itime = int(itime)
        hour = 3600
        minute = 60
        return (itime/hour, (itime/minute) % 60, itime % minute)

    def setTimeWindow(self, tbegin, tend):
        """
        Set the time window begin and end times as absolute epoch times.  This
        method takes care of converting them to the format expected by NIMBUS.
        If either are None then the time window is cleared.
        """
        if not tbegin or not tend:
            self.ti = None
            return
        # We have to know the absolute reference time of the flight, since the
        # time window times are relative to the date of the flight.  See for
        # example: WINTER/C130_N130AR/Production/setup_rf06
        basename = self.getInputFile()
        if not basename:
            raise Exception("Cannot set time window without input file.")
        basename = os.path.basename(basename)
        flightdate = self.timeFromPtime(basename[:8], "%Y%m%d")
        self.ti = "%02d:%02d:%02d-" % self.intervalToHMS(tbegin - flightdate)
        self.ti += "%02d:%02d:%02d" % self.intervalToHMS(tend - flightdate)

    def getPath(self):
        return self.path

    def setPath(self, path):
        self.path = path

    def setInputFile(self, ifile):
        self.ifile = ifile
        return self

    def setOutputFile(self, ofile):
        self.ofile = ofile
        return self

    def __str__(self):
        return self.generateContents()

    def generateContents(self):
        bufr = StringIO.StringIO()
        if self.ifile:
            bufr.write("if=%(ifile)s\n" % (self.__dict__))
        if self.ofile:
            bufr.write("of=%(ofile)s\n" % (self.__dict__))
        if self.ti:
            bufr.write("ti=%(ti)s\n" % (self.__dict__))
        contents = bufr.getvalue()
        bufr.close()
        return contents

    def writeSetupFile(self, dest=None):
        content = self.generateContents()
        if not dest:
            dest = self.getPath()
        # Create the directory if necessary
        if os.path.basename(dest) != dest:
            dpath = os.path.dirname(dest)
            try:
                os.makedirs(dpath)
            except OSError:
                if not os.access(dpath, os.W_OK):
                    raise
        with open(dest, "w") as out:
            out.write(content)
        self.path = dest
        return self

    def clone(self):
        return copy.copy(self)


def test_setup_load():
    config = NimbusSetup()
    assert(config.getInputFile() is None)
    assert(config.getOutputFile() is None)
    assert(config.getTimeWindow() is None)
    config.locate('CONTRAST', 'rf02')
    assert(config.getInputFile() == 
           "/scr/raf_Raw_Data/CONTRAST/20140113_182318_rf02.ads")
    assert(config.getOutputFile() ==
           "/scr/raf_data/CONTRAST/CONTRASTrf02.nc")
    assert(config.getTimeWindow() == "20:07:00-25:13:00")

    filecontents = ""
    with open(config.getPath(), "r") as sf:
        filecontents = sf.read()
    assert(config.generateContents() == filecontents)



def test_setup_files():
    config = NimbusSetup()
    config.setInputFile("/scr/raf/aircraft.ads")
    assert(config.getInputFile() == "/scr/raf/aircraft.ads")
    config.setOutputFile("/scr/raf/data/aircraft.nc")
    assert(config.getOutputFile() == "/scr/raf/data/aircraft.nc")


def test_caseid():
    config = NimbusSetup()
    config.locate('CONTRAST', 'rf02')
    config.setOutputDirectory('CONTRAST')
    pc = config.createCase('prod')
    xc = config.createCase('build')
    assert(pc.getPath() == 'CONTRAST/CONTRAST_rf02_prod_setup')
    assert(xc.getPath() == 'CONTRAST/CONTRAST_rf02_build_setup')
    assert(pc.getOutputFile() == 'CONTRAST/CONTRAST_rf02_prod_CONTRASTrf02.nc')
    assert(xc.getOutputFile() == 'CONTRAST/CONTRAST_rf02_build_CONTRASTrf02.nc')
    # Case with no output directory uses local paths only
    lc = config.createCase('local')
    lc.setOutputDirectory(None)
    assert(lc.getPath() == 'CONTRAST_rf02_local_setup')
    assert(lc.getOutputFile() == 'CONTRAST_rf02_local_CONTRASTrf02.nc')


def test_empty_time():
    config = NimbusSetup()
    config.setInputFile("/scr/raf/aircraft.ads")
    config.setOutputFile("/scr/raf/data/aircraft.nc")
    assert(config.generateContents() == """\
if=/scr/raf/aircraft.ads
of=/scr/raf/data/aircraft.nc
""")


def test_empty_ofile():
    config = NimbusSetup(project="CONTRAST", flight="rf03",
                         ifile="datadir/extract.ads")
    assert(config.ifile == "datadir/extract.ads")
    assert(config.flight == "rf03")
    assert(config.project == "CONTRAST")
    case = config.createCase('test')
    assert(case.ofile == "datadir/CONTRAST_rf03_test_extract.nc")


# WINTER/C130_N130AR/Production/setup_rf06
_winter_rf06 = """if=/scr/raf_Raw_Data/WINTER/20150222_232148_rf06.ads
of=/scr/raf_data/WINTER/WINTERrf06.nc
ti=26:52:00-35:28:00
"""

def test_time_window():
    config = NimbusSetup()
    config.loadLines(_winter_rf06.splitlines())
    assert(config.getTimeWindow() == "26:52:00-35:28:00")
    config.setTimeWindow(None, None)
    assert(config.ti is None)
    tflight = config.timeFromPtime("20150222232148", "%Y%m%d%H%M%S")
    config.setTimeWindow(tflight + 20*60, tflight + 30*60 + 5)
    assert(config.ti == "23:41:48-23:51:53")
    config.setTimeWindow(tflight + 5*3600, tflight + 6*3600 + 150)
    assert(config.ti == "28:21:48-29:24:18")

def test_parse_flight():
    config = NimbusSetup()
    assert(config.parseFlight("setup_rf03") == "rf03")
    assert(config.parseFlight("setup_tf12") == "tf12")
    assert(config.parseFlight("setup_tf1") is None)
    assert(config.parseFlight("setup_ff01_testcase") == "ff01")
    assert(config.parseFlight("tf01_something_else") is None)




