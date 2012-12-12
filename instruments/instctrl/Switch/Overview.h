/// @mainpage Remote Instrument Control (RIC) Overview
/// @section RICMessageHandling RIC Message Handling
///
/// The following presents metacode describing the message flow from a control program
/// to the instrument. The code is NOT syntactically correct. A lot of detail is
/// omitted in the interest of clarity.
///
/// In this configuration, the airborne switch
/// contains an embedded proxy which provides the last hop to the instrument.
/// Each labeled section represents one stand-alone application.
///
/// @code
///
/// ----------------------- Instrument Control Program ---------------------------
/// Instrument control program sends a datagram to the remote proxy.
///
/// ------------------------------- Remote Proxy ---------------------------------
///
/// SslProxy::udpReadyRead() {
///    Protocols::Message msg(datagram);
///    SslClientConnection::send(msg) {
///       std::string s = message.toStdString();
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
/// @endcode
///

