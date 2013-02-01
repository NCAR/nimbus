#include "RicLogger.h"
#include <iostream>

// Track when the openlog request has occurred.
bool RicLogger::_isOpen = false;
std::string* RicLogger::_ident = 0;
bool RicLogger::_toStdOut = false;

/////////////////////////////////////////////////////////////////////
RicLogger::RicLogger(std::string logId, bool toStdOut)
{
	if (!_isOpen) {
		if (_ident) {
			delete _ident;
		}
		_ident = new std::string(logId);
		_isOpen = true;
		_toStdOut = toStdOut;
#ifndef WIN32
		if (*_ident == std::string("")) {
			openlog(_ident->c_str(), LOG_PID, LOG_USER);
		} else {
			openlog(0, LOG_PID, LOG_USER);
		}
#endif
	}

}

/////////////////////////////////////////////////////////////////////
RicLogger::~RicLogger() {

}

/////////////////////////////////////////////////////////////////////
void
RicLogger::log(std::string msg) {
	// Get the identifier, if present.
	std::string id = _ident? *_ident:"";
#ifndef WIN32
	syslog(LOG_INFO, "%s", msg.c_str());
	if (_toStdOut) {
		std::cout << id << ": " << msg << std::endl;
	}
#else
	if (_toStdOut) {
		std::cout << id << ": " << msg << std::endl;
	}
#endif
	}
