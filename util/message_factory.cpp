#include "MQPool/message_factory.h"
#include <string.h>

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

struct rapidMsg MessageFactory::CreateRapidMsg(long messageId_,
						string message_) {
	struct rapidMsg create_msg;
	create_msg.messageId = messageId_;
	/*
	 * 写入message的长度,为了能够让ParseRapidMsg解析出message
	 * 为了方便存储，先将int型的数字转为16进制再放入char temp[8]当中
	 * 因为int型的数据范围是-2^31 ~ 2^31-1 之间，转为16进制就是80 00 00 00 到 7F FF FF FF之间，
	 * 而且message的长度也不可能为负数，那么范围就是0 到 7F FF FF FF之间
	 * 每个16进制数都转为一个字符加以保存的话，8个空间的char数组绝对放的下
	 */
	char temp[8];
	memset(temp, 0, 8);
	int messageLength = message_.length();
	sprintf(temp, "%X", messageLength);
	memcpy(create_msg.buffer, temp, 8);

	/*
	 * 写入message
	 */
	memcpy(create_msg.buffer + 8, message_.c_str(), messageLength);
	return std::move(create_msg);
}

void MessageFactory::ParseRapidMsg(const struct rapidMsg& myMsg_,
		          long& messageId_,
				  string& message_) {
  messageId_ = myMsg_.messageId;

  char temp[8];
  memset(temp, 0, 8);
  memcpy(temp, myMsg_.buffer, 8);
  int messageLength = 0;
  sscanf(temp, "%X", &messageLength);

  message_ = string(myMsg_.buffer + 8, messageLength);
}

