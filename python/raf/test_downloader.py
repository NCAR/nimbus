
from raf.downloader import Downloader
import iss.time_tools as tt
import iss.tests.conftest
import raf.config as config
import os
import re
import pytest

_pattern = 'gis.CDPHE_Monitor.%Y%m%d%H*.ozone_obs.kml'

@pytest.fixture
def dld():
    dld = Downloader()
    dld.setFtp(config.Config().FTP())
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
def test_remote_listing(dld, tmpdir):
    listing = dld.remoteListing()
    assert not listing
    end = tt.parseTime('now')
    begin = end - 24*3600
    dld.setTimeRange(begin, end)
    listing = dld.remoteListing()
    assert listing
    assert listing[-1].path.endswith('ozone_obs.kml')
    assert listing[0] <= listing[-1]
    for de in listing:
        print(de.path)
    latest = listing[-1]
    print("downloading %s..." % (latest.path))
    dld.setLocalDirectory(str(tmpdir))
    npath = dld.download(latest.path)
    xpath = tmpdir.join(os.path.basename(latest.path))
    assert npath == str(xpath)
    assert xpath.exists()
    assert xpath.size() > 0

_source = """
<Style>
<IconStyle>
<color>5014F064</color>
<scale>4</scale>
<heading>0</heading>
<Icon>
<href>http://www.nrlmry.navy.mil/TC/kml/TC/images/windbarbs/000/wb_000_320.png</href>
</Icon>
<gx:headingMode>worldNorth</gx:headingMode>
</IconStyle>
</Style>
<description><![CDATA[<p><b>CDPHE Station: 080310027</b></p> Observations on 20140729 at 1908</p> <p>Ozone  ppm </b></p> <br><p> PM2.5  microg/m3</p> <p> Wind Direction: 320.889825</p> deg <p>Wind Speed: 4.59075927 knots</b><br>]]></description>
"""

_transformed = """
<Style>
<IconStyle>
<color>5014F064</color>
<scale>4</scale>
<heading>0</heading>
<Icon>
<href>http://acserver.raf.ucar.edu/flight_data/display/windbarbs/000/wb_000_320.png</href>
</Icon>
<gx:headingMode>worldNorth</gx:headingMode>
</IconStyle>
</Style>
<description><![CDATA[<p><b>CDPHE Station: 080310027</b></p> Observations on 20140729 at 1908</p> <p>Ozone  ppm </b></p> <br><p> PM2.5  microg/m3</p> <p> Wind Direction: 320.889825</p> deg <p>Wind Speed: 4.59075927 knots</b><br>]]></description>
"""


def test_transform(dld, tmpdir):
    spath = tmpdir.join("source.txt")
    spath.write(_source)
    rx = re.compile(r'www\.nrlmry\.navy\.mil/TC/kml/TC/images')
    dpath = tmpdir.join("latest.txt")
    dld.installLatest(str(spath), str(dpath), transform = lambda line: 
                      rx.sub(r'acserver.raf.ucar.edu/flight_data/display',
                             line))
    transformed = dpath.read()
    assert transformed == _transformed

