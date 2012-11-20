#include "Switch.h"

/////////////////////////////////////////////////////////////////////
Switch::Switch(std::string keyFile,
		std::string certFile,
		int switchPort,
		std::vector<std::string> caDatabase):
_server(keyFile, certFile, switchPort, caDatabase)
{

}

/////////////////////////////////////////////////////////////////////
Switch::~Switch() {

}
