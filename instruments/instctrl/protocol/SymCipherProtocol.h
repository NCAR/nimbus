#ifndef SYMENCRYPTPROTOCOL_H_
#define SYMENCRYPTPROTOCOL_H_

#include <QtCore>
#include <string>
#include <vector>
#include "Protocol.h"
#include "EVPCipher.h"

namespace Protocols
{
	/// A protocol for encrypting a message using EVPCipher. The cipher creates a
	/// new initialization vector, and uses this to and the key to encrypt the
	/// input data. The initialization vector is prepended to the beginning
	/// of the encrypted data, and this is returned as a single packet.
	///
	/// The byte representation of the encrypted messages can be created in
	/// raw (NO_CODING), hex (HEX_CODING) or base64 (BASE64_CODING) encoding.
	/// The alphanumeric codings make it easier to view the encrypted values.
	///
	/// <em>Important: The outgoing product is intended to be transmitted as a discrete
	/// message, such as a datagram, and one complete message must be
	/// provided in each call to incoming().</em>
	class SymCipherProtocol: public Protocol::Protocol, public QObject
	{
	public:
		/// Encoding types.
		enum CODING {NO_CODING, HEX_CODING, BASE64_CODING};
		/// Constructor.
		/// @param key The cipher symetric key. It must be of length EVPCipher::cipherBLockLength().
		/// @param coding The type of encoding to apply to the encrypted messages.
		SymCipherProtocol(std::vector<unsigned char> key, CODING coding = NO_CODING);
		/// Destructor.
		virtual ~SymCipherProtocol();
		/// Convert an incoming protocol stream into a message.
		/// @param s The incoming data bytes. It must be one complete
		/// message from the corresponding outgoing() encryption.
		/// @return Complete messages, if available.
		virtual std::vector<std::string> incoming(std::string s);
		/// Convert a message into an outgoing protocol stream.
		/// @param msg The message to be sent.
		/// @return Data block ready for transmision, if available.
		virtual std::vector<std::string> outgoing(std::string s);

	protected:
		/// The coding to be used on the encrypted message.
		CODING _coding;
		/// The cipher used for the symetrric encryption/decryption.
		EVPCipher* _cipher;
		/// The key for the encryption/decription.
		/// @todo add secure destruction of the key.
		std::vector<unsigned char> _cipherKey;
	};
};

#endif /* SYMENCRYPTPROTOCOL_H_ */
