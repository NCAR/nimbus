# -*- python -*-

import os, sys

sys.path.insert(0, os.path.abspath("../caledit/site_scons"))
import site_init

env = Environment(tools=['default', 'jlocal', 'testing', 'postgres_testdb'])

tests = Split("test_nagios_qc.py")

# Test against the python packages in the source tree
env.AppendENVPath('PYTHONPATH', "#/../python")
env.AppendENVPath('PYTHONPATH', "#/../vardb/python")

runtest = env.TestRun('pytests', tests, "py.test ${SOURCES}")

sources = Split("Checks.py NagiosConfig.py make-qc-config.py")

pg = env.PostgresTestDB()

wqc = env.Command('winter-nagios-qc.cfg', ['make-qc-config.py',
                                           'winter-real-time.sql', 
                                           'winter_vardb.xml',
                                           'winter_checks.xml'] + sources,
                  [ pg.action_run_aircraftdb,
                    "${SOURCE.abspath} --debug "
                    "--db env --checks winter_checks.xml "
                    "--vdb winter_vardb.xml --output ${TARGET}",
                    pg.action_stopdb ], chdir=1)

env.Alias('test', wqc)

