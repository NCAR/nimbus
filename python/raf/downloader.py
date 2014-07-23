"Utility class to download images and kml files."

import iss.time_tools as tt
import raf.config as config
from raf.datepattern import DatePattern

import logging

logger = logging.getLogger(__name__)

class Downloader(object):

    def __init__(self):
        self.local_dir = '/var/www/html/flight_data/GE/'
        self.file_pattern = None
        self.file_type = None
        self.osm_file_name = None
        self.num_images_to_get = 1
        self.label = None
        self.ftp_dir = None
        self.begin = None
        self.end = None
        self.config = config.Config()
        self.ftp = config.Config().FTP()

    def localListing(self):
        return os.listdir(self.local_dir)

    def setFiles(self, file_type, file_pattern):
        self.file_type = file_type
        self.file_pattern = file_pattern

    def setFtpDirectory(self, ftp_dir):
        self.ftp_dir = ftp_dir

    def setTimeRange(self, begin, end):
        self.begin = begin
        self.end = end

    def remoteListing(self):
        self.ftp.open()
        # Generate the patterns we want to select, then accumulate them all
        # into one listing, sorted lexically assuming that also sorts by
        # time.
        dp = DatePattern(self.file_pattern)
        globs = dp.generate(self.begin, self.end)
        listing = self.ftp.listDirectory(self.ftp_dir, globs=globs)
        logger.info("%d %s files found for time range %s to %s" %
                    (len(listing), self.file_type, 
                     tt.formatTime(self.begin), tt.formatTime(self.end)))
        return listing



def test_downloader():
    logging.basicConfig(level=logging.DEBUG)
    dld = Downloader()
    pattern = 'gis.CDPHE_Monitor.%Y%m%d%H*.ozone_obs.kml'
    dld.setFiles('C130_cdphe_ozone', pattern)
    dld.setFtpDirectory('/pub/incoming/OSM/C130')
    assert dld.file_pattern == pattern
    assert dld.file_type == 'C130_cdphe_ozone'
    assert dld.ftp_dir == '/pub/incoming/OSM/C130'
    assert dld.ftp.host == 'catalog.eol.ucar.edu'
    begin = tt.parseTime('20120101')
    end = tt.parseTime('20121231235959')
    dld.setTimeRange(begin, end)
    assert dld.begin == begin
    assert dld.end == end
    listing = dld.remoteListing()
    assert not listing
    end = tt.parseTime('now')
    begin = end - 4*3600
    dld.setTimeRange(begin, end)
    listing = dld.remoteListing()
    assert listing
    assert listing[-1].path.endswith('ozone_obs.kml')
    for de in listing:
        print(de.path)


