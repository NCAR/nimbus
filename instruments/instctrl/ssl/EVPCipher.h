#ifndef EVPCIPHER_
#define EVPCIPHER_

#include <string>
#include <vector>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include "base64.h"

// The IV must be the same as the block size
class EVPCipher
{
public:
	EVPCipher(std::vector<unsigned char>& key);
	virtual ~EVPCipher();

	/// @return The cipher block size (in bytes).
	int blockSize();
	std::vector<unsigned char> encrypt(std::vector<unsigned char>& iv, std::string& input);
	std::vector<unsigned char> encrypt(std::vector<unsigned char>& iv, std::vector<unsigned char>& input);
	std::vector<unsigned char> decrypt(std::vector<unsigned char>& iv, std::string& input);
	std::vector<unsigned char> decrypt(std::vector<unsigned char>& iv, std::vector<unsigned char>& input);
	/// Make a key. It is simply a random number of the desired length.
	/// @param bytes The length, in bytes.
	/// @returns The key.
	static std::vector<unsigned char> makeKey(int bytes);
	/// Read a key from a base64 coded file.
	/// @returns The key.
	static std::vector<unsigned char> getKey(std::string path);
	static std::vector<unsigned char> makeIV(int bytes);
	/// @return an initialization vector of the correct size for this cipher
	std::vector<unsigned char> makeIV();
	static std::string toHexString(std::vector<unsigned char>& input);
	static std::vector<unsigned char> fromHexString(std::string s);
	static std::string toBase64(const std::vector<unsigned char>& input);
	static std::vector<unsigned char> fromBase64(const std::string& input);

protected:
	std::vector<unsigned char> _key;
	EVP_CIPHER_CTX _encrypt;
	EVP_CIPHER_CTX _decrypt;
	int _blockSize;

};

#endif /* EVPCIPHER_ */
