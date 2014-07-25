# -*- python -*-

import pytest
import logging

def pytest_addoption(parser):
    parser.addoption("--quick", dest="quick", action="store", default=False,
                     help="only run tests which are not marked slow.")
    parser.addoption("--dlog", dest="loglevel", action="store_const",
                     const=logging.DEBUG, default=None)
    parser.addoption("--ilog", dest="loglevel", action="store_const",
                     const=logging.INFO, default=None)

def pytest_configure(config):
    # register an additional marker
    config.addinivalue_line("markers",
        "slowtest: mark a slow test which should not run in --quick mode")
    if config.option.loglevel:
        logging.basicConfig(level=logging.DEBUG)
    elif config.option.loglevel:
        logging.basicConfig(level=logging.INFO)
    else:
        logging.basicConfig(level=logging.ERROR)

def pytest_runtest_setup(item):
    # pylint: disable=E1101
    if not isinstance(item, pytest.Function):
        return
    if hasattr(item.obj, 'slowtest'):
        if item.config.option.quick:
            pytest.skip("skipping slow test %s" % (str(item)))
    # pylint: enable=E1101
