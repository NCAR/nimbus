#ifndef SYMENCRYPTPROTOCOL_H_
#define SYMENCRYPTPROTOCOL_H_

#include <QtCore>
#include <QtCrypto>
#include <string>
#include <vector>
#include "Protocol.h"

namespace Protocols {
	class SymCipherProtocol: public Protocol::Protocol, public QObject {
	public:
		SymCipherProtocol(bool hexCoding = true);
		virtual ~SymCipherProtocol();
		virtual std::vector<std::string> incoming(std::string s);
		virtual std::vector<std::string> outgoing(std::string s);

	protected:
        QCA::SymmetricKey *_key;
        QCA::InitializationVector *_iv;
        QCA::Cipher *_cipherEncrypt;
        QCA::Cipher *_cipherDecrypt;
        QCA::Initializer *_init;
        bool _hexCoding;


	};
};



#endif /* SYMENCRYPTPROTOCOL_H_ */
