# -*- python -*-

import os, sys

sys.path.insert(0, os.path.abspath("../caledit/site_scons"))
import site_init

env = Environment(tools=['default', 'jlocal', 'pylint',
                         'testing', 'postgres_testdb'])

sources = Split("""
nagiosqc.py
Checks.py
NagiosCommands.py
NagiosChecks.py
NagiosConfig.py
""")

tests = Split("test_nagios_qc.py")

# Test against the python packages in the source tree
env.AppendENVPath('PYTHONPATH', "#/../python")
env.AppendENVPath('PYTHONPATH', "#/../vardb/python")
env.AppendENVPath('PYTHONPATH', env.Dir('.').get_abspath())

env.PythonLint('lint', sources, PYLINTPYTHONPATH=env['ENV']['PYTHONPATH'])

runtest = env.TestRun('pytests', sources + tests, "py.test ${SOURCES}")

sources = Split("Checks.py NagiosConfig.py nagiosqc.py")

pg = env.PostgresTestDB()

wqc = env.Command(['winter-nagios-qc.cfg', 'winter-results.txt'],
                  ['nagiosqc.py', 'WINTER-rf03-real-time-acserver.sql', 
                   'winter_vardb.xml', 'winter_checks.xml'] + sources,
                  [ pg.action_run_aircraftdb,
                    "${SOURCE.abspath} --debug "
                    "--db env --checks winter_checks.xml "
                    "--vdb winter_vardb.xml --nagios ${TARGETS[0]} config",
                    "${SOURCE.abspath} --debug "
                    "--db env --nagios ${TARGETS[0]} "
                    "--commands ${TARGETS[1]} check",
                    pg.action_stopdb ], chdir=1)

env.Alias('test', wqc)

env.Alias('diff', 
          env.Command("diff", ["expected/winter-nagios-qc.cfg", wqc[0] ],
                      "diff -c ${SOURCES}"))
env.Alias('test', 'diff')
env.Alias('cdiff', 
          env.Command("cdiff", ["expected/winter-results.txt", wqc[1] ],
                      "diff -c ${SOURCES}"))
env.Alias('test', 'cdiff')
