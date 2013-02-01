#include "ZlibCompress.h"
#include <iostream>

/////////////////////////////////////////////////////////////////////
std::string ZlibCompress::compress(std::string s) throw (std::string) {

	// Copy input into a Bytef array
	std::vector<Bytef> inbuf;
	inbuf.resize(s.size());
	for (int i = 0; i < s.size(); i++) {
		inbuf[i] = s[i];
	}

	// Make an output vector large enough to hold compressed result.
	uLongf sz = compressBound(s.size());
	std::vector<Bytef> outbuf;
	outbuf.resize(sz);

	// Compress is.
	int status = ::compress(&outbuf[0], &sz, &inbuf[0], inbuf.size());
	if (status != Z_OK) {
		std::string msg;
		msg += "Error compressing data in ";
		msg += __PRETTY_FUNCTION__;
		msg += ":";
		msg += __LINE__;
		throw (msg);
	}

	// Transfer the result back into a string
	std::string result;
	result.resize(sz);
	for (int i = 0; i < sz; i++) {
		result[i] = outbuf[i];
	}

	// And return it.
	return result;
}

/////////////////////////////////////////////////////////////////////
std::string ZlibCompress::uncompress(std::string s) throw (std::string) {

	// Copy input into a Bytef array
	std::vector<Bytef> inbuf;
	inbuf.resize(s.size());
	for (int i = 0; i < s.size(); i++) {
		inbuf[i] = s[i];
	}

	// Make an output vector large enough to hold uncompressed result.
	// Use 10* the message size, assuming that the message could not have
	// been compressed by more than a factor of 10.
	/// @todo Fix this glaring problem. Implement the correct approach
	/// which iterates using the underlying zlib calls, so that a max
	/// uncompressed size does not need to be specifed.
	uLongf sz = 10*s.size();
	std::vector<Bytef> outbuf;
	outbuf.resize(sz);

	// Uncompress it.
	int status = ::uncompress(&outbuf[0], &sz, &inbuf[0], inbuf.size());
	if (status != Z_OK) {
		std::string msg;
		msg += "Error uncompressing data in ";
		msg += __PRETTY_FUNCTION__;
		msg += ":";
		msg += __LINE__;
		throw (msg);
	}

	// Transfer the result back into a string
	std::string result;
	result.resize(sz);
	for (int i = 0; i < sz; i++) {
		result[i] = outbuf[i];
	}

	// And return it.
	return result;
}

