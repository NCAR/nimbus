#ifndef EVPCIPHER_
#define EVPCIPHER_

#include <string>
#include <vector>
#include <openssl/evp.h>
#include <openssl/rand.h>

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
	static std::vector<unsigned char> makeKey(int bytes);
	static std::vector<unsigned char> makeIV(int bytes);
	/// @return an initialization vector of the correct size for this cipher
	std::vector<unsigned char> makeIV();
	static std::string toHexString(std::vector<unsigned char>& input);
	static std::vector<unsigned char> fromHexString(std::string s);

protected:
	std::vector<unsigned char> _key;
	EVP_CIPHER_CTX _encrypt;
	EVP_CIPHER_CTX _decrypt;
	int _blockSize;

};

#endif /* EVPCIPHER_ */
