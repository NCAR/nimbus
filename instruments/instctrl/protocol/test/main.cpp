#include "Message.h"
#include <iostream>

using namespace Protocol;

int main(int argc, char** argv) {

	// create a Message
	std::cout << "message 1:" << std::endl;
	Message message1("sender", "recipient", "this is a test message");
	std::cout << message1.toStdString() << std::endl;;

	// get the stringified json encoding of the message
	std::string jsonString = message1.toStdString();

	// create a Message from json text
	std::cout << "message 2:" << std::endl;
	Message message2(jsonString);
	std::cout << message2.toStdString() << std::endl;

	// create a Message from an ill-formed json string
	std::cout << "message 3:" << std::endl;
	Message message3("{ this is a bogus json string");
	std::cout << message3.toStdString() << std::endl;

}