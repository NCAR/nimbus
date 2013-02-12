#include <QtCore>
#include <iostream>
#include <sstream>
#include <string>
#include <boost/program_options.hpp>
#include "Switch.h"
#include "SwitchConfig.h"
#include "RicLogger.h"
#include "svnInfo.h"

namespace po = boost::program_options;

/////////////////////////////////////////////////////////////////////
void
parseCommandLine(int argc, char** argv,
	std::string& configFile,
	bool& verbose)
{
  	const char *optarg;
   	bool err = false;
  	verbose = false;

	po::options_description descripts("Options");
	descripts.add_options()
	("config,c", po::value<std::string>(&configFile), "configuration file")
	("verbose,v",                                     "verbose enabled")
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

	if (vm.count("verbose"))
		verbose = true;

	if (vm.count("help") || err) {
		std::cout << descripts << std::endl;
		std::cout << "Edit the configuration file (typically ~/.config/NCAR/Switch.ini) to set configuration parameters" << std::endl;
		exit(1);
	}
}

/////////////////////////////////////////////////////////////////////
void createSslSwitch(SwitchConfig* config, Switch* swtch, bool verbose)
{
	// Get the configuration that applies to the Proxy SSL connection

	// The file containing the private key for the switch to proxy SSL link.
	// This file must be kept private!
	std::string serverKey(config->serverKeyFile());

	// The file containing the private certificate for the switch to proxy SSL link.
	std::string serverCertFile(config->serverCertFile());

	QSslCertificate serverCert(QSslCertificate::fromPath(serverCertFile.c_str())[0]);
	if (serverCert.isNull() || !serverCert.isValid()) {
		std::cout << "Invalid certificate specified in "
				<< serverCertFile << ", switch cannot start" << std::endl;
		exit(1);
	}

	// Get the proxy definitions
	std::vector<std::map<std::string, std::string> > proxiesConfig;
	proxiesConfig = config->proxies();
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
	swtch = new Switch(
			serverKey,
			serverCert,
			config->proxyPort(),
			proxies,
			config->localPort(),
			config->remoteIP(),
			config->remotePort(),
			config->cipherKey(),
			verbose);
}

/////////////////////////////////////////////////////////////////////
void createEmbeddedSwitch(SwitchConfig* config, Switch* swtch, bool verbose)
{
	// Get the instrument definition files
	std::vector<std::map<std::string, std::string> > instruments;
	instruments = config->instruments();

	// Create the instrument configurations
	std::vector<InstConfig> instConfigs;
	for (int i = 0; i < instruments.size(); i++) {
		/// @todo Need error checking for missing map key
		std::string fileName = instruments[i]["InstrumentFile"];
		/// @todo Need to add error handling to the InstConfig constructor.
		instConfigs.push_back(InstConfig(fileName));
	}

	swtch = new Switch(
			instConfigs,
			config->localPort(),
			config->remoteIP(),
			config->remotePort(),
			config->cipherKey(),
			verbose);
}

/////////////////////////////////////////////////////////////////////
int main(int  argc, char** argv)
{
	try {
		RicLogger logger("RICSwitch", true);
		std::string msg("Starting RIC switch: ");
		msg += std::string(argv[0]);
		msg += std::string(" r") + std::string(SVNREVISION);
		logger.log(msg);

		/////////////////////////////////////////////////////////////////
		// Create the Qt application (i.e. event loop)
		QCoreApplication app(argc, argv);

		/////////////////////////////////////////////////////////////////
		// Process command line options
		std::string configFile;
	  	bool verbose;
		parseCommandLine(argc, argv, configFile, verbose);

		/////////////////////////////////////////////////////////////////
		// Get the configuration
		SwitchConfig* config;
		if (configFile.size()) {
			config = new SwitchConfig(configFile);
		} else {
			config = new SwitchConfig("NCAR", "Switch");
		}

		/////////////////////////////////////////////////////////////////
		// Create the desired switch type
		Switch* swtch;
		// If SslProxy is true, an SSL proxy switch is created. Otherwise,
		// an embedded proxy switch is created.
		if (config->sslProxy()) {
			createSslSwitch(config, swtch, verbose);
		} else {
			createEmbeddedSwitch(config, swtch, verbose);
		}

		// Run the event loop
		return app.exec();
	}
	catch (std::string msg) {
		std::cerr << msg << std::endl;
		return 1;
	}
}
