/*
 * JSONProtocol.h
 *
 *  Created on: Nov 20, 2012
 *      Author: martinc
 */

#ifndef JSONPROTOCOL_H_
#define JSONPROTOCOL_H_

#include <string>
#include <list>
#include "Protocol.h"

namespace Protocols {
	/// A simple protocol which simply prepends the message payload with
    /// the paylod length, followed by a colon.
	class StreamMsgProtocol: public Protocol::Protocol {
	public:
		StreamMsgProtocol();
		virtual ~StreamMsgProtocol();
		virtual std::vector<std::string> incoming(std::string s);
		virtual std::vector<std::string> outgoing(std::string s);

	protected:
		/// The incoming message buffer. Messages are buffered here
		/// until a complete JSON package is received, which occurs
		/// when an equal number of open and close braces are received.
		/// At the point, the new message is emitted.
		std::list<char> _msgBuf;
		/// Construct the header here
		std::string _header;
		/// The number of characters in the message
		int _msgSize;
		/// Build a result string here.
		std::string _resultBuf;
	};
};
#endif /* JSONPROTOCOL_H_ */
