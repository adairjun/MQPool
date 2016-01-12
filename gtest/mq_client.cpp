#include <iostream>

#include "MQPool/message_queue.h"
#include "rapidmsg/rapidmsg.pb.h"
#include "rapidmsg/test.151000.153000.pb.h"
#include <boost/lexical_cast.hpp>

#include <string.h>

using namespace std;
using namespace rapidmsg;

#define PERM S_IRUSR|S_IWUSR  

int main(int argc,char **argv)  
{  
  MessageQueue myQueue("key");

  long messageId = test::JUST_TEST_REQUEST;

  RMessage rmessage;

  Head* head = new Head;
  head->set_session_no("1");
  head->set_message_type(messageId);
  head->set_client_ip("127.0.0.1");
  head->set_target_ip("127.0.0.1");
  head->set_target_port(9999);

  Body* body = new Body;

  ::rapidmsg::test::JustTestRequest* request = body->MutableExtension(::rapidmsg::test::just_test_request);
  request->set_test_id(1);
  request->set_test_name("test_request");

  rmessage.set_allocated_head(head);
  rmessage.set_allocated_body(body);

  string str;
  rmessage.SerializeToString(&str); // 将对象序列化到字符串，除此外还可以序列化到fstream等

  RMessage pmessage;
  pmessage.ParseFromString(str);

  struct rapidMsg msg = MessageFactory::Instance().CreateRapidMsg(messageId, str);
  myQueue.SendMsg(&msg);

  memset(&msg, 0, sizeof(struct rapidMsg));

  //接收RESPONSE
  messageId = test::JUST_TEST_RESPONSE;

  myQueue.RecvMsg(messageId, &msg);

  string message;
  MessageFactory::Instance().ParseRapidMsg(msg, messageId, message);

  rmessage.ParseFromString(message);

  // head
  cout << "rmessage.head().session_no()" << rmessage.head().session_no() << endl;
  cout << "rmessage.head().message_type()" << rmessage.head().message_type() << endl;
  cout << "rmessage.head().client_ip()" << rmessage.head().client_ip() << endl;
  cout << "rmessage.head().target_ip()" << rmessage.head().target_ip() << endl;
  cout << "rmessage.head().target_port()" << rmessage.head().target_port() << endl;

  //body
  cout << "rmessage.body().GetExtension(::rapidmsg::test::just_test_response).test_id()"
		  << rmessage.body().GetExtension(::rapidmsg::test::just_test_response).test_id()
		  << endl;

  return 0;
}
