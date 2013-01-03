#include "EVPCipher.h"
#include <iostream>

/// Test the EVP encryption class.
int main(int argc, char** argv) {

	if (argc != 2) {
		std::cerr << "usage: " << argv[0] << " text" << std::endl;
		exit(1);
	}

	// get the test string
	std::string text(argv[1]);

	// set the key length
	int keyLength = EVPCipher::cipherBlockSize();

	// Make key
	std::vector<unsigned char> key = EVPCipher::makeKey(keyLength);

	// Create the cipher
	EVPCipher cipher(key);

	// Make initialization vector
	std::vector<unsigned char> iv  = EVPCipher::makeIV(keyLength);

	int n = 10;

	for (int i = 0; i < n; i++) {
		std::cout << "***************** pass " << i << " *****************" << std::endl;

		if (i > n/2) {
			// make a new iv for later iterations
			iv  = EVPCipher::makeIV(keyLength);
		}

		// Encrypt the text
		std::vector<unsigned char> encrypted = cipher.encrypt(iv, text);
		std::string encryptedHex = EVPCipher::toHexString(encrypted);

		// Decrypt the text
		std::vector<unsigned char> tmp = EVPCipher::fromHexString(encryptedHex);
		std::vector<unsigned char> decrypted = cipher.decrypt(iv, tmp);

		// Results please
		std::cout << "  iv:" << EVPCipher::toHexString(iv) << std::endl;;
		std::cout << " key:" << EVPCipher::toHexString(key) << std::endl;;
		std::cout << "text:" << text << std::endl;
		std::cout << " enc:" << encryptedHex << std::endl;
		std::cout << " dec:" << std::string(decrypted.begin(), decrypted.end()) << std::endl;
	}

	exit(0);
}


