#include <QtGui>
#include "sslserver.h"

int main(int  argc, char** argv)
{

	QApplication app(argc, argv);

	SslServer server("server.key", "server.crt", 50000);

	return app.exec();

}
