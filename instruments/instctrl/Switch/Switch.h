#ifndef SWITCH_H_H
#define SWITCH_H_H

#include <QtCore>
#include <QtNetwork>
#include <string>
#include <vector>

#include "Server.h"

///
class Switch: public QObject {
	Q_OBJECT
public:
	/// @param keyFile Path to the file containing the private key.
	/// @param certFile Path to the file containing the certificate that matched the private key.
	/// @param switchPort The server port number.
	/// @param caDatabase Paths to certs that should be added to the CAdatabase
	Switch(std::string keyFile,
			std::string certFile,
			int switchPort,
			std::vector<std::string> caDatabase);

	virtual ~Switch();
	
protected slots:

protected:
	/// The server which creates new ServerConnections.
	Server _server;

};

#endif

