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

	boost::posix_time::ptime t = boost::posix_time::second_clock::universal_time();
	std::string st = boost::posix_time::to_simple_string(t);

	// Get the identifier, if present.
	std::string id = _ident? *_ident:"";
	std::vector<std::string> ss;
	boost::split(ss, msg, boost::is_any_of("\n"));
	for (size_t i = 0; i < ss.size(); i++) {
		if (_toStdOut) {
			std::cout << st << " " << id << ": " << ss[i] << std::endl;
		}
#ifndef WIN32
		syslog(LOG_INFO, "%s", ss[i].c_str());
#endif
	}
}
