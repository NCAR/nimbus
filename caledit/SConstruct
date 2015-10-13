# -*- python -*-
##  Copyright 2013 UCAR, NCAR, All Rights Reserved

import eol_scons

from SCons.Errors import StopError

env = Environment(tools = ['default', 'qt4', 'qwt', 'gsl'])

qt4Modules = Split('QtSql QtGui QtCore QtNetwork')
if not env.EnableQt4Modules(qt4Modules):
    raise StopError("qt4 modules could not be enabled.")

if not env.EnableQwt():
    raise StopError("qwt.h not found.  "
                    "Do \"scons --config=force\" to redo the check, "
                    "or set QWTDIR.  "
                    "See config.log for more information")

# Compilation generates numerous warnings relative to the Qt4 code base itself when -Weffc++ is enabled
#env['CXXFLAGS'] = ['-Weffc++','-Wall','-O2' ]
env['CXXFLAGS'] = [ '-Wall','-O2' ]

uis = Split("""
    ViewTextDialog.ui
    CalibrationPlot.ui
    CalibrationForm.ui
""")

env.Uic4(uis)

sources = Split("""
    main.cc
    MainWindow.cc
    CalibrationPlot.cc
    CalibrationForm.cc
    polyfitgsl.cc
    SortFilterProxyModel.cc
    ViewTextDialog.cc
""")

caledit = env.Program('caledit', sources)

inode = env.Install('/opt/local/bin', caledit)
env.Clean('install', inode)

options = env.GlobalOptions()
options.Update(env)
Help(options.GenerateHelpText(env))
