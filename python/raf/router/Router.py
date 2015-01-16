"Define a class representing a Router on the network."


import urllib2
import netgear

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
        if (host):
            self.setupHandlers()

    def setupHandlers(self):
        self.rooturl = "http://%s" % (self.host)
        if self.port:
            self.rooturl = self.rooturl + ":" + str(self.port)
        self.pwm = urllib2.HTTPPasswordMgrWithDefaultRealm()
        self.pwm.add_password(None, self.rooturl, self.user, self.pw)
        self.authhandler = urllib2.HTTPBasicAuthHandler(self.pwm)
        self.opener = urllib2.build_opener(self.authhandler)

    def fetchPage(self, path):
        url = "%s/%s" % (self.rooturl, path)
        response = self.opener.open(url)
        text = response.read()
        response.close()
        return text



def test_router_init():
    r = Router("192.168.99.1")
    assert(r.host == "192.168.99.1")
    assert(r.port == None)
    assert(r.user == "admin")
    assert(r.pw == "password")
    assert(r.rooturl == "http://192.168.99.1")


def test_router_getpage():
    r = Router("localhost", 8130)
    assert(r.rooturl == "http://localhost:8130")
    page = r.fetchPage("sysstatus.html")
    status = netgear.parseStatusPage(page)
    assert(status['name'] == "RP614v4")
    assert(status['version'] == "V1.0.1_11.29")
    assert(status['mac'] == "00:18:4D:D7:6E:6B")


def test_status_patterns():
    with open("sysstatus-example.html", "r") as pfile:
        page = pfile.read()
    status = netgear.parseStatusPage(page)
    assert(status['name'] == "RP614v4")
    assert(status['version'] == "V1.0.1_11.29")
    assert(status['mac'] == "00:18:4D:D7:6E:6B")

def test_stats_patterns():
    with open("stats-example.html", "r") as pfile:
        page = pfile.read()
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
    assert(stats['wantxbytes'] == '0')
    assert(stats['wanrxbytes'] == '0')
    assert(stats['wanuptime'] == '01:27:19')
