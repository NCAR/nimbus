// Copyright NCAR 2012
//#include "senderreceiver.h"
//#include "sslserver.h"
//#include "sslclient.h"
//#include "sendudp.h"
#include "proxy.h"
//#include "switch.h"
#include "logx/Logging.h"

LOGGING("main");

int main(int argc, char** argv)
{
	logx::ParseLogArgs (argc, argv, true/*skip usage*/);

	QApplication app(argc, argv);

	//QString serverLog("DatagramLog");
	//logx::LogToFile(serverLog.toStdString());

	Proxy client;
	client.show();

	return app.exec();
	return 0;
}

