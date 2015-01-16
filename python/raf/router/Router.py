"Define a class representing a Router on the network."


import urllib2
import time

class RouterStatus(object):

    "Simple collection of router status values."
    def __init__(self, sdict):
        # Really this should establish a fixed set of common status
        # parameters to which other routers map their status info, but for
        # now it just mirrors whatever dictionary the netgear router gives
        # us.
        self.timestamp = time.time()
        for k, v in sdict.items():
            self.__setattr__(k, v)



class Router(object):

    def __init__(self, host=None, port=None, user="admin", pw="password"):
        self.host = host
        self.port = port
        self.user = user
        self.pw = pw
        self.pwm = None
        self.authhandler = None
        self.rooturl = None
        self.opener = None
        self.status = None
        self.status_helper = None
        if (host):
            self.setupHandlers()

    def setStatusHelper(self, status_helper):
        self.status_helper = status_helper

    def setStatus(self, rs):
        self.status = rs

    def getStatus(self):
        return self.status

    def setupHandlers(self):
        self.rooturl = "http://%s" % (self.host)
        if self.port:
            self.rooturl = self.rooturl + ":" + str(self.port)
        self.pwm = urllib2.HTTPPasswordMgrWithDefaultRealm()
        self.pwm.add_password(None, self.rooturl, self.user, self.pw)
        self.authhandler = urllib2.HTTPBasicAuthHandler(self.pwm)
        self.opener = urllib2.build_opener(self.authhandler)

    def updateStatus(self):
        self.status_helper(self)

    def fetchPage(self, path):
        url = "%s/%s" % (self.rooturl, path)
        response = self.opener.open(url)
        text = response.read()
        response.close()
        return text

    _log_status = "WAN %(wanlink)s: IP=%(wanip)s; TX bytes: %(wantxbytes)s; "
    _log_status += "RX bytes: %(wanrxbytes)s; uptime: %(wanuptime)s"

    def getStatusMessage(self):
        "Build a status log message from the current status info."
        return self._log_status % self.status.__dict__




def test_router_init():
    r = Router("192.168.99.1")
    assert(r.host == "192.168.99.1")
    assert(r.port == None)
    assert(r.user == "admin")
    assert(r.pw == "password")
    assert(r.rooturl == "http://192.168.99.1")


def skip_test_router_getpage():
    r = Router("localhost", 8130)
    assert(r.rooturl == "http://localhost:8130")
    page = r.fetchPage("sysstatus.html")
    import netgear
    status = netgear.parseStatusPage(page)
    assert(status['name'] == "RP614v4")
    assert(status['version'] == "V1.0.1_11.29")
    assert(status['wanmac'] == "00:18:4D:D7:6E:6B")
    assert(status['lanmac'] == "00:18:4D:D7:6E:6A")


def test_status_patterns():
    with open("sysstatus-example.html", "r") as pfile:
        page = pfile.read()
    import netgear
    status = netgear.parseStatusPage(page)
    assert(status['name'] == "RP614v4")
    assert(status['version'] == "V1.0.1_11.29")
    assert(status['wanmac'] == "00:18:4D:D7:6E:6B")
    assert(status['lanmac'] == "00:18:4D:D7:6E:6A")
    assert(status['lanip'] == "192.168.99.1")

def test_stats_patterns():
    with open("stats-example.html", "r") as pfile:
        page = pfile.read()
    import netgear
    stats = netgear.parseStatsPage(page)
    assert(stats['lanlink'] == 'Link Up')
    assert(stats['lantxpkts'] == '350')
    assert(stats['lanrxpkts'] == '10734')
    assert(stats['lancollisions'] == '0')
    assert(stats['lantxbytes'] == '12')
    assert(stats['lanrxbytes'] == '164')
    assert(stats['lanuptime'] == '03:27:19')

    assert(stats['wanlink'] == 'Link Up')
    assert(stats['wantxpkts'] == '0')
    assert(stats['wanrxpkts'] == '0')
    assert(stats['wancollisions'] == '0')
    assert(stats['wantxbytes'] == '1')
    assert(stats['wanrxbytes'] == '2')
    assert(stats['wanuptime'] == '01:27:19')

def test_stats_object():
    with open("stats-example.html", "r") as pfile:
        page = pfile.read()
    import netgear
    dstats = netgear.parseStatsPage(page)
    rs = RouterStatus(dstats)
    assert(rs.lanlink == 'Link Up')
    assert(rs.lantxpkts == '350')
    assert(rs.lanrxpkts == '10734')
    assert(rs.lancollisions == '0')
    assert(rs.lantxbytes == '12')
    assert(rs.lanrxbytes == '164')
    assert(rs.lanuptime == '03:27:19')

    assert(rs.wanlink == 'Link Up')
    assert(rs.wantxpkts == '0')
    assert(rs.wanrxpkts == '0')
    assert(rs.wancollisions == '0')
    assert(rs.wantxbytes == '1')
    assert(rs.wanrxbytes == '2')
    assert(rs.wanuptime == '01:27:19')


def skip_test_update_status():
    r = Router("localhost", 8130)
    import netgear
    r.setStatusHelper(netgear.NetgearStatusHelper)
    r.updateStatus()
    rs = r.getStatus()
    assert(rs.name == "RP614v4")
    assert(rs.version == "V1.0.1_11.29")
    assert(rs.wanmac == "00:18:4D:D7:6E:6B")
    assert(rs.lanmac == "00:18:4D:D7:6E:6A")
    assert(rs.lanip == "192.168.99.1")


def test_log_status():
    r = Router("192.168.99.1")
    import netgear
    r.setStatusHelper(netgear._NetgearStatusTester)
    r.updateStatus()
    assert(r.getStatusMessage() == 
           "WAN Link Up: IP=0.0.0.0; TX bytes: 1; RX bytes: 2; uptime: 01:27:19")
