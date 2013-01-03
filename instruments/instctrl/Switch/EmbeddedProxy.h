#ifndef EMBEDDEDPROXY_H_
#define EMBEDDEDPROXY_H_

#include <QtCore>
#include <QtNetwork>
#include <map>
#include <string>
#include "Message.h"
#include "SslProxy.h"

using namespace Ssl;

class EmbeddedProxy: public QObject {
	Q_OBJECT
public:
	/// A helper class to manage the handling of message types. One instance
	/// will be created for each message identifier (_msgId). The
	/// instrument name (_instName) will be the same for all messages
	/// from a single instrument.
	struct InstMsgInfo {
		/// The instrument name (as used by the SslProxy/Switch system
		std::string _instName;
		/// The message identifier (e.g. "AVAPS")
		std::string _msgId;
		/// Whether this message is delivered via broadcast UDP or unicast UDP.
		bool _broadcast;
		/// The destination port for this message.
		int _destPort;
		/// The destination name or IP for this message. For broadcast messages,
		/// it is ignored.
		std::string _destIP;
	};


	EmbeddedProxy(std::map<std::string, SslProxy::InstMsgInfo> messages);
	virtual ~EmbeddedProxy();
	void send(Protocols::Message msg);

signals:
	void msgFromProxy(Protocols::Message message);

protected slots:
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
	std::map<std::string, SslProxy::InstMsgInfo> _messages;
	/// Port number for incoming datagrams
	int _incomingUdpPort;
	/// The incoming datagram socket.
	QUdpSocket* _incomingUdpSocket;
	/// The outgoing datagram socket.
	QUdpSocket* _outgoingUdpSocket;


};

#endif /* EMBEDDEDPROXY_H_ */
