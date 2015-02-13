"NagiosCommands provides an interface for submitting commands to nagios."

import logging

logger = logging.getLogger(__name__)

import time

class NagiosCommands(object):
    """
    The nagios command pipe accepts commands with an epoch timestamp.
    """

    DefaultCommandFile = "/var/spool/nagios/cmd/nagios.cmd"

    PROCESS_SERVICE_CHECK_RESULT = "PROCESS_SERVICE_CHECK_RESULT"
    
    def __init__(self):
        self.path = None
        self.cmds = None

    def setPath(self, path=None):
        if path:
            self.path = path
        if not self.path:
            self.path = NagiosCommands.DefaultCommandFile

    def open(self, path=None):
        if not self.cmds:
            self.setPath(path)
            self.cmds = open(self.path, "w+")

    def close(self):
        if self.cmds:
            self.cmds.close()
            self.cmds = None

    def getTimestamp(self, when=None):
        if not when:
            when = time.time()
        return "%ld" % (when)

    def createCommand(self, command, line, when=None):
        "Assemble the command line with the timestamp."
        cmdline = "[%s] %s;%s"
        return cmdline % (self.getTimestamp(when), command, line)

    def processServiceCheckResult(self, line):
        "Format and submit a PROCESS_SERVICE_CHECK_RESULT command line."
        cmdline = self.createCommand(
            NagiosCommands.PROCESS_SERVICE_CHECK_RESULT, line)
        self.submitCommand(cmdline)

    def submitCommand(self, cmdline):
        """Write the command line @p cmdline to the nagios command pipe.

        If the commands pipe is not already open, it will be opened just
        long enough to submit the command and then closed.  If many
        commands will be submitted in succession, then multiple calls to
        submitCommand() can be bracketed with calls to open() and close().

        """
        closewhendone = bool(not self.cmds)
        self.open()
        logger.debug("nagios command: %s" % (cmdline))
        self.cmds.write(cmdline + "\n")
        if closewhendone:
            self.cmds.close()

