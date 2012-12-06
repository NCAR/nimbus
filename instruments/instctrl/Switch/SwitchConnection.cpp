#include "SwitchConnection.h"

/////////////////////////////////////////////////////////////////////
SwitchConnection::SwitchConnection(
		int localPort,
		std::string remoteIP,
		int remotePort,
		std::string switchCipherKey):
_localPort(localPort),
_remoteIP(remoteIP),
_remotePort(remotePort),
_switchCipherKey(switchCipherKey),
_switchToSwitchProtocol(EVPCipher::getKey(switchCipherKey), Protocols::SymCipherProtocol::BASE64_CODING)
{
	_incomingSocket.bind(QHostAddress::LocalHost, localPort);

    connect(&_incomingSocket, SIGNAL(readyRead()),
            this, SLOT(readyRead()));

}

/////////////////////////////////////////////////////////////////////
SwitchConnection::~SwitchConnection() {

}

/////////////////////////////////////////////////////////////////////
void SwitchConnection::readyRead() {
    while (_incomingSocket.hasPendingDatagrams()) {

        // Container for the incoming datagram
    	std::vector<char> datagram;
        datagram.resize(_incomingSocket.pendingDatagramSize());

        // Read the datagram
        QHostAddress sender;
         quint16 senderPort;
       _incomingSocket.readDatagram(&datagram[0], datagram.size(),
                                &sender, &senderPort);
       /// @todo Validate that the datagram came from an acceptable sender

        // Decode it
        std::vector<std::string> decoded =
        		_switchToSwitchProtocol.incoming(std::string(datagram.begin(), datagram.end()));

        // Send the message to clients
        for (int i = 0; i < decoded.size(); i++) {
        	Protocols::Message msg(decoded[i]);
        	emit msgFromRemoteSwitch(msg);
        }
    }
}

/////////////////////////////////////////////////////////////////////
void SwitchConnection::sendSwitchMessage(std::string msg) {

	// Encode the message
	std::vector<std::string> outMsgs = _switchToSwitchProtocol.outgoing(msg);

	// Send the encoded message to the remote switch
	for (int i = 0; i < outMsgs.size(); i++) {
		int sent = _outgoingSocket.writeDatagram(
				outMsgs[i].c_str(),
				outMsgs[i].size(),
				QHostAddress(_remoteIP.c_str()),
				_remotePort);

		if (sent != outMsgs[i].size()) {
			qDebug() << "Warning, only" << sent << "bytes out of" << outMsgs[i].size() <<
					"were sent to" << _remoteIP.c_str() << ":" << _remotePort;
			if (sent == -1) {
				qDebug() << "The socket error is:" << _outgoingSocket.error();
			}
		}
	}
}
