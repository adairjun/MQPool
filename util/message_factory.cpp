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


struct myMsg MessageFactory::CreateMyMsg(long messageId_,
		                              MessageType messageType_,
                                      string sendServiceName_,
									  string message_) {
  struct myMsg create_msg;
  create_msg.messageId = messageId_;
  create_msg.buffer[0] = static_cast<char>(messageType_);

  /*
   * 写入serviceName的长度
   */
  char temp[4];
  memset(temp, 0 ,4);
  int sendServiceNameLength = sendServiceName_.length();
  sprintf(temp, "%d", sendServiceNameLength);
  memcpy(create_msg.buffer + 1, temp, 4);

  /*
   * 写入message的长度
   */
  memset(temp,0, 4);
  int messageLength = message_.length();
  sprintf(temp, "%d", messageLength);
  memcpy(create_msg.buffer + 5, temp, 4);

  /*
   * 写入serviceName的主体，message的主体
   */
  memcpy(create_msg.buffer + 9, sendServiceName_.c_str(), sendServiceNameLength);
  memcpy(create_msg.buffer + 9 + sendServiceNameLength, message_.c_str(), messageLength);

  return create_msg;
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
