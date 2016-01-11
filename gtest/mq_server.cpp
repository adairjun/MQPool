#include <iostream>
#include <string.h>  

#include "MQPool/message_queue.h"
#include "rapidmsg.pb.h"
#include "test.151000.153000.pb.h"
#include <boost/lexical_cast.hpp>
using namespace std;
using namespace rapidmsg;

/*
 * 消息队列与protobuf结合起来使用方能凸显威力
 */

int main()  
{  
  MessageQueue myQueue("key");
  struct rapidMsg msg;

  while(1) {

	long messageId = test::JUST_TEST_REQUEST;
    myQueue.RecvMsg(messageId, &msg);

    //取到序列化的字符串
    string message;
    MessageFactory::Instance().ParseRapidMsg(msg, messageId, message);

    //反序列化
	RMessage rmessage;
	rmessage.ParseFromString(message);

	// head
	cout << "rmessage.head().session_no()" << rmessage.head().session_no() << endl;
	cout << "rmessage.head().message_type()" << rmessage.head().message_type() << endl;
	cout << "rmessage.head().client_ip()" << rmessage.head().client_ip() << endl;
	cout << "rmessage.head().target_ip()" << rmessage.head().target_ip() << endl;
	cout << "rmessage.head().target_port()" << rmessage.head().target_port() << endl;

	//body
	cout << "rmessage.body().GetExtension(::rapidmsg::test::just_test_request).test_id()"
			<< rmessage.body().GetExtension(::rapidmsg::test::just_test_request).test_id() << endl;
	cout << "rmessage.body().GetExtension(::rapidmsg::test::just_test_request).test_name()"
					<< rmessage.body().GetExtension(::rapidmsg::test::just_test_request).test_name() << endl;

	messageId = test::JUST_TEST_RESPONSE;
	RMessage pmessage;

	Head* head = new Head;
	head->set_session_no("1");
	head->set_message_type(messageId);
	head->set_client_ip("127.0.0.1");
	head->set_target_ip("127.0.0.1");
	head->set_target_port(9999);

	Body* body = new Body;

	ResponseCode* responseCode = new ResponseCode;
	responseCode->set_retcode(0);
	responseCode->set_error_message("");

	::rapidmsg::test::JustTestResponse* response = body->MutableExtension(::rapidmsg::test::just_test_response);

	response->set_test_id(1);
	response->set_allocated_rc(responseCode);

	pmessage.set_allocated_head(head);
	pmessage.set_allocated_body(body);

	string str;
	pmessage.SerializeToString(&str); // 将对象序列化到字符串，除此外还可以序列化到fstream等

    msg = MessageFactory::Instance().CreateRapidMsg(messageId, str);
	myQueue.SendMsg(&msg);
  }
  /*
   * 这里没有删除消息队列的机会，因为首先客户端肯定不能删除消息队列，否则服务端往队列当中写的消息就全部没有了
   * 然而服务端又处于无限循环当中，执行不到这个DeleteMsgQue的程序，看来只能使用ipcs 和 ipcrm这两个linux命令来删除
   */
  myQueue.DeleteMsgQue();
  return 0;
}
