#include "ProxyMainWindow.h"

/////////////////////////////////////////////////////////////////////
ProxyMainWindow::ProxyMainWindow(SslProxy& sslProxy, QWidget* parent):
QMainWindow(parent),
_sslProxy(sslProxy),
_sslConnected(false),
_windowIcon(QString(":/proxyIcon.png"))
{
	setupUi(this);

	// Set our icon
	setWindowIcon(_windowIcon);

	connect (_connect, SIGNAL(released()), this, SLOT(connectSlot()));
}

/////////////////////////////////////////////////////////////////////
ProxyMainWindow::~ProxyMainWindow() {

}

/////////////////////////////////////////////////////////////////////
void ProxyMainWindow::connectSlot() {

	/// @todo Actually, the connected/disconnected status must be determined
	/// from signals emitted by SslProxy.

	if (!_sslConnected) {
		emit connectToServer();
		_connect->setText("Disconnect");
		_sslConnected = true;
	} else {
		emit disconnectFromServer();
		_connect->setText("Connect");
		_sslConnected = false;
	}
}

/////////////////////////////////////////////////////////////////////
void ProxyMainWindow::switchMessageSlot(std::string s, bool valid){
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

