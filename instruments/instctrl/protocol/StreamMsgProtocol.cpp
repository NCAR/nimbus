#include <iostream>
#include <sstream>
#include "StreamMsgProtocol.h"

using namespace Protocols;

/////////////////////////////////////////////////////////////////////
StreamMsgProtocol::StreamMsgProtocol():
_msgSize(-1)
{

}

/////////////////////////////////////////////////////////////////////
StreamMsgProtocol::~StreamMsgProtocol() {

}

/////////////////////////////////////////////////////////////////////
std::vector<std::string> StreamMsgProtocol::outgoing(std::string s) {

	std::vector<std::string> results;

	// prefix the protocol message with the message length
	std::stringstream ss;
	ss << s.size() << ":";

	// append the message itself
	std::string pmsg;
	pmsg += ss.str();
	pmsg += s;

	// return
	results.push_back(pmsg);

	return results;
}

/////////////////////////////////////////////////////////////////////
std::vector<std::string> StreamMsgProtocol::incoming(std::string s) {

	std::vector<std::string> results;

	// Add the message to the message buffer
	for (int i = 0; i < s.size(); i++) {
		_msgBuf.push_back(s[i]);
	}

	// process the message buffer

	while(_msgBuf.size() > 0) {
		// Are we looking for the header?
		if (_msgSize < 0) {
			while(_msgBuf.size() > 0) {
				char c = _msgBuf.front();
				_msgBuf.pop_front();
				if (c != ':') {
					_header.push_back(c);
				} else {
					// found the ":", extract the length
					std::stringstream ss;
					ss << _header;
					ss >> _msgSize;
					_header.clear();
					break;
				}
			}
		}

		// extract the message
		while(_msgBuf.size() > 0) {
			char c = _msgBuf.front();
			_msgBuf.pop_front();
			_resultBuf.push_back(c);
			if (_resultBuf.size() == _msgSize) {
				results.push_back(_resultBuf);
				_resultBuf.clear();
				_msgSize = -1;
				break;
			}
		}
	}

	return results;
}
