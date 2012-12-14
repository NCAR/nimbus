/// @mainpage Remote Instrument Control (RIC) Overview
/// @section RICMessageHandling RIC Message Handling
///
/// The following sections presents metacode describing the message flow from a control program
/// to the instrument. The code is NOT syntactically correct. A lot of detail is
/// omitted in the interest of clarity.
///
/// The distinct applications are:
/// @ul
/// @li The instruments.
/// @li The aircraft switch. This application combines a switch and embedded proxies. There
/// will be one embedded proxy for each instrument.
/// @li The ground switch.
/// @li The ground proxies. There will be one ground proxy for each remote system and instrument control.
/// @li The instrument control applications.
/// @ul
///
/// @endcode
///
/// @subsection RICInstrumentToControl Message Processing From Instrument to Control
/// @code
///
/// ------------------------------ Instrument ------------------------------------
/// Instrument sends a datagram to the remote instrument control.
///
/// --------------------- Airborne Switch w/ Embedded Proxy ----------------------
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
/// ------------------------------- Ground Switch --------------------------------
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
/// ------------------------------- Remote Proxy ---------------------------------
/// ----------------------- Instrument Control Program ---------------------------
/// Instrument control program receives a datagram from the remote instrument.
///
/// @endcode
///
/// ------------------------------------------------------------------------------
/// @subsection RICControlToInstrument Message Processing From Control to Instrument
/// @code
///
/// ----------------------- Instrument Control Program ---------------------------
/// Instrument control sends a datagram to the remote instrument.
///
/// ------------------------------- Remote Proxy ---------------------------------
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
/// ------------------------------- Ground Switch --------------------------------
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
/// --------------------- Airborne Switch w/ Embedded Proxy ----------------------
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
/// Instrument receives the datagram from the instrument control.
///
/// @endcode
///

