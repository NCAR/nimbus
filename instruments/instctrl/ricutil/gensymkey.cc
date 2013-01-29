#include "EVPCipher.h"
#include <iostream>

int main(int argc, char **argv)
{

	if (argc != 1) {
		std::cout << "usage: " << argv[0] << std::endl;
		std::cout << "  Creates a " << EVPCipher::cipherBlockSize()*8 <<
				" bit key and initialization vector for use with a symmetric cipher." << std::endl;
		std::cout << "  These are encoded in base64 notation." << std::endl;
		exit(1);
	}

	int keyLength = EVPCipher::cipherBlockSize();

	std::vector<unsigned char> key;
    key = EVPCipher::makeKey(keyLength);

    std::vector<unsigned char> iv;
    iv = EVPCipher::makeIV(keyLength);

	std::cout << "Key:" << EVPCipher::toBase64(key) << std::endl;
	std::cout << " IV:" << EVPCipher::toBase64(iv) << std::endl;

}


