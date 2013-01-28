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
		openlog(0, LOG_PID, LOG_USER);
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
		openlog(_ident->c_str(), LOG_PID, LOG_USER);
	}

}

/////////////////////////////////////////////////////////////////////
RicLogger::~RicLogger() {

}

/////////////////////////////////////////////////////////////////////
void
RicLogger::log(std::string msg) {
	syslog(LOG_INFO, "%s", msg.c_str());
}
