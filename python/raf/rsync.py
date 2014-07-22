
import subprocess as sp
import os

import logging

logger = logging.getLogger(__name__)


class Rsync(object):
    "Setup Rsync options and then run rsync."

    def __init__(self, source=None, dest=None, options=None):
        self.dryrun = False
        self.verbose = True
        self.options = []
        if options:
            self.options = options
        self.source = source
        self.dest = dest

    def setDryrun(self, enabled):
        self.dryrun = enabled

    def setup(self, source, dest, options=None):
        if options:
            self.options = options
        self.source = source
        self.dest = dest

    def _generate_command(self):
        cmd = ["rsync", "-a"]
        if self.dryrun:
            cmd += ["-n"]
        if self.verbose:
            cmd += ["-v"]
        cmd += options
        cmd += [self.source, self.dest]
        return cmd
        
    def run(self):
        cmd = self._generate_command()
        logger.info(" ".join(cmd))
        p = sp.Popen(cmd, shell=False)
        _pid_, _status_ = os.waitpid(p.pid, 0)


def runrsync(options, source, dest):
    rsync = Rsync(source, dest, options)
    rsync.run()


