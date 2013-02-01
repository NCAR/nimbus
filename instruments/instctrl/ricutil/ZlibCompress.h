#ifndef ZLIBCOMPRESS_H_
#define ZLIBCOMPRESS_H_

#include <string>
#include <vector>
#include "zlib.h"

/// Zlib compression routines.
class ZlibCompress {
public:
	/// Compress a string
	/// @param s The string to be compressed.
	/// @return The compressed string.
	static std::string compress(std::string s) throw (std::string);
	/// Uncompress a string
	/// @param s The string to be uncompressed.
	/// @return The uncompressed string.
	static std::string uncompress(std::string s) throw (std::string);
};

#endif /* ZLIBCOMPRESS_H_ */
