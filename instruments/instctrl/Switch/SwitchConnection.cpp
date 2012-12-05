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
_switchToSwitchProtocol(EVPCipher::makeKey(16), Protocols::SymCipherProtocol::BASE64_CODING)
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
        QByteArray datagram;
        _incomingSocket.pendingDatagramSize();
        QHostAddress sender;
        quint16 senderPort;

        _incomingSocket.readDatagram(datagram.data(), datagram.size(),
                                &sender, &senderPort);

        qDebug() << datagram.size() << "bytes received from" << sender << ":" << senderPort;
    }
}

/////////////////////////////////////////////////////////////////////
void SwitchConnection::sendSwitchMessage(std::string msg) {

	// Encode the message
	std::vector<std::string> outMsgs = _switchToSwitchProtocol.outgoing(msg);

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
