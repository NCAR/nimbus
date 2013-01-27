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
	("config,c", po::value<std::string>(&configFile), "configuration file")
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
	std::string serverCertFile(config->getString("SwitchSSLCertFile", "./switch.crt"));

	QSslCertificate serverCert(QSslCertificate::fromPath(serverCertFile.c_str())[0]);
	if (serverCert.isNull() || !serverCert.isValid()) {
		std::cout << "Invalid certificate specified in "
				<< serverCertFile << ", switch cannot start" << std::endl;
		exit(1);
	}

	// The port for communications to the SslProxy
	int switchProxyPort = config->getInt("SSLProxyPort", 0);

	// Get the proxy definitions
	std::vector<std::map<std::string, std::string> > proxiesConfig;
	proxiesConfig = config->getArray("SSLProxies", proxiesConfig);
	std::vector<SslProxyServer::SslProxyDef> proxies;
	for (int i = 0; i < proxiesConfig.size(); i++) {
		SslProxyServer::SslProxyDef proxy;
		std::string sslCertFile = proxiesConfig[i]["SSLCertFile"];
		QSslCertificate cert(QSslCertificate::fromPath(sslCertFile.c_str())[0]);
		if (cert.isNull() || !cert.isValid()) {
			std::cout << "Invalid certificate specified in "
					<< sslCertFile << ", proxy will not be registered";
		} else {
			proxy._sslCert = cert;
			proxy._instConfig = InstConfig(proxiesConfig[i]["InstrumentFile"]);
			proxies.push_back(proxy);
		}
	}

	// Create the SSL based switch. The switch creates:
	//  - an SslServer
	//  - a SwitchConnection to the remote switch
	//  - SslServer creates connections to Proxies.
	*swtch = new Switch(
			serverKey,
			serverCert,
			switchProxyPort,
			proxies,
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

	// Create the instrument configurations
	std::vector<InstConfig> instConfigs;
	for (int i = 0; i < instruments.size(); i++) {
		/// @todo Need error checking for missing map key
		std::string fileName = instruments[i]["InstrumentFile"];
		/// @todo Need to add error handling to the InstConfig constructor.
		instConfigs.push_back(InstConfig(fileName));
	}

	*swtch = new Switch(
			instConfigs,
			switchLocalPort,
			switchRemoteIP,
			switchRemotePort,
			switchCipherKey);

}
/////////////////////////////////////////////////////////////////////
int main(int  argc, char** argv){

	try {

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
	catch (std::string msg) {
		std::cerr << msg << std::endl;
		return 1;
	}


}
