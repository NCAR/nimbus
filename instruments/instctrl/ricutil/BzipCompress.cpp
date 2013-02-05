#include "BzipCompress.h"

#include <iostream>

int BZ2_bzBuffToBuffCompress( char*         dest,
                              unsigned int* destLen,
                              char*         source,
                              unsigned int  sourceLen,
                              int           blockSize100k,
                              int           verbosity,
                              int           workFactor );

/////////////////////////////////////////////////////////////////////
std::string BzipCompress::compress(std::string s) throw (std::string) {

	// Copy input into a Bytef array
	//std::vector<char> inbuf;
	//inbuf.resize(s.size());
	//for (int i = 0; i < s.size(); i++) {
	//	inbuf[i] = s[i];
	//}

	// Make an output vector large enough to hold compressed result.
	// bzip docs say to make it 1% larger than the input plus 600 bytes.
	std::string result;
	result.resize(1.01*s.size() + 601);

	// Compress.
	unsigned int sz = result.size();
	int status = BZ2_bzBuffToBuffCompress(&result[0], &sz, &s[0], s.size(), 9, 0, 30);
	if (status != BZ_OK) {
		std::string msg;
		msg += "Error compressing data in ";
		msg += __PRETTY_FUNCTION__;
		msg += ":";
		msg += __LINE__;
		throw (msg);
	}

	result.resize(sz);

	// And return it.
	return result;
}

/////////////////////////////////////////////////////////////////////
std::string BzipCompress::uncompress(std::string s) throw (std::string) {

	// Make an output vector large enough to hold uncompressed result.
	// Use 10* the message size, assuming that the message could not have
	// been compressed by more than a factor of 10.
	/// @todo Fix this glaring problem. Implement the correct approach
	/// which iterates using the underlying zlib calls, so that a max
	/// uncompressed size does not need to be specifed.
	unsigned int sz = 10*s.size();
	std::string result;
	result.resize(sz);

	// Uncompress it.
	int status = BZ2_bzBuffToBuffDecompress(&result[0], &sz, &s[0], s.size(), 0, 0);
	if (status != BZ_OK) {
		std::string msg;
		msg += "Error decompressing data in ";
		msg += __PRETTY_FUNCTION__;
		msg += ":";
		msg += __LINE__;
		throw (msg);
	}

	result.resize(sz);

	// And return it.
	return result;
}



