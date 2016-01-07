#ifndef MQPOOL_INCLUDE_MESSAGE_FACTORY_H_
#define MQPOOL_INCLUDE_MESSAGE_FACTORY_H_

#include "object.h"
#include "logobj.h"
#include <string>

using std::string;

/*
 * 用于消息队列
 * 借鉴了leveldb的Status的存储方式：将返回码，错误信息msg以及长度打包存储于一个字符串数组当中
 * 这里借鉴leveldn的Status思想倒不是由于我机器上内存有多紧张，而是由于System V的消息队列传递的消息格式只能是这样
 * 开头是一个long型的id，后面跟一个数组
 * 我的策略是这样的，buffer[0]到buffer[3]这四个字节就是无符号整数类型的消息长度，也就是网络字节序
 * 后面紧跟的就是用protobuf序列化之后的消息
 */


#define _MYMSG_BUFFER_ 8192

struct rapidMsg {
  long messageId;              //根据proto文件得到
  char buffer[_MYMSG_BUFFER_];
};

class MessageFactory : public Object {
 public:

  //和ClassFactory一样，这里也不能使用public的MessageFactory
  MessageFactory(const MessageFactory&) = delete;
  MessageFactory& operator=(const MessageFactory&) = delete;
  virtual ~MessageFactory();
  void Dump() const;

  static MessageFactory &Instance();

  /*
   * messageId是发送的消息id，定义在proto文件当中，比如在test.151000.153000.proto的 enum MessageType 当中
   * JUST_TEST_REQUEST = 151001;
   * 表明了JUST_TEST_REQUEST这个消息id是151001
   * message是要发送的消息
   */
  struct rapidMsg CreateRapidMsg(long messageId_,
						string message_);
  /*
   * 将myMsg给解析出来,使用引用参数来接收结果
   */
  void ParseRapidMsg(const struct rapidMsg& myMsg_,
		          long& messageId_,
				  string& message_);

 protected:
  //为了singleton模式，不能用public的构造函数，可以用protected和private
  MessageFactory();
};



#endif /* MQPOOL_INCLUDE_MESSAGE_FACTORY_H_ */
