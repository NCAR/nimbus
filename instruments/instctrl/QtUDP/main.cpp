// Copyright NCAR 2012
#include "senderreceiver.h"
#include "sslserver.h"
#include "sslclient.h"

int main(int argc, char** argv)
{
	QApplication app(argc, argv);

//	SenderReceiver user;
//	user.show();

	SslClient client;
	client.show();

	SslServer server;
	server.show();

	return app.exec();
	return 0;
}

