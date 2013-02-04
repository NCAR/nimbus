/**
 * @(#) Protocol.h
 */

#ifndef PROTOCOL_H_H
#define PROTOCOL_H_H

#include <string>
#include <vector>
namespace Protocols {
    /// A Protocol abstracts the logic for a serialization/deserialization protocol.
    /// It acts as an intermediary or filter. It does not know anything about
    /// the underlying transport mechanism.
	///
	/// It works as follows:
    /// The protocol is given a message, and it will
    /// package that into discrete blocks for transmitting. Likewise,
	/// the protocol will convert an incoming stream of bytes back into
	/// one or more messages.
	///
	/// For outgoing messages, give a message to outgoing(). If it is
	/// ready, the function will return a block of bytes that can
	/// be transmitted.
	///
	/// Similarly, the bytes from an incoming data stream are fed to
	/// incoming(). If one or more complete messages have been received,
	/// they are returned by the function.
	///
	/// The default Protocol base class simply copies a message without any
    /// packaging.
	///
	/// The protocol understands
    class Protocol
	{
	public:
    	/// Constructor
		Protocol();
		/// Destructor
		virtual ~Protocol();
		/// Convert an incoming protocol stream into a message.
		/// @param s The incoming data bytes.
		/// @return Complete messages, if available.
		virtual std::vector<std::string> incoming(std::string s);
		/// Convert a message into an outgoing protocol stream.
		/// @param msg The message to be sent.
		/// @return Data block ready for transmision, if available.
		virtual std::vector<std::string> outgoing(std::string msg);
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
	};
};
#endif
