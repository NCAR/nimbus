# -*- python -*-
##  Copyright 2013 UCAR, NCAR, All Rights Reserved

import os

env = Environment()
#ENV = {'PATH' : os.environ['PATH']},
    #tools=['default','svninfo'])

env = env.Clone(tools = ['qt4', 'qwt', 'gsl'])

qt4Modules = Split('QtSql QtGui QtCore QtNetwork')
if not env.EnableQt4Modules(qt4Modules):
    Return()

if not env.EnableQwt():
    print "qwt.h not found.  Do \"scons --config=force\" to redo the check. See config.log for more information"
    Return()

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

edit_cal = env.Program('edit_cal', sources,
    LIBS=[env['LIBS']], LIBPATH=[env['LIBPATH']])

name = env.subst("${TARGET.filebase}", target=edit_cal)

inode = env.Install('/opt/local/bin',edit_cal)
env.Clean('install',inode)

options = env.GlobalOptions()
options.Update(env)
Help(options.GenerateHelpText(env))
