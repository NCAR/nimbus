#ifndef MESSAGE_H_
#define MESSAGE_H_

#include "MessagePayload.h"

namespace Protocols {
	/// A class for encapsulating message meta-data and payload data,
	/// and providing a JSON representation of the structure.
    ///
    /// The JsonCpp package is used for json encoding and decoding. See
    /// http://jsoncpp.sourceforge.net for the documentation
    class Message
    {
	public:
    	enum MESSAGE_TYPE { SYS, INST };
		/// Create a message from supplied parameters.
		Message(MESSAGE_TYPE msgType,
				std::string  instrumentId,
				std::string  msgId,
				std::string  messageText);
		/// Create a message from a json text string.
		Message(std::string jsonString);
		/// Destructor
		virtual ~Message();
		/// @return The message type
		MESSAGE_TYPE msgType();
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
		static std::string extractId(std::string s);

	protected:
		/// The message type, i.e. SYS vs INST
		MESSAGE_TYPE _msgType;
		/// The instrument identity
		std::string _instrumentId;
		/// The message identity
		std::string _msgId;
		/// The message payload
		MessagePayload _payload;
	};
};

#endif /* MESSAGE_H_ */
