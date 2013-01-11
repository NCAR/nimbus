#include <iostream>
#include <string>
#include <sstream>
#include <boost/program_options.hpp>
#include <QtGui>
#include "svnInfo.h"
#include "QtConfig.h"
#include "InstConfig.h"
#include "ProxyMainWindow.h"
#include "SslProxy.h"

namespace po = boost::program_options;

/////////////////////////////////////////////////////////////////////
/// Parse the command line, and return values through the function parameters.
/// @param argc The number of arguments in argv
/// @param argv The command line parameters
/// @param configFile The path to the configuration file is returned here, if specified.
void
parseCommandLine(int argc, char** argv,
	std::string& configFile)
{
  	const char *optarg;
   	bool err = false;

	po::options_description descripts("Options");
	descripts.add_options()
	("config,c", po::value<std::string>(&configFile)->implicit_value(""), "instrument configuration file")
	("help,h",                                                            "help")
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
	// Process command line options
	std::string configFile;
	parseCommandLine(argc, argv, configFile);

	// Get the configuration. Fetch from the specified file
	// if it was provided, otherwise use the default location.
	QtConfig* config;
	if (configFile.size()) {
		config = new QtConfig(configFile);
	} else {
		config = new QtConfig("NCAR", "Proxy");
	}


	// Get configuration values. Default values will be created in the
	// configuration file if they don't exist, so that running the program
	// for the first time will create a configuration template.
	std::string proxyID        = config->getString("ProxyID",         "PROXY");
	std::string proxyKeyFile    (config->getString("ProxyKeyFile",    "./proxy.key"));
	std::string proxyCertFile   (config->getString("ProxyCertFile",   "./proxy.crt"));
	int switchProxyPort        = config->getInt   ("SSLProxyPort", 0);
	std::string switchHostName = config->getString("SwitchHostName",  "127.0.0.1");
	std::string switchCertFile = config->getString("SwitchSSLCertFile",  "./switch.crt");

	// Get the instrument definition file. The proxy program will currently only support
	// one instrument. This could be changed if necessary.
	std::string instrumentFile;
	instrumentFile = config->getString("InstrumentFile", "");

	// Get the configuration for the instrument.
	InstConfig instConfig(instrumentFile);

	// Get the messages for this instrument
	std::vector<InstConfig::MessageInfo> instMsg = instConfig.messages();

	// Build the message list for this proxy
    std::map<std::string, SslProxy::InstMsgInfo> messages;
	for (int i = 0; i < instMsg.size(); i++) {
		// Configuration for one message
		SslProxy::InstMsgInfo msg;
		msg._instName              = instConfig.instrumentName();
		msg._incomingPort          = instConfig.incomingPort();
		msg._destPort              = instConfig.destPort();
		msg._destIP                = instConfig.destIP();
		msg._msgId                 = instMsg[i].msgID;
		msg._broadcast             = instMsg[i].broadcast;

		// save the message info
		messages[msg._msgId] = msg;
	}

    // If the port number is 0, it indicates that the user has not configured
	// the application yet. We wait until this point so that all of the default values
	// will have been added to the configuration file. Force them to take a stab at
	// configuration.
	if (switchProxyPort == 0) {
		std::cout << "Please create a usable configuration by editing " << config->fileName() << std::endl;
		exit(1);
	}

	// add the server certificate to the CA database
	std::vector<std::string> caDatabase;
	caDatabase.push_back(proxyCertFile);
	caDatabase.push_back(switchCertFile);

	// Create the Qt application
	QApplication app(argc, argv);

    // Create the SSL proxy. It will wait to connect with the switch,
	// until requested via a call connectToServer().
	SslProxy sslProxy(
			proxyID,
    		proxyKeyFile,
    		proxyCertFile,
    		switchHostName,
    		switchProxyPort,
    		caDatabase,
    		messages);

	// Create the Proxy user interface.
	ProxyMainWindow proxyMainWindow(sslProxy, 0);
	QString title("Proxy-");
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

	// Show the user interface
	proxyMainWindow.show();

    // Hop on the merry-go-round.
	return app.exec();
}
