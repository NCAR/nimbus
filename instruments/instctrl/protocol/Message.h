/*
 * Message.h
 *
 *  Created on: Nov 15, 2012
 *      Author: martinc
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

#include "MessagePayload.h"

namespace Protocols {
	/// A class for encapsulating message meta-data and payload data in JSON
    /// format.
    ///
    /// The JsonCpp package is used for json encoding and decoding. See
    /// http://jsoncpp.sourceforge.net for the documentation
    class Message {
	public:
		/// Create a message from supplied parameters.
		Message(std::string instrumentId, std::string msgId, std::string messageText);
		/// Create a message from a json text string.
		Message(std::string jsonString);
		/// Destructor
		virtual ~Message();
		/// @return The message identifier
		std::string msgId();
		/// @return As a Json::Value
		Json::Value toJson();
		/// @return As a formatted string
		std::string toJsonStdString();
		/// @return Reference to the payload
		MessagePayload& payload();
		/// Extract the message id from a CSVstring
		/// @param s The source string.
		/// @return The first token delimited by a comma. An empty
		/// string is returned if no token is found.
		/// @todo This needs to be extended to use a regular expression so that non-CSV messages
		/// can be recognized. Currently the sounding messages are dropped becasue they are not CSV.
		static std::string extractId(std::string s);

	protected:
		/// The message payload
		MessagePayload _payload;
		/// The sender identity
		std::string _instrumentId;
		/// The recipient identity
		std::string _msgId;
	};
};



#endif /* MESSAGE_H_ */
