#include "ProxyMainWindow.h"

/////////////////////////////////////////////////////////////////////
ProxyMainWindow::ProxyMainWindow(SslProxy& sslProxy, QWidget* parent):
QMainWindow(parent),
_sslProxy(sslProxy),
_sslConnected(false)
{
	setupUi(this);
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
void ProxyMainWindow::switchMessageSlot(std::string s){
	_msgsFromSwitch->setText(s.c_str());
}

/////////////////////////////////////////////////////////////////////
void ProxyMainWindow::userMessageSlot(std::string s) {
	_msgsFromUser->setText(s.c_str());
}

