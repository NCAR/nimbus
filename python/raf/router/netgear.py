
import re
import logging

_logger = logging.getLogger(__name__)

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
                                (?P<mac>\S+)
                        </td>
                </tr>""", r"""
                <tr>
                        <td width="60%">
                                <b>IP Address </b> 
                        </td>
                        <td width="40%">
                                (?P<ip>\S+)
                        </td>
                </tr>""", r"""
                <tr>
                        <td width="60%">
                                <b>DHCP </b> 
                        </td>
                        <td width="40%">
                                (?P<dhcp>\S+)
                        </td>
                </tr>""", r"""
                <tr>
                        <td width="60%">
                                <b>IP Subnet Mask </b>
                        </td>
                        <td width="40%">
                                (?P<subnet>\S+)
                        </td>
                </tr>""", r"""
                <tr>
                        <td width="60%">
                                <b>Domain Name Server</b>
                        </td>
                        <td width="40%">
                                (?P<dns>\S+)
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
    for pattern in patterns:
        # first replace whitespaces
        pattern = re.sub(r'\s+', r'\s+', pattern, flags=flags)
        matches = re.search(pattern, text, flags=flags)
        if not matches:
            _logger.error("failed to match: %s" % (pattern))
            raise Exception("failed to match: %s" % (pattern))
        else:
            result.update(matches.groupdict())
    return result


def parseStatusPage(page):
    return _match_regex_template(_status_page_patterns, page)

def parseStatsPage(page):
    return _match_regex_template(_stats_page_patterns, page)

