/**
 * @(#) Protocol.h
 */

#ifndef PROTOCOL_H_H
#define PROTOCOL_H_H

#include <string>
#include <vector>
namespace Protocols {
    /// A Protocol abstracts the logic for a communication protocol.
    /// It acts as an intermediary. It does not know anything about
    /// the underlying transport mechanism.
	///
	/// It works as follows: A message is delivered
    /// the Protocol. The Protocol returns
	/// Handle the message transmission for particular protocols.
    /// The protocol is given a message, and it will handle
    /// packaging into discrete blocks for transmitting and
    /// reconstituting the blocks back into the message. The
    /// default protocol simply copies a message without any
    /// packaging.
    class Protocol
	{
	public:
		Protocol();
		virtual ~Protocol();
		/// Convert a message into an outgoing protocol stream.
		virtual std::vector<std::string> outgoing(std::string msg);
		/// Convert an incoming protocol stream into a message.
		/// @param s The incoming text.
		/// @return Complete messages, if available.
		virtual std::vector<std::string> incoming(std::string s);
		/// @retun The most recent error messages. If there have been no
		/// errors, the returned list is empty. Calling this function
		/// resets the accumulated error messages.
		std::vector<std::string> errors();
		/// @return True if there are outstanding errors. Call
		/// errors() in order to reset it.
		bool error();

	protected:
		/// The list of accumulated errors. Cleared when errors() is called.
		std::vector<std::string> _errors;
		///
	};
};
#endif
