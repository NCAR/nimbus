#include "senderreceiver.h"

int main(int argc, char** argv)
{
	QApplication app(argc, argv);
	SenderReceiver user;
	user.show();
	return app.exec();
	return 0;
}

