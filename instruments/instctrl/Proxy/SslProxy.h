#ifndef PROXY_H_H
#define PROXY_H_H

#include <QtCore>
#include <QtNetwork>
#include <string>
#include <vector>
#include <map>

#include "Message.h"
#include "SslClientConnection.h"

/// SslProxy relays messages between a switch and either an instrument or
/// an instrument controller. Note that as far as proxy is concerned,
/// and instrument and a instrument controller look the same.
///
/// The connection to the Switch is managed with a SslClientConnection.
/// The instrument/controller will send messages to specific ports, which
/// the proxy listens on. These messages are forwarded to the switch. Similarly,
/// messages from the switch are sent as datagrams to the instrument/controller.
class SslProxy: public QObject {
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

	/// @param incomingUdpPort The incoming udp port
	/// @param privateKeyFile Path to the file containing the private key.
	/// Specify a blank string if no key is provided.
	/// @param certFile Path to the file containing the certificate that matched the private key.
	/// Specify a blank string if no certificate is provided.
	/// @param serverHost The server host name or IP address.
	/// @param switchPort The server port number.
	/// @param caDatabase Paths to certs that should be added to the CAdatabase.
	/// @param instName The instrument name.
	/// @param messageInfo Routing and processing configuration for message types.
	SslProxy(
			int incomingUdpPort,
			std::string privateKeyFile,
			std::string certFile,
			std::string serverHost,
			int switchPort,
			std::vector<std::string> caDatabase,
			std::string instName,
			std::map<std::string, InstMsgInfo> messages);

	virtual ~SslProxy();
	
protected slots:
	/// Called when a new message has arrived from the instrument or controller
	void udpReadyRead();
	/// Called when there is new data available from the Switch. The message will
	/// be decoded, and the payload will be sent to the recipient as a datagram.
	void msgFromServerSlot(Protocols::Message);

protected:
	/// Initialize the connection to the switch
	void initSwitchConnection();
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
	void sendMsg(InstMsgInfo& info, Protocols::Message& msg);

	/// Port number for incoming datagrams
	int _incomingUdpPort;
	/// Path to the private key.
	std::string _keyFile;
	/// Path to the client certificate.
	std::string _certFile;
	/// The name or IP of the switch.
	std::string _serverHost;
	/// The switch port number.
	int _switchPort;
	/// Paths to extra certificates to be added to the database.
	std::vector<std::string> _caDatabase;
	/// The client identifier.
	std::string _instName;
	/// The connection to the switch.
	Ssl::SslClientConnection* _connection;
	/// The incoming datagram socket.
	QUdpSocket* _incomingUdpSocket;
	/// The outgoing datagram socket.
	QUdpSocket* _outgoingUdpSocket;
	/// Routing information for message types.
	std::map<std::string, InstMsgInfo> _messages;

};

#endif
