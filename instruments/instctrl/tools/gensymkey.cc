#include "EVPCipher.h"
#include <iostream>

int main(int argc, char **argv)
{

	// Take as input the size of the key, if no size indicated make it 64
    int size = (argc >= 2) ? atoi(argv[1]) : 64;

    std::vector<unsigned char> key;
    key = EVPCipher::makeKey(size/8);

    std::vector<unsigned char> iv;
    iv = EVPCipher::makeIV(size/8);

	std::cout << "Key:" << EVPCipher::toHexString(key) << std::endl;
	std::cout << " IV:" << EVPCipher::toHexString(iv) << std::endl;

}


