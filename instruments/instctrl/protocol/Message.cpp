/*
 * Message.cpp
 *
 *  Created on: Nov 15, 2012
 *      Author: martinc
 */

#include "Message.h"

using namespace Protocol;

Message::Message(std::string from, std::string to, std::string messageText):
_payload(messageText),
_from(from),
_to(to)
{

}

Message::Message(std::string jsonString) {
	Json::Value root;
	Json::Reader reader;
	bool parsingSuccessful = reader.parse( jsonString, root );
	if ( !parsingSuccessful )
	{
	    // report to the user the failure and their locations in the document.
	    std::cout  << "Failed to parse json text" << std::endl
	               << reader.getFormattedErrorMessages();
	    return;
	}

	_from    = root["from"].asString();
	_to      = root["to"].asString();
	_payload = MessagePayload(root["payload"]);
}

Message::~Message() {

}

Json::Value Message::toJson() {

	Json::Value root;

	root["from"]    = _from.c_str();
	root["to"]      = _to.c_str();

	root["payload"] = _payload.toJson();

	return root;
}
