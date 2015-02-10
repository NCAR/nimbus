# -*- python -*-

import os, sys

sys.path.insert(0, os.path.abspath("../caledit/site_scons"))
import site_init

env = Environment(tools=['default', 'jlocal', 'testing'])

tests = Split("test_nagios_qc.py")

# Test against the python packages in the source tree
env.AppendENVPath('PYTHONPATH', "#/../python")
env.AppendENVPath('PYTHONPATH', "#/../vardb/python")

runtest = env.TestRun('pytests', tests, "py.test ${SOURCE}")

