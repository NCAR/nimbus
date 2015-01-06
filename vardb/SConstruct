# -*- python -*-

import os
import sys

sys.path.insert(0, os.path.abspath("../caledit/site_scons"))
import site_init

def vardb_global(env):
    "Copy the jlocal settings into the prefixoptions."
    env.Require('jlocal')
    env['DEFAULT_INSTALL_PREFIX'] = "$JLOCAL"
    env.Require('prefixoptions')
    
env = Environment(tools=['default'], GLOBAL_TOOLS=[vardb_global])

SConscript('lib/SConscript')
SConscript('vdbdump/SConscript')
SConscript('vdb2xml/SConscript')
SConscript('vdb2ncml/SConscript')
SConscript('editpy/SConscript')
SConscript('editor/SConscript')
SConscript('tests/SConscript')

env.Alias('apidocs', env.Dir("apidocs"))

variables = env.GlobalVariables()
variables.Update(env)
Help(variables.GenerateHelpText(env))

