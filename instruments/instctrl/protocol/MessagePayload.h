/*
 * MessagePayload.h
 *
 *  Created on: Nov 15, 2012
 *      Author: martinc
 */

#ifndef MESSAGEPAYLOAD_H_
#define MESSAGEPAYLOAD_H_

#include <string>
#include "json/json.h"

namespace Protocols {
	/// Represent the actual message text. The JSON functionality is
	/// provided in order to inter-operate with JSON serialization.
	class MessagePayload {
	public:
		/// Constructor
		/// @param text The message text.
		MessagePayload(std::string text);
		/// @param jsonValue The message text, in JSON format.
		MessagePayload(Json::Value& jsonValue);
		/// A default constructor to create a payload with no text.
		MessagePayload();
		/// Destructor
		virtual ~MessagePayload();
		/// @return The JSON form of the payload.
		Json::Value toJson();
		/// @return The message text.
		std::string text();
	protected:
		/// The message text.
		std::string _text;
	};
};


#endif /* MESSAGEPAYLOAD_H_ */
