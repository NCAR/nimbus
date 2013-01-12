#ifndef PROXY_H_H
#define PROXY_H_H

#include <QtCore>
#include <QtNetwork>
#include <string>
#include <vector>
#include <map>

#include "Message.h"
#include "SslClientConnection.h"

namespace Ssl {
	/// SslProxy relays messages between an SSL switch and either an instrument or
	/// a user application. The purpose of SslProxy is to provide a secure
	/// communication channel between the local network and the remote switch.
	///
	/// Note that as far as SslProxy is concerned,
	/// and instrument and a user look the same; i.e SslProxy can be configured
	/// to interact with either one. In practice, SslProxy will most commonly
	/// be interfaced with the user, since the instrument will usually be communicating
	/// with a switch configured for EmbeddedProxy connections.
	///
	/// The connection to the switch is managed with a SslClientConnection.
	/// The user will send messages via UDP datagrams to specific ports that
	/// are monitored by SslProxy. These messages are forwarded to the switch. Similarly,
	/// messages received from the switch are sent as datagrams to the user.
	class SslProxy: public QObject {
		Q_OBJECT

	public:
		/// A helper class to manage the handling of message types. One instance
		/// will be created for each message identifier (_msgId). The
		/// instrument name (_proxyID) will be the same for all messages
		/// from a single instrument.
		struct InstMsgInfo {
			/// The instrument name (as used by the SslProxy/Switch system
			std::string _instName;
			/// The message identifier (e.g. "AVAPS")
			std::string _msgId;
			/// Whether this message is delivered via broadcast UDP or unicast UDP.
			bool _broadcast;
			/// The receive port for this message.
			int _incomingPort;
			/// The destination port for this message.
			int _destPort;
			/// The destination name or IP for this message. For broadcast messages,
			/// it is ignored.
			std::string _destIP;
		};

		/// @param proxyID An identifier for the proxy.
		/// @param privateKeyFile Path to the file containing the private key.
		/// Specify a blank string if no key is provided.
		/// @param sslCert The certificate that matches the private key.
		/// Specify a blank string if no certificate is provided.
		/// @param switchSslHost The server host name or IP address.
		/// @param switchSslPort The server port number.
		/// @param extraCerts Extra certs that should be added to the CAdatabase.
		/// @param messageInfo Routing and processing configuration for message types.
		SslProxy(
				std::string proxyID,
				std::string privateKeyFile,
				QSslCertificate sslCert,
				std::string switchSslHost,
				int switchSslPort,
				std::vector<QSslCertificate> extraCerts,
				std::map<std::string, InstMsgInfo> messages);

		/// @param proxyID An identifier for the proxy.
		/// @param messages Routing and processing configuration for message types.
		SslProxy(
				std::string proxyID,
				std::map<std::string, InstMsgInfo> messages);

		/// Destructor
		virtual ~SslProxy();

	public slots:
		/// Connect to the server
		void connectToServer();
		/// Disconnect from the server
		void disconnectFromServer();

	signals:
		/// Emitted when a message is received from the switch
		/// @param s The text of the message.
		/// @param valid True if this was an accepted message.
		void switchMessage(std::string s, bool valid);
		/// Emitted when a message is received from the user
		/// @param s The text of the message.
		/// @param valid True if this was an accepted message.
		void userMessage(std::string s, bool valid);


	protected slots:
		/// Called when a new message has arrived from the instrument or controller
		void udpReadyRead();
		/// Called when there is new data available from the Switch. The message will
		/// be decoded, and the payload will be sent to the recipient as a datagram.
		void msgFromServerSlot(Protocols::Message);

	protected:
		/// Initialize the connection to the switch
		void openSslConnection();
		/// Close the connection to the switch
		void closeSslConnection();
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

		std::string _proxyID;
		/// Path to the private key.
		std::string _keyFile;
		/// Path to the client certificate.
		QSslCertificate _sslCert;
		/// The name or IP of the switch.
		std::string _sslHost;
		/// The switch port number.
		int _sslPort;
		/// Paths to extra certificates to be added to the database.
		std::vector<QSslCertificate> _extraCerts;
		/// The connection to the switch.
		Ssl::SslClientConnection* _sslConnection;
		/// The incoming datagram socket.
		QUdpSocket* _incomingUdpSocket;
		/// The outgoing datagram socket.
		QUdpSocket* _outgoingUdpSocket;
		/// Routing information for message types.
		std::map<std::string, InstMsgInfo> _messages;

	};
};

#endif
