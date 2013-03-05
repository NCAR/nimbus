#include "Message.h"
#include <sstream>

using namespace Protocols;

/////////////////////////////////////////////////////////////////////
Message::Message(MESSAGE_TYPE msgType,
				 std::string instrumentId,
				 std::string msgId,
				 std::string messageText) :
_msgType(msgType),
_payload(messageText),
_instrumentId(instrumentId),
_msgId(msgId)
{
}

/////////////////////////////////////////////////////////////////////
Message::Message(std::string jsonString)
{
	Json::Value root;
	Json::Reader reader;
	bool parsingSuccessful = reader.parse( jsonString, root );
	if ( !parsingSuccessful ) {
	    // report to the user the failure and their locations in the document.
	    std::cout  << "Failed to parse json text" << std::endl
	               << reader.getFormattedErrorMessages();
	    return;
	}

	_msgType      = (MESSAGE_TYPE) root["msgType"].asInt();
	_instrumentId = root["instId"].asString();
	_msgId        = root["msgId"].asString();
	_payload      = MessagePayload(root["payload"]);
}

/////////////////////////////////////////////////////////////////////
Message::~Message()
{
}

/////////////////////////////////////////////////////////////////////
Json::Value Message::toJson()
{
	Json::Value root;

	if (!_instrumentId.empty())
		root["instId"]  = _instrumentId.c_str();
	root["msgId"]   = _msgId.c_str();
	if (!_payload.text().empty())
		root["payload"] = _payload.toJson();

	return root;
}

/////////////////////////////////////////////////////////////////////
std::string Message::toJsonStdString()
{
	// turn the message into a string of JSON
	std::stringstream s;
	s << toJson();
	return s.str();
}

/////////////////////////////////////////////////////////////////////
MessagePayload& Message::payload()
{
	return _payload;
}

/////////////////////////////////////////////////////////////////////
Message::MESSAGE_TYPE Message::msgType()
{
	return _msgType;
}

/////////////////////////////////////////////////////////////////////
std::string Message::msgId()
{
	return _msgId;
}

/////////////////////////////////////////////////////////////////////
std::string Message::extractId(std::string s)
{
	size_t c = s.find(',');
	if (c != std::string::npos) {
		return s.substr(0, c);
	}

	return std::string("");
}
