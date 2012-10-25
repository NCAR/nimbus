#include <QtGui>
#include "sslsocket.h"

int main(int  argc, char** argv)
{

	QApplication app(argc, argv);

	SslSocket client("client.key", "client.crt", "localhost", 50000);

	return app.exec();

}



