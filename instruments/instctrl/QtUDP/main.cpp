// Copyright NCAR 2012
#include "senderreceiver.h"
#include "sslserver.h"
#include "sslclient.h"

int main(int argc, char** argv)
{
	QApplication app(argc, argv);

//	SenderReceiver user;
//	user.show();

	SslServer server;
	server.show();

	SslClient client;
	client.show();

	return app.exec();
	return 0;
}

