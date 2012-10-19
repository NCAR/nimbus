/* 
  This is a modified version of code from the qca-2.0.3 library.

 Copyright (C) 2003 Justin Karneges <justin@affinix.com>
 Copyright (C) 2005-2006 Brad Hards <bradh@frogmouth.net>

  I've taken the ciphertest example and modified it to generate a symmetic key
  and associated initialization vector. These are used by the two sides of the
  switch to encrypt and decrypt the UDP packets containing remote instrument control 
  information.

  Author:  Tom Baltzer tbaltzer@ucar.edu
*/

#include <QtCrypto>
#include <stdlib.h>
#include <stdio.h>

#include <QCoreApplication>

int main(int argc, char **argv)
{
    // the Initializer object sets things up, and
    // also does cleanup when it goes out of scope
    QCA::Initializer init;

    QCoreApplication app(argc, argv);

    // Take as input the size of the key, if no size indicated make it 64
    int size = (argc >= 2) ? atoi(argv[1]) : 64;

    // AES128 testing
    if(!QCA::isSupported("aes128-cbc-pkcs7"))
        printf("AES128-CBC not supported!\n");
    else {
        // Create a random key - you'd probably use one from another
        // source in a real application
        QCA::SymmetricKey key(size);
        printf("Key: %s\n", qPrintable(QCA::arrayToHex(key.toByteArray())));

        // Create a random initialisation vector - you need this
        // value to decrypt the resulting cipher text, but it
        // need not be kept secret (unlike the key).
        QCA::InitializationVector iv(size);
        printf("IV: %s\n", qPrintable(QCA::arrayToHex(iv.toByteArray())));

    }
}


