/*
 * Message.h
 *
 *  Created on: Nov 15, 2012
 *      Author: martinc
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

#include "MessagePayload.h"

namespace Protocol {
	/// A class for encapsulating message meta-data and payload data in JSON
    /// format.
    ///
    /// The JsonCpp package is used for json encoding and decoding. See
    /// http://jsoncpp.sourceforge.net for the documentation
    class Message {
	public:
		/// Create a message from supplied parameters.
		Message(std::string from, std::string to, std::string messageText);
		/// Create a message from a json text string.
		Message(std::string jsonString);
		/// Destructor
		virtual ~Message();
		/// @return As a Json::Value
		Json::Value toJson();
		/// @return As a formatted string
		std::string toStdString();

	protected:
		/// The message payload
		MessagePayload _payload;
		/// The sender identity
		std::string _from;
		/// The recipient identity
		std::string _to;
	};
};



#endif /* MESSAGE_H_ */
