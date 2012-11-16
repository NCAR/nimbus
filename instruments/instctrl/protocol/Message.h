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
	class Message {
	public:
		/// Create a message from supplied parameters.
		Message(std::string from, std::string to, std::string messageText);
		/// Create a message from a json text string.
		Message(std::string jsonString);
		virtual ~Message();
		Json::Value toJson();

	protected:
		MessagePayload _payload;
		std::string _from;
		std::string _to;
	};
};



#endif /* MESSAGE_H_ */
