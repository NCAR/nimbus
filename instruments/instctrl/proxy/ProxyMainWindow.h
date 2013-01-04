#ifndef PROXY_H_
#define PROXY_H_

#include <QtGui>
#include "SslProxy.h"
#include "ui_ProxyMainWindow.h"

using namespace Ssl;

/// The user interface for a user/instrument SslProxy. Status information
/// emitted by SslProxy is captured and displayed, and command actions are
/// transmitted from the user interface to SslProxy.
///
/// @todo Add logic to capture and correctly handle the state changes from SslProxy.
/// Right now, we just assume that when a connect request is issued, it connects
/// correctly. This is just plain wrong.
class ProxyMainWindow: public QMainWindow, public Ui::ProxyMainWindow {
	Q_OBJECT
public:
	/// Constructor
	/// @param sslProxy The sslProxy which is managing the communications
	/// between the user and the switch
	/// @param parent The window parent.
	ProxyMainWindow(SslProxy& sslProxy, QWidget* parent = 0);
	/// Destructor
	virtual ~ProxyMainWindow();

public slots:
	/// Called when a new message is received from the switch. The text
	/// is displayed as the most recent instrument message.
	/// @param s The instrument message text.
	/// @param valid Set true if this is an accepted message, false otherwise.
	void switchMessageSlot(std::string s, bool valid);
	/// Called when a new message is received from the user. The text
	/// is displayed as the most recent user message.
	/// @param s The user message text.
	/// @param valid Set true if this is an accepted message, false otherwise.
	void userMessageSlot(std::string s, bool valid);

protected slots:
	/// Called when the connect/disconnect button is pressed.
	/// It will in emit either the connectToServer() or disconnectFromServer()
	/// signal.
	void connectSlot();


signals:
	/// Emitted to request that the SslProxy connect to the switch.
	void connectToServer();
	/// Emitted to request that the SslProxy disconnect from the switch.
	void disconnectFromServer();

protected:
	/// The SSL proxy that manages the SSL connection to the switch and
	/// the reading and writing of the UDP datagrams.
	SslProxy& _sslProxy;
	/// Set true when the SslProxy is connected.
	bool _sslConnected;

};



#endif /* PROXY_H_ */
