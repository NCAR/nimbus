#ifndef SWITCHCONNECTION_H_
#define SWITCHCONNECTION_H_

#include <QtCore>
#include <QtNetwork>
#include "SymCipherProtocol.h"

/// The communication connection between two switches.
class SwitchConnection: public QObject {
public:
	SwitchConnection();
	virtual ~SwitchConnection();

signals:

protected slots:


protected:

};

#endif /* SWITCHCONNECTION_H_ */
