#ifndef PROXYCONFIG_H_
#define PROXYCONFIG_H_

#include <string>
#include <vector>
#include <QtCore>
#include "QtConfig.h"

class InstConfig {
public:
	struct MessageInfo {
		std::string msgID;
		bool broadcast;
		int rateLimit;
	};
public:
	InstConfig(QtConfig& config);
	virtual ~InstConfig();
	int incomingPort();
	int destPort();
	std::string destIP();
    std::string instrumentName();
    std::vector<MessageInfo> messages();

protected:
    QtConfig& _config;
    std::string _instName;
	int _incomingPort;
	std::string _destIP;
	int _destPort;

};


#endif /* PROXYCONFIG_H_ */
