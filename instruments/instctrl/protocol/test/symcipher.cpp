#include "SymCipherProtocol.h"
#include <string>
#include <vector>
#include <iostream>

int main(int argc, char** argv) {

	if (argc != 2) {
		std::cout << "Usage: " << argv[0] << " text" << std::endl;
		exit(1);
	}

	std::string clearText(argv[1]);

	Protocols::SymCipherProtocol cipher;

	std::vector<std::string> codedText;

	codedText = cipher.outgoing(clearText);

	for (int i = 0; i < codedText.size(); i++) {
		std::cout << "encrypted: " << codedText[i] << std::endl;
	}

	for (int i = 0; i < codedText.size(); i++) {
		std::vector<std::string> decodedText;

		decodedText = cipher.incoming(codedText[i]);
		for (int j = 0; j < decodedText.size(); j++) {
			std::cout << "decrypted: " << decodedText[i] << std::endl;
		}
	}

}
