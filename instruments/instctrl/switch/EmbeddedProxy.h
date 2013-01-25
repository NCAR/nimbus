#ifndef EMBEDDEDPROXY_H_
#define EMBEDDEDPROXY_H_

#include <QtCore>
#include <QtNetwork>
#include <map>
#include <string>
#include "Message.h"
#include "SslProxy.h"

using namespace Ssl;

/// An instrument/user proxy which is embedded in a switch. It is used when
/// the proxies and the switch are located on the same machine, and so
/// direct communications can be used between the proxy and the switch.
/// The switch will create one of these for each specified instrument/user.
class EmbeddedProxy: public QObject {
	Q_OBJECT
public:
	/// Constructor.
	/// @param Id The ID for the proxy, usually the name of instrument/user
	/// @param messages Details about each message type that this proxy will handle.
	EmbeddedProxy(std::string Id, std::map<std::string, SslProxy::InstMsgInfo> messages);
	/// destructor.
	virtual ~EmbeddedProxy();
	/// @return the ID of proxy
	std::string Id();
	/// Send a message to the user/instrument
	/// @param The message that is to be sent as a datagram.
	void send(Protocols::Message msg);

signals:
	/// Emitted when the proxy has received a new message from the user/instrument.
	/// @param message The message.
	void msgFromProxy(Protocols::Message message);

protected slots:
	/// Called when there are incoming datagrams available to be read.
	void udpReadyRead();

protected:
	/// Initialize the incoming UDP socket. One socket is required
	/// per port that we are listening on.
	void initIncomingUDPsockets();
	/// Initialize the outgoing UDP socket. One socket can be used to
	/// transmit to any destination port.
	void initOutgoingUDPsocket();
	/// Send a message out to the instrument/controller. The payload text
	/// in the msg will be broadcast.
	/// @param info The specifics of where this message should go.
	/// @param msg The message.
	void sendMsg(SslProxy::InstMsgInfo& info, Protocols::Message& msg);
	/// The proxy ID - usually the name of instrument/user
	std::string _Id;
	/// The messages that this proxy can handle.
	std::map<std::string, SslProxy::InstMsgInfo> _messages;
	/// Port number for incoming datagrams
	int _incomingUdpPort;
	/// The incoming datagram socket.
	QUdpSocket* _incomingUdpSocket;
	/// The outgoing datagram socket.
	QUdpSocket* _outgoingUdpSocket;

};

#endif /* EMBEDDEDPROXY_H_ */
