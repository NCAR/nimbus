
import os

from Checks import ChecksXML
from Checks import Check
from Checks import Bounds
from NagiosQC import NagiosQC
from NagiosConfig import NagiosConfig
import optparse
from vardb import VariableList
import numpy

_testdir = os.path.join(os.path.dirname(__file__))
_checks_xml = os.path.join(_testdir, "sample_checks.xml")
_winter_checks_xml = os.path.join(_testdir, "winter_checks.xml")
_winter_xml = os.path.join(_testdir, "winter_vardb.xml")
_deepwave_xml = os.path.join(_testdir, "../vardb/tests/deepwave_vardb.xml")


def samplechecks():
    xml = ChecksXML()
    xml.load(_checks_xml)
    return xml


def winterchecks():
    xml = ChecksXML()
    xml.load(_winter_checks_xml)
    return xml


def test_sample_checks_xml():
    "Read a checks.xml and make sure it parses and loads correctly."
    xml = samplechecks()
    templates = xml.getCheckTemplates()
    assert(len(templates) == 5)
    fl = templates[0]
    assert(fl.ctype == "flatline")
    assert(fl.vname == "PRES")
    assert(fl.name() == "flatline")


def test_winter_checks_xml():
    xml = winterchecks()
    templates = xml.getCheckTemplates()
    assert(len(templates) == 25)
    assert(templates[-1].ctype == "nodata")
    assert(templates[-1].vname == ".*")
    vlist = VariableList()
    vlist.vdbpath = _winter_xml
    variables = vlist.loadVdbFile()
    # Set an explicit missing_value since some checks need it but
    # it is not provided when reading just from a vdb file.
    for var in variables.values():
        if not var.missing_value:
            var.missing_value = -32767
    vlist.setVariables(variables)
    checks = xml.generateChecks(vlist.getVariables())


def test_nagios_config():
    nqc = NagiosQC()
    parser = optparse.OptionParser()
    options = nqc.parseOptions(parser, ['nagiosqc.py', '--db', 'c130', 'config'])
    assert(bool(options))
    assert(options.db == "c130")
    assert(nqc.getCheckCommand("nagiosqc.py") == 
           "python nagiosqc.py check --db c130")


def test_check_instances():
    xml = samplechecks()
    templates = xml.getCheckTemplates()
    atx = templates[2]

    # Setup a variable list using just a local vdb xml file.
    vlist = VariableList()
    vlist.vdbpath = _deepwave_xml
    variables = vlist.loadVdbFile()
    vlist.setVariables(variables)

    ckatx = atx.generate(vlist.getVariables())
    assert(len(ckatx) == 1)
    ckatx = ckatx[0]
    assert(ckatx.min_limit == -80.0)
    assert(ckatx.max_limit == 50.0)
    assert(ckatx.vname == "ATX")

    atx2 = Check.fromString(ckatx.toString())
    assert(atx2.min_limit == -80.0)
    assert(atx2.max_limit == 50.0)
    assert(atx2.vname == "ATX")
    assert(type(atx2) == Bounds)


def test_check_math():
    check = Check("math")
    check.missing_value = -32767
    assert(check.stddev([1,2,3]) == numpy.sqrt(2.0/3.0))
    assert(check.stddev([1,2,-32767]) == 0.5)
    assert(numpy.isnan(check.stddev([-32767,-32767])))
    assert(numpy.isnan(check.stddev([])))

