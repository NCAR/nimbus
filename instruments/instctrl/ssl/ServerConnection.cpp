#include "ServerConnection.h"
#include <iostream>

using namespace SSL;

/////////////////////////////////////////////////////////////////////
ServerConnection::ServerConnection(SslSocket* sslSocket):
	_sslSocket(sslSocket),
	_braceCount(0),
	_jsonStarted(false)
{

	// react to changes in the state of the SslSocket
	connect(_sslSocket, SIGNAL(stateChanged(SSL::SslSocket::SocketState)),
			this, SLOT(socketStateChanged(SSL::SslSocket::SocketState)));

	// Handle incoming messages from the SslSocket
	connect(_sslSocket, SIGNAL(readyRead()), this, SLOT(sslReadyRead()));

}

/////////////////////////////////////////////////////////////////////
ServerConnection::~ServerConnection() {
	if (_sslSocket) {
		_sslSocket->close();
		delete _sslSocket;
	}
}

/////////////////////////////////////////////////////////////////////
void ServerConnection::socketStateChanged(SSL::SslSocket::SocketState state) {

	switch (state) {
	case SslSocket::SS_Unconnected: {
		std::cout << "socket is unconnected, what does this mean?" << std::endl;
		break;
	}
	case SslSocket::SS_Connected: {
		std::cout << "socket is connected" << std::endl;
		break;
	}
	case SslSocket::SS_Encrypted: {
		std::cout << "socket is encrypted" << std::endl;
		break;
	}
	case SslSocket::SS_Disconnected: {
		std::cout << "socket is disconnected" << std::endl;
		break;
	}
	case SslSocket::SS_SocketError: {
		std::cout << "socket error" << std::endl;
		break;
	}
	default: {
		std::cout << "socket changed to unknown state:" << state << std::endl;
		break;
	}
	};

	// pass on the socket state change
	emit connectionStateChanged(this, state);
}

/////////////////////////////////////////////////////////////////////
void ServerConnection::sslReadyRead() {

	QByteArray data = _sslSocket->readAll();
	qDebug() << "read" << data.size() << "bytes";
	int n = 0;
	for (int i = 0; i < data.size(); i++) {
		char c = data[i];
		if (c == '{') {
			_braceCount++;
			if (_msgBuf.size() > 0 && _braceCount == 1) {
				qDebug() << "JSON parse error" << _msgBuf.c_str();
				_msgBuf = "";
			}
		} else {
			if (c == '}') {
				_braceCount--;
				if (_msgBuf.size() == 0 && _braceCount < 0) {
					qDebug() << "JSON parse error" << _msgBuf.c_str();
					_msgBuf = "";
				}
			}
		}
		_msgBuf += c;;
		if (_braceCount == 0) {
			qDebug() << _msgBuf.c_str();
			_msgBuf = "";
		}
	}
}
