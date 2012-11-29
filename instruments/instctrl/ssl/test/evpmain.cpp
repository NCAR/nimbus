#include "EVPCipher.h"
#include <iostream>

int main(int argc, char** argv) {

	if (argc != 2) {
		std::cerr << "usage: " << argv[0] << " text" << std::endl;
		exit(1);
	}

	// get the test string
	std::string text(argv[1]);

	// set the key length
	int keyLength = 16;

	// Make key and initialization vector
	std::vector<unsigned char> key = EVPCipher::makeKey(keyLength);
	std::vector<unsigned char> iv  = EVPCipher::makeIV(keyLength);

	// Create the cipher
	EVPCipher cipher(iv, key);

	// Encrypt the text
	std::vector<unsigned char> encrypted = cipher.encrypt(text);
	std::string encryptedHex = EVPCipher::toHexString(encrypted);

	// Decrypt the text
	std::vector<unsigned char> tmp = EVPCipher::fromHexString(encryptedHex);
	std::vector<unsigned char> decrypted = cipher.decrypt(tmp);

	// Results please
	std::cout << "  iv:" << EVPCipher::toHexString(iv) << std::endl;;
	std::cout << " key:" << EVPCipher::toHexString(key) << std::endl;;
	std::cout << "text:" << text << std::endl;
	std::cout << " enc:" << encryptedHex << std::endl;
	std::cout << " dec:" << std::string(decrypted.begin(), decrypted.end()) << std::endl;

	exit(0);
}


