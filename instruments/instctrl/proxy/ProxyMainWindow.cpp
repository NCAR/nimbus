#include "ProxyMainWindow.h"

/////////////////////////////////////////////////////////////////////
ProxyMainWindow::ProxyMainWindow(SslProxy& sslProxy,
		std::string sslHost,
		int sslPort,
		QWidget* parent):
QMainWindow(parent),
_sslProxy(sslProxy),
_sslHost(sslHost),
_sslPort(sslPort),
_proxyState(Ssl::SslProxy::PROXY_Unconnected),
_fullView(true),
_windowIcon(QString(":/proxyIcon.png")),
_green  (tr("background-color: #00CC66")),
_dgreen (tr("background-color: #00AA88")),
_red    (tr("background-color: #FF3300")),
_yellow (tr("background-color: #EEEE00"))
{
	setupUi(this);

	// Set our icon
	setWindowIcon(_windowIcon);

	// Set the view button image
	_view->setIcon(_windowIcon);

	// Display the SSL host specifications
	QString hostInfo = QString("%1:%2").arg(QString(_sslHost.c_str())).arg(_sslPort);
	_sslHostInfo->setText(hostInfo);

	// Set the focus back to the connect button.
	_connect->setFocus();

	// Initial status
	proxyStateChangedSlot(Ssl::SslProxy::PROXY_Unconnected);

	// Handle the control button
	connect (_connect, SIGNAL(released()), this, SLOT(connectSlot()));

	// Handle the view button.
	connect(_view, SIGNAL(released()), this, SLOT(viewSlot()));

	// Handle "Stay On Top" selection
	connect(_onTop, SIGNAL(stateChanged(int)), this, SLOT(stayOnTop()));

	// start the timer for updating the remote heartbeat age display
	_rhbTimerId = startTimer(1000);
	// initialize the remote heartbeat age display
	setHeartbeatAge();
}

/////////////////////////////////////////////////////////////////////
ProxyMainWindow::~ProxyMainWindow()
{
	killTimer(_rhbTimerId);
}

/////////////////////////////////////////////////////////////////////
void ProxyMainWindow::timerEvent(QTimerEvent* event)
{
	if (event->timerId() == _rhbTimerId) {
		// update the remote heartbeat ags display
		setHeartbeatAge();
	}
}

/////////////////////////////////////////////////////////////////////
void ProxyMainWindow::viewSlot()
{
	_fullView = !_fullView;

	if (!_fullView) {
		_width = this->width();
		_height = this->height();
	}

	if (_fullView) {
		_validGroup->show();
		_invalidGroup->show();
		statusBar()->show();
	} else {
		_validGroup->hide();
		_invalidGroup->hide();
		statusBar()->hide();
	}

	// Set the focus back to the connect button.
	_connect->setFocus();

	// The following is used to resize the mainwindow
	// to account for the hidden/shown widgets.
	QWidget *parent;
	_validGroup->parentWidget()->layout()->invalidate();
	parent = _validGroup->parentWidget();
	while (parent) {
		parent->adjustSize();
		parent = parent->parentWidget();
	}

	_invalidGroup->parentWidget()->layout()->invalidate();
	parent = _invalidGroup->parentWidget();
	while (parent) {
		parent->adjustSize();
		parent = parent->parentWidget();
	}

	statusBar()->parentWidget()->layout()->invalidate();
	parent = statusBar()->parentWidget();
	while (parent) {
		parent->adjustSize();
		parent = parent->parentWidget();
	}

	if (_fullView) {
		this->resize(_width, _height);
	}
}

/////////////////////////////////////////////////////////////////////
void ProxyMainWindow::stayOnTop()
{
	Qt::WindowFlags flags = this->windowFlags();
	if (_onTop->isChecked())
		this->setWindowFlags(flags | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
	else
        this->setWindowFlags(flags ^ (Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint));
	this->show();
}

/////////////////////////////////////////////////////////////////////
void ProxyMainWindow::connectSlot()
{
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
void ProxyMainWindow::switchMessageSlot(std::string s, bool valid)
{
	// Get latest remote heartbeat time
	if (s.find("RESPONSE") != std::string::npos) {
		_lastRhbTime = QDateTime::currentDateTime();
		return;
	}

	if (valid) {
		_validFromSwitch->setText(s.c_str());
	} else {
		_invalidFromSwitch->setText(s.c_str());
	}
}

/////////////////////////////////////////////////////////////////////
void ProxyMainWindow::userMessageSlot(std::string s, bool valid)
{
	if (valid) {
		_validFromUser->setText(s.c_str());
	} else {
		_invalidFromUser->setText(s.c_str());
	}
}

/////////////////////////////////////////////////////////////////////
void ProxyMainWindow::proxyStateChangedSlot(Ssl::SslProxy::ProxyState state)
{
	switch (state) {
	case Ssl::SslProxy::PROXY_Unconnected:
		_sslStatus->setText("Unconnected");
		_sslStatus->setStyleSheet(_red);
		_proxyState = Ssl::SslProxy::PROXY_Unconnected;
		_connect->setText("Connect");
		_connect->setEnabled(true);
		break;

	case Ssl::SslProxy::PROXY_Connecting:
		_sslStatus->setText("Connecting...");
		_sslStatus->setStyleSheet(_yellow);
		_connect->setText("Cancel");
		_proxyState = Ssl::SslProxy::PROXY_Connecting;
		_connect->setEnabled(true);
		break;

	case Ssl::SslProxy::PROXY_Connected:
		_sslStatus->setText("Connected");
		_sslStatus->setStyleSheet(_green);
		_connect->setText("Disconnect");
		_proxyState = Ssl::SslProxy::PROXY_Connected;
		_connect->setEnabled(true);
		break;

	default:
		break;
	}
}

/////////////////////////////////////////////////////////////////////
void ProxyMainWindow::proxyErrorSlot(QAbstractSocket::SocketError err, std::string errmsg)
{
	emit disconnectFromServer();

	_connect->setEnabled(true);

	// Show the error message in the status bar
	statusBar()->showMessage(tr(errmsg.c_str()));
}

/////////////////////////////////////////////////////////////////////
void ProxyMainWindow::setHeartbeatAge()
{
	// get the current time
	QDateTime currentTime = QDateTime::currentDateTime();

	QString text("<p>Time Since<br/>Last Remote<br/>Heartbeat<br/>");

	if (_lastRhbTime == QDateTime()) {
		text += "<font color=\"red\">never</font></p>";
	} else {
		int delta = currentTime.toTime_t() - _lastRhbTime.toTime_t();
		if (delta == 0) delta = 1;
		QTime t = QTime().addSecs(delta);
		text += "<font color=\"";

		if (delta < 300) {
			text += "green";
		} else if (delta < 600) {
			text += "blue";
		} else {
			text += "red";
		}
		text += "\">";
		text += t.toString("hh:mm:ss");
		text += "</font></p>";
	}

	_heartbeatAge->setText(text);
}
