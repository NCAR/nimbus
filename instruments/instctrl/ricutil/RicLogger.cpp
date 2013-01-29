#include "RicLogger.h"
#include <iostream>

// Track when the openlog request has occured.
bool RicLogger::_isOpen = false;
std::string* RicLogger::_ident = 0;

/////////////////////////////////////////////////////////////////////
RicLogger::RicLogger()
{
	if (!_isOpen) {
		_isOpen = true;
#ifndef WIN32
		openlog(0, LOG_PID, LOG_USER);
#endif
	}

}

/////////////////////////////////////////////////////////////////////
RicLogger::RicLogger(std::string logId)
{
	if (_ident) {
		delete _ident;
		_ident = 0;
	}
	_ident = new std::string(logId);

	if (!_isOpen) {
		_isOpen = true;
#ifndef WIN32
		openlog(_ident->c_str(), LOG_PID, LOG_USER);
#endif
	}

}

/////////////////////////////////////////////////////////////////////
RicLogger::~RicLogger() {

}

/////////////////////////////////////////////////////////////////////
void
RicLogger::log(std::string msg) {
#ifndef WIN32
	syslog(LOG_INFO, "%s", msg.c_str());
#else
	std::cout << *_ident << ": " << msg << std::endl;
#endif
	}
