#include "MQPool/message_factory.h"
#include <string.h>

#include <iostream>
using namespace std;

MessageFactory::MessageFactory() {
}

MessageFactory::~MessageFactory() {
}

void MessageFactory::Dump() const {
  printf("\n=====MessageFactory Dump START ========== \n");
  printf("\n===MessageFactory DUMP END ============\n");
}

MessageFactory& MessageFactory::Instance() {
	static MessageFactory sInstance;
	return sInstance;
}

struct myMsg MessageFactory::CreateMyMsg(long messageId_,
		                              MessageType messageType_,
                                      string sendServiceName_,
									  string message_) {
  struct myMsg create_msg;
  create_msg.messageId = messageId_;

  char temp[4];
  memset(temp, 0 ,4);
  sprintf(temp, "%d", messageType_);
  //由于这里仅仅只有5个枚举类型，所以仅仅只能够写入第一位，我们也只需要写入第一位，把temp设置为4位的目的是为了下面要使用
  create_msg.buffer[0] = temp[0];

  /*
   * 写入serviceName的长度
   */
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
