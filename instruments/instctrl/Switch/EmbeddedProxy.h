/*
 * EmbeddedProxy.h
 *
 *  Created on: Dec 10, 2012
 *      Author: martinc
 */

#ifndef EMBEDDEDPROXY_H_
#define EMBEDDEDPROXY_H_

#include <QtCore>
#include "Message.h"

class EmbeddedProxy: public QObject {
	Q_OBJECT;
public:
	EmbeddedProxy();
	virtual ~EmbeddedProxy();

};

#endif /* EMBEDDEDPROXY_H_ */
