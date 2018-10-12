
import os

from SCons.Script import Builder

import NimbusSetup as ns

# Ignore invalid function names.
# pylint: disable=C0103

def MakeNimbusSetup(env, **kw):
    if kw.has_key('ifile'):
        kw['ifile'] = env.File(kw['ifile']).abspath
    return ns.NimbusSetup(**kw)

# Find raw aircraft files instead of netcdf.  Then generate a setup file
# based on the production file but replacing the if= and of= lines.  Eg,
# read projects/CONTRAST/GV*/Production/setup_rf04, grab the input filename
# but relocate the path if necessary, then change the output path to be a
# local, and run nimbus against that setup file.

# Check if running CONTRAST rf04 this way generates a netcdf file similar
# to the production output.  For that, we must locate the production data
# (eg /scr/raf/Prod_Data/CONTRAST/...)
#
# The data directories on my laptop should be arranged to match the
# hierarchy on EOL systems.
#
# rsync -av barolo:"/scr/raf/Prod_Data/CONTRAST/*rf0[34]*" .
#
# Maybe add a target which syncs the required target data files into a
# subdirectory or some other directory, so they are easily accessible but
# not committed.

# Find aircraft raw data.
#
def FindAircraftRawData(env_, project, filename):
    # Eventually the raw data search path should be environment settings.
    searchdirs = ['$RAW_DATA_DIR/%(project)s',
                  '/scr/raf_Raw_Data/%(project)s']
    idirs = (s % {'project':project} for s in searchdirs)
    return ns.find_file(idirs, filename)


# Find aircraft netcdf output files.
#
def FindAircraftNetCDFData(env_, project, filename):
    searchdirs = ['$DATA_DIR/%(project)s',
                  '/scr/raf_data/%(project)s']
    idirs = (s % {'project':project} for s in searchdirs)
    return ns.find_file(idirs, filename)


def BatchNIMBUS(env, config, valgrind=None, vgsuppressions=None):
    """
    Generate a Command builder to run a NimbusSetup configuration.  If
    valgrind is specified, it should be one of the options recognized by the
    Valgrind() builder. If not specified, then a Command() builder is used
    instead.
    """
    # NIMBUS requires a X11 display connection for motif, but rather than
    # require an existing X server and pass on the DISPLAY setting, use the
    # xvfb tool.
    source = []
    source.append(config.getPath())
    source.append(env['NIMBUS'])
    source.append(config.getInputFile())
    target = [config.getOutputFile()]
    # Echo the PROJ_DIR and DISPLAY settings as a sanity check, but use $$
    # so that scons does not substitute the value from the construction
    # Environment, since really we want to show the actual value in the
    # process environment.
    if valgrind is not None:
        env.LogDebug("Creating NIMBUS builder with valgrind=%s, "
                     "PROJ_DIR=%s" % (valgrind, env['ENV']['PROJ_DIR']))
        if vgsuppressions:
            source.append(vgsuppressions)
        logfile = os.path.join(config.getOutputDirectory(),
                               config.qualifier()+".vg.log")
        output = env.Valgrind(target, source,
                              [env.xvfb_start,
                               "echo PROJ_DIR=$$PROJ_DIR DISPLAY=$$DISPLAY && "
                               "${VALGRIND_COMMAND} "
                               "${NIMBUS} ${NIMBUS_LOGLEVEL} -b ${SOURCE}",
                               env.xvfb_stop],
                              VALGRIND_DEFAULT=valgrind,
                              VALGRIND_LOG=logfile)
    else:
        env.LogDebug("Creating NIMBUS builder with no valgrind, PROJ_DIR=%s"
                     % env['ENV']['PROJ_DIR'])
        output = env.Command(target, source,
                             [env.xvfb_start,
                              "echo PROJ_DIR=$$PROJ_DIR DISPLAY=$$DISPLAY && "
                              "${NIMBUS} ${NIMBUS_LOGLEVEL} -b ${SOURCE}",
                              env.xvfb_stop])
    return output


def CompareNimbusNetCDF(env, ncexpected, ncactual):
    diff = "diff:" + str(ncexpected) + ":" + str(ncactual)
    diff = diff.replace('/', '-')
    return env.Command(diff, [ncexpected, ncactual],
                       "nc_compare --ulps 30 ${SOURCES}")


def DiffNimbusNetCDF(env, ncexpected, ncactual):
    etxt = env.NCDUMP(ncexpected)
    atxt = env.NCDUMP(ncactual)
    diff = "diff:" + str(ncexpected) + ":" + str(ncactual)
    diff = diff.replace('/', '-')
    return env.Command(diff, [etxt, atxt],
                       "diff --side-by-side --width=170 "
                       "--ignore-matching-lines=date_created "
                       "--ignore-matching-lines=ProcessorURL "
                       "--ignore-matching-lines=ProcessorRevision "
                       "--ignore-matching-lines=NIDASrevision "
                       "--ignore-matching-lines=netcdf "
                       "--suppress-common-lines ${SOURCES}")


def NimbusSetupInput(env, project, rfid):
    """Given the project and the name of a NIMBUS setup file, parse it
    into a config dictionary and replace the input file path with a path
    to the same raw file on the local system."""
    config = ns.NimbusSetup()
    config.locate(project, rfid)
    # Grab the raw data filename.
    raw_file = os.path.basename(config.getInputFile())
    raw_path = env.FindAircraftRawData(project, raw_file)
    config.setInputFile(raw_path)
    # The config is now setup for local input, leaving the output to be set
    # by the caller.
    return config


def NimbusSetupOutput(env, path, config):
    """Write the given nimbus config dictionary to path, and set the output
    file to go into the same directory, with a name derived from the input
    file.
    """
    config = config.copy()
    config.setOutputDirectory(os.path.dirname(path))
    env.WriteNimbusSetupFile(config, path)


def NimbusProdENV(env):
    "Setup the environment to run with RAF tools under JLOCAL_PROD."
    env.LogDebug(env.subst("Setting up base environment for "
                           "JLOCAL_PROD=$JLOCAL_PROD"))
    env.PrependENVPath('PATH', env.subst("$JLOCAL_PROD/bin"))
    env.PrependENVPath('LD_LIBRARY_PATH', env.subst("$JLOCAL_PROD/lib"))
    env['NIMBUS'] = env.subst("$NIMBUS_PROD")

def SetProjDirENV(env, projdir=None):
    """
    Propagate a PROJ_DIR setting to the process environment.  By default,
    use the value set by the PROJ_DIR scons variable, otherwise override
    that by specifying projdir.  The specified PROJ_DIR will be converted
    to an absolute path.
    """
    env.LogDebug("SetProjDirENV(%s)" % projdir)
    if not projdir:
        projdir = env['PROJ_DIR']
    projdir = env.Dir(projdir).abspath
    env['ENV']['TMP_PROJ_DIR'] = projdir
    env['ENV']['PROJ_DIR'] = projdir
    env.LogDebug("...PROJ_DIR=%s" % (projdir))


_options = None

def generate(env):
    # JLOCAL_PROD is used to specify a production or baseline installation of
    # RAF tools, especially nimbus.  It must be specified in a scons
    # variable, there is no default.  The path is ignored if it is the same
    # as the JLOCAL install target, otherwise any tests which depend on
    # NIMBUS_PROD would then cause scons to install nimbus over top of the
    # production install.
    # 
    env.Require('xvfb')
    jlocal_prod = None
    global _options
    if not _options:
        projdir = os.environ.get('PROJ_DIR')
        if not projdir:
            projdir = "$JLOCAL/projects"
        _options = env.GlobalVariables()
        _options.Add('JLOCAL_PROD',
                     'Path to production JLOCAL to use for test comparisons',
                     jlocal_prod)
        _options.Add('PROJ_DIR',
                     'Set an explicit value for the PROJ_DIR '
                     'environment variable.',
                     projdir)

    _options.Update(env)
    jlocal_prod = env.get('JLOCAL_PROD')
    if not jlocal_prod:
        print("JLOCAL_PROD not specified.  Baseline comparisons not available.")
    else:
        nimbus_prod = env.File("$JLOCAL_PROD/bin/nimbus")
        env['NIMBUS_PROD'] = nimbus_prod

    # Make sure nc_compare can be found, but prefer the copy built inside
    # this source tree.
    nccmpdir = env.Dir("#/nc_utils/nc_compare").abspath
    env.PrependENVPath('PATH', [nccmpdir, env.subst("$JLOCAL/bin")])

    ncd = Builder(action="ncdump -p 6 ${SOURCE} > ${TARGET}", suffix=".txt")
    env['BUILDERS']['NCDUMP'] = ncd
    env.AddMethod(BatchNIMBUS)
    env.AddMethod(FindAircraftRawData, "FindAircraftRawData")
    env.AddMethod(NimbusSetupInput)
    env.AddMethod(NimbusProdENV)
    env.AddMethod(CompareNimbusNetCDF)
    env.AddMethod(DiffNimbusNetCDF)
    env.AddMethod(SetProjDirENV)
    env.AddMethod(MakeNimbusSetup, "NimbusSetup")

    # Setup the environment with the default PROJ_DIR, but it can be
    # overridden, such as to test against different copies, by calling
    # SetProjDirENV() with a different path.  The default is the value set by
    # the PROJ_DIR scons configuration variable.
    env.SetProjDirENV()


def exists():
    return True

