#ifndef PROXY_H_
#define PROXY_H_

#include <QtGui>
#include "SslProxy.h"
#include "ui_ProxyMainWindow.h"

class ProxyMainWindow: public QMainWindow, public Ui::ProxyMainWindow {
	Q_OBJECT
public:
	ProxyMainWindow(SslProxy& sslProxy, QWidget* parent = 0);
	virtual ~ProxyMainWindow();

public slots:
	void switchMessageSlot(std::string);
	void userMessageSlot(std::string);

protected slots:
	void connectSlot();


signals:
	void connectToServer();
	void disconnectFromServer();

protected:
	SslProxy& _sslProxy;
	bool _sslConnected;

};



#endif /* PROXY_H_ */
