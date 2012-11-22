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

	class MessagePayload {
	public:
		MessagePayload(std::string text);
		MessagePayload(Json::Value& jsonValue);
		MessagePayload();
		virtual ~MessagePayload();
		Json::Value toJson();
	protected:
		std::string _text;
	};
};


#endif /* MESSAGEPAYLOAD_H_ */
