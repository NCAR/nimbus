# -*- python -*-

import os, sys

sys.path.insert(0, os.path.abspath("../caledit/site_scons"))
import site_init

env = Environment(tools=['default', 'jlocal', 'pylint',
                         'testing', 'postgres_testdb'])

sources = Split("""
nagios-qc.py
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

sources = Split("Checks.py NagiosConfig.py nagios-qc.py")

pg = env.PostgresTestDB()

wqc = env.Command('winter-nagios-qc.cfg', ['nagios-qc.py',
                                           'winter-real-time-acserver.sql', 
                                           'winter_vardb.xml',
                                           'winter_checks.xml'] + sources,
                  [ pg.action_run_aircraftdb,
                    "${SOURCE.abspath} --debug "
                    "--db env --checks winter_checks.xml "
                    "--vdb winter_vardb.xml --nagios ${TARGET} config",
                    pg.action_stopdb ], chdir=1)

env.Alias('test', wqc)

