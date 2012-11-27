#include "SymCipherProtocol.h"

using namespace Protocols;

/////////////////////////////////////////////////////////////////////
SymCipherProtocol::SymCipherProtocol(bool hexCoding):
_hexCoding(hexCoding)
{
	// Set up for symmetric decryption capability
           _init = new QCA::Initializer();
           if(!QCA::isSupported("aes128-cbc-pkcs7")) {
              qDebug() << "qca-ossl is not installed on this system\n  Exiting...";
              exit(1);
           }

        // Set up the key and initialization vectors for the cipher
        //  Note: it would probably be best to have these in files shared
        //   between the switch on the ground and the switch on the aircraft.
        _key = new QCA::SymmetricKey(QCA::hexToArray(
                "22971df220ce5ade1f7fd7188e64e36ea5bd5e07a463dcd2dc9b8cc271afd6bbdd3dc54a334c2a1728237c1bd1d981902caaaeaa600f4d0ac38c62ee9e464d2822971df220ce5ade1f7fd7188e64e36ea5bd5e07a463dcd2dc9b8cc271afd6bbdd3dc54a334c2a1728237c1bd1d981902caaaeaa600f4d0ac38c62ee9e464d28"));
        _iv = new QCA::InitializationVector(QCA::hexToArray(
                "b35b2fd47af10dc72e0ab6c98a1d64825264233493f6f42caa93b0f90641ae59ac9b23f77de34bd639dc4b8a4259d32c2093860ac83e1a219b593e4b2fc83f37"));

        // Create the decryption and encryption ciphers
        // use default padding, which is equivalent to PKCS7 for CBC
       _cipherDecrypt = new QCA::Cipher(
    		   QString("aes128"),
    		   QCA::Cipher::CBC,
    		   QCA::Cipher::DefaultPadding,
    		   QCA::Decode,
    		   *_key,
    		   *_iv);
        _cipherEncrypt = new QCA::Cipher(
        		QString("aes128"),
        		QCA::Cipher::CBC,
        		QCA::Cipher::DefaultPadding,
        		QCA::Encode,
        		*_key,
        		*_iv);

}

/////////////////////////////////////////////////////////////////////
SymCipherProtocol::~SymCipherProtocol() {
	delete _cipherEncrypt;
	delete _cipherDecrypt;
	delete _init;
}

/////////////////////////////////////////////////////////////////////
std::vector<std::string> SymCipherProtocol::outgoing(std::string s) {

	QByteArray a(s.data(), s.size());
    QCA::SecureArray f = _cipherEncrypt->process(a);

    // We need to check if that update() call worked.
    if (!_cipherEncrypt->ok()) {
        qDebug() << "Unable to encrypt";
        exit(1);
    }

	std::vector<std::string> results;

	if (_hexCoding) {
		results.push_back(QString(QCA::arrayToHex(f.toByteArray())).toStdString());
	} else {
		results.push_back(QString(f.toByteArray()).toStdString());
	}

	return results;
}

/////////////////////////////////////////////////////////////////////
std::vector<std::string> SymCipherProtocol::incoming(std::string s) {

	QByteArray a(s.data(), s.size());
    QCA::SecureArray f;
    if (_hexCoding) {
    	f = _cipherDecrypt->process(QCA::hexToArray(a));
	} else {
    	f = _cipherDecrypt->process(a);
	}

    // We need to check if that update() call worked.
    if (!_cipherDecrypt->ok()) {
        qDebug() << "Unable to decrypt";
        exit(1);
    }

	std::vector<std::string> results;

	results.push_back(QString(f.data()).toStdString());

	return results;
}

