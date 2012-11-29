#include "SymCipherProtocol.h"
#include <string>
#include <vector>
#include <iostream>
#include <sstream>

int main(int argc, char** argv) {

	if (argc != 3) {
		std::cout << "Usage: " << argv[0] << " 1|0(hex coding yes|no)" << " text" << std::endl;
		exit(1);
	}

	// Do we want the encrypted text to be hex coded?
	bool hexCoding;
	std::stringstream s;
	s << argv[1];
	s >> hexCoding;

	// get the input text
	std::string clearText(argv[2]);
	std::cout << "    input: " << clearText << std::endl;

	// create the cipher
	std::vector<unsigned char> key = EVPCipher::makeKey(16);
	Protocols::SymCipherProtocol cipher(key, hexCoding);

	// encrypt
	std::vector<std::string> codedText;
	codedText = cipher.outgoing(clearText);

	if (hexCoding) {
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
