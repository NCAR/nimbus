#include "SymCipherProtocol.h"

using namespace Protocols;

/////////////////////////////////////////////////////////////////////
SymCipherProtocol::SymCipherProtocol(bool hexCoding):
_hexCoding(hexCoding)
{
        _cipherKey = EVPCipher::makeKey(16);
        _cipherIV  = EVPCipher::makeIV(16);
        _cipher    = new EVPCipher(_cipherIV, _cipherKey);
}

/////////////////////////////////////////////////////////////////////
SymCipherProtocol::~SymCipherProtocol() {
	delete _cipher;
}

/////////////////////////////////////////////////////////////////////
std::vector<std::string> SymCipherProtocol::outgoing(std::string s) {

	std::vector<unsigned char> encrypted;

	encrypted = _cipher->encrypt(s);

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

	std::vector<unsigned char> decrypted;
	std::vector<std::string> results;

    if (_hexCoding) {
    	std::vector<unsigned char> tmp = EVPCipher::fromHexString(s);
    	decrypted = _cipher->decrypt(tmp);
    } else {
    	decrypted = _cipher->decrypt(s);
    }

	results.push_back(std::string(decrypted.begin(), decrypted.end()));

	return results;
}

