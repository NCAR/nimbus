#include "SymCipherProtocol.h"
#include <iostream>

using namespace Protocols;

/////////////////////////////////////////////////////////////////////
SymCipherProtocol::SymCipherProtocol(bool hexCoding):
_hexCoding(hexCoding)
{
        _cipherKey = EVPCipher::makeKey(16);
        _cipherIV  = EVPCipher::makeIV(16);
        _cipher    = new EVPCipher(_cipherKey);
}

/////////////////////////////////////////////////////////////////////
SymCipherProtocol::~SymCipherProtocol() {
	delete _cipher;
}

/////////////////////////////////////////////////////////////////////
std::vector<std::string> SymCipherProtocol::outgoing(std::string s) {

	std::vector<unsigned char> encrypted;

	// encrypt the message
	encrypted = _cipher->encrypt(_cipherIV, s);

	// prepend the iv
	encrypted.insert(encrypted.begin(), _cipherIV.begin(), _cipherIV.end());

	std::vector<std::string> results;
	if (_hexCoding) {
		results.push_back(EVPCipher::toHexString(encrypted));
	} else {
		results.push_back(std::string(encrypted.begin(), encrypted.end()));
	}

	return results;
}

/////////////////////////////////////////////////////////////////////
std::vector<std::string> SymCipherProtocol::incoming(std::string s) {

	// convert incoming string to vector
	std::vector<unsigned char> tmp;
	if (_hexCoding) {
    	tmp = EVPCipher::fromHexString(s);
    } else {
    	tmp = std::vector<unsigned char>(s.begin(), s.end());
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

