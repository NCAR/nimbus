#include "RateLimiter.h"

/////////////////////////////////////////////////////////////////////
RateLimiter::RateLimiter()
{
}

/////////////////////////////////////////////////////////////////////
RateLimiter::~RateLimiter()
{
	// Kill all timers and delete timer id pointers
	while (!_timers.empty()) {
		killTimer(*_timers.begin()->second);
		delete _timers.begin()->second;
		_timers.erase(_timers.begin());
	}

	// Delete all OK flag pointers
	while (!_okFlags.empty()) {
		delete _okFlags.begin()->second;
		_okFlags.erase(_okFlags.begin());
	}

	_rates.clear();
}

/////////////////////////////////////////////////////////////////////
void RateLimiter::timerEvent(QTimerEvent* event)
{
	// Find which timer is timed out
	std::map<std::string, int*>::iterator it;
	for (it = _timers.begin(); it != _timers.end(); it++) {
		if (event->timerId() == *it->second) {
			// Get the message type associated with this timer
			std::string msgId = it->first;
			// Set the OK flag for this message type to true
			*_okFlags[msgId] = true;
		}
	}
}

/////////////////////////////////////////////////////////////////////
void RateLimiter::addMsgType(std::string msgId, double rate)
{
	// If this is a new rate
	if (_rates.find(rate) == _rates.end()) {
		qDebug() << "RateLimiter::addMsgType" << msgId.c_str() << "rate" << rate << "(new)";
		int* timer;
		if (rate == 0) {
			// If rate = 0, set the timer to -1.0 for identification
			timer = new int(-1.0);
		}
		else {
			// Create the timer for this rate and start the timer
			timer = new int();
			*timer = startTimer(rate * 1000);
		}
		// Map the message type to the timer
		_timers[msgId] = timer;

		// Map the rate to its timer
		_rates[rate] = timer;
	}
	// If we already have a timer for this rate
	else {
		qDebug() << "RateLimiter::addMsgType" << msgId.c_str() << "rate" << rate << "(existing)";
		// Map the message type to the timer
		_timers[msgId] = _rates[rate];
	}

	// Create the OK flag for this message type and set it to true
	bool* okFlag = new bool();
	*okFlag = true;
	// Map the message type to the OK flag
	_okFlags[msgId] = okFlag;
}

/////////////////////////////////////////////////////////////////////
bool RateLimiter::checkLimit(Protocols::Message msg)
{
	// Get message ID
	std::string msgId = msg.msgId();

	// Find the OK flag for this type of message
	bool okFlag;
	if (_okFlags.find(msgId) != _okFlags.end()) {
		okFlag = *_okFlags[msgId];
		// If the message has a rate limit of 0, always forward it
		if (*_timers[msgId] == -1.0)
			okFlag = true;
		else
			// Set the OK flag to false for this message type
			*_okFlags[msgId] = false;
	}
	else {
		// This is an unknown message type, pass it anyway
		okFlag = true;
	}

	return okFlag;
}
