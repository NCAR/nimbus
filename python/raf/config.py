"Global configuration dictionary for RAF python utilities."


import logging

logger = logging.getLogger(__name__)

import raf.rsync as rsync


class Config(object):

    def __init__(self):
        self.loglevel = logging.INFO
        self.dryrun = False

    def parseArgs(self, argv):
        "Consume arguments which modify global configuration."
        i = 0
        while i < len(argv):
            if arg == "--debug":
                self.loglevel = logging.DEBUG
                del argv[i]
            elif arg == "--dryrun":
                self.dryrun = True
                del argv[i]
            else:
                i += 1
        return argv

    def setupLogging(self):
        logging.basicConfig(level=self.loglevel)


    # Try a little dependency inversion and create the service objects from
    # Config factory methods, so the Config has a chance to modify the
    # objects according to the current configuration.

    def Rsync(self, *args):
        target = rsync.Rsync(*args)
        target.setDryrun(self.dryrun)
        return target

    def LDM(self, *args):
        target = ldm.LDM(*args)
        target.setDryrun(self.dryrun)
        return target

