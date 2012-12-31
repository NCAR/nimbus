#include <QtCore>
#include <iostream>
#include <sstream>
#include <string>
#include <boost/program_options.hpp>
#include "QtConfig.h"
#include "Switch.h"

namespace po = boost::program_options;

/////////////////////////////////////////////////////////////////////
void
parseCommandLine(int argc, char** argv,
	std::string& configFile)
{
  	const char *optarg;
   	bool err = false;

	po::options_description descripts("Options");
	descripts.add_options()
	("config,c", po::value<std::string>(&configFile)->implicit_value(""), "configuration file")
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
		std::cout << "Edit the configuration file (typically ~/.config/NCAR/Switch.ini) to set configuration parameters" << std::endl;
		exit(1);
	}
}

/////////////////////////////////////////////////////////////////////
void getCommonConfig(
		QtConfig* config,
		int& switchLocalPort,
		std::string& switchRemoteIP,
		int& switchRemotePort,
		std::string& switchCipherKey
		) {

	// Listen on this port for messages from the remote switch
	switchLocalPort = config->getInt("SwitchLocalPort", 0);

	// The remote switch IP
	switchRemoteIP = config->getString("SwitchRemoteIP", "127.0.0.1");

	// The destination port for messages to the remote switch
	switchRemotePort = config->getInt("SwitchRemotePort", 0);

	// The file containing the key for symmetric cipher encryption over SwitchConnection.
	// This file must be kept private!
	switchCipherKey = config->getString("SwitchCipherKey", "./udpcipher.key");

	// If the port number is 0, it indicates that the user has not configured
	// the application yet. We wait until this point so that all of the default values
	// will have been added to the configuration file. Force them to take a stab at
	// configuration.
	if (switchLocalPort == 0 || switchRemotePort == 0) {
		std::cout << "Please create a usable configuration by editing " << config->fileName()
				<< " (make sure ports and IP addresses are valid)"<< std::endl;
		exit(1);
	}
}
/////////////////////////////////////////////////////////////////////
void createSslSwitch(
		int argc,
		char** argv,
		QtConfig* config,
		Switch** swtch,
		int switchLocalPort,
	    std::string switchRemoteIP,
		int switchRemotePort,
		std::string switchCipherKey) {

	// Get the configuration that applies to the Proxy SSL connection

	// The file containing the private key for the switch to proxy SSL link.
	// This file must be kept private!
	std::string serverKey(config->getString("SwitchSSLKeyFile", "./switch.key"));

	// The file containing the private certificate for the switch to proxy SSL link.
	std::string serverCert(config->getString("SwitchSSLCertFile", "./switch.crt"));

	// The port for communications to the SslProxy
	int switchProxyPort = config->getInt("SSLProxyPort", 0);

	// add additional certs to the database
	std::vector<std::string> caDatabase;

	// Add the ClientCerts
	//caDatabase.push_back("./proxy.crt");
	std::vector<std::map<std::string, std::string> > clientCerts;
	clientCerts = config->getArray("ClientCerts", clientCerts);
	for (int i = 0; i < clientCerts.size(); i++) {
		std::map<std::string, std::string>::iterator j;
		for (j = clientCerts[i].begin(); j != clientCerts[i].end(); j++) {
			caDatabase.push_back(j->second);
		}
	}
	// add the server certificate to the CA database
	caDatabase.push_back(serverCert);
	// add extra certs from the command line
	for (int i = 3; i < argc; i++) {
		caDatabase.push_back(argv[i]);
	}

	// Create the SSL based switch. The switch creates:
	//  - an SslServer
	//  - a SwitchConnection to the remote switch
	//  - SslServer creates connections to Proxies.
	*swtch = new Switch(
			serverKey,
			serverCert,
			switchProxyPort,
			caDatabase,
			switchLocalPort,
			switchRemoteIP,
			switchRemotePort,
			switchCipherKey);

}

/////////////////////////////////////////////////////////////////////
void createEmbeddedSwitch(
		QtConfig* config,
		Switch** swtch,
		int switchLocalPort,
	    std::string switchRemoteIP,
		int switchRemotePort,
		std::string switchCipherKey) {

	// Get the instrument definition files
	std::vector<std::map<std::string, std::string> > instruments;
	instruments = config->getArray("Instruments", instruments);

	std::vector<std::string> instrumentFiles;
	for (int i = 0; i < instruments.size(); i++) {
		std::map<std::string, std::string>::iterator j;
		for (j = instruments[i].begin(); j != instruments[i].end(); j++) {
			instrumentFiles.push_back(j->second);
		}
	}

	*swtch = new Switch(
			instrumentFiles,
			switchLocalPort,
			switchRemoteIP,
			switchRemotePort,
			switchCipherKey);

}
/////////////////////////////////////////////////////////////////////
int main(int  argc, char** argv){


	/////////////////////////////////////////////////////////////////
	// Create the Qt application (i.e. event loop)
	QCoreApplication app(argc, argv);


	/////////////////////////////////////////////////////////////////
	// Process command line options
	std::string configFile;
	parseCommandLine(argc, argv, configFile);

	/////////////////////////////////////////////////////////////////
	// Get the configuration
	QtConfig* config;
	if (configFile.size()) {
		config = new QtConfig(configFile);
	} else {
		config = new QtConfig("NCAR", "Switch");
	}

	/////////////////////////////////////////////////////////////////
	// Get configuration parameters that are used by both types of switch

	// Listen on this port for messages from the remote switch
	int switchLocalPort;

	// The remote switch IP
	std::string switchRemoteIP;

	// The destination port for messages to the remote switch
	int switchRemotePort;

	// The file containing the key for symmetric cipher encryption over
	// SwitchConnection. This file must be kept private!
	std::string switchCipherKey;

	getCommonConfig(
			config,
			switchLocalPort,
			switchRemoteIP,
			switchRemotePort,
			switchCipherKey);

	/////////////////////////////////////////////////////////////////
	// Create the desired switch type

	// If SslProxy is true, we will be an SSL proxy switch. Otherwise, we
	// we will be an embedded proxy switch.
	bool sslSwitch = config->getBool("SslProxy", true);

	Switch* swtch;

	if (sslSwitch) {
		createSslSwitch(
				argc,
				argv,
				config,
				&swtch,
				switchLocalPort,
				switchRemoteIP,
				switchRemotePort,
				switchCipherKey);
	} else {
		createEmbeddedSwitch(
				config,
				&swtch,
				switchLocalPort,
				switchRemoteIP,
				switchRemotePort,
				switchCipherKey);
	}

	// Run the event loop
	return app.exec();

}
