#include "ProxyMainWindow.h"

/////////////////////////////////////////////////////////////////////
ProxyMainWindow::ProxyMainWindow(SslProxy& sslProxy, QWidget* parent):
QMainWindow(parent),
_sslProxy(sslProxy),
_proxyState(Ssl::SslProxy::PROXY_Unconnected),
_windowIcon(QString(":/proxyIcon.png")),
_green  (tr("background-color: #00CC66")),
_dgreen (tr("background-color: #00AA88")),
_red    (tr("background-color: #FF3300")),
_yellow (tr("background-color: #EEEE00"))

{
	setupUi(this);

	// Set our icon
	setWindowIcon(_windowIcon);

	// Initial status
	proxyStateChangedSlot(Ssl::SslProxy::PROXY_Unconnected);

	// Handle the control button
	connect (_connect, SIGNAL(released()), this, SLOT(connectSlot()));
}

/////////////////////////////////////////////////////////////////////
ProxyMainWindow::~ProxyMainWindow() {

}

/////////////////////////////////////////////////////////////////////
void ProxyMainWindow::connectSlot() {

	switch (_proxyState) {
	case Ssl::SslProxy::PROXY_Unconnected:
		emit connectToServer();
		break;
	case Ssl::SslProxy::PROXY_Connecting:
		emit disconnectFromServer();
		// Assume that we are disconnected, because if there is
		// no server, we won't get any responses.
		proxyStateChangedSlot(Ssl::SslProxy::PROXY_Unconnected);
		break;
	case Ssl::SslProxy::PROXY_Connected:
		emit disconnectFromServer();
		break;
	default:
		break;
	}

	// clear the status bar
	statusBar()->showMessage("");
}

/////////////////////////////////////////////////////////////////////
void ProxyMainWindow::switchMessageSlot(std::string s, bool valid) {
	if (valid) {
		_validFromSwitch->setText(s.c_str());
	} else {
		_invalidFromSwitch->setText(s.c_str());
	}
}

/////////////////////////////////////////////////////////////////////
void ProxyMainWindow::userMessageSlot(std::string s, bool valid) {
	if (valid) {
		_validFromUser->setText(s.c_str());
	} else {
		_invalidFromUser->setText(s.c_str());
	}
}

/////////////////////////////////////////////////////////////////////
void ProxyMainWindow::proxyStateChangedSlot(Ssl::SslProxy::ProxyState state) {

	switch (state) {
	case Ssl::SslProxy::PROXY_Unconnected:
		_proxyStatus->setText("Unconnected");
		_proxyStatus->setStyleSheet(_red);
		_proxyState = Ssl::SslProxy::PROXY_Unconnected;
		_connect->setText("Connect");
		_connect->setEnabled(true);
		break;

	case Ssl::SslProxy::PROXY_Connecting:
		_proxyStatus->setText("Connecting...");
		_proxyStatus->setStyleSheet(_yellow);
		_connect->setText("Cancel");
		_proxyState = Ssl::SslProxy::PROXY_Connecting;
		_connect->setEnabled(true);
		break;

	case Ssl::SslProxy::PROXY_Connected:
		_proxyStatus->setText("Connected");
		_proxyStatus->setStyleSheet(_green);
		_connect->setText("Disconnect");
		_proxyState = Ssl::SslProxy::PROXY_Connected;
		_connect->setEnabled(true);
		break;

	default:
		break;

	}
}

/////////////////////////////////////////////////////////////////////
void ProxyMainWindow::proxyErrorSlot(QAbstractSocket::SocketError err, std::string errmsg) {

	emit disconnectFromServer();

	_connect->setEnabled(true);

	// Show the error message in the status bar
	statusBar()->showMessage(tr(errmsg.c_str()));

}

