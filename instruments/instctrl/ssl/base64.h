// from Github:https://github.com/ReneNyffenegger/development_misc/tree/master/base64

#ifndef BASE64_H_
#define BASE64_H_

#include <string>

/// Encode a byte string in base64 notation.
/// @param s The character string to be encoded. It may contain non-printing characters.
/// @param len The length of the byte string.
/// @returns A base64 version of the byte string.
std::string base64_encode(unsigned char const* s, unsigned int len);

/// Decode a string encoded in base64 notation
/// @param s The string to be decoded.
/// @returns The decoded byte array. It can contain non-printing characters.
std::string base64_decode(std::string const& s);

#endif /* BASE64_H_ */
