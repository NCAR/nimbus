#include "QtAddress.h"

/////////////////////////////////////////////////////////////////////
QHostAddress QtAddress::address(std::string name) throw (std::string)
{
	// Determine the address of the remote host.
	QHostInfo info = QHostInfo::fromName(name.c_str());
	QList<QHostAddress>	addys = info.addresses();
	if (addys.size() == 0) {
		std::string errmsg("QtAddress: Unable to determine IP address for host: ");
		errmsg += name;
		throw (errmsg);
	}

	// Just use the first address.
	return addys[0];
}
