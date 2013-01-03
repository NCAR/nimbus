// Test the encode/decode to/from base64

#include <string>
#include <vector>
#include <iostream>
#include "base64.h"

/// Test the encode/decode to/from base64
int main(int argc, char** argv) {

	std::string option;
	if (argc == 3) {
		option = std::string(argv[1]);
	}

	if (argc != 3 || (option != "-d" && option != "-e")) {
		std::cerr << "Usage: " << argv[0] << " -d|-e (decode/encode) text" << std::endl;
		return(1);
	}

	std::string text(argv[2]);

	if (option == "-e") {
		std::vector<unsigned char> c(text.begin(), text.end());
		std::cout << base64_encode(&c[0], c.size()) << std::endl;
	} else {
		std::cout << base64_decode(text) << std::endl;
	}

	return(0);
}

