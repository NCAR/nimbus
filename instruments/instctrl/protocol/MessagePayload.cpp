/*
 * Message.cpp
 *
 *  Created on: Nov 15, 2012
 *      Author: martinc
 */

#include "MessagePayload.h"

using namespace Protocol;

MessagePayload::MessagePayload(std::string text):
	_text(text)
{

}

MessagePayload::MessagePayload(Json::Value& jsonValue) {
	_text = jsonValue["text"].asString();
}


MessagePayload::MessagePayload() {

}

MessagePayload::~MessagePayload() {

}

Json::Value MessagePayload::toJson() {

	Json::Value root;
	root["text"] = _text.c_str();

	return root;
}
