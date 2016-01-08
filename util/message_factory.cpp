#include "MQPool/message_factory.h"
#include <string.h>
#include <boost/lexical_cast.hpp>

MessageFactory::MessageFactory() {
}

MessageFactory::~MessageFactory() {
}

void MessageFactory::Dump() const {
  printf("\n=====MessageFactory Dump START ========== \n");
  printf("\n===MessageFactory DUMP END ============\n");
}

/*
 * singleton
 */
MessageFactory& MessageFactory::Instance() {
  static MessageFactory sInstance;
  return sInstance;
}

struct rapidMsg CreateRapidMsg(long messageId_,
						string message_) {
	struct rapidMsg create_msg;
	create_msg.messageId = messageId_;
	/*
	 * 写入message的长度
	 */
	char temp[4];
	memset(temp, 0, 4);
	int messageLength = message_.length();
	sprintf(temp, "%d", messageLength);
	memcpy(create_msg.buffer, temp, 4);

	/*
	 * 写入message
	 */
	memcpy(create_msg.buffer + 4, message_.c_str(), messageLength);
}

void ParseRapidMsg(const struct rapidMsg& myMsg_,
		          long& messageId_,
				  string& message_) {
  messageId_ = myMsg_.messageId;

  char temp[4];
  memset(temp, 0, 4);
  memcpy(temp, myMsg_.buffer, 4);
  int messageLength = boost::lexical_cast<int>(temp);

  message_ = string(myMsg_.buffer + 4, messageLength);
}

