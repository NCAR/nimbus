#ifndef BZIPCOMPRESS_H_
#define BZIPCOMPRESS_H_

#include <string>
#include <vector>
#include "bzlib.h"

/// Bzip compression routines.
class BzipCompress {
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



#endif /* BZIPCOMPRESS_H_ */
