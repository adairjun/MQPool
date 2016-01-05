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
 * 我的策略是这样的，buffer[0]是枚举类型,表示发送的是什么请求：就是MessageType
 * buffer[1]到buffer[4]这四个字节就代表了发送这个消息的程序名称的长度
 * buffer[5]到buffer[8]这四个字节就代表了要发送的消息body的长度
 * 后面紧跟的是发送这个消息的程序名称
 * 后面再跟消息body
 */


#define _MYMSG_BUFFER_ 1024

struct myMsg {
  long messageId;            //在messageID.xml文件当中
  char buffer[_MYMSG_BUFFER_];
};

class MessageFactory : public Object {
 public:
  enum MessageType{
    NVALID = 0,           //非法
	REQUEST = 1,           //请求
	RESPONSE = 2,          //响应
	RETURN = 3,            //自主返回
    BROADCAST = 4         //广播
  };

  //和ClassFactory一样，这里也不能使用public的MessageFactory

  MessageFactory(const MessageFactory&) = delete;
  MessageFactory& operator=(const MessageFactory&) = delete;
  virtual ~MessageFactory();
  void Dump() const;

  static MessageFactory &Instance();

  /*
   * messageId是发送的消息id，定义在messageId.xml文件当中
   * messageType_是请求的类型，比如MessageFactory::REQUEST
   * sendServiceName是发送消息的程序名称，比如ExecuteShell
   * message是要发送的消息
   */
  struct myMsg CreateMyMsg(long messageId_,
		                MessageType messageType_,
						string sendServiceName_,
						string message_);
  /*
   * 将myMsg给解析出来,使用引用参数来接收结果
   */
  void ParseMyMsg(const struct myMsg& myMsg_,
		          long& messageId_,
				  MessageType& messageType_,
				  string& sendServiceName_,
				  string& message_);
 protected:
  //为了singleton模式，不能用public的构造函数，可以用protected和private
  MessageFactory();
};



#endif /* MQPOOL_INCLUDE_MESSAGE_FACTORY_H_ */
