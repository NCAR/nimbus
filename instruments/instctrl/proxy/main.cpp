#include <iostream>
#include <string>
#include <sstream>
#include <boost/program_options.hpp>
#include <QtGui>
#include "svnInfo.h"
#include "ProxyConfig.h"
#include "ProxyMainWindow.h"
#include "SslProxy.h"
#include "RicLogger.h"

namespace po = boost::program_options;

/////////////////////////////////////////////////////////////////////
/// Parse the command line, and return values through the function parameters.
/// @param argc The number of arguments in argv
/// @param argv The command line parameters
/// @param configFile The path to the configuration file is returned here, if specified.
void parseCommandLine(int argc, char** argv, std::string& configFile)
{
  	const char *optarg;
   	bool err = false;

	po::options_description descripts("Options");
	descripts.add_options()
	("config,c", po::value<std::string>(&configFile), "instrument configuration file")
	("help,h",                                        "help")
	;

	po::variables_map vm;
	try
	{
		po::store(po::parse_command_line(argc, argv, descripts), vm);
		po::notify(vm);
	}
	catch (...)
	{
		err++;
	}

	if (vm.count("config")) {
		if (configFile.size() == 0) {
			std::cout << "*** The config parameter requires a file path" << std::endl;
			err++;
		}
	}

	if (vm.count("help") || err) {
		std::cout << descripts << std::endl;
		std::cout << "Edit the configuration file (typically ~/.config/NCAR/Proxy.ini) to set configuration parameters" << std::endl;
		exit(1);
	}
}

/////////////////////////////////////////////////////////////////////
/// Create an SSL proxy and connect it with a user interface. Only
/// one user/instrument is supported.
int main(int argc, char** argv)
{
    try {
        std::string logId = "RICProxy-" + std::string(SVNREVISION);
	RicLogger logger(logId, true);
    	std::string msg("Starting SSL RIC proxy: ");
    	msg += std::string(argv[0]);
    	msg += std::string(" r") + std::string(SVNREVISION);
    	logger.log(msg);

		// Process command line options
		std::string configFile;
		parseCommandLine(argc, argv, configFile);

		// Create the Qt application
		QApplication app(argc, argv);

		// Get the configuration.
		ProxyConfig* config;
		if (configFile.size()) {
			config = new ProxyConfig(configFile);
		} else {
			config = new ProxyConfig("NCAR", "Proxy");
		}

		// Create the SSL proxy. It will wait to connect with the switch,
		// until requested via a call connectToServer().
		SslProxy sslProxy(config);

		// Create the Proxy user interface.
		ProxyMainWindow proxyMainWindow(sslProxy, config->switchHostName(), config->proxyPort(), 0);
		QString title("NCAR/EOL RIC Proxy-");
		title += SVNREVISION;
		proxyMainWindow.setWindowTitle(title);

		// Connect signals from the interface to the SSL proxy.
		proxyMainWindow.connect(&proxyMainWindow, SIGNAL(connectToServer()),
				&sslProxy, SLOT(connectToServer()));

		proxyMainWindow.connect(&proxyMainWindow, SIGNAL(disconnectFromServer()),
				&sslProxy, SLOT(disconnectFromServer()));

		// Connect the signals from the SSL proxy to the interface.
		sslProxy.connect(&sslProxy, SIGNAL(switchMessage(std::string, bool)),
				&proxyMainWindow, SLOT(switchMessageSlot(std::string, bool)));

		sslProxy.connect(&sslProxy, SIGNAL(userMessage(std::string, bool)),
				&proxyMainWindow, SLOT(userMessageSlot(std::string, bool)));

		sslProxy.connect(&sslProxy, SIGNAL(proxyStateChanged(Ssl::SslProxy::ProxyState)),
				&proxyMainWindow, SLOT(proxyStateChangedSlot(Ssl::SslProxy::ProxyState)));

		sslProxy.connect(&sslProxy, SIGNAL(proxyError(QAbstractSocket::SocketError, std::string)),
				&proxyMainWindow, SLOT(proxyErrorSlot(QAbstractSocket::SocketError, std::string)));

		// Show the user interface
		proxyMainWindow.show();

		// Hop on the merry-go-round.
		return app.exec();
    }
    catch (std::string& errmsg) {
		// An error occurred. Pop up an error message box.

        // Create the Qt application
		QApplication app(argc, argv);

		QMessageBox::critical(0, "RIC Proxy Error", errmsg.c_str(),  QMessageBox::Close);
		return 1;
    }
}
