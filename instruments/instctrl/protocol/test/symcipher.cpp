#include "SymCipherProtocol.h"
#include <string>
#include <vector>
#include <iostream>
#include <sstream>

using namespace Protocols;
int main(int argc, char** argv) {

	char coding;
	if (argc == 3) {
		coding = argv[1][0];
	}
	if (argc != 3 ||
			(coding != 'n' &&
			coding != 'h' &&
			coding != 'b' ))
	{
		std::cout << "Usage: " << argv[0] << " n|h|b(none|hex|base64 coding)" << " text" << std::endl;
		exit(1);
	}

	// Do we want the encrypted text to be coded?
	SymCipherProtocol::CODING codingType;
	switch(coding) {
	case 'n':
		codingType = SymCipherProtocol::NO_CODING;
		break;
	case 'h':
		codingType = SymCipherProtocol::HEX_CODING;
		break;
	case 'b':
		codingType = SymCipherProtocol::BASE64_CODING;
		break;
	}

	// get the input text
	std::string clearText(argv[2]);
	std::cout << "    input: " << clearText << std::endl;

	// create the cipher
	std::vector<unsigned char> key = EVPCipher::makeKey(16);
	Protocols::SymCipherProtocol cipher(key, codingType);

	// encrypt
	std::vector<std::string> codedText;
	codedText = cipher.outgoing(clearText);

	if (codingType == SymCipherProtocol::HEX_CODING ||
			codingType == SymCipherProtocol::BASE64_CODING) {
		// don't print binary encrypted text
		for (int i = 0; i < codedText.size(); i++) {
			std::cout << "encrypted: " << codedText[i] << std::endl;
		}
	}

	// decrypt
	for (int i = 0; i < codedText.size(); i++) {
		std::vector<std::string> decodedText;
		decodedText = cipher.incoming(codedText[i]);

		for (int j = 0; j < decodedText.size(); j++) {
			std::cout << "decrypted: " << decodedText[i] << std::endl;
		}
	}
}
