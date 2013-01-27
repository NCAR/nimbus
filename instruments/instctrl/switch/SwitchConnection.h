#ifndef SWITCHCONNECTION_H_
#define SWITCHCONNECTION_H_

#include <QtCore>
#include <QtNetwork>
#include <string>
#include <vector>
#include "Message.h"
#include "SymCipherProtocol.h"
#include "QtAddress.h"

/// The communication connection between two switches. It is bi-directional,
/// exchanging messages with a remote instance of the switch.
///
/// The communications are via encrypted datagrams, implemented with
/// Protocols::SymCipherProtocol.
///
/// Any interface will be used to listen for incoming messages.
class SwitchConnection: public QObject {
	Q_OBJECT
public:
	/// Constructor.
	/// @param localPort The local port for receiving datagrams from the remote switch.
	/// @param remoteHost The IP address or name of the remote switch.
	/// @param remotePort The port number to send datagrams to on the remote switch.
	/// @param switchCipherKey The symmetric encryption key. It must be of
	/// length EVPCipher.cipherBlockLength().
	SwitchConnection(
			int localPort,
			std::string remoteHost,
			int remotePort,
			std::string switchCipherKey) throw(std::string);
	/// Destructor.
	virtual ~SwitchConnection();

signals:
	/// A new message has arrived.
	void msgFromRemoteSwitch(Protocols::Message msg);

protected slots:
	/// datagram(s) are avaialable on _incomingSocket
	void readyRead();

public:
	/// Send a message to the remote switch
	void sendSwitchMessage(std::string msg);

protected:
	/// The socket that receives messages from the remote switch
	QUdpSocket _incomingSocket;
	/// The socket that is used to send outgoing sockets.
	QUdpSocket _outgoingSocket;
	/// The port that we listen for incoming messages on.
	int _localPort;
	/// The remote host that we send messages to.
	std::string _remoteHost;
	/// The remote address that we send messages to.
	QHostAddress _remoteAddress;
	/// The port that we send messages to
	int _remotePort;
	/// The protocol used for message transfers with the remote switch.
	Protocols::SymCipherProtocol _switchToSwitchProtocol;
	/// The file containing the switch udp cipher key
	std::string _switchCipherKey;
};

#endif /* SWITCHCONNECTION_H_ */
