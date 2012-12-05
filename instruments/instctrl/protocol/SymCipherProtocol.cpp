#include "SymCipherProtocol.h"
#include <iostream>

using namespace Protocols;

/////////////////////////////////////////////////////////////////////
SymCipherProtocol::SymCipherProtocol(std::vector<unsigned char> key, SymCipherProtocol::CODING coding):
_cipherKey(key), _coding(coding)
{
        _cipher    = new EVPCipher(_cipherKey);
}

/////////////////////////////////////////////////////////////////////
SymCipherProtocol::~SymCipherProtocol() {
	delete _cipher;
}

/////////////////////////////////////////////////////////////////////
std::vector<std::string> SymCipherProtocol::outgoing(std::string s) {

	std::vector<unsigned char> encrypted;

	std::vector<unsigned char> iv  = _cipher->makeIV();

    // encrypt the message
	encrypted = _cipher->encrypt(iv, s);

	// prepend the iv
	encrypted.insert(encrypted.begin(), iv.begin(), iv.end());

	std::vector<std::string> results;
	switch(_coding) {
	case NO_CODING:
		results.push_back(std::string(encrypted.begin(), encrypted.end()));
		break;
	case HEX_CODING:
		results.push_back(EVPCipher::toHexString(encrypted));
		break;
	case BASE64_CODING:
		results.push_back(EVPCipher::toBase64(encrypted));
		break;
	}

	return results;
}

/////////////////////////////////////////////////////////////////////
std::vector<std::string> SymCipherProtocol::incoming(std::string s) {

	// convert incoming string to vector
	std::vector<unsigned char> tmp;
	switch(_coding) {
	case NO_CODING:
    	tmp = std::vector<unsigned char>(s.begin(), s.end());
		break;
	case HEX_CODING:
    	tmp = EVPCipher::fromHexString(s);
		break;
	case BASE64_CODING:
    	tmp = EVPCipher::fromBase64(s);
		break;
	}

	// remove the IV
	/// @todo sanity check that s at least contains a IV
	std::vector<unsigned char> iv(tmp.begin(), tmp.begin()+16);
	tmp.erase(tmp.begin(), tmp.begin()+16);

	std::vector<unsigned char> decrypted;
	decrypted = _cipher->decrypt(iv, tmp);

	std::vector<std::string> results;
	results.push_back(std::string(decrypted.begin(), decrypted.end()));

	return (results);
}

