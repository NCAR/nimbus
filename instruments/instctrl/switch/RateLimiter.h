#ifndef RATELIMITER_H_
#define RATELIMITER_H_

#include <QtCore>
#include <string>
#include <vector>
#include <map>
#include <set>

#include "Message.h"
#include "RicLogger.h"

/// The RateLimiter class performs the limiting of messages allowed
/// to be forwarded from the proxy to the switch.  Each message type
/// (or msgID) may have specific rate limit (specified in instrument
/// configuration file).  RateLimiter keeps one timer for each rate
/// limit and use the timer to manage the limiting for all types of
/// messages with the same rate limit.
///
/// Each message type is accoiated with an OK flag. When the flag is
/// true, the message is OK to be forwarded to the switch, otherwise,
/// the message will be dropped.
class RateLimiter : public QObject {
	Q_OBJECT
public:
	/// Constructor
	RateLimiter();
	/// Destructor
	virtual ~RateLimiter();

	virtual void timerEvent(QTimerEvent* event);
	/// Add a message type
	/// @param msgId The message ID/type
	/// @param rate The rate limit for this message type
	void addMsgType(std::string msgId, double rate);
	/// Check rate limit for a message
	/// @param msg The message to be checked
	/// @returns True if OK to forward this message, False to drop it
	bool checkLimit(Protocols::Message msg);

protected:
	/// Map each message type with its OK flag.
	std::map<std::string, bool*> _okFlags;
	/// Map each rate limit with its associated timer
	std::map<double, int*> _rates;
	/// Map each message type with its associated timer
	std::map<std::string, int*> _timers;
	/// System message logger
	RicLogger _logger;
};

#endif /* RATELIMITER_H_ */

