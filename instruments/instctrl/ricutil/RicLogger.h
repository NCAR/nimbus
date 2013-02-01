#ifndef RICLOGGER_H_
#define RICLOGGER_H_

#ifndef WIN32
#include <syslog.h>
#endif
#include "log4cpp/Category.hh"
#include "log4cpp/Appender.hh"
#include "log4cpp/FileAppender.hh"
#include "log4cpp/OstreamAppender.hh"
#include "log4cpp/Layout.hh"
#include "log4cpp/BasicLayout.hh"
#include "log4cpp/Priority.hh"
#include <string>

/// A logging facility for the Remote Instrument Control package.
/// The first instantiation of RicLogger will establish the log message
/// identifier; for this reason it is best to create a RicLogger in
/// main. Subsequent calls to RicLogger will not change the identifier.
/// This ir is advisable to use the default constructor, which does not
/// specify an identifier, when RicLogger is used in classes.
/// @todo This class has a memory leak, due to needing to keep a
/// copy of _ident arround. Will need to add reference counting
/// in order to know when to delete it.
class RicLogger {
public:
	/// Constructor
	/// @param logId The log message identifier.
	/// @param toStdOut If true, the messages are also written to stdout
	RicLogger(std::string logId = std::string(""), bool toStdOut = false);
	/// Destructor
	virtual ~RicLogger();
	/// Log a message
	/// @param msg The message.
	void log(std::string msg);

protected:
	/// Set true after the open so that it only occurs once in a process.
	static bool _isOpen;
	/// The ident string. This must exist for the life of the process,
	/// because syslog references it.
	static std::string* _ident;
	/// Set true if the messages are to be echoed to stdout
	static bool _toStdOut;

};

#endif /* RICLOGGER_H_ */
