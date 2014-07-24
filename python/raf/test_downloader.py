
from raf.downloader import Downloader
import iss.time_tools as tt
import iss.tests.conftest
import pytest

_pattern = 'gis.CDPHE_Monitor.%Y%m%d%H*.ozone_obs.kml'

@pytest.fixture
def dld():
    dld = Downloader()
    dld.setFiles('C130_cdphe_ozone', _pattern)
    dld.setFtpDirectory('/pub/incoming/OSM/C130')
    begin = tt.parseTime('20120101')
    end = tt.parseTime('20121231235959')
    dld.setTimeRange(begin, end)
    return dld

def test_downloader(dld):
    assert dld.file_pattern == _pattern
    assert dld.file_type == 'C130_cdphe_ozone'
    assert dld.ftp_dir == '/pub/incoming/OSM/C130'
    assert dld.ftp.host == 'catalog.eol.ucar.edu'
    assert dld.begin == tt.parseTime('20120101')
    assert dld.end == tt.parseTime('20121231235959')

@pytest.mark.slowtest
def test_remote_listing(dld):
    listing = dld.remoteListing()
    assert not listing
    end = tt.parseTime('now')
    begin = end - 24*3600
    dld.setTimeRange(begin, end)
    listing = dld.remoteListing()
    assert listing
    assert listing[-1].path.endswith('ozone_obs.kml')
    for de in listing:
        print(de.path)


