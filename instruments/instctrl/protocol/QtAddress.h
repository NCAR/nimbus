#ifndef QTADDRESS_CPP_
#define QTADDRESS_CPP_

#include <QtNetwork>
#include <string>

/// Find an IP address from either a host name or IP address.
/// This makes up for the fact that QtHostAddress does not
/// use DNS. Currently only the static address() function provides
/// any functionality.

class QtAddress: QObject {
	Q_OBJECT;

public:
	/// Constructor
	QtAddress() { }

	/// Destructor
	virtual ~QtAddress() { };

	/// Find host information using DNS if necessary. If the name can't be resolved,
	/// an error message is thrown as an exception.
	/// @param name The host name or ip address.
	/// @return The host address.
	static QHostAddress address(std::string name) throw (std::string);

protected:

};

#endif /* QTADDRESS_CPP_ */
