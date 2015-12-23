#include "MQPool/message_factory.h"

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

struct myMsg MessageFactory::GetMyMsg(long messageId_,
		                              MessageType messageType_,
                                      string sendServiceName_,
									  string message_) {
  struct myMsg create_msg;
  create_msg.messageId = messageId_;
  create_msg.buffer[0] = messageType_;
  const char* sendServiceName = sendServiceName.c_str();
  int sendServiceNameLength = sizeof(sendServiceName);
  char temp[4];
  sprintf(temp, "%d", sendServiceNameLength);
  memcpy(create_msg.buffer + 1, temp, 4);

  const char* message = message_.c_str();
  int messageLength = sizeof(message);
  sprintf(temp, "%d", messageLength);
  memcpy(create_msg.buffer + 5, temp, 4);

  memcpy(create_msg.buffer + 9, sendServiceName, sendServiceName, sizeof(sendServiceName));
  memcpy(create_msg.buffer + 9 + sendServiceNameLength, message, sizeof(message))

  return create_msg;
}
