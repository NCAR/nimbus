#include <QtCore>
#include <iostream>
#include <string>
#include <boost/program_options.hpp>
#include "Switch.h"
#include "SwitchConfig.h"
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
int main(int  argc, char** argv)
{
	try {
                std::string logId = "RICSwitch-" + std::string(SVNREVISION);
		RicLogger logger(logId, true);
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
		// Create the switch
		Switch* swtch = new Switch(config, verbose);

		// Run the event loop
		return app.exec();
	}
	catch (std::string msg) {
		std::cerr << msg << std::endl;
		return 1;
	}
}
