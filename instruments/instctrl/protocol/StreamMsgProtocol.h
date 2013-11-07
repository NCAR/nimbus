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
	/// A simple protocol where outgoing() simply prepends the message payload with
    /// the payload length, followed by a colon. The incoming() function counts
	/// the bytes specified in the length header, and then extracts the following
	/// bytes.
	///
	/// StreamMsgProtocol is intended to be used with stream connections, such
    /// as TCP, where the underlying transport does not create discreet messages.
	///
	/// @todo Make StreamMsgProtocol resilient to dropped characters. Right now
    /// it willjust fall apart if this happens. Would need to add
    /// a synchronization sequence to the messages and logic to processes them.
	class StreamMsgProtocol: public Protocol::Protocol {
	public:
		/// Constructor
		StreamMsgProtocol();
		/// Destructor
		virtual ~StreamMsgProtocol();
		/// Convert an incoming protocol stream into a message.
		/// @param s The incoming data bytes.
		/// @return Complete messages, if available.
		virtual std::vector<std::string> incoming(std::string s);
		/// Convert a message into an outgoing protocol stream.
		/// @param s The message to be sent.
		/// @return Data block ready for transmision, if available.
		virtual std::vector<std::string> outgoing(std::string s);

	protected:
		/// The incoming message buffer.
		std::list<char> _msgBuf;
		/// Collect the header here
		std::string _header;
		/// The number of characters in the message
		int _msgSize;
		/// Build a result string here.
		std::string _resultBuf;
	};
};
#endif /* JSONPROTOCOL_H_ */
