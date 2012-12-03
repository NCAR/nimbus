#ifndef SWITCHCONNECTION_H_
#define SWITCHCONNECTION_H_

#include <QtCore>
#include <QtNetwork>
#include "Message.h"
#include "SymCipherProtocol.h"

/// The communication connection between two switches. It is bi-directional,
/// exchanging messages with a remote instance of the switch.
///
/// The communications are via encrypted datagrams between Switch instances.
///
/// Any interface will be listened on for incoming messages.
class SwitchConnection: public QObject {
	Q_OBJECT
public:
	SwitchConnection(
			int localPort,
			std::string remoteIP,
			int remotePort,
			std::string switchCipherKey);
	virtual ~SwitchConnection();

signals:
	/// A new message has arrived.
	void newSwitchMessage(Protocols::Message msg);

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
	/// The remote IP that we send messages to
	std::string _remoteIP;
	/// The port that we send messages to
	int _remotePort;
	/// The protocol used for message transfers with the remote switch.
	Protocols::SymCipherProtocol _switchToSwitchProtocol;
	/// The file containing the switch udp cipher key
	std::string _switchCipherKey;
};

#endif /* SWITCHCONNECTION_H_ */
