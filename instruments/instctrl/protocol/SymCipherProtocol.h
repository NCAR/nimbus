#ifndef SYMENCRYPTPROTOCOL_H_
#define SYMENCRYPTPROTOCOL_H_

#include <QtCore>
#include <string>
#include <vector>
#include "Protocol.h"
#include "EVPCipher.h"

namespace Protocols
{
	class SymCipherProtocol: public Protocol::Protocol, public QObject
	{
	public:
		enum CODING {NO_CODING, HEX_CODING, BASE64_CODING};
		SymCipherProtocol(std::vector<unsigned char> key, CODING coding = NO_CODING);
		virtual ~SymCipherProtocol();
		virtual std::vector<std::string> incoming(std::string s);
		virtual std::vector<std::string> outgoing(std::string s);

	protected:
		CODING _coding;

		EVPCipher* _cipher;
		std::vector<unsigned char> _cipherKey;
	};
};

#endif /* SYMENCRYPTPROTOCOL_H_ */
