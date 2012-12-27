/// @mainpage Remote Instrument Control (RIC) Overview
///
/// @section RICTerminology Terminology
/// @ul
/// @li Instrument - The device which is being remotely monitored and controlled. The command and
/// status information is transmitted to and from the instruement using UDP datagrams.
/// @li User Control - A user control application, which sends and receives the
/// datagrams from the instrument.
/// @li Proxy - An application or component which can intercept and relay the instrument
/// datagrams, for networking and security purposes.
/// @li Switch - An application which is deployed in pairs to consolidate messages into
/// a single communications channel. The switches at either end of the channel connect to
/// proxies, aggregate or fan out messages to/from the instruments and user controls.
/// @el
///
/// A proxy may be remote, and communicate with the switch via SSL. Alternatively, it may
/// be embedded in a switch. The latter allows a a switch and its proxies to be deployed as
/// a single application in cases where SSL is not required.
///
/// Because SSL is implemented with network sockets, the switch can contain an SSL server (SslServer),
/// to manage the SSL connections to remote proxies. For consistency, a corresponding server
/// for embedded proxies is implemented as well (EmbeddedProxyServer).
///
/// The typical configuration will have one switch on a ground server, taking SSL connections from
/// remote proxies. This switch will communicate with an aircraft based switch. The aircraft switch
/// contains embedded proxies.However, it is feasible to any combination of remote proxies, SSL
/// proxy switches, and embedded proxy switches.
///
/// @section RICMessageHandling RIC Message Handling
///
/// The following sections presents metacode describing the message flow from a user control program
/// to the instrument. The code is NOT syntactically correct. A lot of detail is
/// omitted in the interest of clarity.
///
/// The distinct applications are:
/// @ul
/// @li The instruments.
/// @li The aircraft switch. This application combines a switch and embedded proxies. There
/// will be one embedded proxy for each instrument.
/// @li The ground switch.
/// @li The ground proxies. There will be one ground proxy for each instrument and user.
/// @li The user control applications.
/// @el
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

