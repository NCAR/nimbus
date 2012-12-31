/// @mainpage Remote Instrument Control (RIC) Overview
///
/// @section RICTerminology Terminology
/// - Instrument - The device which is being remotely monitored and controlled. The command and
/// status information is transmitted to and from the instruement using UDP datagrams.
/// - User Control - A user control application, which sends and receives the
/// datagrams from the instrument. Usually referred to as the 'user'.
/// - Proxy - An application or component which can intercept and relay the instrument
/// datagrams, for networking and security purposes.
/// - Switch - An application which is deployed in pairs to consolidate messages into
/// a single communications channel. The switches at either end of the channel connect to
/// proxies, aggregate or fan out messages to/from the instruments and user controls.
///
/// A proxy may be remote, and communicate with the switch via SSL. Alternatively, it may
/// be embedded within a switch. The latter allows a switch and its proxies to be deployed as
/// a single application in cases where SSL is not required; e.g. on the aircraft.
///
/// Because SSL is implemented with network sockets, the switch can contain an SSL server (SslServer),
/// to manage the SSL connections to remote proxies. For consistency, a corresponding server
/// for embedded proxies is implemented as well (EmbeddedProxyServer).
///
/// The typical configuration will have one switch on a ground server, taking SSL connections from
/// remote proxies. This switch will communicate with an aircraft based switch. The aircraft switch
/// contains embedded proxies. However, it is feasible to any combination of remote proxies, SSL
/// proxy switches, and embedded proxy switches.
///
/// @section RICConfiguration Configuration
/// The switch and proxy applications are configured via configuration files. The location
/// of the configuration file can be specified on the command line. If the path is
/// not so specified, it defaults to the standard QSettings location (~/.config on Linux and OSX).
/// It's a bad idea to use the default location. If the configuration file does not exist when
/// the application is first run, a file containing default values will be created. The defaults values
/// are generally useless, and must be edited in order for the application to function properly.
///
/// The configuration file format is based on the Qt QSettings class, which implements a fairly
/// generic .ini format. The format is pretty straightforward, but the QSettings
/// documentation can provide some illumination. The configuration files may be edited by hand,
/// but it is easy to make subtle mistakes. The files are really intended to be manipulated
/// by configuration editor applications.
///
/// @subsection RICSwitchConfig Switch Configuration
/// The switch can be configured in either an SSL proxy mode or an embedded proxy mode,
/// as set byt the SslProxy parameter. There are some parameters, as described below, which are not
/// relevant to a particular switch mode.
/// @code
///[General]
/// SSLProxyPort=20001
/// SwitchSSLKeyFile=../ssl/test/test_certs/server.key
/// SwitchSSLCertFile=../ssl/test/test_certs/server.crt
/// SwitchLocalPort=10001
/// SwitchRemotePort=10002
/// SwitchRemoteIP=127.0.0.1
/// SwitchCipherKey=../ssl/test/test_certs/udpcipher.key
/// SslProxy=true
///
/// [ClientCerts]
/// 1\ClientSSLCertFile=../ssl/test/test_certs/EOL_AVAPS.crt
/// 2\ClientSSLCertFile=../ssl/test/test_certs/EOL_HSRL.crt
/// size=2
///
/// [Instruments]
/// 1\InstrumentFile=./AVAPS.ini
/// 1\InstrumentFile=./HSRL.ini
/// size=2
/// @endcode
/// @param [General]\SSLProxyPort The port that will be used for SSL connections to SSL proxies. This parameter is
/// ignored  when the switch is configured for embedded proxy use (SslProxy=false).
/// @param [General]\SwitchSSLKeyFile The file containing the private key for the switch SSL certificate.  This parameter is
/// ignored  when the switch is configured for embedded proxy use (SslProxy=false).
/// @param [General]\SwitchSSLCertFile The file containing the public SSL certificate for the switch. This will be transmitted to the SSL proxies
/// during the SSL handshake. If the sertificate is self igned, the client must already have the certificate so that
/// the self-signed certificate is accepted.   This parameter is
/// ignored  when the switch is configured for embedded proxy use (SslProxy=false).
/// @param [General]\SwitchLocalPort The local port for the switch-to-switch datagram communication.
/// @param [General]\SwitchRemotePort The remote port for the switch-to-switch datagram communication.
/// @param [General]\SwitchRemoteIP The IP or host name of the remote switch for the switch-to-switch datagram communication.
/// @param [General]\SwitchCipherKey The path to a file containing the symmetric encryption key for the switch-to-switch
/// datagram communication.
/// @param [General]\SslProxy Set to "true" if the switch connects to SSL proxies. Set to false when configured for embedded proxy
/// usage.
/// @param [ClientCerts]\ClientSSLCertFile The path to a file containing an authorized SSL proxy client. There will be one entry for
/// each authorized proxy. This same file will be given to the user who will be running that proxy. A useful
/// convention is to name the certificate file after the user and instrument. This is not
/// used and is ignored in an embedded proxy switch configuration.
/// @param [ClientCerts]\size Size specifies the number of entries in the ClientCerts array.
/// @param [Instruments]\InstrumentFile The path to a file containing an instrument definition.
/// See below. This is not used and is ignored in an SSL proxy switch configuration.
/// @param [Instruments]\size Size specifies the number of entries in the Instruments array.
///
/// @subsection RICProxyConfiguration SSL Proxy Configuration
/// The SSL proxy application configuration file has the following format. Like the embedded proxy switch
/// configuration, it references instrument configuration files. The proxy application is a client to
/// the SSL switch server.
/// @code
/// [General]
/// ProxyID=EOL
/// ProxyKeyFile=../ssl/test/test_certs/client.key
/// ProxyCertFile=../ssl/test/test_certs/client.crt
/// SSLProxyPort=20001
/// SwitchHostName=localhost
/// SwitchSSLCertFile=../ssl/test/test_certs/server.crt
/// InstrumentFile=./AVAPS.ini
/// @endcode
/// @param [General]\ProxyID An identifier for this proxy. It is one of the authentication tokens that
/// enables communication with the switch.
/// @param [General]\ProxyKeyFile The private key for the proxy.
/// @param [General]\ProxyCertFile The public certificate that the proxy presents to the switch.
/// @param [General]\SSLProxyPort The port on the switch used for the SSL communications.
/// @param [General]\SwitchHostName The host name or IP address of the switch.
/// @param [General]\SwitchSSLCertFile The path to a file containing the switch public certificate. The proxy
/// must have a copy of this certificate in order to accept the self-signed certificate from the switch.
/// @param [General]\InstrumentFile The path to a configuration file describing the instrument that this proxy
/// will be representing. See below.
///
/// @subsection RICInstConfiguration Instrument Configuration
/// The instrument configuration defines the instrument and user IP addresses for
/// instrument interaction, as well as the message identifiers and processing
/// parameters. The same instrument definition format is used by the proxy, SSL
/// switch, embedded switch and embedded proxies, although not all parameters
/// are relevant to each application.
/// @code
/// [General]
/// InstName=AVAPS-GV
/// InstIncomingPort=10120
/// InstHostName=127.0.0.1
/// InstDestPort=10121
///
/// [Messages]
/// 1\ID=AVAPS-GV
/// 1\Broadcast=true
/// 1\RateLimit=0
///
/// 2\ID=AVAPS-ENGR-GV
/// 2\Broadcast=false
/// 2\RateLimit=5
///
/// 3\ID=AVAPS-LOG
/// 3\Broadcast=false
/// 3\RateLimit=5
///
/// 4\ID=AGS
/// 4\Broadcast=false
/// 4\RateLimit=1
///
/// size=4
/// @endcode
/// @param [General]\InstName The instrument name.
/// @param [General]\InstIncomingPort The port to listen on for messages sent by the instrument or user. Used only by the proxies.
/// @param [General]\InstHostName The IP or host name of the instrument or user. Instrument and user messages from
/// the switch will be sent to this address. Used only by the proxies.
/// @param [General]\InstDestPort The port number of the instrument or user. Instrument and user messages from the switch will be sent
/// to this port. Used only by the proxies.
/// @param [Messages]\1\ID An allowed message type. The ID will be extracted from the message and verified
/// against the allowed IDs. Used by the proxies and switches.
/// @param [Messages]\1\Broadcast If set, outgoing messages will be broadcast. Used only by the proxies.
/// @param [Messages]\1\RateLimit A rate limit for this message, in seconds. Fractional values are allowed.
/// Only one message will be allowed through the switch during this timer interval. Used only by the switches.
/// @param [Messages]size The number of message definitions in the [Messages] array.
///
/// @section RICMessageHandling RIC Message Handling
///
/// The following sections presents metacode describing the message flow from a user control program
/// to the instrument. The code is NOT syntactically correct. A lot of detail is
/// omitted in the interest of clarity.
///
/// The distinct applications are:
/// - The instruments.
/// - The aircraft switch. This application combines a switch and embedded proxies. There
/// will be one embedded proxy for each instrument.
/// - The ground switch.
/// - The ground proxies. There will be one ground proxy for each instrument and user.
/// - The user control applications. We may refer to this just as the 'user'.
///
/// @subsection RICInstrumentToControl Message Processing From Instrument to User
/// @code
///
/// ------------------------------ Instrument ------------------------------------
/// Instrument sends a datagram to the user.
///
/// ----------------- Airborne Switch w/ Embedded Proxy (./switch) -----------------
/// EmbeddedProxy::udpReadyRead() {
///    QByteArray datagram = QUdpSocket::readDatagram();
///    Protocols::Message msg(datagram.toStdString());
///    std::string s = msg.toJsonStdString();
///    emit EmbeddedProxy::msgFromProxy(s);
/// }
///
/// msgFromProxy(s) signal is delivered to:
/// EmbeddedProxyServer::msgFromProxySlot(std::string msg) {
///    	emit msgFromProxy(msg);
/// }
///
/// msgFromProxy(msg) signal is delivered to:
/// Switch::msgFromProxySlot(std::string msg) {
///    SwitchConnection::sendSwitchMessage(msg) {
///       	std::vector<std::string> outMsgs =
///            Protocols::SymCipherProtocol::outgoing(msg);
///         for m in msgs {
///            QUdpSocket::writeDatagram(char* m.c_str();
///         }
///    }
/// }
/// ------------------------ Ground Switch (./switch) ------------------------------
///
/// Encrypted datagrams are delivered to:
/// SwitchConnection::readyRead() {
///    while datagrams are available {
///       std::vector<char> datagram = QUdpSocket::readDatagram();
///       std::vector<std::string> decrypted =
///          Protocols::SymCipherProtocol::incoming(datagram);
///       for d in decrypted {
///          Protocols::Message msg(d);
///          emit msgFromRemoteSwitch(msg);
///       }
///    }
/// }
///
/// msgFromRemoteSwitch(msg) is delivered to:
/// Switch::msgFromRemoteSwitch(Protocols::Message msg) {
///     SslProxyServer::sendToProxy(msg) {
///        SslServerConnection::send(msg) {
/// 	      std::string m = msg.toJsonStdString();
///           std::vector<std::string> msgs = _protocolToClient.outgoing(m);
///           for m in msgs {
///              std::string m = msgs[i];
///              SslSocket::write(m.c_str());
///           }
///        }
///    }
/// }
///
/// --------------------------- Remote Proxy (./proxy)-------------------------------
///
/// Remote proxy socket write is read by:
/// SslClientConnection::sslReadyRead() {
///   QByteArray data = SslSocket::readAll();
///   std::string s = QString(data).toStdString();
///   std::vector<std::string> msgs =
///      Protocols::StreamMsgProtocol::incoming(s);
///   for m in msgs {
///      Protocols::Message msg(m);
///		 emit SslClientConnection::msgFromServer(msg);
///   }
/// }
///
/// msgFromServer(msg) is delivered to:
/// SslProxy::msgFromServerSlot(Protocols::Message msg) {
///    std::string msgId = msg.msgId();
///    SslProxy::sendMsg(SslProxy::InstMsgInfo[msgId], msg) {
///       	std::string text = msg.payload().text();
///         QUdpSocket.writeDatagram(text);
///    }
/// }
///
/// ---------------------------------- User ---------------------------------------
///
/// User program receives the datagram from the remote instrument.
///
/// @endcode
///
/// --------------------------------------------------------------------------------
/// @subsection RICControlToInstrument Message Processing From User to Instrument
/// @code
///
/// ---------------------------------- User ---------------------------------------
/// User program sends a datagram to the remote instrument.
///
/// --------------------------- Remote Proxy (./proxy) -----------------------------
///
/// SslProxy::udpReadyRead() {
///    QByteArray datagram = QUdpSocket::readDatagram();
///    Protocols::Message msg(datagram.toStdString());
///    SslClientConnection::send(msg) {
///       std::string s = message.toJsonStdString();
///       std::vector<std::string> msgs =
///          StreamMsgProtocol::outgoing(s);
///       for m in msgs {
///          SslSocket::write(char* m.c_str());
///       }
///    }
/// }
/// --------------------------- Ground Switch (./switch) ----------------------------
///
/// Remote proxy socket write is read by:
/// SslServerConnection::sslReadyRead() {
///   QByteArray data = SslSocket::readAll();
///   std::string s = QString(data).toStdString();
///   std::vector<std::string> msgs =
///      Protocols::StreamMsgProtocol::incoming(s);
///   for m in msgs {
///		 emit SslServerConnection::msgFromClient(m);
///   }
/// }
///
/// msgFromClient(m) signal is delivered to:
/// SslProxyServer::msgFromProxySlot(std::string msg) {
///    emit SslProxyServer::msgFromProxy(msg);
/// }
///
/// msgFromProxy(msg) signal is delivered to:
/// Switch::msgFromProxySlot(std::string msg) {
///    SwitchConnection::sendSwitchMessage(msg) {
///       	std::vector<std::string> outMsgs =
///            Protocols::SymCipherProtocol::outgoing(msg);
///         for m in msgs {
///            QUdpSocket::writeDatagram(char* m.c_str();
///         }
///    }
/// }
///
/// ---------------- Airborne Switch w/ Embedded Proxy (./switch) -------------------
///
/// Encrypted datagrams are delivered to:
/// SwitchConnection::readyRead() {
///    while datagrams are available {
///       std::vector<char> datagram = QUdpSocket::readDatagram();
///       std::vector<std::string> decrypted =
///          Protocols::SymCipherProtocol::incoming(datagram);
///       for d in decrypted {
///          Protocols::Message msg(d);
///          emit msgFromRemoteSwitch(msg);
///       }
///    }
/// }
///
/// msgFromRemoteSwitch(msg) is delivered to:
/// Switch::msgFromRemoteSwitch(Protocols::Message msg) {
///     EmbeddedProxyServer::sendToProxy(msg) {
///        EmbeddedProxy::send(Protocols::Message msg) {
///           EmbeddedProxy::sendMsg(msg) {
///              	std::string text = msg.payload().text();
///                 QUdpDatagram::writeDatagram(text.c_str());
///           }
///        }
///     }
/// }
///
/// ------------------------------ Instrument ------------------------------------
///
/// Instrument receives the datagram from the user.
///
/// @endcode
///

