// Copyright NCAR 2012
#include "sendudp.h"
#include "../logx/Logging.h"

LOGGING("main");

int main(int argc, char** argv)
{
	logx::ParseLogArgs (argc, argv, true/*skip usage*/);

	QApplication app(argc, argv);

	SendUDP *sender;
        sender = new SendUDP(false);
        sender->show();

	return app.exec();
	return 0;
}

