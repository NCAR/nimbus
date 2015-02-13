"Utilities for creating nagios configuration files and entries."


class NagiosConfig(object):

    _service_template = """
define service {
   use                     generic-radar-service
   host_name               %(host)s
   service_description     %(service)s
}
"""

    _host_template = """
define host {
   use                     generic-radar-host
   host_name               %(host)s
   alias                   %(host)s
   address                 0.0.0.0
}
"""

    # The original default path ('/etc/nagios/raf/config.cfg') was in a
    # RAF-specific subdirectory which had to be created and added to the
    # nagios.cfg file.  Instead the default now is to use the nagios/conf.d
    # directory which already exists and is loaded by nagios by default.

    DEFAULT_PATH = '/etc/nagios/conf.d/raf-qc.cfg'

    def __init__(self):

        # The original path by default was in a RAF-specific subdirectory
        # which had to be created and added to the nagios.cfg file.
        # Instead use the nagios/conf.d directory which already exists and
        # is loaded by nagios by default.
        self.path = None
        self.fp = None

    def makeService(self, host, service):
        entry = NagiosConfig._service_template % {
            'host':host, 'service':service.lower()
        }
        return entry

    def makeHost(self, host):
        return NagiosConfig._host_template % { 'host':host }

    def setPath(self, path=None):
        if path:
            self.path = path
        if not self.path:
            self.path = NagiosConfig.DEFAULT_PATH

    def open(self, path=None):
        self.setPath(path)
        self.fp = open(self.path, 'w')

    def openForReading(self, path=None):
        self.setPath(path)
        self.fp = open(self.path, 'r')

    def iterateLines(self, prefix=None):
        for line in self.fp.readlines():
            if not prefix or line.startswith(prefix):
                yield line

    def write(self, text):
        self.fp.write(text)

    def close(self):
        self.fp.close()
        self.fp = None
