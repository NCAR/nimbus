
import re
import logging
from Router import RouterStatus

_logger = logging.getLogger(__name__)


_pppoe_status_patterns = [ r"""
	<tr>
		<td><FONT face="Arial, Helvetica, sans-serif"><B>Connection Time</B></FONT></td>
		<td><FONT face="Arial, Helvetica, sans-serif">(?P<connectiontime>\S+)</FONT></td>
	</tr>
""", r"""
	<tr>
		<td><FONT face="Arial, Helvetica, sans-serif"><B>Getting IP address</B></FONT></td>
		<td><FONT face="Arial, Helvetica, sans-serif">(?P<pppoeip>\S+)</FONT></td>
	</tr>
""" ]



_status_page_patterns = [ r"""
                <tr>
                        <td width="60%">
                                <b>Account Name</b> 
                        </td>
                        <td width="40%">
                                (?P<name>\S+)
                        </td>
                </tr>""", r"""
                <tr>
                        <td width="60%">
                                <b>Firmware Version </b> 
                        </td>
                        <td width="40%">
                                (?P<version>\S+)
                        </td>
                </tr>""", r"""
                <tr>
                        <td width="60%">
                                <b>MAC Address </b> 
                        </td>
                        <td width="40%">
                                (?P<wanmac>\S+)
                        </td>
                </tr>""", r"""
                <tr>
                        <td width="60%">
                                <b>IP Address </b> 
                        </td>
                        <td width="40%">
                                (?P<wanip>\S+)
                        </td>
                </tr>""", r"""
                <tr>
                        <td width="60%">
                                <b>DHCP </b> 
                        </td>
                        <td width="40%">
                                (?P<wandhcp>\S+)
                        </td>
                </tr>""", r"""
                <tr>
                        <td width="60%">
                                <b>IP Subnet Mask </b>
                        </td>
                        <td width="40%">
                                (?P<wansubnet>\S+)
                        </td>
                </tr>""", r"""
                <tr>
                        <td width="60%">
                                <b>Domain Name Server</b>
                        </td>
                        <td width="40%">
                                (?P<dns>\S+)
                        </td>
                </tr>""", r"""
		<tr>
			<td width="60%">
				<b>MAC Address </b> 
			</td>
			<td width="40%">
				(?P<lanmac>\S+)
			</td>
		</tr>""", r"""
		<tr>
			<td width="60%">
				<b>IP Address </b> 
			</td>
			<td width="40%">
				(?P<lanip>\S+)
			</td>
		</tr>""", r"""
		<tr>
			<td width="60%">
				<b>DHCP </b> 
			</td>
			<td width="40%">
				(?P<landhcp>\S+)  
			</td>
		</tr>""", r"""
		<tr>
			<td width="60%">
				<b>IP Subnet Mask </b> 
			</td>
			<td width="40%">
				(?P<lansubnet>\S+)
			</td>
		</tr>"""
                      ]


_stats_page_patterns = [ r"""
       <tr> 
 	<td nowrap width="10%" align="center"> <span class="thead">WAN</span> 
 	</td>
 	<td nowrap width="10%" align="center"> <span class="ttext">(?P<wanlink>[\w\s]+)</span> 
        </td>
        <td nowrap width="10%" align="center"> <span class="ttext">(?P<wantxpkts>[\w\s]+)</span> 
        </td>
        <td nowrap width="10%" align="center"> <span class="ttext">(?P<wanrxpkts>[\w\s]+)</span> 
        </td>
        <td nowrap width="10%" align="center"> <span class="ttext">(?P<wancollisions>[\w\s]+)</span> 
 	</td>
 	<td nowrap width="10%" align="center"> <span class="ttext">(?P<wantxbytes>[\w\s]+)</span> 
 	</td>
 	<td nowrap width="10%" align="center"> <span class="ttext">(?P<wanrxbytes>[\w\s]+)</span> 
        </td>
        <td nowrap width="20%" align="center"> <span class="ttext">(?P<wanuptime>[\w\s:]+)</span> 
 	</td>
       </tr>
 	
       <tr> 
	 <td nowrap width="10%" align="center"> <span class="thead">LAN</span> 
	 </td>
 	<td nowrap width="10%" align="center"> <span class="ttext">(?P<lanlink>[\w\s]+)</span> 
        </td>
        <td nowrap width="10%" align="center"> <span class="ttext">(?P<lantxpkts>[\w\s]+)</span> 
        </td>
        <td nowrap width="10%" align="center"> <span class="ttext">(?P<lanrxpkts>[\w\s]+)</span> 
        </td>
        <td nowrap width="10%" align="center"> <span class="ttext">(?P<lancollisions>[\w\s]+)</span> 
 	</td>
 	<td nowrap width="10%" align="center"> <span class="ttext">(?P<lantxbytes>[\w\s]+)</span> 
 	</td>
 	<td nowrap width="10%" align="center"> <span class="ttext">(?P<lanrxbytes>[\w\s]+)</span> 
        </td>
        <td nowrap width="20%" align="center"> <span class="ttext">(?P<lanuptime>[\w\s:]+)</span> 
 
 	 </td>
        </tr>""" ]
                

def _match_regex_template(patterns, text):
    "Given patterns with named groups to match, return a dictionary of values."
    result = {}

    flags = re.MULTILINE | re.DOTALL
    pos = 0
    for pattern in patterns:
        # first replace whitespaces
        rx = re.compile(r'\s+', flags=flags)
        pattern = rx.sub(r'\s+', pattern)
        rx = re.compile(pattern, flags=flags)
        matches = rx.search(text, pos)
        if not matches:
            _logger.error("failed to match: %s" % (pattern))
            raise Exception("failed to match: %s" % (pattern))
        else:
            result.update(matches.groupdict())
            pos = matches.end()
    return result


def parseStatusPage(page):
    return _match_regex_template(_status_page_patterns, page)

def parseStatsPage(page):
    return _match_regex_template(_stats_page_patterns, page)

def parsePPPOE(page):
    return _match_regex_template(_pppoe_status_patterns, page)

def _update_router_status(router, syspage, statspage, pppoepage):
    dstatus = parseStatusPage(syspage)
    dstatus.update(parseStatsPage(statspage))
    dstatus.update(parsePPPOE(pppoepage))
    router.setStatus(RouterStatus(dstatus))

def NetgearStatusHelper(router):
    "Collect status from a netgear router and update the router object."
    _update_router_status(router, 
                          router.fetchPage("sysstatus.html"),
                          router.fetchPage("mtenstatisticTable.html"),
                          router.fetchPage("pppoestatus.html"))

def _NetgearStatusTester(router):
    with open("sysstatus-example.html", "r") as sysfile:
        with open("stats-example.html", "r") as statfile:
            with open("pppoestatus-example.html", "r") as pppoefile:
                _update_router_status(router, sysfile.read(), statfile.read(),
                                      pppoefile.read())
