#include "EVPCipher.h"
#include <iostream>
#include <sstream>
#include <iomanip>

/////////////////////////////////////////////////////////////////////
EVPCipher::EVPCipher(std::vector<unsigned char>& key):
_key(key)
{
	EVP_CIPHER_CTX_init(&_encrypt);
	EVP_CIPHER_CTX_init(&_decrypt);
}

/////////////////////////////////////////////////////////////////////
EVPCipher::~EVPCipher() {

}

/////////////////////////////////////////////////////////////////////
std::vector<unsigned char> EVPCipher::encrypt(std::vector<unsigned char>& iv, std::string& input) {

	std::vector<unsigned char> vectorinput(input.begin(), input.end());

	return (encrypt(iv, vectorinput));

}

/////////////////////////////////////////////////////////////////////
std::vector<unsigned char> EVPCipher::encrypt(std::vector<unsigned char>& iv, std::vector<unsigned char>& input) {

	int status;
	if (iv.size() != _key.size()) {
		std::cerr << "EVPCipher: key and initilization vector must be of the same size" << std::endl;
		exit(1);
	}

	// initialize the encryption cipher
	status = EVP_EncryptInit_ex(&_encrypt, EVP_aes_128_cbc(), 0, 0, 0);
	status = EVP_CIPHER_CTX_set_key_length(&_encrypt, _key.size());
	status = EVP_EncryptInit_ex(&_encrypt, 0, 0, &_key[0], &iv[0]);

	std::vector<unsigned char> result;
	result.resize(input.size()+EVP_CIPHER_CTX_block_size(&_encrypt));

	int n;
	int n_final = 0;
	status = EVP_EncryptUpdate(&_encrypt, &result[0], &n, &input[0], input.size());
	n_final += n;
	status = EVP_EncryptFinal_ex(&_encrypt, &result[n], &n);
	n_final += n;

	result.resize(n_final);

	return result;
}

/////////////////////////////////////////////////////////////////////
std::vector<unsigned char> EVPCipher::decrypt(std::vector<unsigned char>& iv, std::string& input) {

	std::vector<unsigned char> vectorinput(input.begin(), input.end());
	return (decrypt(iv, vectorinput));

}

/////////////////////////////////////////////////////////////////////
std::vector<unsigned char> EVPCipher::decrypt(std::vector<unsigned char>& iv, std::vector<unsigned char>& input) {

	int status;

	if (iv.size() != _key.size()) {
		std::cerr << "EVPCipher: key and initilization vector must be of the same size" << std::endl;
		exit(1);
	}

	// initialize the decryption cipher
	status = EVP_DecryptInit_ex(&_decrypt, EVP_aes_128_cbc(), 0, 0, 0);
	status = EVP_CIPHER_CTX_set_key_length(&_decrypt, _key.size());
	status = EVP_DecryptInit_ex(&_decrypt, 0, 0, &_key[0], &iv[0]);

    std::vector<unsigned char> result;
	result.resize(input.size()+EVP_CIPHER_CTX_block_size(&_decrypt));

	int n;
	int n_final = 0;
	status = EVP_DecryptUpdate(&_decrypt, &result[0], &n, &input[0], input.size());
	n_final += n;
	status = EVP_DecryptFinal_ex(&_decrypt, &result[n], &n);
	n_final += n;

	result.resize(n_final);

	return result;
}

/////////////////////////////////////////////////////////////////////
std::string EVPCipher::toHexString(std::vector<unsigned char>& input) {
	std::stringstream s;
	s << std::uppercase;
	for (int i = 0; i < input.size(); i++) {
		s << std::hex << std::setw(2) << std::setfill('0') << (int)input[i];
	}

	return s.str();

}

/////////////////////////////////////////////////////////////////////
std::vector<unsigned char> EVPCipher::fromHexString(std::string input) {

	std::stringstream s;
	std::vector<unsigned char> result;

	for (int i = 0; i < input.size(); i += 2) {
		std::string byte = input.substr(i, 2);
		s.clear();
		s << std::hex << byte;
		unsigned int c;
		s >> c;
		result.push_back(c);
	}

	return result;
}

/////////////////////////////////////////////////////////////////////
std::vector<unsigned char> EVPCipher::makeKey(int bytes) {

	int status;

	// initialize the random number generator
	status = RAND_load_file("/dev/urandom", 1024);
	if (!status) {
		std::cerr << "Unable to seed the random number generator" << std::endl;
		exit(1);
	}

	std::vector<unsigned char> key;
	key.resize(bytes);
	status = RAND_bytes(&key[0], key.size());

	return key;
}

/////////////////////////////////////////////////////////////////////
std::vector<unsigned char> EVPCipher::makeIV(int bytes) {

	int status;

	// initialize the random number generator
	status = RAND_load_file("/dev/urandom", 1024);
	if (!status) {
		std::cerr << "Unable to seed the random number generator" << std::endl;
		exit(1);
	}

	std::vector<unsigned char> iv;
	iv.resize(bytes);
	status = RAND_pseudo_bytes(&iv[0], iv.size());

	return iv;

}

