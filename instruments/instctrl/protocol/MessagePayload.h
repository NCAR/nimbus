/*
 * MessagePayload.h
 *
 *  Created on: Nov 15, 2012
 *      Author: martinc
 */

#ifndef MESSAGEPAYLOAD_H_
#define MESSAGEPAYLOAD_H_

#include <string>

namespace Protocol {

	class MessagePayload {
	public:
		MessagePayload(std::string);
		virtual ~MessagePayload();
	};
};


#endif /* MESSAGEPAYLOAD_H_ */
