#ifndef EVPCIPHER_
#define EVPCIPHER_

#include <string>
#include <vector>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include "base64.h"

/// A class for performing symmetric encryption and decryption, using the
/// OpenSSL EVP routines, in this case the AES 128 bit Chained Block
/// Cipher (aes_128_cbc). An encryption key is shared for all encryptions
/// and decryptions performed by an instance of EVPCipher.
///
/// Each call to encrypt or decrypt requires an Initialization Vector (IV). The
/// IV protects against dictionary and other pre-computation attacks. In general
/// use, the IV is publicly visible, and will be transmitted with the encrypted
/// paylod. The IV must be the same length as the as the cipher block size.
///
/// Even though the key and IV generation routines specify a length, this length
/// should be the one returned from cipherBlockSize().
class EVPCipher
{
public:
	/// Constructor
	/// @param key The symmetric cipher key as a vector of binary bytes.
	/// It must have a length matching the cipher block length. Use
	/// getKey() to read and translate a file containing a key in
	/// base64 notation.
	EVPCipher(std::vector<unsigned char>& key);
	/// Destructor
	virtual ~EVPCipher();

	/// @return The cipher block size (in bytes).
	static int cipherBlockSize();
	/// Encrypt a collection of bytes.
	/// @param iv The initialization vector. It must have the same length as the cipher block
	/// sizes. It should be random, and change between successive encryption calls.
	/// @param input The bytes to be encrypted.
	/// @returns The encrypted values, as binary bytes.
	std::vector<unsigned char> encrypt(std::vector<unsigned char>& iv, std::string& input);
	/// Encrypt a collection of bytes.
	/// @param iv The initialization vector. It must have the same length as the cipher block
	/// sizes. It should be random, and change between successive encryption calls.
	/// @param input The bytes to be encrypted.
	/// @returns The encrypted values, as binary bytes.
	std::vector<unsigned char> encrypt(std::vector<unsigned char>& iv, std::vector<unsigned char>& input);
	/// Decrypt a collection of bytes.
	/// @param iv The initialization vector. It must have the same length as the cipher block
	/// sizes. It must match the IV used to encrypt the input.
	/// @param input The bytes to be decrypted.
	/// @returns The decrypted values, as binary bytes.
	std::vector<unsigned char> decrypt(std::vector<unsigned char>& iv, std::string& input) throw (std::string);
	/// Decrypt a collection of bytes.
	/// @param iv The initialization vector. It must have the same length as the cipher block
	/// sizes. It must match the IV used to encrypt the input.
	/// @param input The bytes to be decrypted.
	/// @returns The decrypted values, as binary bytes.
	std::vector<unsigned char> decrypt(std::vector<unsigned char>& iv, std::vector<unsigned char>& input) throw (std::string);
	/// Make an encryption key. It is simply a very random number of the desired length. It
	/// calls RAND_load_file("/dev/urandom", 1024) and RAND_bytes(&key, bytes);
	/// @param bytes The length, in bytes.
	/// @returns The key.
	static std::vector<unsigned char> makeKey(int bytes);
	/// Read a key from a base64 coded text in a file.
	/// @returns The key.
	static std::vector<unsigned char> getKey(std::string path);
	/// Create an initialization vector of the specified length. It creates
	/// a random value using RAND_pseudo_bytes() (as opposed to the more compute
	/// intensive RAND_bytes() function used in makeKey()).
	static std::vector<unsigned char> makeIV(int bytes);
	/// @return an initialization vector of the correct size for this cipher.
	/// It calls makeIV(cipherBlockSize()).
	std::vector<unsigned char> makeIV();
	/// Convert binary bytes to hex characters.
	/// @param input The binary bytes.
	/// @returns A string of printable hexadecimal characters corresponding to the
	/// input.
	static std::string toHexString(std::vector<unsigned char>& input);
	/// Convert a hexidecimal string into binary bytes.
	/// @param s The hexidecimal character input.
	/// @returns The binary representation of the inut.
	static std::vector<unsigned char> fromHexString(std::string s);
	/// Encode a byte string in base64 notation.
	/// @param input The character string to be encoded. It may contain non-printing characters.
	/// @returns A base64 version of the byte string.
	static std::string toBase64(const std::vector<unsigned char>& input);
	/// Decode a string encoded in base64 notation.
	/// @param input The string to be decoded.
	/// @returns The decoded byte array. It can contain non-printing characters.
	static std::vector<unsigned char> fromBase64(const std::string& input);

protected:
	/// The cipher key., as a string of binary bytes. It's length must match the block
	/// size of the cipher.
	std::vector<unsigned char> _key;
	/// The encryption context.
	EVP_CIPHER_CTX _encrypt;
	/// The decryption context.
	EVP_CIPHER_CTX _decrypt;
	/// The cipher block size, in bytes
	int _cipherBlockSize;

};

#endif /* EVPCIPHER_ */
