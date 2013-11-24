#include "EVPCipher.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <assert.h>

/////////////////////////////////////////////////////////////////////
EVPCipher::EVPCipher(std::vector<unsigned char>& key):
_key(key),
_cipherBlockSize(16)
{
	int status;

	// set the cipher type so that we can determine the block size.
	EVP_CIPHER_CTX_init(&_encrypt);
	status = EVP_EncryptInit_ex(&_encrypt, EVP_aes_128_cbc(), 0, 0, 0);

	EVP_CIPHER_CTX_init(&_decrypt);
	status = EVP_DecryptInit_ex(&_decrypt, EVP_aes_128_cbc(), 0, 0, 0);

	if (key.size() != _cipherBlockSize) {
		/// @ this should be changed to an exception rather than an exit.
		std::cerr << __PRETTY_FUNCTION__ << " cipher key length does not match the cipher block length" << std::endl;
		assert(0==1);
		exit(1);
	}

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

	if (iv.size() != _cipherBlockSize) {
		std::cerr << "EVPCipher: initilization vector must match the cipher block size of "
				<< _cipherBlockSize << std::endl;
		exit(1);
	}

	int status;

	// initialize the encryption cipher
	status = EVP_EncryptInit_ex(&_encrypt, EVP_aes_128_cbc(), 0, 0, 0);
	status = EVP_CIPHER_CTX_set_key_length(&_encrypt, _key.size());
	status = EVP_EncryptInit_ex(&_encrypt, 0, 0, &_key[0], &iv[0]);

	std::vector<unsigned char> result;
	result.resize(input.size()+_cipherBlockSize);

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
std::vector<unsigned char> EVPCipher::decrypt(std::vector<unsigned char>& iv, std::string& input) throw (std::string) {

	std::vector<unsigned char> vectorinput(input.begin(), input.end());
	return (decrypt(iv, vectorinput));

}

/////////////////////////////////////////////////////////////////////
std::vector<unsigned char> EVPCipher::decrypt(std::vector<unsigned char>& iv,
		std::vector<unsigned char>& input) throw (std::string) {

	if (iv.size() != _cipherBlockSize) {
		std::cerr << "EVPCipher: initialization vector must match the cipher block size of "
				<< _cipherBlockSize << std::endl;
		exit(1);
	}

	int status;

	// initialize the decryption cipher
	status = EVP_DecryptInit_ex(&_decrypt, EVP_aes_128_cbc(), 0, 0, 0);
	status = EVP_CIPHER_CTX_set_key_length(&_decrypt, _key.size());
	status = EVP_DecryptInit_ex(&_decrypt, 0, 0, &_key[0], &iv[0]);

    std::vector<unsigned char> result;
	result.resize(input.size()+_cipherBlockSize);

	int n;
	int n_final = 0;
	status = EVP_DecryptUpdate(&_decrypt, &result[0], &n, &input[0], input.size());
	if (!status) {
		throw (std::string("Decryption failed (are the cipher keys correct?)"));
	}
	n_final += n;
	status = EVP_DecryptFinal_ex(&_decrypt, &result[n], &n);
	if (!status) {
		throw (std::string("Decryption failed (are the cipher keys correct?)"));
	}
	n_final += n;

	result.resize(n_final);

	return result;
}

/////////////////////////////////////////////////////////////////////
int EVPCipher::cipherBlockSize() {
	return 16;
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

	// initialize the random number generator.
	/// @todo This will probably not work for Windows builds. Correct that.
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
std::vector<unsigned char> EVPCipher::getKey(std::string path) {

	/// @todo catch file open errors
	std::ifstream file(path.c_str());

	std::vector<unsigned char> key;

	if (file.is_open()) {
		// get all of the text in the file
		std::string s;
		while (file.good()) {
			char c;
			file >> std::skipws >> c;
			if (c == '=' ||
				c == '+' ||
				c == '/' ||
				(c >= 'a' && c <= 'z') ||
				(c >= 'A' && c <= 'Z') ||
				(c >= '0' && c <= '9') ) {
				s += c;
			}
		}
		key = fromBase64(s);
	}

	/// @todo catch returning no key
	return key;
}

/////////////////////////////////////////////////////////////////////
std::vector<unsigned char> EVPCipher::makeIV() {
	return makeIV(_cipherBlockSize);
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

/////////////////////////////////////////////////////////////////////
std::string EVPCipher::toBase64(const std::vector<unsigned char>& input)
{

	std::string result;

	result = base64_encode(&input[0], input.size());

	return result;
}

/////////////////////////////////////////////////////////////////////
std::vector<unsigned char> EVPCipher::fromBase64(const std::string& input) {

	std::string decoded = base64_decode(input);

	std::vector<unsigned char> result(decoded.begin(), decoded.end());

	return result;
}
