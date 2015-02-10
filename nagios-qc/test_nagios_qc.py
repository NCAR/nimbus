
import os

from Checks import ChecksXML
from Checks import Check
from Checks import Bounds
from vardb import VariableList

_testdir = os.path.join(os.path.dirname(__file__))
_checks_xml = os.path.join(_testdir, "sample_checks.xml")
_deepwave_xml = os.path.join(_testdir, "../vardb/tests/deepwave_vardb.xml")


def samplechecks():
    xml = ChecksXML()
    xml.path = _checks_xml
    xml.parse()
    return xml


def test_checks_xml():
    "Read a checks.xml and make sure it parses and loads correctly."
    xml = samplechecks()
    checks = xml.getChecks()
    assert(len(checks) == 5)
    fl = checks[0]
    assert(fl.ctype == "flatline")
    assert(fl.vname == "PRES")
    assert(fl.name() == "flatline")



def test_check_instances():
    xml = samplechecks()
    checks = xml.getChecks()
    atx = checks[2]

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
