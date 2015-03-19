
from raf.actrack import Config
from raf.actrack import AircraftTrack
from raf.actrack import ACTrackDriver
from raf.actrack import OSM

import json
import os

_netcdffile = os.path.expandvars("$DATA_DIR/HIPPO/HIPPO-2rf06.nc")

def test_open_and_save():
    config = Config()
    print(config.dump())

def test_track():
    track = AircraftTrack()
    driver = ACTrackDriver()
    driver.loadTrackNetCDF(_netcdffile)
    track = driver.getTrack()
    assert(track.npoints() == 27771)


def test_osm():
    driver = ACTrackDriver()
    driver.loadTrackNetCDF(_netcdffile)
    track = driver.getTrack()
    osm = OSM()
    pos = json.loads(osm.formatPositionJSON(track, -1))
    assert(float(pos['head']) == 41.8)
    assert(float(pos['lon']) == 172.529541)

    # Now just check all of the points...
    for i in xrange(0, track.npoints()):
        pos = json.loads(osm.formatPositionJSON(track, i))
        head = float(pos['head'])
        if not (0 <= head < 360.0):
            print("At i=%d, head=%f, json[head]=%s" % 
                  (i, track.thdg()[i], pos['head']))
        assert(0 <= head < 360.0)



        

