"Utilities for creating nagios configuration files and entries."


_preamble = """
# ****************************************************************
# This file is generated automatically.  Do not edit.
# ****************************************************************

define host {
  name                            nagios_qc_default_host
  register                        0
  notifications_enabled           1
  event_handler_enabled           0
  flap_detection_enabled          1
  failure_prediction_enabled      0
  retain_status_information       1
  retain_nonstatus_information    1
  process_perf_data               0
  check_command                   check-mk-host-ping
  check_interval                  1
  check_period                    24X7
  max_check_attempts              1
  notification_interval           0
  notification_period             24X7
  notification_options            d,u,r,f,s
}

# Template used by all other service templates
define service {
  name                            nagios_qc_default
  register                        0
  active_checks_enabled           1
  passive_checks_enabled          1
  parallelize_check               1
  obsess_over_service             1
  check_freshness                 0
  notifications_enabled           1
  event_handler_enabled           0
  flap_detection_enabled          1
  failure_prediction_enabled      1
  process_perf_data               0
  retain_status_information       1
  retain_nonstatus_information    1
  notification_interval           0
  is_volatile                     0
  normal_check_interval           10
  retry_check_interval            10
  max_check_attempts              1
  notification_options            u,c,w,r,f,s
  notification_period             24X7
  check_period                    24X7
}

# This is the service that actively
# calls check_mk. Each host has exactly one service
# like this. Here you can configure, how often
# each host should be checked.
# 
define service {
  name                            nagios_qc_active
  use                             nagios_qc_default
  register                        1
  check_command                   nagios_qc_check
  active_checks_enabled           1
}

# check_mk actually generates a specific command for each passive service check, so it can warn
# about that one specific service check being used as an active check instead of passive.
# We'll just use one command for all the passive checks until we think we need otherwise.

define service {
  name                            nagios_qc_passive
  use                             nagios_qc_default
  check_command                   nagios_qc_check_error
  register                        0
  active_checks_enabled           0
}

define command {
  command_name nagios_qc_check_error
  command_line echo "ERROR - nagios-qc checks must be passive only, not active" && exit 1
}

"""

_active_check_template = """
# Run the nagiosqc script to generate the passive check results.
define command {
  command_name  nagios_qc_check
  command_line  %(nagios_qc_check)s
}
"""


_host_template = """
define host {
   name                    %(host)s
   use                     nagios_qc_default_host
   host_name               %(host)s
   alias                   %(host)s
   address                 0.0.0.0
}
"""

_service_template = """
define service {
   use                     nagios_qc_passive
   name                    %(service)s
   host_name               %(host)s
   service_description     %(service)s
}
"""


class NagiosConfig(object):

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
        self.check = "python /home/local/raf/nagios-qc/nagiosqc check"

    def makeService(self, host, service):
        entry = _service_template % {
            'host':host, 'service':service.lower()
        }
        return entry

    def makeHost(self, host):
        return _host_template % {'host':host}

    def setPath(self, path=None):
        if path:
            self.path = path
        if not self.path:
            self.path = NagiosConfig.DEFAULT_PATH

    def open(self, path=None):
        self.setPath(path)
        self.fp = open(self.path, 'w')
        self.fp.write(_preamble)

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
