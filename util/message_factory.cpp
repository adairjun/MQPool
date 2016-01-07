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

}

void MessageFactory::ParseMyMsg(const struct myMsg& myMsg_,
		                        long& messageId_,
				                MessageType& messageType_,
				                string& sendServiceName_,
				                string& message_) {
  messageId_ = myMsg_.messageId;
  //先转成int型，再把int转成enum型
  messageType_ = MessageType(static_cast<int>(myMsg_.buffer[0]));

  char temp[4];
  memset(temp, 0, 4);
  memcpy(temp, myMsg_.buffer + 1, 4);
  int sendServiceNameLength = boost::lexical_cast<int>(temp);

  memset(temp, 0, 4);
  memcpy(temp, myMsg_.buffer + 5, 4);
  int messageLength = boost::lexical_cast<int>(temp);

  sendServiceName_= string(myMsg_.buffer + 9, sendServiceNameLength);
  message_ = string(myMsg_.buffer + 9 + sendServiceNameLength, messageLength);
}
