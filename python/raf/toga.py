"Transfer TOGA data directories to the ground as zip files."


import os
import subprocess as sp
import glob
import time

import raf.datepattern as datepattern
import raf.config

import logging

logger = logging.getLogger(__name__)

# rsync the toga data directory from the toga pc.
# look for the second-to-last .D directory.
# if no zip file yet, zip it and insert into ldm.

# GV_N677F 41% rsync -av rsync://toga-pc/toga/'2014-*' .
# receiving incremental file list
# 2014-01-09-1833.b/
# 2014-01-09-1833.b/2014-01-09-1833_002.D/
# 2014-01-09-1833.b/2014-01-09-1833_002.D/Audit.txt
# 2014-01-09-1833.b/2014-01-09-1833_002.D/CONTRAST_03.RES
# 2014-01-09-1833.b/2014-01-09-1833_002.D/CONTRAST_03.XLS
# 2014-01-09-1833.b/2014-01-09-1833_002.D/DATA.MS
# 2014-01-09-1833.b/2014-01-09-1833_002.D/PRE_POST.INI
# 2014-01-09-1833.b/2014-01-09-1833_002.D/acqmeth.txt
# 2014-01-09-1833.b/2014-01-09-1833_002.D/fileinfo.txt
# 2014-01-09-1833.b/2014-01-09-1833_002.D/fooport2.txt
# 2014-01-09-1833.b/2014-01-09-1833_003.D/
# 2014-01-09-1833.b/2014-01-09-1833_003.D/TEMPBASE
# 2014-01-09-1833.b/2014-01-09-1833_003.D/TEMPDAT
# 2014-01-09-1833.b/2014-01-09-1833_003.D/TEMPDIR
# 2014-01-09-1833.b/2014-01-09-1833_003.D/runstart.txt
# 2014-01-09-1833.b/CONTRAST_03.M/LastData.mac
# 2014-01-09-1833.b/CONTRAST_03.M/QDB.BAK
# 2014-01-09-1833.b/CONTRAST_03.M/tempevt.mac
# 
# sent 1186 bytes  received 425924 bytes  170844.00 bytes/sec
# total size is 114901475  speedup is 269.02

def zipfile_from_folder(path):
    cwd = os.path.abspath(os.path.dirname(path))
    if not cwd:
        raise Exception("path does not exist: " + path)
    base = os.path.basename(path)
    zipfile = os.path.join(cwd, 'TOGA_'+base+'.zip')
    return zipfile

def zip_toga_folder(path):
    # Run in the parent directory of the .D data folder
    cwd = os.path.abspath(os.path.dirname(path))
    base = os.path.basename(path)
    zipfile = zipfile_from_folder(path)
    args = ['zip', '-rv', zipfile, base]
    p = sp.Popen(args, shell=False, cwd=cwd)
    _pid_, _status_ = os.waitpid(p.pid, 0)

def find_latest_dir(togatop):
    dpath = None
    dirs = glob.glob(togatop+"/*/*.D")
    dirs.sort()
    if len(dirs) > 1:
        dpath = dirs[-2]
        logging.info("latest finished directory: %s" % (dpath))
    return dpath


def main(argv):
    config = raf.config.Config()
    config.parseArgs(argv)
    config.setupLogging()
    togatop = "/mnt/r1/toga"
    togadirpattern = "%Y-%m-%d-%H*.b"
    now = time.time()
    dp = datepattern.DatePattern(togadirpattern)
    options = dp.generateRsyncRules(now - 8*3600, now)
    rsync = config.Rsync("rsync://toga-pc/toga/", togatop, options)
    rsync.run()
    dpath = find_latest_dir(togatop)
    if not dpath:
        logging.info("No TOGA data directories found in " + togatop)
        return 1
    zipfile = zipfile_from_folder(dpath)
    if os.path.exists(zipfile):
        logging.info("zipfile already exists: %s" % (zipfile))
    else:
        logging.info("zipping toga folder: %s" % (dpath))
        zip_toga_folder(dpath)
        ldm = config.LDM()
        ldm.insert(zipfile)
    return 0
